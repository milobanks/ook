#ifndef OOK_LIBKERN_SPINTEX_HPP
#define OOK_LIBKERN_SPINTEX_HPP

#include <atomic>
#include <sys/libkern/Lock.hpp>

namespace ook::libkern {
    class Spintex {
      private:
        struct Lock lock;
        std::atomic_bool state;

      public:
        Spintex(const char* name);
        void lock_spin();
        void unlock_spin();
    };
}; // namespace ook::libkern

#endif /* OOK_LIBKERN_SPINTEX_HPP */
