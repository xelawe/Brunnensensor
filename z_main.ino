void setup() {
  cy_serial::start(__FILE__);

  motor_stop_all();
  
  wifi_init(gc_hostname);
  delay(500);

  init_ota(gv_clientname);

  init_mqtt_local( );

  init_pressure();

  motor_start_all();

  for (int i = 0; i < 10; i++) {
    float pressure_hPa = mpr.readPressure();
    Serial.print("Pressure (hPa): "); Serial.println(pressure_hPa);
    //Serial.print("Pressure (PSI): "); Serial.println(pressure_hPa / 68.947572932);
    delay(1000);
  }

  motor_stop_all();

  for (int i = 0; i < 5; i++) {
    float pressure_hPa = mpr.readPressure();
    Serial.print("Pressure (hPa): "); Serial.println(pressure_hPa);
    //Serial.print("Pressure (PSI): "); Serial.println(pressure_hPa / 68.947572932);
    delay(1000);
  }
}

void loop() {
  static String inputString;

  check_ota();

  check_mqtt_reset();

  // Kommandos über serielle Schnittstelle
  if (Serial.available()) {
    char inChar = (char)Serial.read();
    if ((inChar == '\r') || (inChar == '\n')) {
      if (inputString == "?") {
        Serial.println("Kommandos: ");
        //        Serial.println("p1 = Pumpe EIN");
        //        Serial.println("p0 = Pumpe AUS");
        //        Serial.println("v1 = Ventil ZU");
        //        Serial.println("v0 = Ventil AUF");
        Serial.println("start = Messung starten");
        Serial.println();
      }
      //      else if (inputString == "p1") {
      //        Serial.println("Pumpe EIN");
      //        digitalWrite(PUMPE, EIN);
      //      }
      //      else if (inputString == "p0") {
      //        Serial.println("Pumpe AUS");
      //        digitalWrite(PUMPE, AUS);
      //      }
      //      else if (inputString == "v1") {
      //        Serial.println("Ventil ZU");
      //        digitalWrite(VENTIL, ZU);
      //      }
      //      else if (inputString == "v0") {
      //        Serial.println("Ventil AUF");
      //        digitalWrite(VENTIL, AUF);
      //      }
      else if (inputString == "start") {
        if (messSchritt == 0) {
          Serial.println("Messung gestartet");
          messSchritt = 1;
        }
      }
      inputString = "";
    } else inputString += inChar;
  }

  // Alle 10 ms Sensorwert auslesen
  if (messTakt < millis()) {
    // Messwert in Pascal auslesen und filtern
    messDruck = ((messDruck * 50) + int(mpr.readPressure())) / 51;
    // Umrechnung Pa in mmH2O
    wassersaeule = (messDruck - atmDruck) * 10197 / 100000;
    if (wassersaeule < 0) wassersaeule = 0;
    messTakt = millis() + 10;
  }

  // Sicherheitsabschaltung der Pumpe bei Überdruck
  if ((messSchritt == 0) && (wassersaeule > (maxFuellhoehe + 300))) {
    Serial.println("Fehler: Überdruck!");
    messSchritt = 4;
  }

  // State-Machine
  messablauf();

}
