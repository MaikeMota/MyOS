#ifndef __MYOS__HARDWARECOMMUNICATION__INTERRUPTS_H
#define __MYOS__HARDWARECOMMUNICATION__INTERRUPTS_H

#include <common/types.h>
#include <hardwarecommunication/port.h>
#include <gdt.h>

namespace MyOS
{
namespace hardwarecommunication
{
class InterruptManager;

class InterruptHandler
{
  protected:
     MyOS::types::uint8_t InterruptNumber;
    InterruptManager *interruptManager;
    InterruptHandler(InterruptManager *interruptManager,  MyOS::types::uint8_t InterruptNumber);
    ~InterruptHandler();

  public:
    virtual  MyOS::types::uint32_t HandleInterrupt( MyOS::types::uint32_t esp);
};

class InterruptManager
{
    friend class InterruptHandler;

  protected:
    static InterruptManager *ActiveInterruptManager;
    InterruptHandler *handlers[256];
    /*TaskManager *taskManager;*/

    struct GateDescriptor
    {
        MyOS::types::uint16_t handlerAddressLowBits;
        MyOS::types::uint16_t gdt_codeSegmentSelector;
        MyOS::types::uint8_t reserved;
        MyOS::types::uint8_t access;
        MyOS::types::uint16_t handlerAddressHighBits;
    } __attribute__((packed));

    static GateDescriptor interruptDescriptorTable[256];

    struct InterruptDescriptorTablePointer
    {
        MyOS::types::uint16_t size;
        MyOS::types::uint32_t base;
    } __attribute__((packed));

     MyOS::types::uint16_t hardwareInterruptOffset;
    static void SetInterruptDescriptorTableEntry(MyOS::types::uint8_t interrupt,
                                                 MyOS::types::uint16_t codeSegmentSelectorOffset, void (*handler)(),
                                                 MyOS::types::uint8_t DescriptorPrivilegeLevel, MyOS::types::uint8_t DescriptorType);

    static void InterruptIgnore();

    static void HandleInterruptRequest0x00();
    static void HandleInterruptRequest0x01();
    static void HandleInterruptRequest0x02();
    /*    static void HandleInterruptRequest0x03();
    static void HandleInterruptRequest0x04();
    static void HandleInterruptRequest0x05();
    static void HandleInterruptRequest0x06();
    static void HandleInterruptRequest0x07();
    static void HandleInterruptRequest0x08();
    static void HandleInterruptRequest0x09();
    static void HandleInterruptRequest0x0A();
    static void HandleInterruptRequest0x0B();
    static void HandleInterruptRequest0x0C();
    static void HandleInterruptRequest0x0D();
    static void HandleInterruptRequest0x0E();
    static void HandleInterruptRequest0x0F();
    static void HandleInterruptRequest0x31();

    static void HandleInterruptRequest0x80();

    static void HandleException0x00();
    static void HandleException0x01();
    static void HandleException0x02();
    static void HandleException0x03();
    static void HandleException0x04();
    static void HandleException0x05();
    static void HandleException0x06();
    static void HandleException0x07();
    static void HandleException0x08();
    static void HandleException0x09();
    static void HandleException0x0A();
    static void HandleException0x0B();
    static void HandleException0x0C();
    static void HandleException0x0D();
    static void HandleException0x0E();
    static void HandleException0x0F();
    static void HandleException0x10();
    static void HandleException0x11();
    static void HandleException0x12();
    static void HandleException0x13();*/

    static MyOS::types::uint32_t HandleInterrupt(MyOS::types::uint8_t interrupt, MyOS::types::uint32_t esp);
    MyOS::types::uint32_t DoHandleInterrupt(MyOS::types::uint8_t interrupt, MyOS::types::uint32_t esp);

    MyOS::hardwarecommunication::Port8BitSlow programmableInterruptControllerMasterCommandPort;
    MyOS::hardwarecommunication::Port8BitSlow programmableInterruptControllerMasterDataPort;
    MyOS::hardwarecommunication::Port8BitSlow programmableInterruptControllerSlaveCommandPort;
    MyOS::hardwarecommunication::Port8BitSlow programmableInterruptControllerSlaveDataPort;

  public:
    InterruptManager(MyOS::types::uint16_t hardwareInterruptOffset, MyOS::GlobalDescriptorTable *globalDescriptorTable /*, TaskManager *taskManager*/);
    ~InterruptManager();
    MyOS::types::uint16_t HardwareInterruptOffset();
    void Activate();
    void Deactivate();
};
}
}
#endif