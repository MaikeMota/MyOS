#ifndef __KEYBOAR_H
#define __KEYBOARD_H

#include <types.h>
#include <interrupts.h>
#include <port.h>
#include <driver.h>

class KeyboardDriver : public InterruptHandler, public Driver
{
  Port8Bit dataPort;
  Port8Bit commandPort;

public:
  KeyboardDriver(InterruptManager *interruptManager);
  ~KeyboardDriver();
  virtual uint32_t HandleInterrupt(uint32_t esp);
  virtual void Activate();
};

#endif