#ifndef __MOUSE_H
#define __MOUSE_H

#include <types.h>
#include <interrupts.h>
#include <port.h>
#include <driver.h>

class MouseDriver : public InterruptHandler, public Driver
{
  Port8Bit dataPort;
  Port8Bit commandPort;

  uint8_t buffer[3];
  uint8_t offset;
  uint8_t buttons;

public:
  MouseDriver(InterruptManager *interruptManager);
  ~MouseDriver();
  virtual uint32_t HandleInterrupt(uint32_t esp);
  virtual void Activate();
};

#endif