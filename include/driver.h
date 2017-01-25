#ifndef __DRIVER_H
#define __DRIVER_H

#include <types.h>

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

#endif