#include <mouse.h>

#define MOUSE_DATA_PORT 0x60
#define MOUSE_COMMAND_PORT 0x64

MouseEventHandler::MouseEventHandler()
{
}

void MouseEventHandler::OnActivate()
{
}
void MouseEventHandler::OnMouseDown(uint8_t button) {}
void MouseEventHandler::OnMouseUp(uint8_t button) {}
void MouseEventHandler::OnMouseMove(int xOffset, int yOffset) {}

MouseDriver::MouseDriver(InterruptManager *interruptManager, MouseEventHandler *mouseEventHandler)
    : InterruptHandler(interruptManager, 0x2C),
      dataPort(MOUSE_DATA_PORT),
      commandPort(MOUSE_COMMAND_PORT)
{
    this->eventHandler = mouseEventHandler;
}

MouseDriver::~MouseDriver()
{
}

void MouseDriver::Activate()
{
    offset = 0;
    buttons = 0;

    commandPort.Write(0xA8); // Active Interrupts
    commandPort.Write(0x20); // Get current State
    uint8_t status = (dataPort.Read() | 2);
    commandPort.Write(0x60); // Set State
    dataPort.Write(status);

    commandPort.Write(0xD4);
    dataPort.Write(0xF4);
    dataPort.Read();

    if (eventHandler != 0)
    {
        eventHandler->OnActivate();
    }
}

void printf(char *);

uint32_t MouseDriver::HandleInterrupt(uint32_t esp)
{
    uint8_t status = commandPort.Read();
    if ((!(status & 0x20)) || eventHandler == 0)
    {
        return esp;
    }

    buffer[offset] = dataPort.Read();
    offset = (offset + 1) % 3;

    if (offset == 0)
    {

        if (buffer[1] != 0 || buffer[2] != 0)
        {
            eventHandler->OnMouseMove(buffer[1], -buffer[2]);
        }

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