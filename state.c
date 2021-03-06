#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>

#include "state.h"
#include "graph.h"

/** returns number of nanoseconds between two timespecs */
long timespec_subtract(struct timespec* after, struct timespec* before) {
  return (after->tv_sec - before->tv_sec)*1000000000 + (after->tv_nsec - before->tv_nsec);
}

/*** CPU ***/

glibtop_cpu old_cpu;
glibtop_cpu new_cpu;

void update_cpu_stats() {
  old_cpu = new_cpu;
  glibtop_get_cpu(&new_cpu);
}
gboolean update_cpu_graph(gpointer data) {
  update_cpu_stats();
  graph_update((Graph*) data);
  return TRUE;
}

gdouble measure_cpu_user() {
  gdouble old = old_cpu.user + old_cpu.nice;
  gdouble new = new_cpu.user + new_cpu.nice;
  return (new - old) / (new_cpu.total - old_cpu.total);
}
gdouble measure_cpu_sys() {
  return ((gdouble) (new_cpu.sys - old_cpu.sys)) / (new_cpu.total - old_cpu.total);
}
gdouble measure_cpu_iowait() {
  gdouble old = old_cpu.iowait + old_cpu.irq + old_cpu.softirq;
  gdouble new = new_cpu.iowait + new_cpu.irq + new_cpu.softirq;
  return (new - old) / (new_cpu.total - old_cpu.total);
}
gdouble measure_cpu_idle() {
  return ((gdouble) (new_cpu.idle - old_cpu.idle)) / (new_cpu.total - old_cpu.total);
}

/*** RAM ***/

glibtop_mem mem;

gboolean update_mem_graph(gpointer data) {
  Graph* g = (Graph*) data;

  glibtop_get_mem(&mem);

  graph_update(g);
  return TRUE;
}

gdouble measure_mem_user() {
  return mem.total - mem.buffer - mem.cached - mem.free;
}
gdouble measure_mem_cached() {
  return mem.cached + mem.buffer;
}
gdouble measure_mem_free() {
  return mem.free;
}

/*** DISK ***/

guint64 old_disk_read;
guint64 old_disk_write;
guint64 new_disk_read;
guint64 new_disk_write;
struct timespec old_disk_poll_time;
struct timespec new_disk_poll_time;
gdouble disk_dt_corr;

void update_disk_stats() {
  old_disk_read = new_disk_read;
  old_disk_write = new_disk_write;
  new_disk_read = 0;
  new_disk_write = 0;

  glibtop_mountlist mountlist;
  glibtop_mountentry* mountentries = glibtop_get_mountlist(&mountlist, FALSE);

  for (int i = 0; i < mountlist.number; i++) {
    if (strcmp(mountentries[i].type, "smbfs") == 0 ||
        strcmp(mountentries[i].type, "nfs") == 0 ||
        strcmp(mountentries[i].type, "cifs") == 0) {
      continue;
    }
    glibtop_fsusage fsusage;
    glibtop_get_fsusage(&fsusage, mountentries[i].mountdir);
    new_disk_read += fsusage.read * fsusage.block_size;
    new_disk_write += fsusage.write * fsusage.block_size;
  }

  g_free(mountentries);

  old_disk_poll_time = new_disk_poll_time;
  clock_gettime(CLOCK_MONOTONIC, &new_disk_poll_time);
  disk_dt_corr = 1000000000 / (gdouble) timespec_subtract(&new_disk_poll_time, &old_disk_poll_time);
}
gboolean update_disk_graph(gpointer data) {
  update_disk_stats();
  graph_update((Graph*) data);
  return TRUE;
}
gdouble measure_disk_read() {
  return (new_disk_read - old_disk_read) * disk_dt_corr;
}
gdouble measure_disk_write() {
  return (new_disk_write - old_disk_write) * disk_dt_corr;
}

/*** NETWORK ***/

gulong old_net_in;
gulong old_net_out;
gulong old_net_local;
gulong new_net_in;
gulong new_net_out;
gulong new_net_local;
struct timespec old_net_poll_time;
struct timespec new_net_poll_time;
gdouble net_dt_corr;

void update_net_stats() {
  old_net_in = new_net_in;
  old_net_out = new_net_out;
  old_net_local = new_net_local;
  new_net_in = 0;
  new_net_out = 0;
  new_net_local = 0;

  glibtop_netlist netlist;
  gchar** devices = glibtop_get_netlist(&netlist);
  for (int i = 0; i < netlist.number; i++) {
    glibtop_netload netload;
    glibtop_get_netload(&netload, devices[i]);
    if (!(netload.if_flags & (1L << GLIBTOP_IF_FLAGS_UP))) continue;
    if (netload.if_flags & (1L << GLIBTOP_IF_FLAGS_LOOPBACK)) {
      new_net_local += netload.bytes_in;
      continue;
    }
    new_net_in += netload.bytes_in;
    new_net_out += netload.bytes_out;
  }

  g_strfreev(devices);

  old_net_poll_time = new_net_poll_time;
  clock_gettime(CLOCK_MONOTONIC, &new_net_poll_time);
  net_dt_corr = 1000000000 / (gdouble) timespec_subtract(&new_net_poll_time, &old_net_poll_time);
}
gboolean update_net_graph(gpointer data) {
  update_net_stats();
  graph_update((Graph*) data);
  return TRUE;
}
gdouble measure_net_in() {
  return (new_net_in - old_net_in) * net_dt_corr;
}
gdouble measure_net_out() {
  return (new_net_out - old_net_out) * net_dt_corr;
}
gdouble measure_net_local() {
  return (new_net_local - old_net_local) * net_dt_corr;
}
