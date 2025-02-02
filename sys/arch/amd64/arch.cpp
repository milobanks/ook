#include <sys/arch/arch.hpp>
#include <sys/arch/gdt.hpp>
#include <sys/arch/idt.hpp>
#include <sys/arch/pic.hpp>

using namespace ook;

void arch::init() {
    gdt::init();
    pic::init();
    idt::init();
}
