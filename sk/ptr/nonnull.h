#pragma once

#include <ostream>

#include "../optional.h"
#include "../fmt.h"

namespace sk {
    template<typename T>
    struct NonNull {
        // === Data ===
        T* _ptr;

        // === Constructors / Assignments ===
    private:
        // Made private to prevent an implicit unchecked
        // construction.
        NonNull(T* ptr) noexcept :
            _ptr(ptr)
        {
        }

    public:
        NonNull() noexcept = default;
        NonNull(std::nullptr_t) noexcept = delete;
        NonNull(const NonNull<T>&) noexcept = default;
        NonNull(NonNull<T>&&) noexcept = default;

        inline static Optional<NonNull<T>> make(T* ptr) {
            if (ptr == nullptr) {
                return None;
            }
            return NonNull{ ptr };
        }

        inline static NonNull<T> make_unchecked(T* ptr) {
            return NonNull{ ptr };
        }

        NonNull<T>& operator=(const NonNull<T>&) noexcept = default;
        NonNull<T>& operator=(NonNull<T>&&) noexcept = default;

        inline Optional<NonNull<T>&> set(T* ptr) noexcept {
            if (ptr == nullptr) {
                return None;
            }
            this->_ptr = ptr;
            return *this;
        }

        inline NonNull<T>& set_unchecked(T* ptr) noexcept {
            this->_ptr = ptr;
            return *this;
        }

        // === Associated Functions ===
        T& operator*() const noexcept {
            return *this->_ptr;
        }

        T* operator->() const noexcept {
            return this->_ptr;
        }

        T* as_ptr() const noexcept {
            return this->_ptr;
        }

        template<typename U>
        NonNull<U> cast() const noexcept {
            U* casted_ptr = reinterpret_cast<U*>(this->_ptr);
            return NonNull{ casted_ptr };
        }

        bool operator==(const NonNull<T>& other) const noexcept {
            return this->_ptr == other._ptr;
        }

        bool operator==(T* other) const noexcept {
            return this->_ptr == other; 
        }

        bool operator!=(const NonNull<T>& other) const noexcept {
            return !(*this == other);
        }

        bool operator!=(T* other) const noexcept {
            return !(*this == other);
        }

        bool replace(T* new_ptr) noexcept {
            if (new_ptr == nullptr) {
                return false;
            }
            this->_ptr = new_ptr;
            return true;
        }

        void swap(NonNull<T>& other) noexcept {
            T* ptr = this->_ptr;
            this->_ptr = other._ptr;
            other._ptr = ptr;
        }

        // === Friends ===
        friend std::ostream& operator<<(std::ostream& s, const NonNull<T>& p) {
            return s << p._ptr;
        }
    };

    template<typename T>
    struct Formatter<NonNull<T>> {
        static void format(const NonNull<T>& p, std::string_view fmt, Writer& writer) {
            auto format = Format::from(fmt);
            writer.write_ptr(p.as_ptr(), format);
        }
    };
}
