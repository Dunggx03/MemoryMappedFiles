#include "memory_monitor.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
 
#define LOG_FILE "memory_usage.csv"
 
void log_memory_usage(const char *label) {
    FILE *status_fp = fopen("/proc/self/status", "r");
    if (status_fp == NULL) {
        perror("fopen /proc/self/status");
        return;
    }
 
    FILE *log_fp = fopen(LOG_FILE, "a");
    if (log_fp == NULL) {
        perror("fopen log file");
        fclose(status_fp);
        return;
    }
 
    char line[256];
    char vm_size[64] = "";
    char vm_rss[64] = "";
    char vm_shared[64] = "";
    char vm_pte[64] = "";
 
    while (fgets(line, sizeof(line), status_fp)) {
        if (strncmp(line, "VmSize:", 7) == 0) {
            strncpy(vm_size, line + 7, sizeof(vm_size));
        } else if (strncmp(line, "VmRSS:", 6) == 0) {
            strncpy(vm_rss, line + 6, sizeof(vm_rss));
        } else if (strncmp(line, "RssShmem:", 9) == 0) {
            strncpy(vm_shared, line + 9, sizeof(vm_shared));
        } else if (strncmp(line, "VmPTE:", 6) == 0) {
            strncpy(vm_pte, line + 6, sizeof(vm_pte));
        }
    }
 
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    fprintf(log_fp, "\"%02d:%02d:%02d\",\"%s\",%s,%s,%s,%s",
        t->tm_hour, t->tm_min, t->tm_sec, label,
        vm_size[0] ? vm_size : "0 kB\n",
        vm_rss[0] ? vm_rss : "0 kB\n",
        vm_shared[0] ? vm_shared : "0 kB\n",
        vm_pte[0] ? vm_pte : "0 kB\n");
 
    fclose(status_fp);
    fclose(log_fp);
}