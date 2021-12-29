// **************************************************************************************
// State-Machine Füllstandsmessung
//
void messablauf() {
  switch (messSchritt) {
    case 0:  // Regelmäßig aktuellen atmosphärischen Druck erfassen
      if (!gv_motor_run) {
        atmDruck = messDruck;
      }
      break;

    case 1:  // Messung gestartet
      vergleichswert = messDruck;

      motor_start_all( );

      messung = millis() + 2000;
      messSchritt = 2;
      break;

    case 2:  // warten solange Druck steigt
      if (messDruck > vergleichswert + 10) {
        vergleichswert = messDruck;
        messung = millis() + 1000;
      }
      if (wassersaeule > (maxFuellhoehe + 200)) {
        Serial.println("Fehler: Messleitung verstopft!");
        messSchritt = 4;
      }
      else if (messung < millis()) {
        motor_stop_pumpe();
        messung = millis() + 100;
        messSchritt = 3;
      }
      break;

    case 3:  // Beruhigungszeit abgelaufen, Messwert ermitteln
      if (messung < millis()) {
        hoehe   = wassersaeule / 10;
        volumen = (wassersaeule / 10) * A / 100;
        // Umrechnung Wassersäule in 0 - 100%
        fuellstand = map(wassersaeule, 0, maxFuellhoehe, 0, 100);
        Serial.println( hoehe);
        //        Serial.println("Volumen: " + volumen);
        //        Serial.println("Füllstand: " + fuellstand);
        Serial.println();
        messSchritt = 4;
      }
      break;

    case 4:  // Ablauf beenden
      motor_stop_all();
      messSchritt = 0;
      break;

    default:
      messSchritt = 0;
      break;
  }
}
