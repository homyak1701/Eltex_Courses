Запуск ядра без файловой системы

qemu-system-arm -nographic -machine vexpress-a9 -kernel zImage -dtb vexpress-v2p-ca9.dtb -append "console=ttyAMA0"