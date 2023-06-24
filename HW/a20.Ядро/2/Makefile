
.PHONY := clean

bin1 := client
bin2 := serv
CC:= gcc

all: arm

arm:
		arm-linux-gnueabihf-gcc -static lib.c -o init
		echo init | cpio -o -H newc | gzip > initramfs.cpio.gz	

run:
		QEMU_AUDIO_DRV=none qemu-system-arm -nographic -machine vexpress-a9 -kernel zImage -dtb vexpress-v2p-ca9.dtb -append "console=ttyAMA0" -initrd initramfs.cpio.gz

clean: 
		rm -rf init *.gz
		

