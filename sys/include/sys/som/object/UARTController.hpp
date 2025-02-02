#ifndef OOK_LIBKERN_SOM_OBJECT_UART_CONTROLLER_HPP
#define OOK_LIBKERN_SOM_OBJECT_UART_CONTROLLER_HPP

#include <cstddef>
#include <cstdint>
#include <sys/arch/port.hpp>
#include <sys/libkern/Slice.hpp>
#include <sys/libkern/happy.hpp>
#include <sys/som/interface/IStream.hpp>

namespace ook::som::object {
    class UARTController: public ook::som::interface::IStream<uint8_t> {
      public:
        UARTController(uint32_t baudrate, size_t id);

        void send(uint8_t c) const override;
        void send(const uint8_t* ptr, size_t count) const override;
        uint8_t receive() const override;

      private:
        OOK_HAPPY_CLASS;

        uint64_t base;
        uint32_t baudrate;
        uint32_t frequency;
    };
}; // namespace ook::som::object

#endif /* OOK_LIBKERN_SOM_OBJECT_UART_CONTROLLER_HPP */
