#include <drivers/driver.h>

using namespace MyOS::types;
using namespace MyOS::drivers;

void printf(char *);
void printfHex(uint8_t);

Driver::Driver()
{
}
Driver::~Driver()
{
}

void Driver::Activate()
{
}

void Driver::Deactivate()
{
}
int Driver::Reset()
{
    return 0;
}

DriverManager::DriverManager()
{
    numDrivers = 0;
}

void DriverManager::AddDriver(Driver *driver)
{
    drivers[numDrivers++] = driver;
}

void DriverManager::ActivateAll()
{
    for (int i = 0; i < numDrivers; i++)
    {
        drivers[i]->Activate();
        printf("\nActivated driver ");
        printfHex(i + 1);
        printf(" of ");
        printfHex(numDrivers);
    }
}

void DriverManager::DeactivateAll()
{
    for (int i = 0; i < numDrivers; i++)
    {
        drivers[i]->Deactivate();
        printf("\nDeactivate driver ");
        printfHex(i + 1);
        printf(" of ");
        printfHex(numDrivers);
    }
}