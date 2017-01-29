#include <hardwarecommunication/pci.h>

using namespace MyOS::types;
using namespace MyOS::drivers;
using namespace MyOS::hardwarecommunication;

PeripheralComponentInterconnectDeviceDescriptor::PeripheralComponentInterconnectDeviceDescriptor()
{
}

PeripheralComponentInterconnectDeviceDescriptor::~PeripheralComponentInterconnectDeviceDescriptor()
{
}

PeripheralComponentInterconnectController::PeripheralComponentInterconnectController()
    : dataPort(0xCFC),
      commandPort(0xCF8)
{
}

PeripheralComponentInterconnectController::~PeripheralComponentInterconnectController()
{
}

uint32_t PeripheralComponentInterconnectController::Read(uint16_t bus, uint16_t device, uint16_t function, uint32_t registerOffset)
{
    uint32_t identifier =
        0x1 << 31 | ((bus & 0xFF) << 16) | ((device & 0x1F) << 11) | ((function & 0x07) << 8) | (registerOffset & 0xFC);

    commandPort.Write(identifier);
    uint32_t result = dataPort.Read();
    return result >> (8 * (registerOffset % 4));
}

void PeripheralComponentInterconnectController::Write(uint16_t bus, uint16_t device, uint16_t function, uint32_t registerOffset, uint32_t value)
{
    uint32_t identifier =
        0x1 << 31 | ((bus & 0xFF) << 16) | ((device & 0x1F) << 11) | ((function & 0x07) << 8) | (registerOffset & 0xFC);

    commandPort.Write(identifier);
    dataPort.Write(value);
}

bool PeripheralComponentInterconnectController::DeviceHasFunctions(uint16_t bus, uint16_t device)
{
    return Read(bus, device, 0, 0x0E) & (1 << 7);
}

void printf(char *);
void printfHex(uint8_t);

void PeripheralComponentInterconnectController::SelectDrivers(DriverManager *driverManager)
{

    printf("\n");
    for (int bus = 0; bus < 8; bus++)
    {
        for (int device = 0; device < 32; device++)
        {
            int numFunctions = DeviceHasFunctions(bus, device) ? 8 : 1;
            for (int function = 0; function < numFunctions; function++)
            {
                PeripheralComponentInterconnectDeviceDescriptor descriptor = GetDeviceDescriptor(bus, device, function);
                if (descriptor.vendor_id == 0x0000 | descriptor.vendor_id == 0xFFFF)
                {
                    continue;
                }
                printf("PCI BUS ");
                printfHex(bus & 0xFF);
                printf(", DEVICE ");
                printfHex(device & 0xFF);
                printf(", FUNCTION ");
                printfHex(function & 0xFF);
                printf(", VENDOR ");
                printfHex((descriptor.vendor_id & 0xFF00) >> 8);
                printfHex((descriptor.vendor_id & 0xFF));
                printf(", DEVICE ");
                printfHex((descriptor.device_id & 0xFF00) >> 8);
                printfHex((descriptor.device_id & 0xFF));
                printf("\n");
            }
        }
    }
}

PeripheralComponentInterconnectDeviceDescriptor PeripheralComponentInterconnectController::GetDeviceDescriptor(uint16_t bus, uint16_t device, uint16_t function)
{
    PeripheralComponentInterconnectDeviceDescriptor result;

    result.bus = bus;
    result.device = device;
    result.function = function;

    result.vendor_id = Read(bus, device, function, 0x00);
    result.device_id = Read(bus, device, function, 0x02);

    result.class_id = Read(bus, device, function, 0x0B);
    result.subclass_id = Read(bus, device, function, 0x0A);
    result.interface_id = Read(bus, device, function, 0x09);

    result.revision = Read(bus, device, function, 0x08);
    result.interrupt = Read(bus, device, function, 0x3C);

    return result;
}