#ifndef DALI_CORE_COMMON_TYPES_H_
#define DALI_CORE_COMMON_TYPES_H_

#include <stddef.h>

typedef char  int8;
typedef short int16;
typedef int   int32;
typedef long  int64;

typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned int   uint32;
typedef unsigned long  uint64;

typedef float  float32;
typedef double float64;

typedef enum DaliBool {
    FALSE,
    TRUE
} bool_t;

typedef unsigned char byte_t;

#endif // DALI_CORE_COMMON_TYPES_H_