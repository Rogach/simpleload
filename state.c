#include <stdlib.h>
#include <gtk/gtk.h>

#include "state.h"
#include "graph.h"

gboolean update_cpu(gpointer data) {
  Graph* g = (Graph*) data;

  old_cpu = new_cpu;
  glibtop_get_cpu(&new_cpu);

  graph_update(g);
  return TRUE;
}
