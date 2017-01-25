#include <types.h>
#include <gdt.h>
#include <interrupts.h>
#include <keyboard.h>
#include <mouse.h>
#include <driver.h>

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

extern "C" void kernelMain(void *multiboot_structure, uint32_t magicNumber)
{
    printf("Bem vindo ao MyOS 0.0.2\n");
    printf("Created by: Maike Mota");

    printf("\n\nInitializing Hardware Stage 1");
    GlobalDescriptorTable gdt;
    InterruptManager interrupts(0x20, &gdt);

    DriverManager driverManager;

    printf("\nInitializing Keyboard Driver...");
    KeyboardDriver keyboard(&interrupts);
    driverManager.AddDriver(&keyboard);

    printf("\nRegistering Mouse Driver...");
    MouseDriver mouse(&interrupts);
    driverManager.AddDriver(&mouse);

    printf("\n\nInitializing Hardware Stage 2");
    driverManager.ActivateAll();

    printf("\n\nInitializing Hardware Stage 3");
    interrupts.Activate();
    printf("\nSystem is Loaded! Have fun!");

    while (1)
    {
    }
}

void printf(char *str)
{
    static uint16_t *VideoMemory = (uint16_t *)0xb8000;

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
            VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | str[i];
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
                    VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | ' ';
            x = 0;
            y = 0;
        }
    }
}

void printfHex(uint8_t key)
{
    char *foo = "00";
    char *hex = "0123456789ABCDEF";
    foo[0] = hex[(key >> 4) & 0xF];
    foo[1] = hex[key & 0xF];
    printf(foo);
}