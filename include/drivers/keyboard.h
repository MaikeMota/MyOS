#ifndef __MYOS__DRIVERS__KEYBOAR_H
#define __MYOS__DRIVERS__KEYBOAR_H

#include <common/types.h>
#include <hardwarecommunication/interrupts.h>
#include <hardwarecommunication/port.h>
#include <drivers/driver.h>

namespace MyOS
{
namespace drivers
{
class KeyboardEventHandler
{
public:
  KeyboardEventHandler();

  virtual void OnKeyDown(char key);
  virtual void OnKeyUp(char key);
};

class KeyboardDriver : public MyOS::hardwarecommunication::InterruptHandler, public Driver
{
  MyOS::hardwarecommunication::Port8Bit dataPort;
  MyOS::hardwarecommunication::Port8Bit commandPort;

  KeyboardEventHandler *eventHandler;

public:
  KeyboardDriver(MyOS::hardwarecommunication::InterruptManager *interruptManager, KeyboardEventHandler *keyboardEventHandler);
  ~KeyboardDriver();
  virtual MyOS::types::uint32_t HandleInterrupt(MyOS::types::uint32_t esp);
  virtual void Activate();
};
}
}

#endif