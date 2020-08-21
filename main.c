#include <stdlib.h>
#include <gtk/gtk.h>

#include "graph.h"
#include "config.h"
#include "state.h"

int main(int argc, char* argv[]) {
  gtk_init(&argc, &argv);

  // init state variables eagerly
  update_cpu_stats();
  update_disk_stats();
  update_net_stats();

  GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "multiload");
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
  gtk_window_set_wmclass(GTK_WINDOW(window), "multiload", "multiload");
#pragma GCC diagnostic pop
  gtk_window_set_type_hint(GTK_WINDOW(window), GDK_WINDOW_TYPE_HINT_DOCK);
  gtk_window_set_default_size(GTK_WINDOW(window), WINDOW_WIDTH, WINDOW_HEIGHT);
  gtk_window_move(GTK_WINDOW(window), WINDOW_X, WINDOW_Y);
  GdkRGBA background;
  gdk_rgba_parse(&background, COLOR_BACKGROUND);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
  gtk_widget_override_background_color(window, GTK_STATE_FLAG_NORMAL, &background);
#pragma GCC diagnostic pop
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  GtkWidget* box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
  gtk_widget_set_margin_start(box, 1);
  gtk_widget_set_margin_end(box, 1);
  gtk_container_add(GTK_CONTAINER(window), box);

  Graph* graph_cpu = malloc(sizeof(Graph));
  graph_cpu->get_max = graph_max_simple;
  graph_cpu->measure_count = 4;
  graph_cpu->measures = malloc(sizeof(Measure) * graph_cpu->measure_count);
  measure_init(graph_cpu->measures + 0, measure_cpu_user, COLOR_CPU_USER);
  measure_init(graph_cpu->measures + 1, measure_cpu_sys, COLOR_CPU_SYSTEM);
  measure_init(graph_cpu->measures + 2, measure_cpu_iowait, COLOR_CPU_IOWAIT);
  measure_init(graph_cpu->measures + 3, measure_cpu_idle, COLOR_CPU_IDLE);
  g_timeout_add(UPDATE_INTERVAL, update_cpu_graph, graph_cpu);
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
  g_timeout_add(UPDATE_INTERVAL, update_mem_graph, graph_mem);
  gtk_box_pack_start(GTK_BOX(box), graph_init(graph_mem), TRUE, TRUE, 0);

  Graph* graph_disk = malloc(sizeof(Graph));
  graph_disk->get_max = graph_max_min_mb;
  graph_disk->measure_count = 2;
  graph_disk->measures = malloc(sizeof(Measure) * graph_disk->measure_count);
  measure_init(graph_disk->measures + 0, measure_disk_read, COLOR_DISK_READ);
  measure_init(graph_disk->measures + 1, measure_disk_write, COLOR_DISK_WRITE);
  g_timeout_add(UPDATE_INTERVAL, update_disk_graph, graph_disk);
  gtk_box_pack_start(GTK_BOX(box), graph_init(graph_disk), TRUE, TRUE, 0);

  Graph* graph_net = malloc(sizeof(Graph));
  graph_net->get_max = graph_max_min_mb;
  graph_net->measure_count = 3;
  graph_net->measures = malloc(sizeof(Measure) * graph_net->measure_count);
  measure_init(graph_net->measures + 0, measure_net_in, COLOR_NET_IN);
  measure_init(graph_net->measures + 1, measure_net_out, COLOR_NET_OUT);
  measure_init(graph_net->measures + 2, measure_net_local, COLOR_NET_LOCAL);
  g_timeout_add(UPDATE_INTERVAL, update_net_graph, graph_net);
  gtk_box_pack_start(GTK_BOX(box), graph_init(graph_net), TRUE, TRUE, 0);

  gtk_widget_show_all(window);
  gtk_main();
  return 0;
}
