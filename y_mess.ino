// **************************************************************************************
// State-Machine Füllstandsmessung
//
void messablauf() {

  if ( !gv_mprls_ok) {
    return;
  }

  // Alle 10 ms Sensorwert auslesen
  if (messTakt < millis()) {
    check_pressure(false);

    if (messSchritt == 0) {
      atmDruck = messDruck;
    }

    // Umrechnung Pa in mmH2O
    wassersaeule = (messDruck - atmDruck) * 10197 / 100000;
    // Korrektur um Druck durch Schlauchlänge
    wassersaeule = wassersaeule - gv_wassersaeule_kalib;
    if (wassersaeule < 0)
      wassersaeule = 0;
    //Serial.print("Wassersäule: "); Serial.println(wassersaeule);

    messTakt = millis() + 10;
  }


  switch (messSchritt) {
    case 0:  // Regelmäßig aktuellen atmosphärischen Druck erfassen

      // Sicherheitsabschaltung der Pumpe bei Überdruck
      if (wassersaeule > (maxFuellhoehe + 300)) {
        Serial.println("Fehler: Überdruck!");
        gv_error = true;
        motor_stop_all();
        messSchritt = 4;
      }

      if (!gv_motor_run) {
        atmDruck = messDruck;
      }
      break;

    case 1:  // Messung gestartet
      gv_error = false;
      vergleichswert = messDruck;

      if (gv_kalib_done) {
        motor_start_all( );
      } else {
        // Bei Kalibrierung: Ventil offen lassen
        //gv_wassersaeule_kalib = 0;
        motor_start_pumpe();
      }

      messung = millis() + 2000;
      messSchritt = 2;
      break;

    case 2:  // warten solange Druck steigt
      if (messDruck > vergleichswert + 10) {
        vergleichswert = messDruck;
        messung = millis() + 1000;
        Serial.print("messdruck: "); Serial.println(messDruck);
        Serial.print("Wassersäule: "); Serial.println(wassersaeule);
      }
      if (wassersaeule > (maxFuellhoehe + 200)) {
        Serial.println("Fehler: Messleitung verstopft!");
        gv_error = true;
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

        if (gv_kalib_done) {
          hoehe   = wassersaeule / 10;
          volumen = (wassersaeule / 10) * A / 100;
          // Umrechnung Wassersäule in 0 - 100%
          fuellstand = map(wassersaeule, 0, maxFuellhoehe, 0, 100);
          Serial.print("Höhe: "); Serial.println(hoehe);
          Serial.print("Volumen: "); Serial.println(volumen);
          Serial.print("Füllstand: "); Serial.println(fuellstand);
          Serial.println();
        } else {
          // Kalibrireungswert für wassersaeule  speichern
          gv_wassersaeule_kalib = wassersaeule + gv_wassersaeule_kalib;
          Serial.print("Wassersäule Kalib: "); Serial.println(gv_wassersaeule_kalib);
        }
        messung = millis() + 2000;
        messSchritt = 4;
      }
      break;

    case 4:  // Ablauf beenden
      motor_stop_all();
      if (gv_kalib_done) {
        pub_sens();
        messSchritt = 0;
      } else {
        // Nach der Kalibreirung mit der eigentlichen M1ssung beginnen
        if (messung < millis()) {
          gv_kalib_done = true;
          messSchritt = 1;
        }
      }
      break;
    case 5:  // OTA Aktiv - Messung verhindern
      break;
    default:
      messSchritt = 0;
      break;
  }
}
