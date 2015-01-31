#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>

#include "state.h"
#include "graph.h"

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

gboolean update_mem_graph(gpointer data) {
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
}
gboolean update_disk_graph(gpointer data) {
  update_disk_stats();
  graph_update((Graph*) data);
  return TRUE;
}
gdouble measure_disk_read() {
  return new_disk_read - old_disk_read;
}
gdouble measure_disk_write() {
  return new_disk_write - old_disk_write;
}

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
}
gboolean update_net_graph(gpointer data) {
  update_net_stats();
  graph_update((Graph*) data);
  return TRUE;
}
gdouble measure_net_in() {
  return new_net_in - old_net_in;
}
gdouble measure_net_out() {
  return new_net_out - old_net_out;
}
gdouble measure_net_local() {
  return new_net_local - old_net_local;
}
