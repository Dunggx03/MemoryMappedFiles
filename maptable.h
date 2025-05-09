#ifndef MAPTABLE_H
#define MAPTABLE_H

#define FILE_SIZE 100
#define MAX_FILES 10

struct file_mapping_entry {
    char file_name[50];
    void *mapped_address;
    int fd;
};

extern struct file_mapping_entry file_mapping_table[MAX_FILES];
extern int table_index;

void add_file_mapping_entry(const char *file_name, void *mapped_address, int fd);

#endif 
