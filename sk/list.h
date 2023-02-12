#pragma once

#include <stdint.h>
#include <initializer_list>

#include "optional.h"
#include "mem/allocator.h"
#include "ptr/owned.h"
#include "fmt.h"

namespace sk {
    template<typename T>
    struct List {
        // === Data ===
        size_t capacity;
        size_t len;
        T* items;

        // === Constructors / Assignments ===
        List() noexcept : capacity(0), len(0), items(nullptr) {}
        List(const List<T>&) noexcept = default;
        List(List<T>&&) noexcept = default;

        // === Associated Functions ===
        size_t size() const noexcept {
            return this->len;
        }

        ssize_t ssize() const noexcept {
            return static_cast<ssize_t>(this->len);
        }

        T& operator[](size_t index) const noexcept {
            assert(index < this->len);
            return this->items[index];
        }

        Optional<T&> at(size_t index) const noexcept {
            if (index >= this->len) {
                return None;
            }
            return this->items[index];
        }

        Optional<T&> first() const noexcept {
            return this->at(0);
        }

        Optional<T&> last() const noexcept {
            return this->at(this->len - 1);
        }

        void destroy(Allocator& ator) noexcept {
            ator.free(this->capacity, this->items);
        }

        List<T> clone(Allocator& ator) const noexcept {
            List<T> clone;
            clone.len = this->len;
            clone.capacity = this->len;
            clone.items = ator.alloc<T>(this->len).items;
            memcpy(clone.items, this->items, this->len * sizeof(T));

            return clone;
        }

        bool append(Allocator& ator, const T& item) noexcept {
            if (this->len >= this->capacity) {
                auto new_capacity = this->capacity > 0 ? this->capacity * 2 : 1;
                auto new_items = ator.resize(this->capacity, this->items, new_capacity);
                if (new_items.is_none()) {
                    return false;
                }

                this->capacity = new_capacity;
                this->items = new_items.unwrap();
            }

            this->items[this->len++] = item;
            return true;
        }

        // === Iterator Stuff ===
        T* begin() noexcept {
            return this->items;
        }

        T* end() noexcept {
            return this->items + this->len;
        }

        // === Friends ===
        friend std::ostream& operator<<(std::ostream& s, const List<T>& list) noexcept {
            s << '[';
            for (size_t i = 0; i < list.len; i++) {
                s << list[i];
                if (i + 1 < list.len) {
                    s << ", ";
                }
            }
            return s << ']';
        }
    };

    template<typename T> 
    struct Formatter<List<T>> {
        static void format(const List<T>& list, std::string_view fmt, Writer& writer) {
            bool alternate = fmt == "#";
            writer.write("[");
            for (size_t i = 0; i < list.len; i++) {
                writer.print("{}{}", alternate ? "\n\t" : "", list[i]);
                if (i + 1 < list.len) {
                    writer.print(",{}", alternate ? "" : " ");
                }
            }
            writer.print("{}]", alternate ? "\n" : "");
        }
    };

    template<typename T>
    struct Owned<List<T>> : public IOwned<List<T>> {
        // === Data ===
        Allocator& allocator;

        // === Constructors / Assignments ===
        Owned(Allocator& allocator) noexcept : IOwned<List<T>>(), allocator(allocator) {}
        Owned(const Owned<List<T>>&) noexcept = default;
        Owned(Owned<List<T>>&&) noexcept = default;

        // === Destructor ===
        ~Owned() {
            this->destroy(allocator);
        }

        // === Associated Functions ===
        List<T> clone() const noexcept {
            return this->as_ref().clone(allocator);
        }

        bool append(const T& item) noexcept {
            return this->as_ref().append(allocator, item);
        }
    };

    template<typename T>
    using OwnedList = Owned<List<T>>;
}
