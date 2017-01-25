#ifndef __KEYBOAR_H
#define __KEYBOARD_H

#include <types.h>
#include <interrupts.h>
#include <port.h>
#include <driver.h>

class KeyboardEventHandler
{
public:
  KeyboardEventHandler();

  virtual void OnKeyDown(char key);
  virtual void OnKeyUp(char key);
};

class KeyboardDriver : public InterruptHandler, public Driver
{
  Port8Bit dataPort;
  Port8Bit commandPort;

  KeyboardEventHandler *eventHandler;

public:
  KeyboardDriver(InterruptManager *interruptManager, KeyboardEventHandler *keyboardEventHandler);
  ~KeyboardDriver();
  virtual uint32_t HandleInterrupt(uint32_t esp);
  virtual void Activate();
};

#endif