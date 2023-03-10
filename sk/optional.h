#pragma once

#include "fmt.h"
#include "internal.h"

#include <ostream>
#include <type_traits>

namespace sk {
    namespace internal {
        struct TNone {
            friend std::ostream &operator<<(std::ostream &s, const TNone&) {
                return s << "None";
            }
        };
    }

    inline constexpr internal::TNone None = {};

    template<typename T>
    struct Optional {
        // === Data ===
        bool _has_value;
        T _value;

        // === Constructors / Assignments ===
        Optional() noexcept :
            _has_value(false)
        {
        }

        Optional(T value) noexcept :
            _has_value(true),
            _value(std::move(value))
        {
        }

        Optional(internal::TNone) noexcept :
            _has_value(false)
        {
        }

        Optional<T>& operator=(const Optional<T>&) noexcept = default;
        Optional<T>& operator=(Optional<T>&&) noexcept = default;

        Optional<T>& operator=(internal::TNone) noexcept {
            this->_has_value = false;
            return *this;
        }

        Optional<T>& operator=(const T& value) noexcept {
            this->_has_value = true;
            this->_value = value;
            return *this;
        }

        // === Associated Functions ===
        bool is_some() const noexcept {
            return this->_has_value;
        }

        bool is_none() const noexcept {
            return !this->_has_value;
        }

        T& unwrap() noexcept {
            internal::ensure(this->_has_value, "Unwrap of None value attempted.");
            return this->_value;
        }

        T& expect(const char* message) noexcept {
            internal::ensure(this->_has_value, message);
            return this->_value;
        }

        T unwrap_or(const T& default_value) noexcept {
            if (this->_has_value) {
                return this->_value;
            }
            return default_value;
        }

        bool try_unwrap(T& out_value) const noexcept {
            if (this->_has_value) {
                out_value = this->_value;
                return true;
            }
            return false;
        }

        template<typename F>
        Optional<std::invoke_result_t<F, T>> map(F&& f) noexcept {
            if (!this->_has_value) {
                return None;
            }
            return f(this->_value);
        }

        template<typename F>
        std::invoke_result_t<F, T> map_or(const std::invoke_result_t<F, T>& default_value, F&& f) noexcept {
            if (!this->_has_value) {
                return default_value;
            }
            return f(this->_value);
        }

        // === Friends ===
        friend std::ostream &operator<<(std::ostream& s, const Optional<T>& opt) noexcept {
            T val;
            if (opt.try_unwrap(val)) {
                s << "Some(" << val << ")";
            } else {
                s << "None";
            }

            return s;
        }
    };

    template<typename T>
    struct Optional<T&> {
        // === Data ===
        T* _value;


        // === Constructors / Assignments ===
        Optional(T& value) noexcept :
            _value(&value)
        {
        }

        Optional(internal::TNone) noexcept :
            _value(nullptr)
        {
        }

        Optional<T&>& operator=(const Optional<T&>&) noexcept = default;
        Optional<T&>& operator=(Optional<T&>&&) noexcept = default;

        Optional<T&>& operator=(internal::TNone) noexcept {
            this->_value = nullptr;
            return *this;
        }

        Optional<T&>& operator=(const T& value) noexcept {
            *this->_value = value;
            return *this;
        }

        // === Associated Functions ===
        bool is_some() const noexcept {
            return this->_value != nullptr;
        }

        bool is_none() const noexcept {
            return this->_value == nullptr;
        }

        T& unwrap() noexcept {
            internal::ensure(this->is_some(), "Unwrap of None value attempted.");
            return *this->_value;
        }

        T& expect(const char* message) noexcept {
            internal::ensure(this->is_some(), message);
            return *this->_value;
        }

        T unwrap_or(const T& default_value) noexcept {
            if (this->is_some()) {
                return *this->_value;
            }
            return default_value;
        }

        bool try_unwrap(T& out_value) const noexcept {
            if (this->is_some()) {
                out_value = *this->_value;
                return true;
            }
            return false;
        }

        template<typename F>
        Optional<std::invoke_result_t<F, T>> map(F&& f) noexcept {
            if (this->is_none()) {
                return None;
            }
            return f(*this->_value);
        }

        template<typename F>
        std::invoke_result_t<F, T> map_or(const std::invoke_result_t<F, T>& default_value, F&& f) noexcept {
            if (this->is_none()) {
                return default_value;
            }
            return f(*this->_value);
        }

        // === Friends ===
        friend std::ostream& operator<<(std::ostream& s, const Optional<T&>& opt) noexcept {
            T val;
            if (opt.try_unwrap(val)) {
                s << "Some(" << val << ")";
            } else {
                s << "None";
            }
            
            return s;
        }
    };

    template<typename T>
    Optional<T> Some(T value) noexcept {
        return { std::move(value) };
    }

    template<typename T>
    struct Formatter<Optional<T>> {
        static void format(const Optional<T>& opt, std::string_view fmt, Writer& writer) {
            std::remove_reference_t<T> val;
            if (opt.try_unwrap(val)) {
                writer.print("Some({})", val);
            } else {
                writer.write_string("None");
            }
        }
    };

    template<> struct Formatter<internal::TNone> {
        static void format(const internal::TNone&, std::string_view, Writer& writer);
    };
}
