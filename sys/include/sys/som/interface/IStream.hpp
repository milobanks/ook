#ifndef OOK_LIBKERN_SOM_INTERFACE_ISTREAM_HPP
#define OOK_LIBKERN_SOM_INTERFACE_ISTREAM_HPP

#include <cstddef>
#include <sys/libkern/happy.hpp>
#include <sys/libkern/string.hpp>
#include <sys/som/interface/Interface.hpp>

namespace ook::som::interface {
    template<typename T>
    class IStream: public Interface {
      private:
        OOK_HAPPY_CLASS;

      public:
        virtual void send(const T element) const = 0;
        virtual void send(const T* element, const size_t count) const = 0;

        virtual void sendz(const T* buffer) {
            size_t len = libkern::string_length((const char*)buffer);
            send(buffer, len);
        }

        virtual T receive() const = 0;
    };
}; // namespace ook::som::interface

#endif /* OOK_LIBKERN_SOM_INTERFACE_ISTREAM_HPP */
