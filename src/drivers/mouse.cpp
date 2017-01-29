#include <drivers/mouse.h>

using namespace MyOS::types;
using namespace MyOS::drivers;
using namespace MyOS::hardwarecommunication;

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
      dataPort(0x60),
      commandPort(0x64)
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

    if (eventHandler != 0)
    {
        eventHandler->OnActivate();
    }

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
void printfHex(uint8_t);

uint32_t MouseDriver::HandleInterrupt(uint32_t esp)
{
    uint8_t status = commandPort.Read();

   /* printf("\nMouse status: ");
    printfHex(status);*/

    if (!(status & 0x20))
    {
        return esp;
    }

    buffer[offset] = dataPort.Read();

    if (eventHandler == 0)
    {
        return esp;
    }

    offset = (offset + 1) % 3;

    if (offset == 0)
    {

        if (buffer[1] != 0 || buffer[2] != 0)
        {
            eventHandler->OnMouseMove(buffer[1], -(buffer[2]));
        }

        for (uint8_t i = 0; i < 3; i++)
        {
            if ((buffer[0]) & (0x01 << i) != (buttons && (0x01 << i)))
            {
                if (buttons & (0x1 << 1))
                {
                    eventHandler->OnMouseUp(i + 1);
                }
                else
                {
                    eventHandler->OnMouseDown(i + 1);
                }
            }
        }
        buttons = buffer[0];
    }

    return esp;
}