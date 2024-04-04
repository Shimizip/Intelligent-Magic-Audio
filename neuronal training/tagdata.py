import os
import shutil
import csv
from pydub.utils import mediainfo
from uuid import uuid4
import keyboard

from audio_player import AudioPlayer
player = AudioPlayer()

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

def list_files_recursive(folder):
    file_list = []
    for root, dirs, files in os.walk(folder):
        for file in files:
            file_path = os.path.join(root, file)
            file_list.append(file_path)
    return file_list


def tag_audio_files(input_folder, output_folder, tags_csv):
    # Determine the version number
    current_version = "0.1c"
    version_folder = os.path.join("learndata", f"{current_version}")
    input_folder = os.path.join(version_folder, "input_folder")
    output_folder = os.path.join(version_folder, "output_folder")

    # Create the version directory and subdirectories
    if os.path.exists(version_folder):
        print(f"Error: Version folder '{version_folder}' already exists.")
    else:
        os.makedirs(version_folder)
        print(f"Version folder '{version_folder}' created.")
        os.makedirs(input_folder)
        os.makedirs(output_folder)
    tags_csv = (os.path.join(version_folder, tags_csv))
    # shutil.copy2(tags_csv, version_folder)
    
    # Open CSV file for writing tags
    with open(tags_csv, 'w', newline='') as csvfile:
        fieldnames = ['UID', 'File', tag1, tag2, tag3, tag4]
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
        writer.writeheader()
        
        # Initialize a dictionary to keep track of file information
        file_info_dict = {}
        
        # Iterate through audio files in the input folder
        if not os.listdir(input_folder):
            print("Fill input_folder with audio files!")
            return
        files = list_files_recursive(input_folder)    
        # files = os.listdir(input_folder)
        files.sort()
        
        # Initialize FileInfo objects for each file name
        for file_path in files:
            if file_path.endswith('.mp3') or file_path.endswith('.wav') or file_path.endswith('.aif'):
                file_name = os.path.basename(file_path)
                file_info_dict[file_path] = FileInfo(file_path, file_name)
        
        current_index = 0
        start_index = 0  # Index of the first visible file
        end_index = min(len(files), os.get_terminal_size().lines - 4)  # Index of the last visible file
        redraw = True  # Flag to determine if console needs to be redrawn
        
        while True:
            if redraw:
                os.system('cls' if os.name == 'nt' else 'clear')
                
                # Display list of files
                print("Audio Files:")
                for i in range(start_index, end_index):
                    file_path = list(file_info_dict.keys())[i]
                    if file_path.endswith('.mp3') or file_path.endswith('.wav') or file_path.endswith('.aif'):
                        # Get file information from the dictionary
                        file_info = file_info_dict[file_path]
                        # Get tags and added status for the file
                        tags_str = ', '.join(key for key, value in file_info.tags.items() if value == 1)
                        added_status = "Added" if file_info.added_to_csv else ""
                        # Construct the line to display
                        line = f"  {file_info.file_name:<30} | Tags: {tags_str:<45} | Status: {added_status}"
                        # Highlight the selected file
                        if i == current_index:
                            print(f"> {line}")
                        else:
                            print(f"  {line}")
                redraw = False
            
            # Get selected file name
            key_event = keyboard.read_event()
            file_path = list(file_info_dict.keys())[current_index]
            
            if key_event.event_type == keyboard.KEY_DOWN:    
                if keyboard.is_pressed('down'):
                    current_index = min(current_index + 1, len(files) - 1)
                    if current_index >= end_index:
                        start_index += 1
                        end_index += 1
                    redraw = True
                elif keyboard.is_pressed('up'):
                    current_index = max(current_index - 1, 0)
                    if current_index < start_index:
                        start_index -= 1
                        end_index -= 1
                    redraw = True
                elif keyboard.is_pressed('space'):
                    player.load_audio(file_path)
                    player.play()  # Start playback
                    # Play the audio file (add your playback logic here)
                    pass
                elif keyboard.is_pressed('1'):
                    if file_info_dict[file_path].tags[tag1] == 1:
                        file_info_dict[file_path].tags[tag1] = 0
                    else:
                        file_info_dict[file_path].tags[tag1] = 1
                    redraw = True
                elif keyboard.is_pressed('2'):
                    if file_info_dict[file_path].tags[tag2] == 1:
                        file_info_dict[file_path].tags[tag2] = 0
                    else:
                        file_info_dict[file_path].tags[tag2] = 1
                    redraw = True
                elif keyboard.is_pressed('3'):
                    if file_info_dict[file_path].tags[tag3] == 1:
                        file_info_dict[file_path].tags[tag3] = 0
                    else:
                        file_info_dict[file_path].tags[tag3] = 1
                    redraw = True
                elif keyboard.is_pressed('4'):
                    if file_info_dict[file_path].tags[tag4] == 1:
                        file_info_dict[file_path].tags[tag4] = 0
                    else:
                        file_info_dict[file_path].tags[tag4] = 1
                    redraw = True

                elif keyboard.is_pressed('enter'):
                    # Generate unique ID for the file
                    uid = uuid4().hex
                    file_info_dict[file_path].uid = uid
                    # Copy and rename audio file to output folder with unique ID
                    output_file_path = os.path.join(output_folder, f"{uid}.wav")
                    shutil.copy2(file_path, output_file_path)
                    
                    print(f"File '{file_name}' tagged and copied to '{output_folder}' with UID '{uid}'")
                    # Save tags to CSV
                    writer.writerow({'UID': uid,
                                    'File': file_path,
                                    tag1: file_info_dict[file_path].tags[tag1],
                                    tag2: file_info_dict[file_path].tags[tag2],
                                    tag3: file_info_dict[file_path].tags[tag3],
                                    tag4: file_info_dict[file_path].tags[tag4]})
                    print(f"Tags for file '{file_name}' saved to CSV.")
                    # Update the dictionary to mark the file as added to the CSV
                    file_info_dict[file_path].added_to_csv = True
                    redraw = True
                elif keyboard.is_pressed('delete'):
                    # Check if file has been added to CSV
                    if file_info_dict[file_path].added_to_csv:
                        # Remove file from CSV
                        uid = file_info_dict[file_name].uid
                        writer.writerow({'UID': uid, 'File': file_name})
                        print(f"File '{file_name}' removed from CSV.")
                        # Delete copied file
                        copied_file_path = os.path.join(output_folder, f"{uid}.wav")
                        os.remove(copied_file_path)
                        print(f"Copied file '{copied_file_path}' deleted.")
                        # Reset file info
                        file_info_dict[file_name] = FileInfo()
                        redraw = True
                    else:
                        print(f"File '{file_name}' has not been added to CSV yet.")

# Example usage
input_folder = 'input_folder'
output_folder = 'output_folder'
tags_csv = 'tags.csv'
tag_audio_files(input_folder, output_folder, tags_csv)
