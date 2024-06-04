### Projektübersicht: KI-basierte Sortierung von Samples nach Spektrogrammen

#### Basis:
- **Microcontroller:** STM32F7-Serie

#### Aufgabe:
- **Ziel:** Bau eines Moduls zur Sortierung von Samples anhand von Spektrogrammen unter Verwendung von KI und vorgetaggten Merkmalsklassen.
- **Mein Verantwortungsbereich:** Entwicklung des Interface und Integration der SD-Karte für Datenspeicherung und -auslesung.

### Interface

#### Komponenten:
1. **Encoder**
2. **Zwei Potentiometer als Schieberegler**
3. **OLED-Display**

#### Verbindung:
- Komponenten sind über ein Breadboard mit dem Mikrocontroller verbunden.

#### Funktionalität:
- **Encoder:**
  - Navigiert zwischen verschiedenen Dateien im Menü auf dem OLED-Display.
  - Durch Drehen und Drücken des Encoders werden Interrupts ausgelöst, welche die Funktion `HAL_GPIO_EXTI_Callback` aktivieren.
  - **Navigation:**
    - Drehung nach oben: `cursorUp(&fm)` - Erhöht den Index, der die Position des Cursors auf dem Display angibt.
    - Drehung nach unten: `cursorDown(&fm)` - Verringert den Index, der die Position des Cursors auf dem Display angibt.
  - **Auswahl:**
    - Drücken des Push Buttons löst `HAL_GPIO_EXTI_Callback` aus und ruft `selectFile(&fm);` auf, um die Datei auszuwählen.
  - **Debouncing:** Sorgt für die korrekte Funktion des Encoders durch Auswerten der Pins in `HAL_GPIO_EXTI_Callback` und if()-Abfragen mit Hilfe von einem Timer und einer boolichen debounce Variable.

#### Herausforderungen und Lösungen:
- **Herausforderungen:**
  - Fehlende Stabilität in der Stromversorgung(Sitzt nicht richtig im Breadboard) und mechanische Prellungen beim Drehen des Encoders.
  - Unerwünschte Mehrfachauslösungen bei schnellen und langsamen Bewegungen des Encoders.
  -Integration des Encoder mit dem Bildschirm
  -Umskalieren des Bildschirms stellt sich als schwierig dar. 

- **Lösungen:**
  - Sicherstellung der stabilen 3.3V-Stromversorgung umstieg auf PLatine.
  - Einsatz eines geeigneten Entprellungs-Algorithmus und Anpassung der Interrupt-Auslöser.
  - Einbindung von Methoden in `HAL_GPIO_EXTI_Callback` die das Verhalten des Bildschirm beeinflussen durch bewegen des Encoders. 
  -Anpassung der display.c Klasse notwendig

#### Pins:
- **Encoder:**
  - `enc_a_clk_in1_Pin`
  - `enc_b_dt_in2_Pin`
  - `enc_switch_in3_Pin`
  - `GND 1/2`

### Potentiometer

#### Funktionalität:
- **ADC und DMA:**
  - Wandeln die Widerstandswerte der Potentiometer in Werte zwischen 0-4096 um.
  - Werte werden zyklisch vom DMA in einem Array gespeichert durch `HAL_ADC_Start_DMA(&hadc1, currentValues, NUM_CHANNELS)`.
  - **Glättung:** smoothedValues[i] = (uint32_t)(ALPHA * currentValues[i] + (1 - ALPHA) * smoothedValues[i]);
    Berechnung gibt Stabilerewerte.
  - **Bedingungen:** Bei einer Änderung von +/- 41 wird ein neuer Wert berechnet. Dieser wird als der aktuelle Abgespeichert.
  - **Umrechnung in Prozent:** ` smoothedValues[i] / 41`, um Werte zwischen 1-100 zu erhalten.

#### Herausforderungen und Lösungen:
- **Herausforderungen:**
  - Schwankende Werte und Rauschen in den ADC-Ausgaben.
  - Unzuverlässige Mittelwertberechnung aufgrund unzureichender Glättung.

- **Lösungen:**
  - Implementierung einer fortschrittlicheren Glättungstechnik mit größerem Mittelwert-Fenster.
  - Anpassung der ADC-Referenzspannung und Sampling-Rate zur Reduzierung von Rauschen.

#### Pins:
- **Potentiometer:**
  - `FADER_IN1_Pin`
  - `AFADER_IN2_Pin`
  - `VCC 1/2`
  - `GND 1/2`

### SD-Kartenintegration

#### Initiale Implementierung:
- **Externer Treiber:**
  - Ansteuerung der SD-Karte über einen SD-Kartenleser via SPI.
  - Ausfühung eines Testprotokolls.

#### Pins:
- **SPI:**
  - `CS`
  - `MISO`
  - `MOSI`
  - `VCC`
  - `GND`

#### Herausforderungen und Lösungen:
- **Herausforderungen:**
  - Verwendung falscher SD-Karten (MMC) führte zu Schwierigkeiten beim Einbinden des Treibers.
  - Kompatibilität nur mit SD-Karten der Typen v1-v2 SSD.
  - Langsame Schreib- und Leseoperationen, ungeeignet für Soundwiedergabe aufgrund kleiner Puffergrößen.

- **Lösungen:**
  - Wechsel zu SD-Karten der Typen v1-v2 SSD nach Debugging und Tests.
  - Verbesserung der Performance durch Wechsel zu SDIO für effizientere Datenübertragung mit größeren Puffern.

#### Verbesserung:
- **SDIO Ansatz:**
  - Verwendet 1-Bit Wide Bus für effizientere Datenübertragung mit größeren Buffern.

#### Pins:
- **SDIO:**
  - `CD`
  - `DI1`
  - `VCC`
  - `GND`

#### Zukunftspläne:
- Umstieg auf 4-Bit Wide Bus zur weiteren Verbesserung der Effizienz und Datenübertragungsrate.
