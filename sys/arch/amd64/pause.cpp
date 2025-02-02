#include <sys/arch/pause.hpp>

using namespace ook::arch;

void pause::pause() {
    asm volatile(
        "hlt\n"
        "ret"
    );
}
