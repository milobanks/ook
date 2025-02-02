#ifndef OOK_LIBKERN_SOM_INTERFACE_IBUFFER_HPP
#define OOK_LIBKERN_SOM_INTERFACE_IBUFFER_HPP

#include <cstddef>
#include <sys/libkern/happy.hpp>
#include <sys/som/interface/Interface.hpp>

namespace ook::som::interface {
    class IBuffer: public Interface {
      private:
        OOK_HAPPY_CLASS;

      public:
        virtual void* get_buffer() const = 0;
        virtual size_t get_size() const = 0;
    };
}; // namespace ook::som::interface

#endif /* OOK_LIBKERN_SOM_INTERFACE_IBUFFER_HPP */
