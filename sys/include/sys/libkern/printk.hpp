#ifndef OOK_LIBKERN_PRINTK
#define OOK_LIBKERN_PRINTK

#include <cstdarg>
#include <sys/som/interface/IStream.hpp>

#define ook_debug(fmt, ...) \
    ook::libkern::printk( \
        ook::libkern::LogLevel::Debug, \
        MODULE, \
        __FILE__, \
        __func__, \
        __LINE__, \
        fmt __VA_OPT__(, ) __VA_ARGS__ \
    )

#define ook_info(fmt, ...) \
    ook::libkern::printk( \
        ook::libkern::LogLevel::Info, \
        MODULE, \
        __FILE__, \
        __func__, \
        __LINE__, \
        fmt __VA_OPT__(, ) __VA_ARGS__ \
    )

#define ook_warn(fmt, ...) \
    ook::libkern::printk( \
        ook::libkern::LogLevel::Warn, \
        MODULE, \
        __FILE__, \
        __func__, \
        __LINE__, \
        fmt __VA_OPT__(, ) __VA_ARGS__ \
    )

#define ook_error(fmt, ...) \
    ook::libkern::printk( \
        ook::libkern::LogLevel::Error, \
        MODULE, \
        __FILE__, \
        __func__, \
        __LINE__, \
        fmt __VA_OPT__(, ) __VA_ARGS__ \
    )

#define ook_fatal(fmt, ...) \
    ook::libkern::printk( \
        ook::libkern::LogLevel::Fatal, \
        MODULE, \
        __FILE__, \
        __func__, \
        __LINE__, \
        fmt __VA_OPT__(, ) __VA_ARGS__ \
    )

#define OOK_LOG_METADATA_PARAMETERS \
    enum LogLevel level, const char *module, const char *file, \
        const char *func, const int line
#define OOK_LOG_METADATA_PASSTHROUGH level, module, file, func, line

namespace ook::libkern {
    enum class LogLevel {
        Debug,
        Info,
        Warn,
        Error,
        Fatal,
    };

    extern som::interface::IStream<uint8_t>* printk_sink;

    void printk(OOK_LOG_METADATA_PARAMETERS, const char* fmt, ...);
    void vprintk(OOK_LOG_METADATA_PARAMETERS, const char* fmt, va_list args);
    void printks(
        som::interface::IStream<uint8_t>* sink,
        OOK_LOG_METADATA_PARAMETERS,
        const char* fmt,
        ...
    );
    void vprintks(
        som::interface::IStream<uint8_t>* sink,
        OOK_LOG_METADATA_PARAMETERS,
        const char* fmt,
        va_list args
    );
    void printkr(som::interface::IStream<uint8_t>* sink, const char* fmt, ...);
    void vprintkr(
        som::interface::IStream<uint8_t>* sink,
        const char* fmt,
        va_list args
    );
} // namespace ook::libkern

#endif /* OOK_LIBKERN_PRINTK */
