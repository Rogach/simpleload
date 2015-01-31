#include <stdlib.h>
#include <gtk/gtk.h>

#include "graph.h"
#include "config.h"
#include "state.h"

int main(int argc, char* argv[]) {
  gtk_init(&argc, &argv);

  // init state variables eagerly
  glibtop_get_cpu(&new_cpu);

  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "multiload");
  gtk_window_set_wmclass(GTK_WINDOW(window), "multiload", "multiload");
  gtk_window_set_type_hint(GTK_WINDOW(window), GDK_WINDOW_TYPE_HINT_DOCK);
  gtk_window_set_default_size(GTK_WINDOW(window), WINDOW_WIDTH, WINDOW_HEIGHT);
  gtk_window_move(GTK_WINDOW(window), WINDOW_X, WINDOW_Y);
  gtk_window_set_has_resize_grip(GTK_WINDOW(window), FALSE);
  GdkRGBA background;
  gdk_rgba_parse(&background, COLOR_BACKGROUND);
  gtk_widget_override_background_color(window, GTK_STATE_FLAG_NORMAL, &background);
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  GtkWidget* box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
  gtk_widget_set_margin_left(box, 1);
  gtk_widget_set_margin_right(box, 1);
  gtk_container_add(GTK_CONTAINER(window), box);

  Graph* graph_cpu = malloc(sizeof(Graph));
  graph_cpu->get_max = graph_max_simple;
  graph_cpu->measure_count = 5;
  graph_cpu->measures = malloc(sizeof(Measure) * graph_cpu->measure_count);
  measure_init(graph_cpu->measures + 0, measure_cpu_user, COLOR_CPU_USER);
  measure_init(graph_cpu->measures + 1, measure_cpu_nice, COLOR_CPU_NICE);
  measure_init(graph_cpu->measures + 2, measure_cpu_sys, COLOR_CPU_SYSTEM);
  measure_init(graph_cpu->measures + 3, measure_cpu_iowait, COLOR_CPU_IOWAIT);
  measure_init(graph_cpu->measures + 4, measure_cpu_idle, COLOR_CPU_IDLE);
  g_timeout_add(UPDATE_INTERVAL, update_cpu, graph_cpu);
  gtk_box_pack_start(GTK_BOX(box), graph_init(graph_cpu), TRUE, TRUE, 0);

  Graph* graph_mem = malloc(sizeof(Graph));
  graph_mem->get_max = graph_max_simple;
  graph_mem->measure_count = 5;
  graph_mem->measures = malloc(sizeof(Measure) * graph_mem->measure_count);
  measure_init(graph_mem->measures + 0, measure_mem_user, COLOR_MEM_USER);
  measure_init(graph_mem->measures + 1, measure_mem_shared, COLOR_MEM_SHARED);
  measure_init(graph_mem->measures + 2, measure_mem_buffer, COLOR_MEM_BUFFER);
  measure_init(graph_mem->measures + 3, measure_mem_cached, COLOR_MEM_CACHED);
  measure_init(graph_mem->measures + 4, measure_mem_free, COLOR_MEM_FREE);
  g_timeout_add(UPDATE_INTERVAL, update_mem, graph_mem);
  gtk_box_pack_start(GTK_BOX(box), graph_init(graph_mem), TRUE, TRUE, 0);

  gtk_widget_show_all(window);
  gtk_main();
  return 0;
}
