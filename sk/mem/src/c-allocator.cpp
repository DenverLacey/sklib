#include "../c-allocator.h"

#include <stdlib.h>

namespace sk {
    Array<uint8_t> CAllocator::on_alloc(size_t size, uint32_t align) {
        void *allocation = malloc(size);
        return Array{
            size,
            reinterpret_cast<uint8_t *>(allocation)
        };
    }

    Optional<Array<uint8_t>> CAllocator::on_resize(Array<uint8_t> buf, uint32_t buf_align, size_t new_size) {
        void *new_allocation = realloc(buf.items, new_size);
        if (!new_allocation) {
            return None;
        }

        auto new_buf = Array{
            new_size,
            reinterpret_cast<uint8_t *>(new_allocation),
        };

        return new_buf;
    }

    void CAllocator::on_free(Array<uint8_t> buf, uint32_t buf_align) {
        std::free(buf.items);
    }
}
