#pragma once

namespace sk {
    template<typename F>
    struct Defer {
        // === Data ===
        F _on_exit;

        // === Constructors / Destructors ===
        Defer(F&& on_exit) noexcept :
            _on_exit(on_exit)
        {
        }

        ~Defer() noexcept {
            _on_exit();
        }
    };
}

#define SK_INTERNAL_DEFER_IDENT_MAKE(line) __defer ## line
#define SK_INTERNAL_DEFER_IDENT(line) SK_INTERNAL_DEFER_IDENT_MAKE(line)
#define defer ::sk::Defer SK_INTERNAL_DEFER_IDENT(__LINE__) = [&]() -> void
