#include <sys/arch/isr.hpp>
#include <sys/libkern/printk.hpp>

#include "sys/arch/idt.hpp"
#include "sys/libkern/panic.hpp"

using namespace ook::arch;
using namespace ook::arch::isr;

/* Fallback. */
static void unpopulated_vector(vector_t vector, const InterruptFrame* frame) {
    ook_error("vector {D} has no installed handler.", vector);
}

func_t isr::funcs[] = {&unpopulated_vector};

void isr::register_func(vector_t vector, func_t func) {
    funcs[vector] = func;
}

/* See `isr_thunks.S`. */
extern "C" void ook_arch_isr_grand_handler(
    uint64_t vector,
    const struct InterruptFrame* frame
) {
    if (vector <= 32) {
        ook::libkern::panic(idt::exception_names[vector]);
    }

    isr::func_t func = isr::funcs[vector];
    if (func) {
        func(vector, frame);
    } else {
        ook_error("no registered handler for vector.\n");
    }
}
