#include <stdlib.h>
#include <gtk/gtk.h>

#include "measure.h"
#include "state.h"

void measure_init(Measure* meas, GetMeasure measure_func, gchar* color) {
  meas->get_measure = measure_func;
  meas->data = NULL;
  gdk_rgba_parse(&(meas->color), color);
}

void measure_init_data(Measure* meas, guint width) {
  meas->data = calloc(width, sizeof(gdouble));
}

void measure_poll_next(Measure* meas, guint width) {
  // shift data to the left
  for (int i = 1; i < width; i++) {
    meas->data[i-1] = meas->data[i];
  }
  meas->data[width-1] = meas->get_measure();
}

gdouble measure_cpu_user() {
  return new_cpu.user - old_cpu.user;
}
gdouble measure_cpu_nice() {
  return new_cpu.nice - old_cpu.nice;
}
gdouble measure_cpu_sys() {
  return new_cpu.sys - old_cpu.sys;
}
gdouble measure_cpu_iowait() {
  gdouble old = old_cpu.iowait + old_cpu.irq + old_cpu.softirq;
  gdouble new = new_cpu.iowait + new_cpu.irq + new_cpu.softirq;
  return new - old;
}
gdouble measure_cpu_idle() {
  return new_cpu.idle - old_cpu.idle;
}
