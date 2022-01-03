void init_pressure() {

Serial.println();
  if (! mpr.begin()) {
    gv_mprls_ok = false;
    Serial.println("----- !!! ---- Failed to communicate with MPRLS sensor, check wiring?");
    Serial.println();
    //    while (1) {
    //      delay(10);
    //    }
    return;
  }
  gv_mprls_ok = true;
  Serial.println("Found MPRLS sensor");

  check_pressure(true);
  atmDruck = messDruck;
}


void check_pressure(boolean iv_print) {
  // Messwert in Pascal auslesen und filtern
  pressure_hPa = mpr.readPressure();
  float median = medianFilter.AddValue(pressure_hPa);

  pressure_Pa = median * 100;
  //messDruck = ((messDruck * 50) + int(pressure_Pa)) / 51;
  messDruck = pressure_Pa;



  if (iv_print || (messSchritt != 0)) {
    Serial.print("Pressure (Pa): "); Serial.println(pressure_Pa);
    Serial.print("messdruck: "); Serial.println(messDruck);
  }
}
