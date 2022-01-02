void check_sched() {

  if (gv_1m) {
    gv_1m = false;
  }

  if (gv_10m) {
    if (messSchritt == 0) {
      messSchritt = 1;
    }
    gv_10m = false;
  }
}


void do_sched_1s() {
  gv_1s = true;

  gv_1m_cnt += 1;
  if ( gv_1m_cnt >= 60 ) {
    gv_1m = true;
    gv_1m_cnt = 0;

    gv_10m_cnt += 1;
    if ( gv_10m_cnt >= 10 ) {
      gv_10m = true;
      gv_10m_cnt = 0;
    }
  }
}

void init_sched() {
  sched_1s.attach(1, do_sched_1s);
  gv_1m = true;
  gv_10m = true;
}
