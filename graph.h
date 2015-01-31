#include <gtk/gtk.h>

#include "measure.h"

typedef struct _Graph Graph;
typedef gdouble (*GetMax) (Graph*);

struct _Graph {
  guint width;
  guint height;
  cairo_surface_t* surface;
  guint measure_count;
  Measure* measures;
  GetMax get_max;
  GtkWidget* widget;
};

gboolean graph_update(Graph* g);
void graph_draw(GtkWidget* widget, cairo_t* cr, gpointer data);
GtkWidget* graph_init(Graph* g);

gdouble graph_max_simple(Graph* g);
