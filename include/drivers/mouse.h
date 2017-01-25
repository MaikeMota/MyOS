#ifndef __MYOS__DRIVERS__MOUSE_H
#define __MYOS__DRIVERS__MOUSE_H

#include <common/types.h>
#include <hardwarecommunication/interrupts.h>
#include <hardwarecommunication/port.h>
#include <drivers/driver.h>

namespace MyOS
{
namespace drivers
{
class MouseEventHandler
{
public:
  MouseEventHandler();
  virtual void OnActivate();
  virtual void OnMouseDown(MyOS::types::uint8_t button);
  virtual void OnMouseUp(MyOS::types::uint8_t button);
  virtual void OnMouseMove(int xOffset, int yOffset);
};

class MouseDriver : public MyOS::hardwarecommunication::InterruptHandler, public Driver
{
  MyOS::hardwarecommunication::Port8Bit dataPort;
  MyOS::hardwarecommunication::Port8Bit commandPort;

  MyOS::types::uint8_t buffer[3];
  MyOS::types::uint8_t offset;
  MyOS::types::uint8_t buttons;

  MouseEventHandler *eventHandler;

public:
  MouseDriver(MyOS::hardwarecommunication::InterruptManager *interruptManager, MouseEventHandler *eventHandler);
  ~MouseDriver();
  virtual MyOS::types::uint32_t HandleInterrupt(MyOS::types::uint32_t esp);
  virtual void Activate();
};
}
}
#endif