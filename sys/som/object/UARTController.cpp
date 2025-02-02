#include <sys/arch/uart.hpp>
#include <sys/som/object/UARTController.hpp>

#include "sys/drivers/uart16650.hpp"

using namespace ook;
using namespace ook::som::object;

UARTController::UARTController(uint32_t baudrate, size_t id) :
    base(arch::uart::ID_COM_MAPPING[id]),
    baudrate(baudrate),
    frequency(0) {}

void UARTController::send(uint8_t c) const {
    drivers::uart16650::transmit(base, c);
};

void UARTController::send(const uint8_t* ptr, size_t len) const {
    drivers::uart16650::transmit(base, ptr, len);
};

uint8_t UARTController::receive() const {
    return drivers::uart16650::read(base);
}
