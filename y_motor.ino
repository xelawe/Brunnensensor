void motor_start_all() {
  Pumpe.setmotor(_CW, 100);
  Ventil.setmotor(_CW, 100);
  gv_motor_run = true;
}

void motor_stop_all() {
  motor_stop_pumpe();
  Ventil.setmotor(_STOP);
  gv_motor_run = false;
}

void motor_stop_pumpe() {
  Pumpe.setmotor(_STOP);
}
