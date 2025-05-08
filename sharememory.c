#include "sharememory.h"

void ensure_file_size(int fd, size_t size) {
    if (lseek(fd, size - 1, SEEK_SET) == -1) {
        perror("lseek");
        close(fd);
        exit(EXIT_FAILURE);
    }
    if (write(fd, "", 1) == -1) {
        perror("write");
        close(fd);
        exit(EXIT_FAILURE);
    }
}

void *map_file(int fd, int prot, int flags) {
    void *map = mmap(NULL, FILE_SIZE, prot, flags, fd, 0);
    if (map == MAP_FAILED) {
        perror("mmap");
        close(fd);
        exit(EXIT_FAILURE);
    }
    return map;
}

void write_to_map(void *map, const char *message) {
    sprintf((char *)map, "%s", message);
}

void sync_to_file(void *map, size_t size) {
    if (msync(map, size, MS_SYNC) == -1) {
        perror("msync");
    }
}

void print_map_contents(const char *label, const char *map) {
    printf("%s: %s\n", label, map);
}

void cleanup(void *map1, void *map2, int fd) {
    if (munmap(map1, FILE_SIZE) == -1) perror("munmap map1");
    if (munmap(map2, FILE_SIZE) == -1) perror("munmap map2");
    close(fd);
}
