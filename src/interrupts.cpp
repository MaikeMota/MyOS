#include <interrupts.h>

void printf(char *str);

InterruptHandler::InterruptHandler(uint8_t interruptNumber, InterruptManager *interruptManager)
{
    this->interruptNumber = interruptNumber;
    this->interruptManager = interruptManager;
    interruptManager->handlers[interruptNumber] = this;
}

InterruptHandler::~InterruptHandler()
{
    if (interruptManager->handlers[interruptNumber] = this)
    {
        interruptManager->handlers[interruptNumber] = 0;
    }
}

uint32_t InterruptHandler::HandleInterrupt(uint32_t esp)
{
    return esp;
}

InterruptManager::GateDescriptor InterruptManager::InterruptDescriptorTable[256];
InterruptManager *InterruptManager::ActiveInterruptManager = 0;

void InterruptManager::SetInterruptDescriptorTableEntry(
    uint8_t interruptNumber,
    uint16_t codeSegment,
    void (*handler)(),
    uint8_t descrtiptorPrivilegeLevel,
    uint8_t descriptorType)
{
    const uint8_t IDT_DESC_PRESENT = 0x80;

    InterruptDescriptorTable[interruptNumber].handlerAddressLowBits = ((uint32_t)handler) & 0xFFFF;
    InterruptDescriptorTable[interruptNumber].handlerAddressHighBits = (((uint32_t)handler) >> 16) && 0xFFFF;
    InterruptDescriptorTable[interruptNumber].gdt_codeSegmentSelector = codeSegment;
    InterruptDescriptorTable[interruptNumber].access = IDT_DESC_PRESENT | ((descrtiptorPrivilegeLevel & 3) << 5) | descriptorType;
    InterruptDescriptorTable[interruptNumber].reserved = 0;
}

InterruptManager::InterruptManager(GlobalDescriptorTable *gdt)
    : picMasterCommand(0x20),
      picMasterData(0x21),
      picSlaveCommand(0xA0),
      picSlaveData(0xA1)
{

    printf("\nInterruptManager Constructor");
    uint16_t codeSegment = gdt->CodeSegmentSelector();

    const uint8_t IDT_INTERRUPT_GATE = 0xE;

    printf("\nStarted Registering Interrupts");
    for (uint16_t i = 255; i > 0; --i)
    {
        SetInterruptDescriptorTableEntry(i, codeSegment, &IgnoreInterruptRequest, 0, IDT_INTERRUPT_GATE);
        handlers[i] = 0;
    }
    printf("\nEnded Registering Interrupts");

    SetInterruptDescriptorTableEntry(0, codeSegment, &IgnoreInterruptRequest, 0, IDT_INTERRUPT_GATE);
    handlers[0] = 0;

    printf("\nEnable Timer Interrupt");
    SetInterruptDescriptorTableEntry(0x20, codeSegment, &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
    printf("\nEnable Keyboard Interrupt");
    SetInterruptDescriptorTableEntry(0x21, codeSegment, &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);

    picMasterCommand.Write(0x11);
    picSlaveCommand.Write(0x11);

    picMasterData.Write(0x20);
    picSlaveData.Write(0x28);

    picMasterData.Write(0x04);
    picSlaveData.Write(0x02);

    picMasterData.Write(0x01);
    picSlaveData.Write(0x01);

    picMasterData.Write(0x00);
    picSlaveData.Write(0x00);
    InterruptDescriptorTablePointer idt_pointer;
    idt_pointer.size = 256 * sizeof(GateDescriptor) - 1;
    idt_pointer.base = (uint32_t)InterruptDescriptorTable;

    printf("\nLoading Interrupt Descriptor to Processor");
    asm volatile("lidt %0"
                 :
                 : "m"(idt_pointer)); //loads the interrupt descriptor table to processor

}

InterruptManager::~InterruptManager()
{
    Deactivate();
}

void InterruptManager::Activate()
{
    if (ActiveInterruptManager != 0)
    {
        printf("\nInterrupts already Activated");
        ActiveInterruptManager->Deactivate();
    }
    ActiveInterruptManager = this;
    printf("\nEnabling Interrupts");
    asm("sti");
    printf("\nInterrupts Enabled!");
}

void InterruptManager::Deactivate()
{
    if (ActiveInterruptManager == this)
    {
        ActiveInterruptManager = 0;
        printf("\nDisabling Interrupts");
        asm("cli");
    }
}

uint32_t InterruptManager::HandleInterrupt(uint8_t interruptNumber, uint32_t esp)
{
    printf("\nHandleInterrupt");
    if (ActiveInterruptManager != 0)
    {
        return ActiveInterruptManager->DoHandleInterrupt(interruptNumber, esp);
    }
    return esp;
}

uint32_t InterruptManager::DoHandleInterrupt(uint8_t interruptNumber, uint32_t esp)
{

    if (handlers[interruptNumber] != 0)
    {
        esp = handlers[interruptNumber]->HandleInterrupt(esp);
    }
    else if (interruptNumber != 0x20)
    {
        char *foo = "\nUNHANDLED INTERRUPT 0x00";
        char *hex = "0123456789ABCDEF";
        foo[22] = hex[(interruptNumber >> 4) & 0x0F];
        foo[22] = hex[interruptNumber & 0x0F];
        printf(foo);
    }
    if (0x20 <= interruptNumber && interruptNumber < 0x30)
    {
        picMasterCommand.Write(0x20);
        if (0x28 <= interruptNumber)
        {
            picSlaveCommand.Write(0x20);
        }
    }
    return esp;
}