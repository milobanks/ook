#include <sys/libkern/String.hpp>
#include <sys/libkern/panic.hpp>
#include <sys/libkern/printk.hpp>

/* Max for int64_t in binary: log_2(2*(sizeof(int64_t)*8)) + 1 */
#define INT_BUFFER_SIZE 65

using namespace ook;

som::interface::IStream<uint8_t>* libkern::printk_sink = nullptr;

static void print_char(som::interface::IStream<uint8_t>* sink, va_list args) {
    char c = va_arg(args, int);
    sink->send(c);
}

static void
print_cstring(som::interface::IStream<uint8_t>* sink, va_list args) {
    const char* str = va_arg(args, const char*);
    sink->sendz((uint8_t*)str);
}

static void
print_int(som::interface::IStream<uint8_t>* sink, short base, va_list args) {
    static const char* digit_repr = "0123456789abcdefghijklmnopqrstuv";

    /* Sanity. */
    if (base > 32) {
        libkern::panic("print_int: base exceeds 32.");
    }

    int64_t number = va_arg(args, int64_t);
    char buffer[INT_BUFFER_SIZE] = {0};
    unsigned short digits = 0;

    /* Safety. */
    if (number == 0) {
        sink->send('0');
        return;
    } else if (number > 0) {
        sink->send('-');
    }

    /* Extract the digits. */
    while (number != 0) {
        buffer[digits++] = digit_repr[number % base];
        number /= 10;
    }

    /* Print the digits. */
    for (ptrdiff_t i = digits - 1; i >= 0; i--) {
        sink->send(buffer[i]);
    }
}

static void
print_uint(som::interface::IStream<uint8_t>* sink, short base, va_list args) {
    static const char* digit_repr = "0123456789abcdefghijklmnopqrstuv";

    /* Sanity. */
    if (base > 32) {
        libkern::panic("print_int: base exceeds 32.");
    }

    uint64_t number = va_arg(args, uint64_t);
    char buffer[((INT_BUFFER_SIZE - 1) / 2) + 1] = {0};
    unsigned short digits = 0;

    /* Safety. */
    if (number == 0) {
        sink->send('0');
        return;
    }

    /* Extract the digits. */
    while (number != 0) {
        buffer[digits++] = digit_repr[number % base];
        number /= base;
    }

    /* Print the digits. */
    for (ptrdiff_t i = digits; i >= 0; i--) {
        sink->send(buffer[i]);
    }
}

void libkern::printk(OOK_LOG_METADATA_PARAMETERS, const char* fmt, ...) {
    va_list list;

    va_start(list, fmt);
    vprintks(printk_sink, OOK_LOG_METADATA_PASSTHROUGH, fmt, list);
    va_end(list);
}

void libkern::vprintk(
    OOK_LOG_METADATA_PARAMETERS,
    const char* fmt,
    va_list args
) {
    vprintks(printk_sink, OOK_LOG_METADATA_PASSTHROUGH, fmt, args);
}

void libkern::printks(
    som::interface::IStream<uint8_t>* sink,
    OOK_LOG_METADATA_PARAMETERS,
    const char* fmt,
    ...
) {
    va_list list;

    va_start(list, fmt);
    vprintks(sink, OOK_LOG_METADATA_PASSTHROUGH, fmt, list);
    va_end(list);
}

void libkern::vprintks(
    som::interface::IStream<uint8_t>* sink,
    OOK_LOG_METADATA_PARAMETERS,
    const char* fmt,
    va_list args
) {
    printkr(sink, "{s}: ", func);
    vprintkr(sink, fmt, args);
}

void libkern::printkr(
    som::interface::IStream<uint8_t>* sink,
    const char* fmt,
    ...
) {
    va_list list;

    va_start(list, fmt);
    vprintkr(sink, fmt, list);
    va_end(list);
}

void libkern::vprintkr(
    som::interface::IStream<uint8_t>* sink,
    const char* fmt,
    va_list args
) {
    if (sink == nullptr) {
        return;
    }

    while (*fmt) {
        char c = *fmt;

        /* Format specifier? Escape? */
        if (c == '{') {
            switch (*++fmt) {
                case 'c':
                    print_char(sink, args);
                    break;

                case 's':
                    print_cstring(sink, args);
                    break;

                case 'd':
                    print_int(sink, 10, args);
                    break;

                case 'D':
                    print_uint(sink, 10, args);
                    break;

                case 'x':
                    print_int(sink, 16, args);
                    break;

                case 'p':
                case 'X':
                    print_uint(sink, 16, args);
                    break;

                case '\0':
                case '{':
                    sink->send(c);
                    break;
            }

            /* Advance past the specifier and closing '}'. */
            fmt += 2;
            continue;
        }

        /* Output and advance. */
        sink->send(c);
        fmt++;
    }
}
