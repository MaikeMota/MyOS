#include <types.h>
#include <gdt.h>
#include <interrupts.h>
#include <keyboard.h>

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

    GlobalDescriptorTable gdt;
    InterruptManager interrupts(&gdt);

    printf("\nRegistering Keyboard Driver");
    KeyboardDriver keyboard(&interrupts);

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