#pragma once

#include <stdint.h>

#include "../optional.h"
#include "../array.h"

namespace sk {
    struct Allocator {
        virtual Array<uint8_t> on_alloc(size_t size, uint32_t align) = 0;
        virtual Optional<Array<uint8_t>> on_resize(Array<uint8_t> buf, uint32_t buf_align, size_t new_size) = 0;
        virtual void on_free(Array<uint8_t> buf, uint32_t buf_align) = 0;

        template<typename T>
        Array<T> alloc(size_t len, uint32_t align = 0) {
            uint32_t _align = align == 0 ? alignof(T) : align;
            auto raw_allocaton = this->on_alloc(len * sizeof(T), _align);
            return Array {
                raw_allocaton.len / sizeof(T),
                reinterpret_cast<T *>(raw_allocaton.items)
            };
        }

        template<typename T>
        Optional<T *> resize(size_t old_len, T *ptr, size_t new_len, uint32_t align = 0) {
            auto buf = Array{ old_len, ptr };
            auto new_buf = this->resize(buf, new_len, align);
            if (new_buf.is_none()) {
                return None;
            }

            return new_buf.unwrap().items;
        }

        template<typename T>
        Optional<Array<T>> resize(Array<T> buf, size_t new_len, uint32_t align = 0) {
            uint32_t _align = align == 0 ? alignof(T) : align;
            auto byte_buf = Array<uint8_t> {
                buf.len * sizeof(T),
                reinterpret_cast<uint8_t *>(buf.items)
            };
            
            auto opt_byte_buf = this->on_resize(byte_buf, _align, new_len * sizeof(T));
            if (opt_byte_buf.is_none()) {
                return None;
            }

            auto new_byte_buf = opt_byte_buf.unwrap();

            auto new_buf = Array{
                new_byte_buf.len / sizeof(T),
                reinterpret_cast<T *>(new_byte_buf.items)
            };

            return new_buf;
        }

        template<typename T>
        void free(size_t len, T *ptr, uint32_t align = 0) {
            auto buf = Array{ len, ptr };
            this->free(buf, align);
        }

        template<typename T>
        void free(Array<T> buf, uint32_t buf_align = 0) {
            uint32_t _align = buf_align == 0 ? alignof(T) : buf_align;
            auto byte_buf = Array {
                buf.len * sizeof(T),
                reinterpret_cast<uint8_t *>(buf.items)
            };
            this->on_free(byte_buf, _align);
        }

        template<typename T>
        T *create(uint32_t align = 0) {
            uint32_t _align = align == 0 ? alignof(T) : align;
            auto allocation = this->on_alloc(sizeof(T), _align);
            return reinterpret_cast<T *>(allocation.items);
        }

        template<typename T>
        void destroy(T *ptr) {
            this->free(Array{ 1, ptr });
        }
    };
}
