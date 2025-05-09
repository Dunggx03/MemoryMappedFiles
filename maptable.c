#include "maptable.h"
#include <string.h>

struct file_mapping_entry file_mapping_table[MAX_FILES];
int table_index = 0;

void add_file_mapping_entry(const char *file_name, void *mapped_address, int fd) {
    if (table_index < MAX_FILES) {
        strcpy(file_mapping_table[table_index].file_name, file_name);
        file_mapping_table[table_index].mapped_address = mapped_address;
        file_mapping_table[table_index].fd = fd;
        table_index++;
    }
}
