#include <stdlib.h>
#include <gtk/gtk.h>
#include <glibtop/cpu.h>
#include <glibtop/mem.h>

glibtop_cpu old_cpu;
glibtop_cpu new_cpu;
gboolean update_cpu(gpointer data);

gdouble measure_cpu_user();
gdouble measure_cpu_nice();
gdouble measure_cpu_sys();
gdouble measure_cpu_iowait();
gdouble measure_cpu_idle();

glibtop_mem mem;
gboolean update_mem(gpointer data);

gdouble measure_mem_user();
gdouble measure_mem_shared();
gdouble measure_mem_buffer();
gdouble measure_mem_cached();
gdouble measure_mem_free();
