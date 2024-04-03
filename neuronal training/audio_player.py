# audio_player.py
import os
import wave
import pyaudio

class AudioPlayer:
    def __init__(self):
        self.audio = None
        self.playing = False
        self.temp_dir = os.path.abspath("./temp")  # Specify a temporary directory
        if not os.path.exists(self.temp_dir):
            os.makedirs(self.temp_dir)
        self.pyaudio_instance = pyaudio.PyAudio()

    def load_audio(self, file_path):
        self.audio = wave.open(file_path, 'rb')

    def play(self):
        if self.audio and not self.playing:
            self.playing = True
            stream = self.pyaudio_instance.open(
                format=self.pyaudio_instance.get_format_from_width(self.audio.getsampwidth()),
                channels=self.audio.getnchannels(),
                rate=self.audio.getframerate(),
                output=True
            )
            data = self.audio.readframes(1024)
            while data:
                stream.write(data)
                data = self.audio.readframes(1024)
            stream.stop_stream()
            self.playing = False
            stream.close()

    def pause(self):
        # Not supported in this implementation
        pass

    def stop(self):
        if self.playing:
            self.playing = False
            self.audio.rewind()
