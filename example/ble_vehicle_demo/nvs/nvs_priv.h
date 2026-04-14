
#ifndef __NVS_PRIV_H_
#define __NVS_PRIV_H_

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif
	
#define EFBIG 27	/* File too large */
#define ENOSPC 28       /* No space left on device */
#define ESPIPE 29       /* Illegal seek */
#define EROFS 30	/* Read-only file system */
#define EMLINK 31       /* Too many links */
#define EPIPE 32	/* Broken pipe */
#define EDEADLK 33      /* Resource deadlock avoided */
#define ENOLCK 34       /* No locks available */
#define EACCES	35
#define	EINVAL	36
#define ENOENT	37
/*
 * MASKS AND SHIFT FOR ADDRESSES
 * an address in nvs is an u32_t where:
 *   high 2 bytes represent the sector number
 *   low 2 bytes represent the offset in a sector
 */
#define ADDR_SECT_MASK 0xFFFF0000
#define ADDR_SECT_SHIFT 16
#define ADDR_OFFS_MASK 0x00000FFF

/*
 * Status return values
 */
#define NVS_STATUS_NOSPACE 1

#define NVS_BLOCK_SIZE 32

/* Allocation Table Entry */
struct nvs_ate {
	u16_t id;	/* data id */
	u16_t offset;	/* data offset within sector */
	u16_t len;	/* data len within sector */
	u8_t part;	/* part of a multipart data - future extension */
	u8_t crc8;	/* crc8 check of the entry */
};


#ifdef __cplusplus
}
#endif

#endif /* __NVS_PRIV_H_ */
