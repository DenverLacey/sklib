#pragma once

#include "../mem/allocator.h"
#include "../fmt.h"

namespace sk {
    template<typename T>
    struct IOwned : public T {
        // === Assignment Operators ===
        IOwned<T>& operator=(const T& value) noexcept {
            *reinterpret_cast<T*>(this) = value;
        }

        // === Associated Functions ===
        T* as_ptr() noexcept {
            return reinterpret_cast<T*>(this);
        }

        const T* as_ptr() const noexcept {
            return reinterpret_cast<const T*>(this);
        }

        T& as_ref() noexcept {
            return *this->as_ptr();
        }

        const T& as_ref() const noexcept {
            return *this->as_ptr();
        }
    };

    template<typename T>
    struct Owned : public IOwned<T> {
        // === Destructor / Assignments
        ~Owned() {
            this->destroy();
        }
    };

    template<typename T>
    struct Formatter<Owned<T>> {
        static void format(const Owned<T>& value, std::string_view fmt, Writer& writer) {
            writer.write("Owned(");
            writer.write(value.as_ref(), fmt);
            writer.write(')');
        }
    };
}
