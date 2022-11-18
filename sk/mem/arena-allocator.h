#pragma once

#include "allocator.h"

#include <stdint.h>

namespace sk {
    struct ArenaAllocator : Allocator {
        // === Structures ===
        struct MemoryBlock {
            MemoryBlock* next;
            size_t allocated;
            size_t size;
            uint8_t memory[1];
        };

        struct Mark {
            size_t _index;
            MemoryBlock* _block;
        };

        // === Data ===
        MemoryBlock* _blocks;
        size_t block_size;
        Allocator* ator;

        // === Constructors / Assignments ===
        ArenaAllocator() noexcept = default;
        ArenaAllocator(Allocator* allocator) noexcept;
        ArenaAllocator(Allocator* allocator, size_t block_size) noexcept;
        ArenaAllocator(const ArenaAllocator&) noexcept = default;
        ArenaAllocator(ArenaAllocator&&) noexcept = default;

        ArenaAllocator& operator=(const ArenaAllocator&) noexcept = default;
        ArenaAllocator& operator=(ArenaAllocator&&) noexcept = default;

        // === Associated Functions ===
        static size_t _memory_block_allocation_size(size_t memory_size);
        MemoryBlock* _make_block(size_t size) const noexcept;
        Mark mark() const noexcept;
        bool rollback(Mark mark) noexcept;
        void destroy() noexcept;

        // === Inherited Functions ===
        Array<uint8_t> on_alloc(size_t size, uint32_t align) override;
        Optional<Array<uint8_t>> on_resize(Array<uint8_t> buf, uint32_t buf_align, size_t new_size) override;
        void on_free(Array<uint8_t> buf, uint32_t buf_align) override;
    };
}
