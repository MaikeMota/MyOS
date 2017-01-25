#include <mouse.h>

#define MOUSE_DATA_PORT 0x60
#define MOUSE_COMMAND_PORT 0x64

#define MOUSE_START_X 40
#define MOUSE_START_Y 12

MouseDriver::MouseDriver(InterruptManager *interruptManager)
    : InterruptHandler(interruptManager, 0x2C),
      dataPort(MOUSE_DATA_PORT),
      commandPort(MOUSE_COMMAND_PORT)
{
}

MouseDriver::~MouseDriver()
{
}

void MouseDriver::Activate()
{
    offset = 0;
    buttons = 0;

    uint8_t x = MOUSE_START_X, y = MOUSE_START_Y;

    static uint16_t *VideoMemory = (uint16_t *)0xB8000;
    VideoMemory[80 * y + x] = ((VideoMemory[80 * y + x] & 0xF000) >> 4) | ((VideoMemory[80 * y + x] & 0x0F00) << 4) | (VideoMemory[80 * y + x] & 0x00FF);

    commandPort.Write(0xA8); // Active Interrupts
    commandPort.Write(0x20); // Get current State
    uint8_t status = (dataPort.Read() | 2);
    commandPort.Write(0x60); // Set State
    dataPort.Write(status);

    commandPort.Write(0xD4);
    dataPort.Write(0xF4);
    dataPort.Read();
}

void printf(char *);

uint32_t MouseDriver::HandleInterrupt(uint32_t esp)
{
    uint8_t status = commandPort.Read();
    if (!(status & 0x20))
    {
        return esp;
    }

    static uint8_t x = MOUSE_START_X, y = MOUSE_START_Y;

    buffer[offset] = dataPort.Read();
    offset = (offset + 1) % 3;

    if (offset == 0)
    {

        static uint16_t *VideoMemory = (uint16_t *)0xB8000;

        VideoMemory[80 * y + x] = ((VideoMemory[80 * y + x] & 0xF000) >> 4) | ((VideoMemory[80 * y + x] & 0x0F00) << 4) | (VideoMemory[80 * y + x] & 0x00FF);

        x += buffer[1];

        if (x < 0)
        {
            x = 0;
        }
        else if (x > 80)
        {
            x = 80;
        }

        y -= buffer[2];

        if (y < 0)
        {
            y = 0;
        }
        else if (y > 25)
        {
            y = 24;
        }

        VideoMemory[80 * y + x] = ((VideoMemory[80 * y + x] & 0xF000) >> 4) | ((VideoMemory[80 * y + x] & 0x0F00) << 4) | (VideoMemory[80 * y + x] & 0x00FF);

        for (uint8_t i = 0; i < 3; i++)
        {
            if ((buffer[0]) & (0x01 << i) != (buttons && (0x01 << i)))
            {
                // the state of buttons has been changed since the last check
            }
        }
        buttons = buffer[0];
    }

    return esp;
}