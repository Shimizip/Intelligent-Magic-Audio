import os
import shutil
import csv
from pydub.utils import mediainfo
from uuid import uuid4
import keyboard

from audio_player import AudioPlayer
player = AudioPlayer()

class FileInfo:
    def __init__(self):
        self.tags = {'Tag1': '', 'Tag2': '', 'Tag3': '', 'Tag4': ''}
        self.added_to_csv = False
        self.uid = None

def tag_audio_files(input_folder, output_folder, tags_csv):
    # Create output folder if it doesn't exist
    if not os.path.exists(output_folder):
        os.makedirs(output_folder)
    
    # Open CSV file for writing tags
    with open(tags_csv, 'w', newline='') as csvfile:
        fieldnames = ['UID', 'File', 'Tag1', 'Tag2', 'Tag3', 'Tag4']
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
        writer.writeheader()
    
    # Initialize a dictionary to keep track of file information
    file_info_dict = {}
    
    # Iterate through audio files in the input folder
    files = os.listdir(input_folder)
    files.sort()
    
    # Initialize FileInfo objects for each file name
    for file_name in files:
        if file_name.endswith('.mp3') or file_name.endswith('.wav') or file_name.endswith('.aif'):
            file_info_dict[file_name] = FileInfo()
    
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
                file_name = files[i]
                if file_name.endswith('.mp3') or file_name.endswith('.wav') or file_name.endswith('.aif'):
                    # Get file information from the dictionary
                    file_info = file_info_dict[file_name]
                    # Get tags and added status for the file
                    tags_str = ', '.join(f"{value}" for key, value in file_info.tags.items() if value)
                    added_status = "Added" if file_info.added_to_csv else ""
                    # Construct the line to display
                    line = f"  {file_name:<30} | Tags: {tags_str:<45} | Status: {added_status}"
                    # Highlight the selected file
                    if i == current_index:
                        print(f"> {line}")
                    else:
                        print(f"  {line}")
            redraw = False
        
        # Get selected file name
        key_event = keyboard.read_event()
        file_name = files[current_index]
        file_path = os.path.join(input_folder, file_name)
        
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
                file_info_dict[file_name].tags['Tag1'] = "melodic"
                redraw = True
            elif keyboard.is_pressed('2'):
                file_info_dict[file_name].tags['Tag2'] = "fartiy"
                redraw = True
            elif keyboard.is_pressed('3'):
                file_info_dict[file_name].tags['Tag3'] = "schlotzy"
                redraw = True
            elif keyboard.is_pressed('4'):
                file_info_dict[file_name].tags['Tag4'] = "schmoddrig"
                redraw = True
            elif keyboard.is_pressed('enter'):
                with open(tags_csv, 'a', newline='') as csvfile:
                    writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
                    # Generate unique ID for the file
                    uid = uuid4().hex
                    file_info_dict[file_name].uid = uid
                    # Copy and rename audio file to output folder with unique ID
                    output_file_path = os.path.join(output_folder, f"{uid}.wav")
                    shutil.copy2(file_path, output_file_path)
                    
                    print(f"File '{file_name}' tagged and copied to '{output_folder}' with UID '{uid}'")
                    # Save tags to CSV
                    writer.writerow({'UID': uid,
                                    'File': file_name,
                                    'Tag1': file_info_dict[file_name].tags['Tag1'],
                                    'Tag2': file_info_dict[file_name].tags['Tag2'],
                                    'Tag3': file_info_dict[file_name].tags['Tag3'],
                                    'Tag4': file_info_dict[file_name].tags['Tag4']})
                    print(f"Tags for file '{file_name}' saved to CSV.")
                    # Update the dictionary to mark the file as added to the CSV
                    file_info_dict[file_name].added_to_csv = True
                    redraw = True
            elif keyboard.is_pressed('delete'):
                # Check if file has been added to CSV
                if file_info_dict[file_name].added_to_csv:
                    # Remove file from CSV
                    uid = file_info_dict[file_name].uid
                    with open(tags_csv, 'r', newline='') as csvfile:
                        reader = csv.DictReader(csvfile)
                        rows = [row for row in reader if row['UID'] != uid]

                    with open(tags_csv, 'w', newline='') as csvfile:
                        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
                        writer.writeheader()
                        writer.writerows(rows)
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
