#include <sys/arch/hcf.hpp>
#include <sys/arch/pause.hpp>

using namespace ook::arch;

__attribute__((noreturn)) void hcf::hcf() {
    while (true) {
        pause::pause();
    }
}
