#ifndef DEVICES_DISK_H
#define DEVICES_DISK_H

#include <inttypes.h>
#include <stdint.h>

/*
블록 디바이스로의 섹터 기반 읽기/쓰기를 제공한다. 이 인터페이스는 블록 디바이스로 스왑 파티션에 접근할 때 사용하게 될 것이다.
*/

/* Size of a disk sector in bytes. */
#define DISK_SECTOR_SIZE 512

/* Index of a disk sector within a disk.
 * Good enough for disks up to 2 TB. */
typedef uint32_t disk_sector_t;

/* Format specifier for printf(), e.g.:
 * printf ("sector=%"PRDSNu"\n", sector); */
#define PRDSNu PRIu32

void disk_init (void);
void disk_print_stats (void);

struct disk *disk_get (int chan_no, int dev_no);
disk_sector_t disk_size (struct disk *);
void disk_read (struct disk *, disk_sector_t, void *);
void disk_write (struct disk *, disk_sector_t, const void *);

void 	register_disk_inspect_intr ();
#endif /* devices/disk.h */
