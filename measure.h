#include <gtk/gtk.h>

typedef gdouble (*GetMeasure) ();

typedef struct _Measure Measure;
struct _Measure {
  gdouble* data;
  GdkRGBA color;
  GetMeasure get_measure;
};

void measure_init(Measure* meas, GetMeasure measure_func, gchar* color);
void measure_init_data(Measure* meas, guint width);
void measure_poll_next(Measure* meas, guint width);

gdouble small_rand();
