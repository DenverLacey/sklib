#pragma once

#include <assert.h>

#include "optional.h"
#include "fmt.h"

namespace sk {
template<typename T>
    struct Array {
        // === Data ===
        size_t len;
        T* items;

        // === Constructors / Assignments ===
        Array() noexcept : len(0), items(nullptr) {}
        Array(const Array<T>&) noexcept = default;
        Array(Array<T>&&) noexcept = default;

        Array(size_t len, T* items) noexcept :
            len(len),
            items(items)
        {
        }

        Array<T>& operator=(const Array<T>&) noexcept = default;
        Array<T>& operator=(Array<T>&&) noexcept = default;

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
            if (this->len == 0) {
                return None;
            }
            return this->items[0];
        }

        Optional<T&> last() const noexcept {
            if (this->len == 0) {
                return None;
            }
            return this->items[this->len - 1];
        }

        // === Iterator Stuff ===
        T* begin() noexcept {
            return this->items;
        }

        T* end() noexcept {
            return this->items + this->len;
        }

        // === Friends ===
        friend std::ostream& operator<<(std::ostream& s, const Array<T>& arr) noexcept {
            s << '[';
            for (size_t i = 0; i < arr.len; i++) {
                s << arr[i];
                if (i + 1 < arr.len) {
                    s << ", ";
                }
            }
            return s << ']';
        }
    };

    template<typename T>
    struct Formatter<Array<T>> {
        static void format(const Array<T>& arr, std::string_view fmt, Writer& writer) {
            bool alternate = fmt == "#";
            writer.write("[");
            for (size_t i = 0; i < arr.len; i++) {
                writer.print("{}{}", alternate ? "\n\t" : "", arr[i]);
                if (i + 1 < arr.len) {
                    writer.print(",{}", alternate ? "" : " ");
                }
            }
            writer.print("{}]", alternate ? "\n" : "");
        }
    };
}
