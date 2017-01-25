#ifndef __MYOS__HARDWARECOMMUNICATION__PORT_H
#define __MYOS__HARDWARECOMMUNICATION__PORT_H

#include <common/types.h>

namespace MyOS
{
namespace hardwarecommunication
{
class Port
{
protected:
  MyOS::types::uint16_t portNumber;
  Port(MyOS::types::uint16_t portNumber);
  ~Port();
};

class Port8Bit : public Port
{
public:
  Port8Bit(MyOS::types::uint16_t portNumber);
  ~Port8Bit();
  virtual void Write(MyOS::types::uint8_t data);
  MyOS::types::uint8_t Read();
};

class Port8BitSlow : public Port
{
public:
  Port8BitSlow(MyOS::types::uint16_t portNumber);
  ~Port8BitSlow();
  virtual void Write(MyOS::types::uint8_t data);
};

class Port16Bit : public Port
{
public:
  Port16Bit(MyOS::types::uint16_t portNumber);
  ~Port16Bit();
  virtual void Write(MyOS::types::uint16_t data);
  virtual MyOS::types::uint16_t Read();
};

class Port32Bit : public Port
{
public:
  Port32Bit(MyOS::types::uint16_t portNumber);
  ~Port32Bit();
  virtual void Write(MyOS::types::uint32_t data);
  virtual MyOS::types::uint32_t Read();
};
}
}
#endif