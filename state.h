#include <stdlib.h>
#include <gtk/gtk.h>
#include <glibtop/cpu.h>
#include <glibtop/mem.h>
#include <glibtop/mountlist.h>
#include <glibtop/fsusage.h>

glibtop_cpu old_cpu;
glibtop_cpu new_cpu;
void update_cpu();
gboolean update_cpu_graph(gpointer data);

gdouble measure_cpu_user();
gdouble measure_cpu_nice();
gdouble measure_cpu_sys();
gdouble measure_cpu_iowait();
gdouble measure_cpu_idle();

glibtop_mem mem;
gboolean update_mem_graph(gpointer data);

gdouble measure_mem_user();
gdouble measure_mem_shared();
gdouble measure_mem_buffer();
gdouble measure_mem_cached();
gdouble measure_mem_free();

guint64 old_disk_read;
guint64 old_disk_write;
guint64 new_disk_read;
guint64 new_disk_write;
void update_disk();
gboolean update_disk_graph(gpointer data);

gdouble measure_disk_read();
gdouble measure_disk_write();
