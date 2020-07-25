#include <stdlib.h>
#include <gtk/gtk.h>
#include <glibtop/cpu.h>
#include <glibtop/mem.h>
#include <glibtop/mountlist.h>
#include <glibtop/fsusage.h>
#include <glibtop/netlist.h>
#include <glibtop/netload.h>
#include <time.h>

void update_cpu_stats();
gboolean update_cpu_graph(gpointer data);

gdouble measure_cpu_user();
gdouble measure_cpu_nice();
gdouble measure_cpu_sys();
gdouble measure_cpu_iowait();
gdouble measure_cpu_idle();

gboolean update_mem_graph(gpointer data);

gdouble measure_mem_user();
gdouble measure_mem_shared();
gdouble measure_mem_buffer();
gdouble measure_mem_cached();
gdouble measure_mem_free();

void update_disk_stats();
gboolean update_disk_graph(gpointer data);

gdouble measure_disk_read();
gdouble measure_disk_write();

void update_net_stats();
gboolean update_net_graph(gpointer data);

gdouble measure_net_in();
gdouble measure_net_out();
gdouble measure_net_local();
