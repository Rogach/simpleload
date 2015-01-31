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

gboolean update_mem(gpointer data) {
  Graph* g = (Graph*) data;

  glibtop_get_mem(&mem);

  graph_update(g);
  return TRUE;
}

gdouble measure_mem_user() {
  return mem.user;
}
gdouble measure_mem_shared() {
  return mem.shared;
}
gdouble measure_mem_buffer() {
  return mem.buffer;
}
gdouble measure_mem_cached() {
  return mem.cached;
}
gdouble measure_mem_free() {
  return mem.total - mem.user - mem.shared - mem.buffer - mem.cached;
}
