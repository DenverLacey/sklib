#include "../string.h"

#include "../fmt/writer.h"

#include <assert.h>

namespace sk {
    String::String() noexcept :
        len(0),
        chars(nullptr)
    {
    }

    String::String(const char* s) noexcept :
        len(strlen(s)),
        chars(s)
    {
    }

    String::String(size_t len, const char* chars) noexcept :
        len(len),
        chars(chars)
    {
    }

    String::String(const std::string& s) noexcept :
        len(s.size()),
        chars(s.c_str())
    {
    }

    String::String(std::string_view s) noexcept :
        len(s.size()),
        chars(s.data())
    {
    }

    String& String::operator=(const char* s) noexcept {
        this->len = strlen(s);
        this->chars = s;
        return *this;
    }

    String& String::operator=(const std::string& s) noexcept {
        this->len = s.size();
        this->chars = s.c_str();
        return *this;
    }

    String& String::operator=(std::string_view s) noexcept {
        this->len = s.size();
        this->chars = s.data();
        return *this;
    }

    const char* String::c_str() const noexcept {
        return this->chars;
    }

    std::string String::str() const noexcept {
        return std::string{ this->chars, this->len };
    }

    std::string_view String::view() const noexcept {
        return std::string_view{ this->chars, this->len };
    }

    size_t String::size() const noexcept {
        return this->len;
    }

    ssize_t String::ssize() const noexcept {
        return static_cast<ssize_t>(this->len);
    }

    char String::operator[](size_t index) const noexcept {
        assert(index < this->len);
        return this->chars[index];
    }

    Optional<char> String::at(size_t index) const noexcept {
        if (index >= this->len) {
            return None;
        }
        return this->chars[index];
    }

    Optional<char> String::first() const noexcept {
        if (this->len == 0) {
            return None;
        }
        return this->chars[0];
    }

    Optional<char> String::last() const noexcept {
        if (this->len == 0) {
            return None;
        }
        return this->chars[this->len - 1];
    }

    const char* String::begin() const noexcept {
        return this->chars;
    }

    const char* String::end() const noexcept {
        return this->chars + this->len;
    }

    std::ostream& operator<<(std::ostream& s, const String& str) noexcept {
        auto sv = str.view();
        return s << sv;
    }

    StringBuilder::StringBuilder() noexcept :
        max_capacity(-1),
        capacity(0),
        len(0),
        chars(nullptr)
    {
    }

    StringBuilder::StringBuilder(size_t capacity) noexcept :
        max_capacity(-1),
        capacity(capacity),
        len(0),
        chars(new char[capacity])
    {
    }

    StringBuilder::StringBuilder(size_t capacity, size_t max_capacity) noexcept :
        max_capacity(max_capacity),
        capacity(capacity),
        len(0),
        chars(new char[capacity])
    {
    }

    StringBuilder& StringBuilder::append(String s) noexcept {
        if (this->capacity >= s.len) {
            memcpy(&this->chars[this->len], s.chars, s.len);
            this->len += s.len;
        } else {
            auto new_capacity = this->capacity == 0                    ? s.len :
                                this->capacity * 2 < this->len + s.len ? this->len + s.len : this->capacity * 2;
            new_capacity = new_capacity > this->max_capacity ? this->max_capacity : new_capacity;

            auto new_chars = reinterpret_cast<char*>(realloc(this->chars, new_capacity));
            assert(new_chars);

            auto num_chars_to_write = s.len;
            if (this->len + num_chars_to_write > new_capacity) {
                num_chars_to_write = new_capacity - this->len;
            }

            memcpy(&new_chars[this->len], s.chars, num_chars_to_write);

            this->capacity = new_capacity;
            this->len += num_chars_to_write;
            this->chars = new_chars;
        }
        
        return *this;
    }

    std::ostream& operator<<(std::ostream& s, const StringBuilder& builder) noexcept {
        auto sv = std::string_view{ builder.chars, builder.len };
        return s << "StringBuilder{ \"" << sv << "\" }";
    }
}

void sk::Formatter<sk::String>::format(const sk::String& s, std::string_view fmt, sk::Writer& writer) {
    auto format = Format::from(fmt);
    writer.write_string(s.len, s.chars, format);
}

void sk::Formatter<sk::StringBuilder>::format(const sk::StringBuilder& builder, std::string_view fmt, sk::Writer& writer) {
    auto s = String{ builder.len, builder.chars };
    writer.print("StringBuilder{{ \"{}\" }}", s);
}
