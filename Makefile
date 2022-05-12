all: BootLoader Kernel32 Kernel64 ImageMaker Disk.img

BootLoader:
	echo "============== Build Boot Loader =============="
	make -C 00.BootLoader
	echo "============== Build Complete =============="

Kernel32:
	echo "============== Build 32bit Kernel =============="
	make -C 01.Kernel32
	echo "============== Build Complete =============="

Kernel64:
	echo "============== Build 64bit Kernel =============="
	make -C 02.Kernel64
	echo "============== Build Complete =============="

ImageMaker:
	make -C 04.Utility/00.ImageMaker

Disk.img: 00.BootLoader/BootLoader.bin 01.Kernel32/Kernel32.bin 02.Kernel64/Kernel64.bin
	echo ============== Disk Image Build Start ==============
	./04.Utility/00.ImageMaker/ImageMaker $^
	echo ============== All Build Complete ==============

clean:
	make -C 00.BootLoader clean
	make -C 01.Kernel32 clean
	make -C 02.Kernel64 clean
	make -C 04.Utility/00.ImageMaker
	rm -f Disk.img
