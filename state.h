#include <stdlib.h>
#include <gtk/gtk.h>
#include <glibtop/cpu.h>

glibtop_cpu old_cpu;
glibtop_cpu new_cpu;

gboolean update_cpu(gpointer data);
