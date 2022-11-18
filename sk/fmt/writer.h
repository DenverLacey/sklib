#pragma once

#include <string_view>
#include <iostream>
#include <assert.h>

#include "formatter.h"

namespace sk {
    class Writer;
    typedef void(*Printer)(const void* obj, std::string_view fmt, Writer& writer);
    
    struct Arg {
        Printer printer;
        const void* value_ptr;
    };

    class Args {
    public:
        Args(size_t size, Arg* args);
        ~Args();

        Args(const Args& other) = delete;
        Args(Args&& other) = delete;

    public:
        Arg& operator[](size_t index) const;
        size_t size() const;
        Arg* args() const;

    private:
        size_t _size;
        Arg* _args;
    };

    namespace internal {
        template<typename T> struct _formatter_of { using type = Formatter<T>; };
        template<typename T> struct _formatter_of<T*> { using type = Formatter<void*>; };
        template<>  struct _formatter_of<const char*> { using type = Formatter<char*>; };
        template<size_t N> struct _formatter_of<const char[N]> { using type = Formatter<char*>; };
        template<size_t N> struct _formatter_of<char[N]> { using type = Formatter<char*>; };

        template<typename T> using formatter_of = typename _formatter_of<T>::type;
    }

    template<typename T>
    Arg to_arg(const T& arg) {
        return {
            reinterpret_cast<Printer>(internal::formatter_of<T>::format),
            &arg
        };
    }

    template<typename... Ts>
    Args to_args(const Ts&... args) {
        return Args {
            sizeof...(Ts),
            new Arg[sizeof...(Ts)] {
                to_arg(args)...
            }
        };
    }

    class Writer {
    public:
        Writer(std::ostream& stream);

    public:
        void write(const char* s, Format fmt = {});
        void write(size_t n, const char* s, Format fmt = {});

        void write(bool b, Format fmt = {});

        void write(char c, Format fmt = {});

        void write(short d, Format fmt = {});
        void write(int d, Format fmt = {});
        void write(long d, Format fmt = {});
        void write(long long d, Format fmt = {});
        void write(unsigned short d, Format fmt = {});
        void write(unsigned int d, Format fmt = {});
        void write(unsigned long d, Format fmt = {});
        void write(unsigned long long d, Format fmt = {});

        void write(float f, Format fmt = {});
        void write(double f, Format fmt = {});
        
        void write(void* p, Format fmt = {});

        void flush();

        void print(const char* fmt, const Args& args);
        void println(const char* fmt, const Args& args);

        template<typename... Ts>
        void print(const char* fmt, const Ts&... args) {
            if constexpr (sizeof...(Ts) == 0) {
                write(fmt);
            } else {
                auto packed = to_args(args...);
                print(fmt, packed);
            }
        }

        template<typename... Ts>
        void println(const char* fmt, const Ts&... args) {
            if constexpr (sizeof...(Ts) == 0) {
                write(fmt);
                write('\n');
            } else {
                auto packed = to_args(args...);
                println(fmt, packed);
            }
        }

        template<typename T>
        void write(const T& value, std::string_view fmt = "") {
            auto arg = to_arg(value);
            arg.printer(arg.value_ptr, fmt, *this);
        }

    private:
        std::ostream& stream;
    };
}
