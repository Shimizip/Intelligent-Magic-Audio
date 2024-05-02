Unsere 32GB MMC Sd Karte hat nicht funktioniert. 
Bei

```c
    if (!SPI_Timer_Status() || send_cmd(CMD16, 512) != 0)	/* Set block length: 512 */
        ty = 0;
```

ist die Karte auf uninitialized (`ty = 0`) gesetzt worden. Vermutlich ist das setzen der Blocklength auf 512 fehlgeschlagen (`send_cmd(CMD16, 512)`).