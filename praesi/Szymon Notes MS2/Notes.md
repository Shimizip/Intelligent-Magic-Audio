Das Projekt das ich wir als Gruppe machen ist der Bau eines Moduls welcher Sampels Anhand seiner Spektogramme mit Hillfe von KI Anhand der vorgetaggten Merkmalsklassen sortiert.

Die Basis dafür ist ein F7 Microcontroller aus dem Hause STM.

Mein Part bei der Aufgabe ist der Baus des Interface sowie die Einbindung der SD Karte in das System damit daten gespeichert und ausgelesen werden können.

Das Interface besteht aus einem Encoder 2 Potentionmetern als Schieberegler und einem OLED-Display. Das ein binden des OLED war weniger mein Part ehr die Connection vom Encoder und dem OLED. 

Sie sind über ein Breadboard mit dem Controller verbunden. 

Mit dem Encoder kann man zwichen verschiedenen Dateien im Menü wählen auf dem Display und durch drücken des Push Buttons diese auswählen.  Durch das Drehen und Drücken des Encoders werden Interrupts ausgelöst welche den HAL_GPIO_EXTI_Callback auslösen. Es gibt einen Index der Zeigt wo man sich auf dem Bildschrim befindet. Falls der Encoder hochgedreht wird wird die Methode cursorUp(&fm); aufgerufen welche den Index des Displays erhöht und der Pfeil welcher dargestellt wird eine Zeile höher geht auf dem Display und somit ein neuer File anvisiert wird.  Durch das drehen in umgekehrte Richtung passiert das Gegenteil die Methode cursorDown(&fm) wird auf gerufen und der Pfeil geht runter. Durch das drücken des Push Buttons wird wird ebenfalls ein Interrupt ausgelöst der zur HAL_GPIO_EXTI_Callback führt. Dort wird die Methode renderSelectedFile() aufgerufen der File wird übernommen und unter der Liste erscheint dessen Name. Durch Debouncing soll sicher gestellt werden das der Encoder auch richtig funktioniert und durch Auswerten der Pins in HAL_GPIO_EXTI_Callback und if() abfragen auch das Richtigen Verhalten des Display sichergestellt wird. 

PINS: enc_a_clk_in1_Pin enc_b_dt_in2_Pin enc_switch_in3_Pin GND 1/2 

Die Übertragung auf eine größeren Bildschirm aufgrund der bisherigen Skalierung nicht möglich. 


Mit Hilfe vom ADC und DMA werden die Wiederstandswerte des Potentiometers umgewandelt und in Werte zwischen 0-4096 umgewandelt. Diese werden Cyklisch vom DMA in einen Array gespeichert durch den Aufruf von HAL_ADC_Start_DMA(&hadc1, currentValues, NUM_CHANNELS);
. Jede Position im Array entspricht einem derzeitgen Wert eines der Potentiometer. Diese Werte schwanken sehr desswegen wird eine Glättung mit 10 zwischen gespiecherten Werten vorgenommen welche. 10 Werte Pro Potentiometer .Diese 10 Werte werden separat verrechnet, um ein Mittewert für jedes der Potetiometer zu erhalten. Diese Endwerte dienen als Bedingungen. Sobald sich der Wert um +/- 41 verändert wird ein neuer Wert berechnet. Dieser Wert soll die Mittelwerte in Prozent darstellen und den alten Wert der vorhanden war ersetzen. Dieser alte Wert befindet sich in einem Array und entspricht ebenfalls der alten Prozentuallen darstellung der Wiederstands stärke nach der Glätung. Die Umrechung in Prozent erfolgt so: Mittelwert / 41 da die vom ADC Ausgelesenen Werte zwischen 0-4096 Varieren und durchteilen die Werte zwischen 1-100 rauskommen.

PINS: FADER_IN1_Pin AFADER_IN2_Pin VCC 1/2 GND 1/2

Die Glättung muss noch angepasst werden da die Wert immer noch schwanken und die Division liefert auch keine zufällesigen Werte.

Die SD Karte würde zunächst mit Hilfe von einem Extrenen Treiber Betrieben welcher die SD Karte über einen SD-Kartenreader über die SPI angesprochen hat. 

PINS: CS MISO MOSI VCC GND

Das Einbinden des Treibers ist erstmal schiefgegangen da wir nicht die richtigen SD karten hatten sondern nur MMC. Durchtesting verschiedener Karten und dem Debugger könnten wir rausfinden das wir nur Karten des Typs v1-v2 SSD benutzen können. Das Schreiben und Lesen hat zwar dann funktioniert allerdings war das ansprechen der Karte zu langsam für das Abspielen von Sounds, da nur kleine Buffer benutzt werden könnten. Durch Recherche habe ich rausgefunden das SDIO ein besserer Ansatz ist um die Übertraung der Sounds mit größeren Buffer möglich ist und zwar über 1 Bit Wide Bus. 

PINS: CD DI1 VCC GND 

Es funktioniert schin ganz gut allerdings wollen wir zukünfitg auf 4 Wide Bits umsteigen um die Effizien anzupassen.   



Testfälle Encoder in verbindung mit OLED
    Stromversorgung:

    Prüfen, ob der Encoder mit der richtigen Spannung und Stromstärke versorgt wird.
    Sicher stellen, dass die Stromversorgung stabil ist und keine Schwankungen auftreten.

    3.3 V sind optimal.
    
    Verbindung:

    Überprüfung der physikalische Verbindung zwischen dem Encoder und dem STM32-Mikrocontroller.
    Sicher stellen, dass alle Pins korrekt verbunden sind und keine Beschädigungen vorliegen.
   
    
    Grundfunktionalität:

    Prüfen Sie, ob der Encoder bei Drehung in beide Richtungen Impulse ausgibt,
    gegenprüfen mit dem Multimeter.
    Sicher stellen, dass beim richtungs drehen die richtigen Pins angesprochen werden(Verifiziere die Impulsabgabe des Encoders 
    bei Drehung in beide Richtungen mit einem Oszilloskop.) und mit Hilfe vom
    Debugger prüfen ob ein Interrupt ausgelöst wird und man in die richtige Callback reingeht. 
    
    Debouncing:

    Testen des Debouncing-Verhaltens des Encoders, indem Sie man Ihn schnell und langsam
    in beide Richtungen dreht und eine Zählvariable einbindet.
    Sicher stellen, dass keine Prellen oder Mehrfachauslösungen auftreten.
    Verifizieren Sie, dass der Encoder bei langsamem Drehen zuverlässig funktioniert in dem man
    Linear zu den Drehschritten die Zahlvariable vergleicht..
    
    Push-Button:

    Prüfen, ob der Push-Button beim Drücken einen Interrupt auslöst.
    Sicher stellen, dass der Interrupt sauber und eindeutig erkannt wird und die Pins .
    Verifizieren Sie, dass der Push-Button keine Prellen oder Mehrfachauslösungen erzeugt.
    Funktionale Testfälle
    
    Menünavigation:

    Testen Sie die Navigation im Menü mithilfe des Encoders.
    Sicher stellen, dass der Cursor sich bei Drehung des Encoders in beide Richtungen korrekt bewegt.
    
    Dateiauswahl:

    Wählen Sie mithilfe des Encoders verschiedene Dateien im Menü aus.
    Stellen Sie sicher, dass die ausgewählte Datei nach dem Drücken des Push-Buttons korrekt angezeigt wird.
    Verifizieren Sie, dass die Dateiauswahl durch Drehen des Encoders in die entgegengesetzte Richtung wieder rückgängig gemacht werden kann.
    
    Interrupt-Handling:

    Überprüfen, ob die Interrupt-Handler für den Encoder und den Push-Button korrekt ausgeführt werden.
    Sicher stellen, dass die richtigen Funktionen bei Interrupt-Auslösung aufgerufen werden.
    Verifizieren, dass die Interrupt-Handler die Programmlogik nicht beeinträchtigen.
    
    Debouncing im Kontext:

    Testen Sie das Debouncing-Verhalten des Encoders im Kontext der Dateiauswahl.
    Sicher stellen, dass die Dateiauswahl nicht durch Prellen oder Mehrfachauslösungen des Encoders beeinträchtigt wird.
    Verifizieren, dass die Dateiauswahl auch bei schnellem Drehen des Encoders zuverlässig funktioniert.
    

    
    Erfolgskriterien:

Alle Testfälle sollten erfolgreich abgeschlossen sein, um die Funktionalität des Encoder-Teils des Moduls zu gewährleisten. Das Modul sollte in der Lage sein, die  Drehbewegungen des Encoders korrekt zu erfassen und in Menünavigation und Dateiauswahl umzusetzen. Die Push-Button-Funktion sollte zuverlässig zum Auswählen von Dateien verwendet werden können. Debouncing sollte sicherstellen, dass die Encoder- und Push-Button-Eingaben sauber und eindeutig interpretiert werden.


Testverfahren Potentiometer:

Verbindung überprüfen:

Sicher stellen, dass die Schieberegler korrekt mit dem Microkontroller über die Pins verbunden ist, mit Hilfe von dem Multimeter,
wird geprüft ob der Kotroller Signale empfängt, über seine vorkonfigurierten Input Pins. Es wird ebenfalls geprüft ob der DMA die Werte ausliest und 
dann in die Callback reingeht, um diese weiter verarbeiten zu können.

ADC-Konfiguration überprüfen:

Sicher stellen, dass der ADC korrekt konfiguriert ist, um die Eingangssignale von den Schiebereglern zu erfassen.
Überprüfen Sie die Auflösung, die Sample-Rate und die Referenzspannung des ADCs.

DMA-Konfiguration überprüfen:

Sicher stellen, dass der DMA korrekt konfiguriert ist, um die ADC-Daten in ein Puffer zu übertragen.
Überprüfen Sie die Größe des Puffers, die Übertragungsrate und den Übertragungsmodus des DMAs.

Daten analysieren:

Analysieren Sie die aufgezeichneten ADC-Werte mit Hilfe von dem Debugger, 
um sicherzustellen, dass sie den Werten entsprechen, die von den Schiebereglern ausgegeben werden.
Überprüfen Sie, ob die ADC-Werte linear mit der Position der Schieberegler zunehmen.
Sicher stellen, dass die ADC-Werte keine unerwarteten Sprünge oder Schwankungen aufweisen.

Fehlerbehebung:

Wenn Sie Fehler bei der Datenerfassung oder -übertragung feststellen, beheben Sie die entsprechenden Konfigurationen oder Verbindungen.
Wiederholen Sie die Tests, bis die Schnittstelle korrekt funktioniert.





