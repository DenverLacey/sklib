#pragma once

#include "allocator.h"

namespace sk {
    struct CAllocator : Allocator {
        Array<uint8_t> on_alloc(size_t size, uint32_t align) override;
        Optional<Array<uint8_t>> on_resize(Array<uint8_t> buf, uint32_t buf_align, size_t new_size) override;
        void on_free(Array<uint8_t> buf, uint32_t buf_align) override;
    };

    inline CAllocator c_allocator;
}
