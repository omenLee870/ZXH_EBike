

#ifndef _TYPES_H_
#define _TYPES_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef signed char         s8_t;
typedef signed short        s16_t;
typedef signed int          s32_t;
typedef signed long long    s64_t;

typedef unsigned char       u8_t;
typedef unsigned short      u16_t;
typedef unsigned int        u32_t;
typedef unsigned long long  u64_t;
typedef unsigned int 				off_t;

/* 32 bits on ILP32 builds, 64 bits on LP64 builts */
typedef unsigned long       ulong_t;

	
#ifdef __cplusplus
}
#endif

#endif 
