Das Projekt, das wir als Gruppe machen, ist der Bau eines Moduls, welches Samples anhand ihrer Spektrogramme mithilfe von KI anhand der vorgetaggten Merkmalsklassen sortiert.

Die Basis dafür ist ein F7-Mikrocontroller aus dem Hause STM.

Mein Teil der Aufgabe ist der Bau des Interfaces sowie die Einbindung der SD-Karte in das System, damit Daten gespeichert und ausgelesen werden können.

Das Interface besteht aus einem Encoder, zwei Potentiometern als Schieberegler und einem OLED-Display. Das Einbinden des OLED-Displays war weniger mein Part, eher die Verbindung vom Encoder und dem OLED.

Sie sind über ein Breadboard mit dem Controller verbunden.

Mit dem Encoder kann man zwischen verschiedenen Dateien im Menü wählen auf dem Display und durch Drücken des Push-Buttons diese auswählen. Durch das Drehen und Drücken des Encoders werden Interrupts ausgelöst, welche den HAL_GPIO_EXTI_Callback auslösen. Es gibt einen Index, der zeigt, wo man sich auf dem Bildschirm befindet. Falls der Encoder hochgedreht wird, wird die Methode cursorUp(&fm); aufgerufen, welche den Index des Displays erhöht und der Pfeil, welcher dargestellt wird, eine Zeile höher geht auf dem Display und somit eine neue Datei anvisiert wird. Durch das Drehen in umgekehrte Richtung passiert das Gegenteil; die Methode cursorDown(&fm) wird aufgerufen und der Pfeil geht runter. Durch das Drücken des Push-Buttons wird ebenfalls ein Interrupt ausgelöst, der zur HAL_GPIO_EXTI_Callback führt. Dort wird die Methode selectFile() aufgerufen, die Datei wird übernommen und unter der Liste erscheint deren Name. Durch Debouncing soll sichergestellt werden, dass der Encoder auch richtig funktioniert und durch Auswerten der Pins in HAL_GPIO_EXTI_Callback und if()-Abfragen auch das richtige Verhalten des Displays sichergestellt wird.

PINS: enc_a_clk_in1_Pin, enc_b_dt_in2_Pin, enc_switch_in3_Pin, GND 1/2

Die Übertragung auf einen größeren Bildschirm ist aufgrund der bisherigen Skalierung nicht möglich.

Mit Hilfe des ADC und DMA werden die Widerstandswerte des Potentiometers umgewandelt und in Werte zwischen 0-4096 umgewandelt. Diese werden zyklisch vom DMA in einem Array gespeichert durch den Aufruf von HAL_ADC_Start_DMA(&hadc1, currentValues, NUM_CHANNELS);. Jede Position im Array entspricht einem derzeitigen Wert eines der Potentiometer. Diese Werte schwanken sehr, deswegen wird eine Glättung mit 10 zwischengespeicherten Werten vorgenommen. 10 Werte pro Potentiometer. Diese 10 Werte werden separat verrechnet, um einen Mittelwert für jedes der Potentiometer zu erhalten. Diese Endwerte dienen als Bedingungen. Sobald sich der Wert um +/- 41 verändert, wird ein neuer Wert berechnet. Dieser Wert soll die Mittelwerte in Prozent darstellen und den alten Wert, der vorhanden war, ersetzen. Dieser alte Wert befindet sich in einem Array und entspricht ebenfalls der alten prozentualen Darstellung der Widerstandsstärke nach der Glättung. Die Umrechnung in Prozent erfolgt so: Mittelwert / 41, da die vom ADC ausgelesenen Werte zwischen 0-4096 variieren und durch das Teilen die Werte zwischen 1-100 herauskommen.

PINS: FADER_IN1_Pin, FADER_IN2_Pin, VCC 1/2, GND 1/2

Die Glättung muss noch angepasst werden, da die Werte immer noch schwanken und die Division liefert auch keine zufälligen Werte.

Die SD-Karte würde zunächst mithilfe eines externen Treibers betrieben, welcher die SD-Karte über einen SD-Kartenleser über die SPI ansprach.

PINS: CS, MISO, MOSI, VCC, GND

Das Einbinden des Treibers ist erst einmal schiefgegangen, da wir nicht die richtigen SD-Karten hatten, sondern nur MMC. Durch Testen verschiedener Karten und den Debugger konnten wir herausfinden, dass wir nur Karten des Typs v1-v2 SSD benutzen können. Das Schreiben und Lesen hat zwar dann funktioniert, allerdings war das Ansprechen der Karte zu langsam für das Abspielen von Sounds, da nur kleine Buffer benutzt werden konnten. Durch Recherche habe ich herausgefunden, dass SDIO ein besserer Ansatz ist, um die Übertragung der Sounds mit größeren Buffern möglich zu machen, und zwar über 1-Bit-Wide-Bus.

PINS: CD, DI1, VCC, GND

Es funktioniert schon ganz gut, allerdings wollen wir zukünftig auf 4 Wide Bits umsteigen, um die Effizienz zu verbessern.

Testfälle Encoder in Verbindung mit OLED:

1. **Stromversorgung:**
    - Prüfen, ob der Encoder mit der richtigen Spannung und Stromstärke versorgt wird.
    - Sicherstellen, dass die Stromversorgung stabil ist und keine Schwankungen auftreten.
    - 3.3 V sind optimal.

2. **Verbindung:**
    - Überprüfung der physischen Verbindung zwischen dem Encoder und dem STM32-Mikrocontroller.
    - Sicherstellen, dass alle Pins korrekt verbunden sind und keine Beschädigungen vorliegen.

3. **Grundfunktionalität:**
    - Prüfen, ob der Encoder bei Drehung in beide Richtungen Impulse ausgibt, gegenprüfen mit dem Multimeter.
    - Sicherstellen, dass beim Drehen in die richtige Richtung die richtigen Pins angesprochen werden (Verifizierung der Impulsabgabe des Encoders bei Drehung in beide Richtungen mit einem Oszilloskop) und mit Hilfe des Debuggers prüfen, ob ein Interrupt ausgelöst wird und man in die richtige Callback-Funktion gelangt.

4. **Debouncing:**
    - Testen des Debouncing-Verhaltens des Encoders, indem man ihn schnell und langsam in beide Richtungen dreht und eine Zählvariable einbindet.
    - Sicherstellen, dass keine Prellen oder Mehrfachauslösungen auftreten.
    - Verifizieren, dass der Encoder bei langsamem Drehen zuverlässig funktioniert, indem man linear zu den Drehschritten die Zählvariable vergleicht.

5. **Push-Button:**
    - Prüfen, ob der Push-Button beim Drücken einen Interrupt auslöst.
    - Sicherstellen, dass der Interrupt sauber und eindeutig erkannt wird und die Pins.
    - Verifizieren, dass der Push-Button keine Prellen oder Mehrfachauslösungen erzeugt.

**Funktionale Testfälle:**

1. **Menünavigation:**
    - Testen der Navigation im Menü mithilfe des Encoders.
    - Sicherstellen, dass der Cursor sich bei Drehung des Encoders in beide Richtungen korrekt bewegt.

2. **Dateiauswahl:**
    - Mithilfe des Encoders verschiedene Dateien im Menü auswählen.
    - Sicherstellen, dass die ausgewählte Datei nach dem Drücken des Push-Buttons korrekt angezeigt wird.
    - Verifizieren, dass die Dateiauswahl durch Drehen des Encoders in die entgegengesetzte Richtung wieder rückgängig gemacht werden kann.

3. **Interrupt-Handling:**
    - Überprüfen, ob die Interrupt-Handler für den Encoder und den Push-Button korrekt ausgeführt werden.
    - Sicherstellen, dass die richtigen Funktionen bei Interrupt-Auslösung aufgerufen werden.
    - Verifizieren, dass die Interrupt-Handler die Programmlogik nicht beeinträchtigen.

4. **Debouncing im Kontext:**
    - Testen des Debouncing-Verhaltens des Encoders im Kontext der Dateiauswahl.
    - Sicherstellen, dass die Dateiauswahl nicht durch Prellen oder Mehrfachauslösungen des Encoders beeinträchtigt wird.
    - Verifizieren, dass die Dateiauswahl auch bei schnellem Drehen des Encoders zuverlässig funktioniert.

**Erfolgskriterien:**

Alle Testfälle sollten erfolgreich abgeschlossen sein, um die Funktionalität des Encoder-Teils des Moduls zu gewährleisten. Das Modul sollte in der Lage sein, die Drehbewegungen des Encoders korrekt zu erfassen und in Menünavigation und Dateiauswahl umzusetzen. Die Push-Button-Funktion sollte zuverlässig zum Auswählen von Dateien verwendet werden können. Debouncing sollte sicherstellen, dass die Encoder- und Push-Button-Eingaben sauber und eindeutig interpretiert werden.

**Testverfahren Potentiometer:**

1. **Verbindung überprüfen:**
    - Sicherstellen, dass die Schieberegler korrekt mit dem Mikrocontroller über die Pins verbunden sind. Mit Hilfe des Multimeters prüfen, ob der Controller Signale empfängt, über seine vorkonfigurierten Input-Pins. Es wird ebenfalls geprüft, ob der DMA die Werte ausliest und dann in die Callback-Funktion gelangt, um diese weiter verarbeiten zu können.

2. **ADC-Konfiguration überprüfen:**
    - Sicherstellen, dass der ADC korrekt konfiguriert ist, um die Eingangssignale von den Schiebereglern zu erfassen.
    - Überprüfen der Auflösung, der Sample-Rate und der Referenzspannung des ADCs.

3. **DMA-Konfiguration überprüfen:**
    - Sicherstellen, dass der DMA korrekt konfiguriert ist, um die ADC-Daten in einen Puffer zu übertragen.
    - Überprüfen der Größe des Puffers, der Übertragungsrate und des Übertragungsmodus des DMAs.

4. **Daten analysieren:**
    - Analysieren der aufgezeichneten ADC-Werte mithilfe des Debuggers, um sicherzustellen, dass sie den Werten entsprechen, die von den Schiebereglern ausgegeben werden.
    - Überprüfen, ob die ADC-Werte linear mit der Position der Schieberegler zunehmen.
    - Sicherstellen, dass die ADC-Werte keine unerwarteten Sprünge oder Schwankungen aufweisen.



5. **Fehlerbehebung:**
    - Falls Fehler bei der Datenerfassung oder -übertragung festgestellt werden, entsprechende Konfigurationen oder Verbindungen beheben.
    - Wiederholen der Tests, bis die Schnittstelle korrekt funktioniert.