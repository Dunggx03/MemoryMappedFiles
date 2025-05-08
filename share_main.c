#include "sharememory.h"

int main() {
    int fd = open(FILE_PATH, O_RDWR | O_CREAT, 0600);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    ensure_file_size(fd, FILE_SIZE);

    char *map_shared = (char *)map_file(fd, PROT_READ | PROT_WRITE, MAP_SHARED);
    write_to_map(map_shared, "Hello from MAP_SHARED!\n");
    sync_to_file(map_shared, FILE_SIZE);

    char *map_private = (char *)map_file(fd, PROT_READ | PROT_WRITE, MAP_PRIVATE);
    write_to_map(map_private, "Hello from MAP_PRIVATE!");

    print_map_contents("Data from MAP_SHARED", map_shared);
    print_map_contents("Data from MAP_PRIVATE", map_private);

    cleanup(map_shared, map_private, fd);
    return 0;
}
