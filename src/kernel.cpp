#include <common/types.h>
#include <gdt.h>
#include <hardwarecommunication/interrupts.h>
#include <hardwarecommunication/pci.h>
#include <drivers/driver.h>
#include <drivers/vga.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>

using namespace MyOS;
using namespace MyOS::types;
using namespace MyOS::drivers;
using namespace MyOS::hardwarecommunication;

typedef void (*constructor)();

extern "C" constructor start_ctors;
extern "C" constructor end_ctors;

extern "C" void callConstructors()
{
    for (constructor *i = &start_ctors; i != &end_ctors; i++)
    {
        (*i)();
    }
}

void printf(char *str);
void printfHex(uint8_t);

class PrintfKeyboardEventHandler : public KeyboardEventHandler
{
  public:
    void OnKeyDown(char key)
    {
        char *value = " ";
        value[0] = key;
        printf(value);
    }
};

class MouseToConsole : public MouseEventHandler
{

    uint8_t x, y;

  public:
    void OnActivate()
    {

        uint8_t x = 40, y = 12;

        static uint16_t *VideoMemory = (uint16_t *)0xB8000;
        VideoMemory[80 * y + x] = ((VideoMemory[80 * y + x] & 0xF000) >> 4) | ((VideoMemory[80 * y + x] & 0x0F00) << 4) | (VideoMemory[80 * y + x] & 0x00FF);
    }
    void OnMouseMove(int xOffset, int yOffset)
    {
        static uint16_t *VideoMemory = (uint16_t *)0xB8000;

        VideoMemory[80 * y + x] = ((VideoMemory[80 * y + x] & 0xF000) >> 4) | ((VideoMemory[80 * y + x] & 0x0F00) << 4) | (VideoMemory[80 * y + x] & 0x00FF);

        x += xOffset;

        if (x < 0)
        {
            x = 0;
        }
        else if (x > 80)
        {
            x = 80;
        }

        y -= yOffset;

        if (y < 0)
        {
            y = 0;
        }
        else if (y > 25)
        {
            y = 24;
        }

        VideoMemory[80 * y + x] = ((VideoMemory[80 * y + x] & 0xF000) >> 4) | ((VideoMemory[80 * y + x] & 0x0F00) << 4) | (VideoMemory[80 * y + x] & 0x00FF);
    }
    void OnMouseDown(uint8_t button) {}
};

extern "C" void kernelMain(void *multiboot_structure, uint32_t magicNumber)
{
    printf("Bem vindo ao MyOS 0.0.2\n");
    printf("Created by: Maike Mota");

    printf("\n\nInitializing Hardware Stage 1");
    GlobalDescriptorTable gdt;
    InterruptManager interrupts(0x20, &gdt);

    DriverManager driverManager;

    PrintfKeyboardEventHandler keyboardEventHandler;
    KeyboardDriver keyboard(&interrupts, &keyboardEventHandler);
    driverManager.AddDriver(&keyboard);

    MouseToConsole mouseToConsole;
    MouseDriver mouse(&interrupts, &mouseToConsole);
    driverManager.AddDriver(&mouse);

    PeripheralComponentInterconnectController pciController;
    pciController.SelectDrivers(&driverManager, &interrupts);

    VideoGraphicsArray vga;

    printf("\n\nInitializing Hardware Stage 2\n");
    driverManager.ActivateAll();

    printf("\n\nInitializing Hardware Stage 3\n");
    interrupts.Activate();

    printf("\nSystem is Loaded! Have fun!");

    for (uint32_t y = 0; y < 0xFFFFFF; y++)
        ;

    vga.SetMode(320, 200, 8);

    for (uint32_t x = 0; x < 160; x++)
    {
        for (uint32_t y = 0; y < 25; y++)
        {
            vga.PutPixel(x, y, 0x01);
        }
    }
    for (uint32_t x = 0; x < 160; x++)
    {
        for (uint32_t y = 25; y < 50; y++)
        {
            vga.PutPixel(x, y, 0x02);
        }
    }

    for (uint32_t x = 160; x < 320; x++)
    {
        for (uint32_t y = 0; y < 25; y++)
        {
            vga.PutPixel(x, y, 0x03);
        }
    }

    for (uint32_t x = 160; x < 320; x++)
    {
        for (uint32_t y = 25; y < 50; y++)
        {
            vga.PutPixel(x, y, 0x04);
        }
    }

    /*  for (uint32_t y = 0; y < 5; y++)
    {
        for (uint32_t x = 0; x < 160; x++)
        {
            vga.PutPixel(x, y, 0x00, 0xA8, 0x00);
        }
    }*/

    while (1)
    {
    }
}

void printf(char *str)
{
    static uint16_t *VideoMemory = (uint16_t *)0xB8000;

    static uint8_t x = 0, y = 0;

    for (int i = 0; str[i] != '\0'; ++i)
    {
        switch (str[i])
        {
        case '\n':
        {
            x = 0;
            y++;
            break;
        }
        /*case '\r':
        {
            x = 0;
            break;
        }*/
        default:
            VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFFFFFF00) | str[i];
            x++;
            break;
        }

        if (x >= 80)
        {
            x = 0;
            y++;
        }

        if (y >= 25)
        {
            for (y = 0; y < 25; y++)
                for (x = 0; x < 80; x++)
                    VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFFFFFF00) | ' ';
            x = 0;
            y = 0;
        }
    }
}

void printfHex(MyOS::types::uint8_t key)
{
    char *foo = "00";
    char *hex = "0123456789ABCDEF";
    foo[0] = hex[(key >> 4) & 0xF];
    foo[1] = hex[key & 0xF];
    printf(foo);
}
