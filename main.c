#include <stdio.h>

#include <stdlib.h>

#include <fcntl.h>

#include <unistd.h>

#include <string.h>
 
#include "mmap_utils.h"

#include "mmap_copy.h"

#include "sharememory.h"

#include "maptable.h"

#include "memory_monitor.h"
 
#define TEST_FILE "test.txt"

#define COPY_FILE "copy.txt"

#define MESSAGE "Hello from mmap!\n"
 
int main() {

    printf("\n=== [0] Start: MemoryMappedFiles Demo ===\n");

    log_memory_usage("Start of program");
 
    // 1. Ghi nội dung vào file bằng mmap

    printf("\n=== [1] mmap_write ===\n");

    log_memory_usage("Before mmap_write");

    if (mmap_write(TEST_FILE, 0, MESSAGE) != EXIT_SUCCESS) {

        fprintf(stderr, "mmap_write failed\n");

    }

    log_memory_usage("After mmap_write");
 
    // 2. Sao chép file qua mmap

    printf("\n=== [2] mmap_copy_file ===\n");

    log_memory_usage("Before mmap_copy_file");

    if (mmap_copy_file(TEST_FILE, COPY_FILE) != EXIT_SUCCESS) {

        fprintf(stderr, "mmap_copy_file failed\n");

    }

    log_memory_usage("After mmap_copy_file");
 
    // 3. Tạo file chia sẻ, ánh xạ và ghi nội dung

    printf("\n=== [3] Shared Memory Mapping ===\n");

    int fd = open("shared.txt", O_RDWR | O_CREAT, 0666);

    if (fd == -1) {

        perror("open shared.txt");

        return EXIT_FAILURE;

    }
 
    ensure_file_size(fd, FILE_SIZE);

    log_memory_usage("Before shared memory map");

    void *map1 = map_file(fd, PROT_READ | PROT_WRITE, MAP_SHARED);

    log_memory_usage("After shared memory map");
 
    write_to_map(map1, "Shared memory content.");

    sync_to_file(map1, FILE_SIZE);

    print_map_contents("SharedMap", (char *)map1);

    log_memory_usage("After writing to shared memory");
 
    // 4. Ghi nhận ánh xạ vào MapTable

    printf("\n=== [4] MapTable Entry ===\n");

    add_file_mapping_entry("shared.txt", map1, fd);

    printf("File mapped: %s -> addr: %p\n", file_mapping_table[0].file_name, file_mapping_table[0].mapped_address);

    log_memory_usage("After adding to MapTable");
 
    // 5. Dọn dẹp

    printf("\n=== [5] Cleanup ===\n");

    cleanup(map1, map1, fd);

    log_memory_usage("After cleanup");
 
    printf("\n=== [6] End of Program ===\n");

    return 0;

}

 