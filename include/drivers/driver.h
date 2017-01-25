#ifndef __MYOS_DRIVERS_DRIVER_H
#define __MYOS_DRIVERS_DRIVER_H

#include <common/types.h>

namespace MyOS
{
namespace drivers
{

class Driver
{
public:
  Driver();
  ~Driver();

  virtual void Activate();
  virtual void Deactivate();
  virtual int Reset();
};

class DriverManager
{
private:
  Driver *drivers[265];
  int numDrivers;

public:
  DriverManager();
  void ActivateAll();
  void DeactivateAll();
  void AddDriver(Driver *driver);
};
}
}
#endif