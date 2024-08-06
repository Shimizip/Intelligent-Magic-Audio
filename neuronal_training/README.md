# Audio File Tagger

This Python script is used to generate learning data for a neuronal network, which classifies audio in 4 classes.

It allows users to tag audio files and copy them to an output folder with a unique ID.
A .csv is generated, which saves all the tags with their corresponding uids.

## Features

- Tag audio files with customizable tags.
- Play audio files for preview.
- Copy tagged audio files to an output folder with a unique ID.
- Save tags to a CSV file.

## Dependencies

- `pydub` for audio file handling.
- `keyboard` for keyboard input detection.
- `pyaudio` for audio playback

## Usage

1. Ensure you have Python installed on your system.
2. Install the required dependencies using pip:

`pip install pydub keyboard`


3. Place your audio files in the `input_folder`.
4. Run the script with the following command:

`python audio_tagger.py`


5. Follow the on-screen instructions to tag the audio files.
6. Tagged files will be copied to the `output_folder` with unique IDs and their tags will be saved to `tags.csv`.

## Keyboard Shortcuts

- **Spacebar**: Play selected audio file.
- **1-4**: Toggle tags (corresponding to custom tags).
- **Enter**: Tag the selected file and copy it to the output folder.
- **Delete**: Remove tagged file from CSV and delete copied file.

## Author

Created by [Your Name].
