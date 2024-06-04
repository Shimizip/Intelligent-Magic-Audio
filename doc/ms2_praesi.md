# MS2 Präsi

## Szymon
	- Fader
	- encoder
	- SDCard - SPI


## jonas

### Hardware
#### SDIO (Schnittstelle Erklärung)
    - 

#### Display

#### PCM5102A Audio Codec 
    - agiert als DAC
    - Speziell für PCM (Pulse-code modulation)
        - Methode zur digitalen Represäntation von analogen Signalen
        - Amplitude zu gleichen Zeitabschnitten Samplen (Samplingfrequenz - bei uns 44.100)
        - Quantisierung der gesampleten Werte (in unserem Fall 16 Bit)
    - Nutzt I2S Schnittstelle
        - unidirektionale Übertragung von Audiodaten
        - 3 Clocks (SCK, LCK, BCK, ) 
            - SCK: Systemclock - 45MHz
            - LCK/WS: Wordclock - 44.1kHz * 16Bit * 2 - zeig linken/rechten Kanal an
            - BCK: Bitclock - 44.1kHz * 16Bit - Audiosample Clock
        - Serielle Daten DIN (L und R Kanal abwechselnd)
        - Spannungsversorgung VIN, GND
        - STM32 bietet native Treiber und Konfiguration über CubeMX

#### DMA Double Buffering
    - Circular DMA (Direct Memory Access) -> ohne CPU kann Speicher an eine Peripherie gesendet werden
        - wird circular automatisch immer wiederholt

    - Double Buffering für performantes Signal Prozessing
        - Buffer, welcher zweigeteilt ist.
        - DMA liest aus der einen Hälfte des Buffers
        - C-Programm Schreibt Samples in die andere Hälfte des Buffers
            - 1. Samples von SD Karte lesen
            - 2. Eventuelles Signal Prozessing, wie Pitching etc.
        - Groeße des Buffers bestimmt CPU Auslastung und Latenz
            - Je groeßer der Buffer, desto geringer die CPU Auslastung (CPU hat mehr Zeit zum bearbeiten der zu sendenden Daten)
            - aber desto hoeher die Latenz (Mehr Zeit vergeht vom Drücken des Play Knopfes - bis zum Spielen des Audios -> schlecht für Musikinstrument)
        - Angepeilte Latenz -> unter 10ms
            -> Buffer_Size/SampleRate = Latenz
            <=> 256/44.100 = 5.8ms

#### WAV PCM
    - Dateiformat zur Speicherung von PCM Daten
    - 44 Byte Header
        - Verschiedene Metadaten werden darin gespeichert, wie bspw:
        - Anzahl der Datenbytes
        - Anzahl der Kanäle
        - Samplerate der Datei
        - Quantisierung der Datei
    - Header Metadaten koennen vor Abspielen des Samples ausgelesen werden, und Algorithmus zum Abspielen dann dynamisch angepasst
        - (bspw. Änderung der Samplerate von I2S)

### Probleme
    - Immer noch hoerbare Audio Knackser (Grund ist *wahrscheinlich* nicht die CPU Auslastung!) (sind nach dem letzten Codefix besser geworden)
    - Buffersize im Moment nicht unter 256 Samples - Sobald auf F7 Board ist jedoch 4Bit SDIO Connection moeglich, sodass die Buffersize durch schnellere Zugriffszeiten verkleinert werden kann -> geringere Latenz
        - 256 ist schon nicht schlecht in Bezug auf Echtzeitfähigkeit
    - Sample Playback ist leicht schneller als das Original (Screenshot Zeigen)
    
### TODO
    - Nebenläufigkeiten: 
        1. Audio Playback (evtl auch Pitching)
        2. Interfacing (Display, Encoder)
        3. Suchfilter durch Fadereinstellungen ?

    - Verheiraten der Interfacing und Audio Komponenten
    - dann Umzug auf F7 Board und Heirat mit NN
    - Implementierung der finalen Dateistruktur/Objektstruktur (Audioplayer, Filemanager, Fadermanager, Framebuffer/File List)
    - Audio Wave Display/Oszilloskop?
    - 

## für präsi
	- evtl 2 Teilprojekte vorstellen 
		1. Display, Encoder, Fader
		2. SDCard, Audio Codec

    - F4 Board für "Interface und Sound Funktionalität"

    - Umzug auf F7 Board, sobald neuronales Netz versprechende Ergebnisse liefert

    - 