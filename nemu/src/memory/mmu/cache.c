#include "memory/mmu/cache.h"
#include "memory/memory.h"
#include <stdlib.h>
#include <time.h>

int b = 0;

typedef struct Cacheline {
    bool valid;
    uint32_t sign;
    uint8_t data[64];
}Cacheline;
Cacheline cache[128][8];
static uint32_t get_block(paddr_t paddr) {
    return (paddr << 26) >> 26;
}

static uint32_t get_group(paddr_t paddr) {
    return (paddr << 19) >> (19 + 6);
}

static uint32_t get_sign(paddr_t paddr) {
    return (paddr >> (7 + 6));
}
static void get_cache_write(uint32_t paddr, size_t len, uint32_t data) {
    uint32_t block = get_block(paddr);
    uint32_t group = get_group(paddr);
    uint32_t sign = get_sign(paddr);
    
    bool have_mem = 0;
    int id = 99999;
    for (int i = 0; i < 8; i++) {
        if (cache[group][i].valid && cache[group][i].sign == sign) {
            id = i;
            have_mem = 1;
            break;
        }
    }
    if(have_mem){
        memcpy(cache[group][id].data + block, &data, len);
        memcpy(hw_mem + paddr, &data, len);
    }else{
        bool have_free = 0;
        for (int i = 0; i < 8; i++) {
            if (!cache[group][i].valid) {
                id = i;
                have_free = 1;
                break;
            }
        }
        if (!have_free) {
            srand((unsigned)time(NULL));
            id = rand()%8;
        }
        cache[group][id].valid = 1;
        cache[group][id].sign = sign;
        memcpy(hw_mem + paddr, &data, len);
        memcpy(cache[group][id].data, hw_mem + ((paddr>>6)<<6), 64);
    }
}




static uint32_t get_cache_read(uint32_t paddr, size_t len) {
    uint32_t ret = 0;
    uint32_t block = get_block(paddr);
    uint32_t group = get_group(paddr);
    uint32_t sign = get_sign(paddr);
    
    bool have_mem = 0;
    int id = 99999;
    for (int i = 0; i < 8; i++) {
        if (cache[group][i].valid && cache[group][i].sign == sign) {
            id = i;
            have_mem = 1;
            break;
        }
    }
    if(have_mem){
        memcpy(&ret, cache[group][id].data + block, len);
    }else{
        bool have_free = 0;
        for (int i = 0; i < 8; i++) {
            if (!cache[group][i].valid) {
                id = i;
                have_free = 1;
                break;
            }
        }
        if (!have_free) {
            id = 0;
        }
        cache[group][id].valid = 1;
        cache[group][id].sign = sign;
        memcpy(cache[group][id].data, hw_mem + ((paddr>>6)<<6), 64);
        memcpy(&ret, cache[group][id].data + block, len);
    }
    return ret;
}


// init the cache

void init_cache()
{
    for (int i = 0; i < 128; i++)
        for (int j = 0; j < 8; j++)
            cache[i][j].valid = 0;
}

// write data to cache
void cache_write(paddr_t paddr, size_t len, uint32_t data)
{

    uint32_t block = get_block(paddr);

    if(block + len > 64){
        get_cache_write(paddr, 64-block, data);
        data = data >> (8*(64-block));
        uint32_t back_paddr = paddr + len -1;
        back_paddr = (back_paddr>>6)<<6;
        get_cache_write(back_paddr, len-(64-block), data);
    }else{
        get_cache_write(paddr,len,data);
    }



}

// read data from cache
uint32_t cache_read(paddr_t paddr, size_t len)
{

    uint32_t ret = 0;

    uint32_t block = get_block(paddr);
    
    if(block + len > 64){
        uint32_t ret1 = get_cache_read(paddr, 64-block);
        uint32_t back_paddr = paddr + len -1;
        back_paddr = (back_paddr>>6)<<6;
        uint32_t ret2 = get_cache_read(back_paddr, len-(64-block));
        ret = ret1 + (ret2 << 8*(64-block));
    }else{
        ret = get_cache_read(paddr, len);
    }
    
    
    


    return ret;
}
