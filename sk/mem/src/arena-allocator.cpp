#include "../arena-allocator.h"

namespace sk {
    ArenaAllocator::ArenaAllocator(Allocator* allocator) noexcept :
        _blocks(nullptr),
        block_size(0),
        ator(allocator)
    {
    }

    ArenaAllocator::ArenaAllocator(Allocator* allocator, size_t block_size) noexcept :
        _blocks(nullptr),
        block_size(block_size),
        ator(allocator)
    {
    }

    size_t ArenaAllocator::_memory_block_allocation_size(size_t memory_size) {
        return sizeof(MemoryBlock) + memory_size - 1; // `-1` because we declare the memory block to be a length 1 array
    }
    
    ArenaAllocator::MemoryBlock* ArenaAllocator::_make_block(size_t size) const noexcept {
        auto allocation_size = ArenaAllocator::_memory_block_allocation_size(size);
        auto block_allocation = this->ator->alloc<uint8_t>(allocation_size);
        auto block = reinterpret_cast<MemoryBlock*>(block_allocation.items);

        block->next = nullptr;
        block->allocated = 0;
        block->size = size;

        return block;
    }

    ArenaAllocator::Mark ArenaAllocator::mark() const noexcept {
        if (this->_blocks == nullptr) {
            return { 0, nullptr };
        }

        return { this->_blocks->allocated, this->_blocks };
    }

    bool ArenaAllocator::rollback(Mark mark) noexcept {
        if (mark._block == nullptr) {
            this->destroy();
            return true;
        }

        auto it = this->_blocks;
        while (it && it != mark._block) {
            auto next = it->next;

            auto allocation_size = ArenaAllocator::_memory_block_allocation_size(it->size);
            this->ator->free(allocation_size, reinterpret_cast<uint8_t*>(it));

            it = next;
        }

        if (it) {
            it->allocated = mark._index;
        }

        return true;
    }

    void ArenaAllocator::destroy() noexcept {
        auto it = this->_blocks;
        while (it != nullptr) {
            auto next = it->next;

            auto allocation_size = ArenaAllocator::_memory_block_allocation_size(it->size);
            this->ator->free(allocation_size, reinterpret_cast<uint8_t*>(it));

            it = next;
        }
    }

    Array<uint8_t> ArenaAllocator::on_alloc(size_t size, uint32_t align) {
        // @TODO:
        // Handle alignment
        // 

        if (size > this->block_size) {
            auto block = ArenaAllocator::_make_block(size);
            block->next = this->_blocks;
            this->_blocks = block;
        } else if (!this->_blocks || size + this->_blocks->allocated > this->_blocks->size) {
            auto block = ArenaAllocator::_make_block(this->block_size);
            block->next = this->_blocks;
            this->_blocks = block;
        }

        auto block = this->_blocks;
        auto ptr = &block->memory[block->allocated];
        block->allocated += size;

        return { size, ptr };
    }

    Optional<Array<uint8_t>> ArenaAllocator::on_resize(Array<uint8_t> buf, uint32_t buf_align, size_t new_size) {
        auto block = this->_blocks;
        
        if (block && new_size <= this->block_size) {
            auto memory_start = (uintptr_t)block->memory;
            auto free_memory_start = memory_start + block->allocated;
            auto last_allocation = free_memory_start - buf.len;
            auto this_allocation = (uintptr_t)buf.items;
            if (last_allocation == this_allocation) {
                auto size_difference = new_size - buf.len;
                block->allocated += size_difference;
                buf.len = new_size;
                return buf;
            }
        }

        auto new_allocation = this->on_alloc(new_size, buf_align);
        memcpy(new_allocation.items, buf.items, buf.len);

        return new_allocation;
    }

    void ArenaAllocator::on_free(Array<uint8_t> buf, uint32_t buf_align) {
        // We do nothing because ArenaAllocator is a dump-it-all-together type of allocator
    }
}
