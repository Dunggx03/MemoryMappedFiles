#ifndef USERPROG_MEMSTAT_H
#define USERPROG_MEMSTAT_H

#include <stddef.h>
#include "lib/kernel/hash.h"
/* Gọi định kỳ hoặc thủ công để in thống kê bộ nhớ */
void print_memory_usage(void);

/* Cập nhật khi cấp phát frame */
void memstat_frame_alloc(void);

/* Cập nhật khi giải phóng frame */
void memstat_frame_free(void);

/* Cập nhật số slot đã sử dụng trong swap */
void memstat_set_swap_used(size_t used);

/* Cập nhật pointer tới SPT hiện tại */
void memstat_set_spt(struct hash *spt);

#endif /* userprog/memstat.h */
