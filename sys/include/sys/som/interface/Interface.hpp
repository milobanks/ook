#ifndef OOK_LIBKERN_SOM_INTERFACE_INTERFACE_H
#define OOK_LIBKERN_SOM_INTERFACE_INTERFACE_H

#include <sys/libkern/happy.hpp>

namespace ook::som::interface {
    class Interface {
      public:
        virtual ~Interface() = default;

        /* Getting other interfaces. */
        /* Reference counting. */
      private:
        OOK_HAPPY_CLASS;
    };
} // namespace ook::som::interface

#endif /* OOK_LIBKERN_SOM_INTERFACE_INTERFACE_H */
