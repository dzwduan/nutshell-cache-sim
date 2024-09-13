#ifndef _CACHE_H_
#define _CACHE_H_

#include <stdint.h>

#define bool int
#define true 1
#define fasle 0
#define ro false
#define totalSzie 32
#define ways 4


typedef struct {
    uint64_t data;
    uint64_t tag;
    bool valid;
    bool dirty;
} Meta;





#enidf