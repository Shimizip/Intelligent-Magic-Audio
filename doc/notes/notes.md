- elders-boll fragen bzgl anwendungen für arm7
- user feedback schleife? -> Entwickelt sich das NN aus neu eingefügten Samples weiter

gops/watt ?

STM32CubeAI mit Cortex M4/M7
-> NUCLEO-F722ZE
https://eu.mouser.com/ProductDetail/STMicroelectronics/NUCLEO-F722ZE?qs=DXv0QSHKF4wVcMHJD76qIg%3D%3D

- Brauchen Leistungsfähigere MCU für Bildklassifizierung, deshalb M7

AI Komponente
Funktinalität:
	1. Audiodatei wird in Spektrogramm umgewandelt
	2. Neuronales Netz analysiert Spektrogramm und tagg entsprechend Dateien auf SD Karte


## Hardware Komponenten
- Wifi Modul für Webserver (File Transfer)
- SD Kartenmodul für Dateiablage (Hält auch Klassifizierungsdatenbank)
- (evtl extra AD/DA Wandler Audio ein/ausgabe)
- LCD Display
- 2 Encoder 
- 4 Fader mit denen man Anteil der Klassen festlegen kann (z.B. https://www.mouser.de/ProductDetail/Bourns/PTL45-15R0-103B2?qs=XxajcJZMfNwybdu%2Fz5bMWA%3D%3D)
- Play Knopf

### Klassifizierung
1. Klassen festlegen (Auch Anzahl der Klassen)
2. Sample Library händisch klassifizieren
3. Neuronales Netz mit SampleLibrary trainieren
4. 

### Klassen

- Dauer (Oneshots oder längerer Sound)
- Klangfarbe (Frequenzanteile)
- Stimmung (Unsettling/Disharmonisch - Harmonisch)
- Tonhöhe/Pitch
- Organisch - Synthetisch
- Perkussiv - Flächig

### Funktion

Das Modul ist ein Audioplayback(/Recorder) mit AI gestützter Sampleklassifizierung.
Es können Module per Audio Input aufgenommen, oder per Webserver auf einer SD Karte gespeichert werden.
Nach dem Recorden/Dateitransfer auf die SD Karte, analisiert das Neuronale Netz das Sample, und speichert die anteilige Klassifizierung in einer simplen dateibasierten Datenbank (.csv o.ä.).


## Ziel

### Shortterm
- Audioplayer
- AI gestützte Sampleklassifizierung


### Longterm
- Eurorack fähiges Modul

## TODO

- Elders-Boll fragen wegen Nucleo F7 Board
- Krawutschke fragen: 
    - wegen F4 Boards
    - LCD Display
    - 2 Encoder
    - 4 Analoge Fader mit LEDs (z.B. https://www.mouser.de/ProductDetail/Bourns/PTL45-15R0-103B2?qs=XxajcJZMfNwybdu%2Fz5bMWA%3D%3D)
    - 3.5mm Klinkenbuchsen (Stereo)


## Zuständigkeiten

### Szymon
- Interfacing Specialist
    - Encoder
    - LCD
- Dateisystem für Nutzdaten
    - SD-Card Read/Write

### Jonas
- Audio Engine Engineer
    - Sample Record
    - Sample Playback
    - Analoge Fader
- LearnData Generation (Aufbereitung der Lerndaten)
- Integration Eurorack Standard (Elektronisch)
- Hardware Design

### Leon
- Neuronal Navigator
    - Neuronales Netz Topologie entwickeln
    - NN Trainieren und Anpassen
    - Portierung für STM32CubeAI
    - Impl Spektrogramm aus Audio Datei erstellen
- LED Master

## Für Präsi
- Modul Designen (Faceplate)
- Mockup für Fileupload-Web-Applikation
- Grobe Darstellung des UX/technischen Programmablauf des Eingebetten Systems


## Grober Plan für AI Engineering

1. Toolchain Testen
    - Learndaten erzeugen 
    - Grober Entwurf neuronales Netz
    - Auf STM32CubeAI portieren und Performance Test
2. Neuronales Netz Erzeugen
3. Training

## Für STM32CubeAI Testing
- Performance Evaluation Program für STM32 entwerfen
