#include "memory_monitor.h"
#include <stdio.h>
#include <string.h>
 
void log_memory_usage(const char *label) {
    FILE *fp = fopen("/proc/self/status", "r");
    if (fp == NULL) {
        perror("fopen");
        return;
    }
 
    char line[256];
    printf("[Memory Monitor] %s\n", label);
    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "VmSize:", 7) == 0 || strncmp(line, "VmRSS:", 6) == 0) {
            printf("  %s", line);
        }
    }
    fclose(fp);
}