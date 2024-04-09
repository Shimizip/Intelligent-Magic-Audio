import os
import shutil
import csv
import curses
import threading
import time
from pydub.utils import mediainfo
from uuid import uuid4
import keyboard

from audio_player import AudioPlayer

tag1 = 'Drum'
tag2 = 'Vocal'
tag3 = 'Harmonic'
tag4 = 'Percussive'

class FileInfo:
    def __init__(self, file_path, file_name):
        self.tags = {tag1: '', tag2: '', tag3: '', tag4: ''}
        self.added_to_csv = False
        self.uid = None
        self.file_path = file_path
        self.file_name = file_name

class Learner:
    fieldnames = ['UID', 'File', tag1, tag2, tag3, tag4]
    def __init__(self, version):
        # curses init
        self.stdscr = curses.initscr()
        self.stdscr.clear()  # Clear the screen
        self.stdscr.refresh()  # Refresh the screen to apply changes
        curses.curs_set(0)  # Hide the cursor

        self.version_folder = os.path.join("learndata", version)
        self.input_folder = os.path.join(self.version_folder, "input_folder")
        self.output_folder = os.path.join(self.version_folder, "output_folder")
        self.tags_csv = os.path.join(self.version_folder, "tags.csv")
        
        self.player = AudioPlayer()
        self.playing_indicator = "▶"  # Playing indicator
        self.currently_playing = None  # Variable to store the currently playing file path
        self.audio_playing_event = threading.Event()
        self.waiting_for_play_sig = False
        
        self.files = []
        self.file_info_dict = {}
        self.current_index = 0
        self.start_index = 0
        self.end_index = 0
        self.redraw = True
        


    def create_version_folder(self):
        if os.path.exists(self.version_folder):
            print(f"Error: Version folder '{self.version_folder}' already exists.")
        else:
            os.makedirs(self.version_folder)
            print(f"Version folder '{self.version_folder}' created.")
            os.makedirs(self.input_folder)
            os.makedirs(self.output_folder)

    def list_files_recursive(self, folder):
        for root, dirs, files in os.walk(folder):
            for file in files:
                file_path = os.path.join(root, file)
                if file_path.endswith('.mp3') or file_path.endswith('.wav') or file_path.endswith('.aif'):
                    self.files.append(file_path)

    def initialize_file_info_objects(self):
        for file_path in self.files:
            file_name = os.path.basename(file_path)
            # Check if FileInfo object already exists for the file_path
            if file_path not in self.file_info_dict:
                self.file_info_dict[file_path] = FileInfo(file_path, file_name)

    def display_status_message(self, message):
        max_y, max_x = self.stdscr.getmaxyx()
        self.stdscr.move(max_y - 1, 0)  # Move cursor to the last row
        self.stdscr.clrtoeol()  # Clear the last row
        self.stdscr.addstr(max_y - 1, 0, message)  # Print the message
        self.stdscr.refresh()  # Refresh the screen
        
        # Define a function to clear the message after 1 second
        def clear_message():
            time.sleep(1)
            self.stdscr.move(max_y - 1, 0)  # Move cursor to the last row
            self.stdscr.clrtoeol()  # Clear the last row
            self.stdscr.refresh()  # Refresh the screen to remove the message
        
        # Start a new thread to execute the clear_message function
        threading.Thread(target=clear_message).start()

    def display_file_list(self):
        self.stdscr.clear()
        max_y, max_x = self.stdscr.getmaxyx()

        self.stdscr.addstr(0, 0, "Audio Files:")
        
        # Calculate start and end indices for visible items
        start_index = max(0, min(self.current_index - max_y // 2, len(self.file_info_dict) - max_y + 2))
        end_index = min(len(self.file_info_dict), start_index + max_y - 2)
        
        # Display visible items
        for i, (file_path, file_info) in enumerate(list(self.file_info_dict.items())[start_index:end_index], start=start_index):
            tags_str = ', '.join(key for key, value in file_info.tags.items() if value == 1)
            added_status = "Added" if file_info.added_to_csv else ""

            playing_indicator_col = self.playing_indicator if file_path == self.currently_playing and self.audio_playing_event.is_set() else " "
            line = f"{playing_indicator_col:<4} {file_info.file_name:<30} | Tags: {tags_str:<45} | Status: {added_status}"
            if self.waiting_for_play_sig:
                self.audio_playing_event.wait()
                self.waiting_for_play_sig  = False
            if i == self.current_index:
                self.stdscr.addstr(i - start_index + 1, 0, f"> {line}", curses.A_BOLD)
            else:
                self.stdscr.addstr(i - start_index + 1, 0, f"  {line}")

        # Refresh the screen
        self.stdscr.refresh()

    def move_cursor(self, direction):
        if direction == 'down':
            self.current_index = min(self.current_index + 1, len(self.files) - 1)
            if self.current_index >= self.end_index:
                self.start_index += 1
                self.end_index += 1
        elif direction == 'up':
            self.current_index = max(self.current_index - 1, 0)
            if self.current_index < self.start_index:
                self.start_index -= 1
                self.end_index -= 1

    def play_audio(self, file_path):
            def _play_audio():
                self.player.load_audio(file_path)
                self.audio_playing_event.set() 
                self.player.play()
                self.audio_playing_event.clear()

            audio_thread = threading.Thread(target=_play_audio)
            audio_thread.start()
            self.currently_playing = file_path
            self.redraw = True

    def play_current_audio(self):
        file_path = self.files[self.current_index]
        self.play_audio(file_path)

    def tag_file(self, tag):
        file_path = self.files[self.current_index]
        if self.file_info_dict[file_path].tags[tag] == 1:
            self.file_info_dict[file_path].tags[tag] = 0
        else:
            self.file_info_dict[file_path].tags[tag] = 1

    def save_file_to_csv(self, file_path):
        file_info = self.file_info_dict[file_path]
        if any(file_info.tags.values()):
            if not self.file_info_dict[file_path].uid:
                uid = uuid4().hex
                self.file_info_dict[file_path].uid = uid
            else:
                uid = self.file_info_dict[file_path].uid
            output_file_path = os.path.join(self.output_folder, f"{uid}.wav")
            shutil.copy2(file_path, output_file_path)
            with open(self.tags_csv, 'a', newline='') as csvfile:
                writer = csv.DictWriter(csvfile, fieldnames=self.fieldnames)
                writer.writerow({'UID': uid,
                                'File': file_path,
                                tag1: file_info.tags[tag1],
                                tag2: file_info.tags[tag2],
                                tag3: file_info.tags[tag3],
                                tag4: file_info.tags[tag4]})
            self.file_info_dict[file_path].added_to_csv = True
        else:
            self.display_status_message("No tags assigned to the current file. Skipping saving to CSV.")

    def load_existing_tags_from_csv(self):
        if os.path.exists(self.tags_csv):
            with open(self.tags_csv, 'r') as csvfile:
                reader = csv.reader(csvfile)
                # Skip the header row
                next(reader, None)
                for row in reader:
                    uid = row[0]
                    file_path = row[1]
                    file_name = os.path.basename(file_path)
                    self.file_info_dict[file_path] = FileInfo(file_path, file_name)
                    self.file_info_dict[file_path].added_to_csv = True
                    self.file_info_dict[file_path].uid = uid
                    for i, tag in enumerate([tag1, tag2, tag3, tag4], start=2):
                        # Check if the tag value is not empty
                        if row[i] and row[i] != '':
                            # Convert the value to int and set the corresponding tag value in file_info_dict
                            self.file_info_dict[file_path].tags[tag] = int(row[i])


    def save_tags_to_csv(self):
        file_path = self.files[self.current_index]
        self.save_file_to_csv(file_path)

    def save_all_tagged_files_to_csv(self):
        with open(self.tags_csv, 'w', newline='') as csvfile:
            writer = csv.DictWriter(csvfile, fieldnames=self.fieldnames)
            writer.writeheader()
        for file_path, file_info in self.file_info_dict.items():
            if any(file_info.tags.values()):
                self.save_file_to_csv(file_path)

        def delete_file(self):
            file_path = self.files[self.current_index]
            if self.file_info_dict[file_path].added_to_csv:
                uid = self.file_info_dict[file_path].uid
                with open(self.tags_csv, 'w', newline='') as csvfile:
                    writer = csv.DictWriter(csvfile, fieldnames=['UID', 'File'])
                    writer.writerow({'UID': uid, 'File': file_path})
                copied_file_path = os.path.join(self.output_folder, f"{uid}.wav")
                os.remove(copied_file_path)
                self.file_info_dict[file_path] = FileInfo("", "")
            else:
                print(f"File '{self.file_info_dict[file_path].file_name}' has not been added to CSV yet.")

    def run(self):
        self.create_version_folder()
        self.list_files_recursive(self.input_folder)
        self.initialize_file_info_objects()
        self.end_index = min(len(self.files), os.get_terminal_size().lines - 4)
        
        while True:
            # if self.redraw:
                # self.display_file_list()
                # self.redraw = False

            self.display_file_list()

            key_event = keyboard.read_event()
            if key_event.event_type == keyboard.KEY_DOWN:
                if keyboard.is_pressed('down') or keyboard.is_pressed('up'):
                    self.move_cursor('down' if keyboard.is_pressed('down') else 'up')
                    self.redraw = True
                elif keyboard.is_pressed('space'):
                    self.play_current_audio()
                    self.redraw = True
                    self.waiting_for_play_sig = True
                elif keyboard.is_pressed('1') or keyboard.is_pressed('2') or keyboard.is_pressed('3') or keyboard.is_pressed('4'):
                    self.tag_file(tag1 if keyboard.is_pressed('1') else (tag2 if keyboard.is_pressed('2') else (tag3 if keyboard.is_pressed('3') else tag4)))
                    self.redraw = True
                elif keyboard.is_pressed('enter'):
                    # self.save_tags_to_csv()
                    self.save_all_tagged_files_to_csv()
                    self.redraw = True
                elif keyboard.is_pressed('delete'):
                    self.delete_file()
                    self.redraw = True

if __name__ == "__main__":
    learner = Learner("0.1c")
    learner.load_existing_tags_from_csv()
    learner.run()
