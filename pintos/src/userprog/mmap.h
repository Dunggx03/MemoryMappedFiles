#ifndef USERPROG_MMAP_H
#define USERPROG_MMAP_H

#include <lib/kernel/list.h>
#include "threads/thread.h"
#include "filesys/file.h"

#include "vm/page.h"
struct mmap_entry {
    int mapid;                       // ID của mmap
    struct file *file;              // File ánh xạ
    void *start_addr;               // Địa chỉ bắt đầu vùng ánh xạ
    size_t length;                  // Kích thước file
    struct list_elem elem;         // Cho vào danh sách mmap
};

void mmap_init (void);
int mmap (int fd, void *addr);
void munmap (int mapid);
void munmap_all (void);  // Gọi khi process exit
#endif
