#ifndef SHAREMEMORY_H
#define SHAREMEMORY_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#define FILE_PATH "example.txt"
#define FILE_SIZE 4096

void ensure_file_size(int fd, size_t size);
void *map_file(int fd, int prot, int flags);
void write_to_map(void *map, const char *message);
void sync_to_file(void *map, size_t size);
void print_map_contents(const char *label, const char *map);
void cleanup(void *map1, void *map2, int fd);

#endif // SHAREMEMORY_H
