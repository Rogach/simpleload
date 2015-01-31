#include <stdlib.h>
#include <gtk/gtk.h>
#include <math.h>

#include "graph.h"
#include "config.h"

gboolean graph_update(Graph* g) {
  GtkAllocation* alloc = g_new(GtkAllocation, 1);
  gtk_widget_get_allocation(g->widget, alloc);
  if (g->width != alloc->width || g->height != alloc->height) {
    g->width = alloc->width;
    g->height = alloc->height;
    if (g->surface == NULL) {
      cairo_surface_destroy(g->surface);
    }
    for (int i = 0; i < g->measure_count; i++) {
      measure_init_data(g->measures + i, g->width);
    }
  }
  g_free(alloc);
  if (g->surface == NULL) {
    g->surface = gdk_window_create_similar_surface(gtk_widget_get_window(g->widget),
                                                   CAIRO_CONTENT_COLOR,
                                                   g->width,
                                                   g->height);
  }

  for (int i = 0; i < g->measure_count; i++) {
    measure_poll_next(g->measures + i, g->width);
  }

  gdouble max = g->get_max(g);
  gdouble* stack = malloc(g->width * sizeof(gdouble));
  for (int t = 0; t < g->width; t++) {
    stack[t] = 0;
  }

  cairo_t* cr = cairo_create(g->surface);

  GdkRGBA black;
  gdk_rgba_parse(&black, COLOR_BLACK);
  gdk_cairo_set_source_rgba(cr, &black);
  cairo_rectangle(cr, 0, 0, g->width, g->height);
  cairo_fill(cr);

  cairo_set_line_width(cr, 1.0);
  cairo_set_line_cap(cr, CAIRO_LINE_CAP_SQUARE);

  for (int i = 0; i < g->measure_count; i++) {
    Measure* meas = g->measures + i;
    gdk_cairo_set_source_rgba(cr, &(meas->color));
    for (int t = 0; t < g->width; t++) {
      guint ymin = stack[t] * g->height / max;
      guint ymax = (stack[t] + meas->data[t]) * g->height / max;
      cairo_move_to(cr, t - 0.5, g->height - ymin - 0.5);
      cairo_line_to(cr, t - 0.5, g->height - ymax - 0.5);
      stack[t] += meas->data[t];
    }
    cairo_stroke(cr);
  }

  cairo_destroy(cr);

  gtk_widget_queue_draw(g->widget);
  return TRUE;
}

void graph_draw(GtkWidget* widget, cairo_t* cr, gpointer data) {
  Graph* g = (Graph*) data;
  if (g->surface != NULL) {
    cairo_set_source_surface(cr, g->surface, 0, 0);
    cairo_paint(cr);
  } else {
    GtkAllocation* alloc = g_new(GtkAllocation, 1);
    gtk_widget_get_allocation(g->widget, alloc);
    GdkRGBA background;
    gdk_rgba_parse(&background, COLOR_BACKGROUND);
    gdk_cairo_set_source_rgba(cr, &background);
    cairo_rectangle(cr, 0, 0, alloc->width, alloc->height);
    cairo_fill(cr);
    g_free(alloc);
  }
}

GtkWidget* graph_init(Graph* g) {
  g->surface = NULL;
  g->width = 0;
  g->height = 0;
  GtkWidget* da = gtk_drawing_area_new();
  g->widget = da;
  g_signal_connect(da, "draw", G_CALLBACK(graph_draw), g);
  return da;
}

gdouble graph_max_simple(Graph* g) {
  gdouble max = 0;
  for (int t = 0; t < g->width; t++) {
    gdouble sum = 0;
    for (int i = 0; i < g->measure_count; i++) {
      Measure* meas = g->measures + i;
      sum += meas->data[t];
    }
    if (sum > max) max = sum;
  }
  return max;
}

gdouble graph_max_stepped_mb(Graph* g) {
  gdouble max = graph_max_simple(g);
  gdouble lg = log10(max);
  gint pw;
  if (lg < 6) {
    pw = 6;
  } else {
    pw = ceil(lg);
  }
  return pow(10, pw);
}

gdouble graph_max_min_mb(Graph* g) {
  gdouble max = graph_max_simple(g);
  if (max < 1024 * 1024) {
    return 1024 * 1024;
  } else {
    return max;
  }
}
