#pragma once

#include "fmt.h"
#include "internal.h"

namespace sk {
    namespace internal {
        struct VoidRef {};

        template<typename Ok, typename Err>
        struct ResultInner {
            bool _is_ok;
            union {
                Ok _ok;
                Err _err;
            };
        
            Ok& unwrap() noexcept {
                internal::ensure(this->_is_ok, "Unwrap of non-existent Ok value.");
                return this->_ok;
            }

            Err& unwrap_err() noexcept {
                internal::ensure(!this->_is_ok, "Unwrap of non-existent Err value.");
                return this->_err;
            }

            const Ok& unwrap() const noexcept {
                internal::ensure(this->_is_ok, "Unwrap of non-existent Ok value.");
                return this->_ok;
            }

            const Err& unwrap_err() const noexcept {
                internal::ensure(!this->_is_ok, "Unwrap of non-existent Err value.");
                return this->_err;
            }
        };

        template<typename Ok, typename Err>
        struct ResultInner<Ok&, Err&> {
            bool _is_ok;
            union {
                Ok* _ok;
                Err* _err;
            };

            Ok& unwrap() noexcept {
                internal::ensure(this->_is_ok, "Unwrap of non-existent Ok value.");
                return *this->_ok;
            }
            
            Err& unwrap_err() noexcept {
                internal::ensure(!this->_is_ok, "Unwrap of non-existent Err value.");
                return *this->_err;
            }

            const Ok& unwrap() const noexcept {
                internal::ensure(this->_is_ok, "Unwrap of non-existent Ok value.");
                return *this->_ok;
            }
            
            const Err& unwrap_err() const noexcept {
                internal::ensure(!this->_is_ok, "Unwrap of non-existent Err value.");
                return *this->_err;
            }
        };

        template<typename Ok>
        struct ResultInner<Ok, void> {
            bool _is_ok;
            Ok _ok;
        
            Ok& unwrap() noexcept {
                internal::ensure(this->_is_ok, "Unwrap of non-existent Ok value.");
                return this->_ok;
            }

            VoidRef unwrap_err() noexcept {
                internal::ensure(!this->_is_ok, "Unwrap of non-existent Err value.");
                return {};
            }

            const Ok& unwrap() const noexcept {
                internal::ensure(this->_is_ok, "Unwrap of non-existent Ok value.");
                return this->_ok;
            }

            const VoidRef unwrap_err() const noexcept {
                internal::ensure(!this->_is_ok, "Unwrap of non-existent Err value.");
                return {};
            }
        };

        template<typename Err>
        struct ResultInner<void, Err> {
            bool _is_ok;
            Err _err;

            VoidRef unwrap() noexcept {
                internal::ensure(this->_is_ok, "Unwrap of non-existent Ok value.");
                return {};
            }

            Err& unwrap_err() noexcept {
                internal::ensure(!this->_is_ok, "Unwrap of non-existent Err value.");
                return this->_err;
            }

            const VoidRef unwrap() const noexcept {
                internal::ensure(this->_is_ok, "Unwrap of non-existent Ok value.");
                return {};
            }

            const Err& unwrap_err() const noexcept {
                internal::ensure(!this->_is_ok, "Unwrap of non-existent Err value.");
                return this->_err;
            }
        };

        template<>
        struct ResultInner<void, void> {
            bool _is_ok;

            VoidRef unwrap() noexcept {
                internal::ensure(this->_is_ok, "Unwrap of non-existent Ok value.");
                return {};
            }

            VoidRef unwrap_err() noexcept {
                internal::ensure(!this->_is_ok, "Unwrap of non-existent Err value.");
                return {};
            }

            const VoidRef unwrap() const noexcept {
                internal::ensure(this->_is_ok, "Unwrap of non-existent Ok value.");
                return {};
            }

            const VoidRef unwrap_err() const noexcept {
                internal::ensure(!this->_is_ok, "Unwrap of non-existent Err value.");
                return {};
            }
        };
    }

    template<typename Ok, typename Err>
    struct Result : public internal::ResultInner<Ok, Err> {
        // === Associated Functions ===
        bool is_ok() const noexcept {
            return this->_is_ok;
        }

        bool is_err() const noexcept {
            return !this->_is_ok;
        }
    };

    template<typename E, typename O>
    Result<O, E> Ok(const O& ok) {
        Result<O, E> r;
        r._is_ok = true;
        r._ok = ok;
        return r;
    }

    template<typename O, typename E>
    Result<O, E> Err(const E& err) {
        Result<O, E> r;
        r._is_ok = false;
        r._err = err;
        return r;
    }

    template<typename E>
    Result<void, E> Ok() {
        Result<void, E> r;
        r._is_ok = true;
        return r;
    }

    template<typename O>
    Result<O, void> Err() {
        Result<O, void> r;
        r._is_ok = false;
        return r;
    }

    template<>
    struct Formatter<internal::VoidRef> {
        static void format(const internal::VoidRef&, std::string_view, Writer&) {
            // print nothing
        }
    };

    template<typename Ok, typename Err>
    struct Formatter<Result<Ok, Err>> {
        static void format(const Result<Ok, Err>& result, std::string_view fmt, Writer& writer) {
            if (result.is_ok()) {
                writer.print("Ok({})", result.unwrap());
            } else {
                writer.print("Err({})", result.unwrap_err());
            }
        }
    };
}