#include <stdlib.h>
#include <gtk/gtk.h>

#include "measure.h"

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

gdouble small_rand() {
  return (gdouble) (rand() % 2000000);
}
