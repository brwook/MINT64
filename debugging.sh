#!/bin/bash
gdb-multiarch \
	-ex "symbol-file 01.Kernel32/Temp/Kernel32.elf" \
	-ex "symbol-file 02.Kernel64/Temp/Kernel64.elf" \
	-ex "b* 0x10200" \
	-ex "target remote :1234" \
	-ex "c"
#-ex "add-symbol-file Disk.img 0x7c00"
