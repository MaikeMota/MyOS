GCCPARAMS = -m32 -Iinclude -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings -O0
ASPARAMS = --32
LDPARAMS = -melf_i386

objects = obj/loader.o \
		obj/gdt.o \
		obj/hardwarecommunication/port.o \
		obj/hardwarecommunication/interruptstubs.o \
		obj/hardwarecommunication/interrupts.o \
		obj/hardwarecommunication/pci.o \
		obj/drivers/driver.o \
		obj/drivers/keyboard.o \
		obj/drivers/mouse.o \
		obj/kernel.o

obj/%.o: src/%.cpp
	mkdir -p $(@D)
	gcc $(GCCPARAMS) -c -o $@ $<

obj/%.o: src/%.s
	mkdir -p $(@D)
	as $(ASPARAMS) -o $@ $<

mykernel.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

run: mykernel.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp $< iso/boot/
	echo 'set timeout=0' >> iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo '' >> iso/boot/grub/grub.cfg
	echo 'menuentry "MyOS - By Maike Mota ;D" {' >> iso/boot/grub/grub.cfg
	echo '	multiboot /boot/mykernel.bin' >> iso/boot/grub/grub.cfg
	echo '	boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg	
	grub-mkrescue --output=bin/mykernel.iso iso
	rm -rf iso
	rm -rf mykernel.bin

run-linux: mykernel.iso
	(killall VirtualBox && sleep 1) || true
	VirtualBox --startvm "MyOS" &

.PHONY: clean
clean: 
	rm -rf obj mykernel.bin bin/mykernel.iso