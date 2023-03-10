#include "../writer.h"

#include <stdio.h>
#include <charconv>

namespace sk {
    Args::Args(size_t size, Arg* args) : 
        _size(size),
        _args(args)
    {
    }

    Args::~Args() {
        delete[] _args;
    }

    Arg& Args::operator[](size_t index) const {
        return _args[index];
    }

    size_t Args::size() const {
        return _size;
    }

    Arg* Args::args() const {
        return _args;
    }

    Writer::Writer(std::ostream& stream) :
        stream(stream)
    {
    }

    void Writer::write_string(const char* s, Format fmt) {
        auto old_flags = stream.flags();
        stream.clear();

        // set flags
        stream.fill(fmt.fill);
        stream.width(fmt.width);

        switch (fmt.align) {
            case Format::Align::Omitted:
            case Format::Align::Left:
                stream << std::left;
                break;
            case Format::Align::Right:
                stream << std::right;
                break;
            case Format::Align::Center:
                stream << std::internal;
                break;
        }

        if (fmt.alternate) {
            stream << '"' << s << '"';
        } else {
            stream << s;
        }

        stream.flags(old_flags);
    }

    void Writer::write_string(size_t n, const char* s, Format fmt) {
        auto old_flags = stream.flags();
        stream.clear();

        // set flags
        stream.fill(fmt.fill);
        stream.width(fmt.width);

        switch (fmt.align) {
            case Format::Align::Omitted:
            case Format::Align::Left:
                stream << std::left;
                break;
            case Format::Align::Right:
                stream << std::right;
                break;
            case Format::Align::Center:
                stream << std::internal;
                break;
        }

        if (fmt.alternate) {
            stream << '"' << std::string_view{ s, n } << '"';
        } else {
            stream << std::string_view{ s, n };
        }

        stream.flags(old_flags);
    }   

    void Writer::write_bool(bool b, Format fmt) {
        auto old_flags = stream.flags();
        stream.clear();

        stream.fill(fmt.fill);
        stream.width(fmt.width);

        switch (fmt.align) {
            case Format::Align::Omitted:
            case Format::Align::Left:
                stream << std::left;
                break;
            case Format::Align::Right:
                stream << std::right;
                break;
            case Format::Align::Center:
                stream << std::internal;
                break;
        }

        switch (fmt.type) {
            case Format::Type::Decimal:
                stream << (b ? '1' : '0');
                break;
            case Format::Type::Char:
                stream << (b ? 't' : 'f');
                break;
            case Format::Type::CharBig:
                stream << (b ? 'T' : 'F');
                break;
            default:
                stream << (b ? "true" : "false");
                break;
        }

        stream.flags(old_flags);
    }

    void Writer::write_char(char c, Format fmt) {
        auto old_flags = stream.flags();
        stream.clear();

        stream.fill(fmt.fill);
        stream.width(fmt.width);

        switch (fmt.align) {
            case Format::Align::Omitted:
            case Format::Align::Left:
                stream << std::left;
                break;
            case Format::Align::Right:
                stream << std::right;
                break;
            case Format::Align::Center:
                stream << std::internal;
                break;
        }

        switch (fmt.type) {
            case Format::Type::CharBig:
                stream << std::uppercase << c;
                break;
            case Format::Type::Decimal:
                stream << static_cast<int>(c);
                break;
            default:
                stream << c;
                break;
        }

        stream.flags(old_flags);
    }

    void Writer::write_int(short d, Format fmt) {
        this->write_int(static_cast<long long>(d), fmt);
    }

    void Writer::write_int(int d, Format fmt) {
        this->write_int(static_cast<long long>(d), fmt);
    }

    void Writer::write_int(long d, Format fmt) {
        this->write_int(static_cast<long long>(d), fmt);
    }

    void Writer::write_int(long long d, Format fmt) {
        auto old_flags = stream.flags();
        stream.clear();

        // set format
        stream.fill(fmt.fill);
        stream.precision(fmt.precision);
        stream.width(fmt.width);

        switch (fmt.align) {
            case Format::Align::Left:
                stream << std::left;
                break;
            case Format::Align::Omitted:
            case Format::Align::Right:
                stream << std::right;
                break;
            case Format::Align::Center:
                stream << std::internal;
                break;
        }

        switch (fmt.sign) {
            case Format::Sign::Both:
                if (d >= 0) stream << '+';
                break;
            case Format::Sign::Space:
                if (d >= 0) stream << ' ';
                break;
        }

        switch (fmt.type) {
            case Format::Type::Binary:
            case Format::Type::BinaryBig:
                // @TODO
                break;
            case Format::Type::Decimal:
                stream << std::dec;
                break;
            case Format::Type::Hex:
                if (fmt.alternate) stream << "0x";
                stream << std::hex;
                break;
            case Format::Type::HexBig:
                if (fmt.alternate) stream << "0x";
                stream << std::uppercase << std::hex;
                break;
            case Format::Type::Octal:
                if (fmt.alternate) stream << "0o";
                stream << std::oct;
                break;
        }

        stream << d;

        stream.flags(old_flags);
    }

    void Writer::write_int(unsigned short d, Format fmt) {
        this->write_int(static_cast<unsigned long long>(d), fmt);
    }

    void Writer::write_int(unsigned int d, Format fmt) {
        this->write_int(static_cast<unsigned long long>(d), fmt);
    }

    void Writer::write_int(unsigned long d, Format fmt) {
        this->write_int(static_cast<unsigned long long>(d), fmt);
    }

    void Writer::write_int(unsigned long long d, Format fmt) {
        auto old_flags = stream.flags();
        stream.clear();

        // set format
        stream.fill(fmt.fill);
        stream.precision(fmt.precision);
        stream.width(fmt.width);

        switch (fmt.align) {
            case Format::Align::Left:
                stream << std::left;
                break;
            case Format::Align::Omitted:
            case Format::Align::Right:
                stream << std::right;
                break;
            case Format::Align::Center:
                stream << std::internal;
                break;
        }

        switch (fmt.sign) {
            case Format::Sign::Both:
                if (d >= 0) stream << '+';
                break;
            case Format::Sign::Space:
                if (d >= 0) stream << ' ';
                break;
        }

        switch (fmt.type) {
            case Format::Type::Binary:
            case Format::Type::BinaryBig:
                // @TODO
                break;
            case Format::Type::Decimal:
                stream << std::dec;
                break;
            case Format::Type::Hex:
                if (fmt.alternate) stream << "0x";
                stream << std::hex;
                break;
            case Format::Type::HexBig:
                if (fmt.alternate) stream << "0x";
                stream << std::uppercase << std::hex;
                break;
            case Format::Type::Octal:
                if (fmt.alternate) stream << "0o";
                stream << std::oct;
                break;
        }

        stream << d;

        stream.flags(old_flags);
    }

    void Writer::write_float(float f, Format fmt) {
        this->write_float(static_cast<double>(f), fmt);
    }

    void Writer::write_float(double f, Format fmt) {
        auto old_flags = stream.flags();
        stream.clear();

        // set flags
        stream.fill(fmt.fill);
        stream.precision(fmt.precision);
        stream.width(fmt.width);

        switch (fmt.align) {
            case Format::Align::Left:
                stream << std::left;
                break;
            case Format::Align::Omitted:
            case Format::Align::Right:
                stream << std::right;
                break;
            case Format::Align::Center:
                stream << std::internal;
                break;
        }

        switch (fmt.sign) {
            case Format::Sign::Both:
                if (f >= 0) stream << '+';
                break;
            case Format::Sign::Space:
                if (f >= 0) stream << ' ';
                break;
        }

        switch (fmt.type) {
            case Format::Type::Exp:
                stream << std::scientific;
                break;
            case Format::Type::ExpBig:
                stream << std::uppercase << std::scientific;
                break;
            case Format::Type::HexFloat:
                stream << std::hexfloat;
                break;
            case Format::Type::HexFloatBig:
                stream << std::uppercase << std::hexfloat;
                break;
            case Format::Type::General:
            case Format::Type::GeneralBig:
                // do nothing, already in general mode
                break;
            default:
                stream << std::fixed;
                break;
        }

        stream << f;

        stream.flags(old_flags);
    }

    void Writer::write_ptr(const void* p, Format fmt) {
        auto old_flags = stream.flags();
        stream.clear();

        // set format
        stream.fill(fmt.fill);
        stream.precision(fmt.precision);
        stream.width(fmt.width);

        switch (fmt.align) {
            case Format::Align::Left:
                stream << std::left;
                break;
            case Format::Align::Omitted:
            case Format::Align::Right:
                stream << std::right;
                break;
            case Format::Align::Center:
                stream << std::internal;
                break;
        }

        stream << p;

        stream.flags(old_flags);
    }

    void Writer::flush() {
        stream.flush();
    }

    void Writer::println(const char* fmt, const Args& args) {
        print(fmt, args);
        write_char('\n');
    }

    void Writer::print(const char* fmt, const Args& args) {
        const char *s = fmt;
        size_t s_len = 0;
        size_t current_arg = 0;
        while (s[s_len] != '\0') {
            if (s[s_len] == '{' && s[s_len + 1] != '{') {
                write_string(s_len, s);

                size_t i = 1;

                size_t arg_index;
                if (!isdigit(s[s_len + i])) {
                    arg_index = current_arg;
                    current_arg++;
                } else {
                    const char *index_start = &s[s_len + i];

                    while (isdigit(s[s_len + i])) i++;
                    const char *index_end = &s[s_len + i];

                    std::from_chars(index_start, index_end, arg_index);
                }

                assert(arg_index < args.size());

                size_t fmt_len = 0;
                if (s[s_len + i] == ':') {
                    i++;
                    while (s[s_len + i + fmt_len] != '}') fmt_len++;
                }

                auto fmt = std::string_view{ &s[s_len + i], fmt_len };
                auto arg = args[arg_index];
                arg.printer(arg.value_ptr, fmt, *this);

                s = &s[s_len + i + fmt_len + 1];
                s_len = 0;
            } else if (s[s_len] == '{') {
                s_len++;
                write_string(s_len, s);

                s = &s[s_len + 1];
                s_len = 0;
            } else if (s[s_len] == '}' && s[s_len + 1] == '}') {
                s_len++;
                write_string(s_len, s);

                s = &s[s_len + 1];
                s_len = 0;
            } else if (s[s_len] == '}') {
                assert(!"Stray `}` character in format string. use `}}` to escape or add missing `{`.");
            } else {
                s_len++;
            }
        }

        if (s_len != 0) {
            write_string(s_len, s);
        }
    }
}
