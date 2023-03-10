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

        template<typename T>
        class OptionalInner {
        public:
            OptionalInner() noexcept : _has_value(false) {}
            OptionalInner(TNone) noexcept : _has_value(false) {}
            OptionalInner(T value) noexcept : _has_value(true), _value(std::move(value)) {}

        public:
            bool is_some() const noexcept { return this->_has_value; }
            bool is_none() const noexcept { return !this->_has_value; }

        protected:
            T& get() noexcept { assert(_has_value); return this->_value; }
            const T& get() const noexcept { assert(_has_value); return this->_value; }
            void set(const T& value) noexcept { _has_value = true; _value = value; }

        private:
            bool _has_value;
            T _value;
        };

        template<typename T>
        class OptionalInner<T&> {
        public:
            OptionalInner() noexcept : _value(nullptr) {}
            OptionalInner(TNone) noexcept : _value(nullptr) {}
            OptionalInner(T& value) noexcept : _value(&value) {}

        public:
            bool is_some() const noexcept { return this->_value != nullptr; }
            bool is_none() const noexcept { return this->_value == nullptr; }

        protected:
            T& get() noexcept { assert(_value != nullptr); return *this->_value; }
            const T& get() const noexcept { assert(_value != nullptr); return *this->_value; }
            void set(const T& value) noexcept { *_value = value; }

        private:
            T* _value;
        };
    }

    inline constexpr internal::TNone None = {};

    template<typename T>
    class Optional : public internal::OptionalInner<T> {
    public:
        Optional() noexcept : internal::OptionalInner<T>() {}
        Optional(T value) noexcept : internal::OptionalInner<T>(value) {}
        Optional(internal::TNone) noexcept : internal::OptionalInner<T>(None) {}

    public:
        Optional<T>& operator=(const Optional<T>&) noexcept = default;
        Optional<T>& operator=(Optional<T>&&) noexcept = default;

        Optional<T>& operator=(internal::TNone) noexcept {
            this->_has_value = false;
            return *this;
        }

        Optional<T>& operator=(const T& value) noexcept {
            this->set(value);
            return *this;
        }

        // === Associated Functions ===
        T& unwrap() noexcept {
            internal::ensure(this->is_some(), "Unwrap of None value attempted.");
            return this->get();
        }

        T& expect(const char* message) noexcept {
            internal::ensure(this->is_some(), message);
            return this->get();
        }

        T unwrap_or(const T& default_value) noexcept {
            if (this->is_some()) {
                return this->get();
            }
            return default_value;
        }

        bool try_unwrap(T& out_value) const noexcept {
            if (this->is_some()) {
                out_value = this->get();
                return true;
            }
            return false;
        }

        template<typename F>
        Optional<std::invoke_result_t<F, T>> map(F&& f) noexcept {
            if (this->is_none()) {
                return None;
            }
            return f(this->get());
        }

        template<typename F>
        std::invoke_result_t<F, T> map_or(const std::invoke_result_t<F, T>& default_value, F&& f) noexcept {
            if (this->is_none()) {
                return default_value;
            }
            return f(this->get());
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
