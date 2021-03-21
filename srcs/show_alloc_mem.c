#include "memory_manager.h"

static size_t
show_alloc_tiny(void) {
    size_t tiny_total = 0;

    printf("TINY : %p\n", (void*)memory_manager.tiny);
        for (t_zone_header * tiny = memory_manager.tiny; tiny != NULL; tiny = tiny->next_zone_header)
            for (t_block_manager * block_manager = (void*)tiny + sizeof(t_zone_header);
            (size_t)((void*)tiny + sizeof(t_zone_header) + tiny->zone_size - (void*)block_manager) > sizeof(block_manager);
            block_manager = (void*)block_manager + sizeof(t_block_manager) + block_manager->block_size)
                if (!block_manager->is_free || 1) {
                    printf("%p - %p : %lu octets\n", (void*)block_manager + sizeof(t_block_manager),
                            (void*)block_manager + sizeof(t_block_manager) + block_manager->block_size, block_manager->block_size);
                    tiny_total += block_manager->block_size;
                }
    return (tiny_total);
}

static size_t
show_alloc_small(void) {
    size_t small_total = 0;

    printf("SMALL : %p\n", (void*)memory_manager.small);
        for (t_zone_header * small = memory_manager.small; small != NULL; small = small->next_zone_header)
            for (t_block_manager * block_manager = (void*)small + sizeof(t_zone_header);
            (size_t)((void*)small + sizeof(t_zone_header) + small->zone_size - (void*)block_manager) > sizeof(block_manager);
            block_manager = (void*)block_manager + sizeof(t_block_manager) + block_manager->block_size)
                if (!block_manager->is_free || 1) {
                    printf("%p - %p : %lu octets\n", (void*)block_manager + sizeof(t_block_manager),
                            (void*)block_manager + sizeof(t_block_manager) + block_manager->block_size, block_manager->block_size);
                    small_total += block_manager->block_size;
                }
    return (small_total);
}

static size_t
show_alloc_large(void) {
    size_t              large_total = 0;
    t_block_manager *   block_manager = NULL;

    printf("LARGE : %p\n", (void*)memory_manager.large);
    for (t_zone_header * large = memory_manager.large; large != NULL; large = large->next_zone_header)
    {
            block_manager = (void*)large + sizeof(t_zone_header);
            if (!block_manager->is_free || 1) {
                printf("%p - %p : %lu octets\n", (void*)block_manager + sizeof(t_block_manager),
                        (void*)block_manager + sizeof(t_block_manager) + block_manager->block_size, block_manager->block_size);
                large_total += block_manager->block_size;
            }
    }
    return (large_total);
}

void
show_alloc_mem(void) {
    size_t total = 0;

    total += show_alloc_tiny();
    total += show_alloc_small();
    total += show_alloc_large();
    printf("Total : %lu octets\n", total);
}