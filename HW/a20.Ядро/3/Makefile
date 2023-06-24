
.PHONY := clean

bin1 := client
bin2 := serv
CC:= gcc

all: run
	
run:
		QEMU_AUDIO_DRV=none qemu-system-arm -nographic -machine vexpress-a9 -kernel zImage -dtb vexpress-v2p-ca9.dtb -append "console=ttyAMA0 rdinit=/bin/ash" -initrd initramfs.cpio.gz

clean: 
		rm -rf init *.gz
		

