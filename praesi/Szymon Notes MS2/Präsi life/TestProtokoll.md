### Testprotokoll: Encoder in Verbindung mit OLED-Display

#### 1. Stromversorgung
- **Testfall:** Überprüfung der Stromversorgung des Encoders.
- **Schritte:**
  1. Schließen Sie das System an eine stabile Stromquelle an.
  2. Messen Sie die Spannung an den Versorgungsanschlüssen des Encoders.
- **Erwartete Werte:** 
  - Spannung: 3.3 V ± 5%.
  - Stromstärke: Im spezifizierten Bereich des Encoders.

#### 2. Verbindung
- **Testfall:** Überprüfung der physischen Verbindung zwischen Encoder und Mikrocontroller.
- **Schritte:**
  1. Prüfen Sie die Kabelverbindungen mit einem Multimeter.
  2. Sicherstellen, dass alle Pins korrekt verbunden sind.
- **Erwartete Werte:** 
  - Kontinuität und korrekte Verbindung an allen relevanten Pins.

#### 3. Grundfunktionalität
- **Testfall:** Prüfung der Impulsausgabe des Encoders bei Drehung.
- **Schritte:**
  1. Drehen Sie den Encoder langsam in beide Richtungen. 
  2. Überprüfen Sie die Impulsausgabe mit einem Oszilloskop.
  3. Debuggen Sie, um sicherzustellen, dass die Interrupts korrekt ausgelöst werden,.
- **Erwartete Werte:** 
  - Impulse werden bei Drehung in beide Richtungen zuverlässig erkannt.
  - Interrupts werden korrekt ausgelöst.
  

#### 4. Debouncing
- **Testfall:** Prüfung des Debouncing-Verhaltens.
- **Schritte:**
  1. Drehen Sie den Encoder schnell und langsam in beide Richtungen.
  2. Beobachten Sie die Zählvariable und prüfen Sie auf Prellen.
- **Erwartete Werte:** 
  - Keine Mehrfachauslösungen oder Prellen.
  - Zählvariable zählt wie erwatet hoch und runter.

#### 5. Push-Button
- **Testfall:** Prüfung der Funktionalität des Push-Buttons.
- **Schritte:**
  1. Drücken Sie den Push-Button.
  2. Überprüfen Sie, ob der Interrupt ausgelöst wird.
  3. `debounce` Variable wird wird gesetzt.
  4. Prüfen Sie auf Prellen.
- **Erwartete Werte:** 
  - Interrupt wird bei jedem Drücken sauber und eindeutig ausgelöst.
  - Keine Prellen oder Mehrfachauslösungen.
  - Variable wird wie erwartet auf True gesetzt und dannach auf False.

#### 6. Menünavigation
- **Testfall:** Testen der Navigation im Menü mithilfe des Encoders.
- **Schritte:**
  1. Drehen Sie den Encoder und beobachten Sie die Cursor-Bewegung.
  2. Drücken Sie den Push-Button zur Auswahl.
- **Erwartete Werte:** 
  - Cursor bewegt sich korrekt bei Drehung.
  - Ausgewählte Datei wird korrekt angezeigt.

### Testprotokoll: Potentiometer

#### 1. Verbindung überprüfen
- **Testfall:** Überprüfung der Verbindung zwischen Potentiometer und Mikrocontroller.
- **Schritte:**
  1. Prüfen Sie die Kabelverbindungen mit einem Multimeter.
  2. Sicherstellen, dass die Signale an den vorgesehenen Pins ankommen.
- **Erwartete Werte:** 
  - Kontinuität und korrekte Verbindung an allen relevanten Pins.

#### 2. ADC-Konfiguration überprüfen
- **Testfall:** Überprüfung der ADC-Konfiguration.
- **Schritte:**
  1. Überprüfen Sie die Auflösung, Sample-Rate und Referenzspannung des ADCs.
  2. Stellen Sie sicher, dass der ADC korrekt konfiguriert ist.
- **Erwartete Werte:** 
  - Auflösung: 12-bit (0-4096 Werte).
  - Sample-Rate und Referenzspannung entsprechen den Spezifikationen.

#### 3. DMA-Konfiguration überprüfen
- **Testfall:** Überprüfung der DMA-Konfiguration.
- **Schritte:**
  1. Stellen Sie sicher, dass der DMA die ADC-Daten in einen Puffer `currentValues` überträgt.
  2. Überprüfen Sie die Übertragungsart und Übertragungsrate.
- **Erwartete Werte:** 
  - Korrekte Konfiguration des DMA, WORD und Übertragungsmodus : Circular.

#### 4. Daten analysieren
- **Testfall:** Analyse der aufgezeichneten ADC-Werte.
- **Schritte:**
  1. Analysieren Sie die ADC-Werte `currentValues`, `previousADCValue`,`smoothedValues` und`classPercentADC`  mit einem Debugger.
  2. Prüfen Sie auf lineare Zunahme der Werte, entsprechen der Stellung des Schiebe Potentiometers.
- **Erwartete Werte:** 
  - ADC-Werte entsprechen den Potentiometer-Positionen.
  - Keine unerwarteten Sprünge oder Schwankungen.(Grundrauschen um wenige Volt normal)

#### 5. Glättung der Werte
- **Testfall:** Prüfung der Glättung der ADC-Werte.
- **Schritte:**
  1. Berechung des `smoothwertes`
    (Erfassen Sie 10 aufeinanderfolgende ADC-Werte.)
  (2. Berechnen Sie den Mittelwert und glätten Sie die Daten.)
  2. Prüfen Sie auf Schwankungen und plötzliche Änderungen.
- **Erwartete Werte:** 
  - Geglättete Werte zeigen eine stabile Wert entwicklung.
  - Schwankungen sind minimal und innerhalb akzeptabler Toleranzen.

#### 6. Fehlerbehebung
- **Testfall:** Behebung eventueller Fehler bei der Datenerfassung oder -übertragung.
- **Schritte:**
  1. Überprüfen Sie Konfigurationen und Verbindungen bei Fehlern.
  2. Wiederholen Sie die Tests nach Fehlerbehebung.
- **Erwartete Werte:** 
  - Alle Funktionen arbeiten fehlerfrei und wie erwartet.

### Zusammenfassung
- **Erfolgskriterien:**
  - Alle Testfälle werden erfolgreich abgeschlossen.
  - Das Modul reagiert korrekt auf Eingaben vom Encoder und Potentiometer.
  - Debouncing funktioniert zuverlässig.
  - Daten werden korrekt erfasst, geglättet und verarbeitet.

### Testzielsetzung

Dieser Test überprüft die Funktionalität der Lese- und Schreibfunktionen für die SD-Karte über die SPI-Schnittstelle.

### Testdurchführung

1. **Mounten des Dateisystems:**
    * Die Funktion `f_mount` wird aufgerufen, um das Dateisystem der SD-Karte zu mounten.
    * Der Rückgabewert (`fres`) wird überprüft. Im Fehlerfall wird eine Meldung ausgegeben und der Test abgebrochen.

2. **SD-Karten-Statistiken:**
    * Die Funktion `f_getfree` wird aufgerufen, um freie Cluster, freie Sektoren und Gesamtsektoren der SD-Karte zu ermitteln.
    * Im Fehlerfall wird eine Meldung ausgegeben und der Test abgebrochen.
    * Die Gesamt- und freien Speicherplatzwerte werden berechnet und über `myprintf` ausgegeben.

3. **Lesen einer Datei:**
    * Die Datei "test.txt" wird mit der Funktion `f_open` im Lesemodus geöffnet.
    * Der Rückgabewert (`fres`) wird überprüft. Im Fehlerfall wird eine Meldung ausgegeben und der Test abgebrochen.
    * Es wird versucht, 30 Bytes aus der Datei zu lesen (`f_gets`).
    * Gelingt das Lesen, wird der Inhalt der gelesenen Daten über `myprintf` ausgegeben.
    * Im Fehlerfall wird eine Meldung ausgegeben.
    * Die Datei wird mit `f_close` geschlossen.

4. **Schreiben einer Datei:**
    * Die Datei "write.txt" wird mit der Funktion `f_open` im Schreibmodus mit Flags zum Anlegen der Datei geöffnet.
    * Der Rückgabewert (`fres`) wird überprüft. Im Fehlerfall wird eine Meldung ausgegeben.
    * Ein String ("a new file is made!") wird in den Puffer `readBuf` kopiert.
    * Die Daten aus `readBuf` werden mit `f_write` in die Datei geschrieben.
    * Die Anzahl der geschriebenen Bytes wird über `myprintf` ausgegeben.
    * Im Fehlerfall wird eine Meldung ausgegeben.
    * Die Datei wird mit `f_close` geschlossen.

5. **Dismounten des Dateisystems:**
    * Die Funktion `f_mount` wird mit `NULL` aufgerufen, um das Dateisystem der SD-Karte zu dismounten.

### Auswertung

Der Test gilt als erfolgreich, wenn alle Schritte ohne Fehlermeldungen durchlaufen werden. Die ausgegebenen Informationen über Dateioperationen und Speicherplatz sollten plausibel sein.

### Anmerkungen

* Das vorgestellte Codebeispiel bietet grundlegende Tests für das Lesen und Schreiben von Dateien.
* Für eine umfassendere Prüfung sollten Tests mit unterschiedlichen Dateigrößen, Schreibzyklen und Dateitypen durchgeführt werden.
* Fehlerbehandlung und Robustheit des Codes können durch zusätzliche Testfälle mit simulierten Fehlern (z.B. volle SD-Karte) verbessert werden.

