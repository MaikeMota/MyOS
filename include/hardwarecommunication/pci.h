#ifndef __MYOS__HARDWARECOMMUNICATION__PCI_H
#define __MYOS__HARDWARECOMMUNICATION__PCI_H

#include <common/types.h>
#include <hardwarecommunication/port.h>
#include <hardwarecommunication/interrupts.h>
#include <drivers/driver.h>

namespace MyOS
{
namespace hardwarecommunication
{
class PeripheralComponentInterconnectDeviceDescriptor
{
public:
  MyOS::types::uint32_t basePort;
  MyOS::types::uint32_t interrupt;

  MyOS::types::uint16_t bus;
  MyOS::types::uint16_t device;
  MyOS::types::uint16_t function;

  MyOS::types::uint16_t vendor_id;
  MyOS::types::uint16_t device_id;

  MyOS::types::uint8_t class_id;
  MyOS::types::uint8_t subclass_id;
  MyOS::types::uint8_t interface_id;

  MyOS::types::uint8_t revision;

  PeripheralComponentInterconnectDeviceDescriptor();
  ~PeripheralComponentInterconnectDeviceDescriptor();
};

class PeripheralComponentInterconnectController
{
  MyOS::hardwarecommunication::Port32Bit dataPort;
  MyOS::hardwarecommunication::Port32Bit commandPort;

public:
  PeripheralComponentInterconnectController();
  ~PeripheralComponentInterconnectController();

  MyOS::types::uint32_t Read(MyOS::types::uint16_t bus, MyOS::types::uint16_t device, MyOS::types::uint16_t function, MyOS::types::uint32_t registerOffset);
  void Write(MyOS::types::uint16_t bus, MyOS::types::uint16_t device, MyOS::types::uint16_t function, MyOS::types::uint32_t registerOffset, MyOS::types::uint32_t value);
  bool DeviceHasFunctions(MyOS::types::uint16_t bus, MyOS::types::uint16_t device);

  void SelectDrivers(MyOS::drivers::DriverManager *driverManager);
  PeripheralComponentInterconnectDeviceDescriptor GetDeviceDescriptor(MyOS::types::uint16_t bus, MyOS::types::uint16_t device, MyOS::types::uint16_t function);
};
}
}
#endif