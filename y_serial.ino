void check_serial() {
  static String inputString;

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
      else if (inputString == "cal") {
        if (messSchritt == 0) {
          Serial.println("Kalibrierung gestartet");
          gv_kalib_done = false;
          messSchritt = 1;
        }
      }
      inputString = "";
    } else inputString += inChar;
  }
}
