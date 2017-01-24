#include <keyboard.h>

#define KEYBOARD_DATA_PORT 0x60

KeyboardDriver::KeyboardDriver(InterruptManager *interruptManager)
    : InterruptHandler(0x21, interruptManager),
      dataPort(KEYBOARD_DATA_PORT),
      commandPort(0x64)
{
    while (commandPort.Read() & 0x1)
    {
        dataPort.Read();
    }
    commandPort.Write(0xAE); // Active Interrupts
    commandPort.Write(0x20); // Get current State
    uint8_t status = (dataPort.Read() | 1) & ~0x10;
    commandPort.Write(0x60); // Set State
    dataPort.Write(status);

    dataPort.Write(0xF4);
}

KeyboardDriver::~KeyboardDriver()
{
}

void printf(char *);

uint32_t KeyboardDriver::HandleInterrupt(uint32_t esp)
{
    uint8_t key = dataPort.Read();

    if (key < 0x80)
    {

        switch (key)
        {
        case 0xFA:
        case 0x45:
        case 0xC5:
        {
            break;
        }
        case 0x1E:
        {
            printf("a");
            break;
        }
        default:
        {
            char *foo = "\nKEYBOARD 0x00";
            char *hex = "0123456789ABCDEF";
            foo[11] = hex[(key >> 4) & 0x0F];
            foo[12] = hex[key & 0x0F];
            printf(foo);

            break;
        }
        }
    }

    return esp;
}