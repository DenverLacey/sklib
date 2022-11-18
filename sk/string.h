#pragma once
#include <stdint.h>
#include <string>
#include <string_view>

#include "optional.h"

namespace sk {
    struct String {
        // === Data ===
        size_t len;
        const char* chars;

        // === Constructors / Assignments ===
        String() noexcept;
        String(const String&) noexcept = default;
        String(String&&) noexcept = default;

        String(const char* s) noexcept;
        String(size_t len, const char* chars) noexcept;
        String(const std::string& s) noexcept;
        String(std::string_view s) noexcept;
        template<size_t N> String(const char s[N]) noexcept : len(N), chars(s) {}

        String& operator=(const String&) noexcept = default;
        String& operator=(String&&) noexcept = default;

        String& operator=(const char* s) noexcept;
        String& operator=(const std::string& s) noexcept;
        String& operator=(std::string_view s) noexcept;

        template<size_t N>
        String& operator=(const char s[N]) noexcept {
            this->len = N;
            this->chars = s;
            return *this;
        }

        // === Associated Functions ===
        const char* c_str() const noexcept;
        std::string str() const noexcept;
        std::string_view view() const noexcept;

        size_t size() const noexcept;
        ssize_t ssize() const noexcept;

        char operator[](size_t index) const noexcept;
        Optional<char> at(size_t index) const noexcept;
        Optional<char> first() const noexcept;
        Optional<char> last() const noexcept;

        // === Iterator Stuff ===
        const char* begin() const noexcept;
        const char* end() const noexcept;

        // === Friends ===
        friend std::ostream& operator<<(std::ostream& s, const String& str) noexcept;
    };

    // @Research:
    // C# StringBuilder Docs: https://learn.microsoft.com/en-us/dotnet/api/system.text.stringbuilder?view=net-7.0
    //
    struct StringBuilder {
        // === Data ===
        size_t capacity;
        size_t max_capacity;
        size_t len;
        char* chars;

        // === Constuctors / Assignments ===
        StringBuilder() noexcept;
        StringBuilder(const StringBuilder&) noexcept = default;
        StringBuilder(StringBuilder&&) noexcept = default;

        StringBuilder(size_t capacity) noexcept;
        StringBuilder(size_t capacity, size_t max_capacity) noexcept;

        StringBuilder& operator=(const StringBuilder&) = default;
        StringBuilder& operator=(StringBuilder&&) = default;

        // === Associated Functions ===
        StringBuilder& append(String s) noexcept;

        // === Friends ===
        friend std::ostream& operator<<(std::ostream& s, const StringBuilder& builder) noexcept;
    };

    template<typename> struct Formatter;
    class Writer;

    template<> struct Formatter<String> {
        static void format(const String& s, std::string_view fmt, Writer& writer);
    };

    template<> struct Formatter<StringBuilder> {
        static void format(const StringBuilder& r, std::string_view fmt, Writer& writer);
    };
}
