void setup() {
  cy_serial::start(gc_file);

  motor_stop_all();
  delay(500);
  init_pressure();

  wifi_init(gc_hostname);
  delay(500);

  init_ota_local();

  init_mqtt_local( );

  init_sched();
}

void loop() {

  check_ota();

  check_mqtt_reset();

  check_sched();

  check_serial();

  // State-Machine
  messablauf();

}
