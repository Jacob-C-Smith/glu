export TARGET=i686-elf
export PATH="$HOME/opt/cross/bin:$PATH"
export AS=$HOME/opt/cross/bin/$TARGET-as
export CC=$HOME/opt/cross/bin/$TARGET-gcc
export OBJDIR=build

$AS src/x86/boot.s -o boot.o &&
$CC -ggdb -m32 -c src/kernel.c       -o kernel.o   -std=gnu99 -ffreestanding -O2 -Wall -Wextra &&
$CC -ggdb -m32 -c src/glu/io.c       -o io.o       -std=gnu99 -ffreestanding -O2 -Wall -Wextra &&
$CC -ggdb -m32 -c src/glu/mem.c      -o mem.o      -std=gnu99 -ffreestanding -O2 -Wall -Wextra &&
$CC -ggdb -m32 -c src/glu/teletype.c -o teletype.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra &&
$CC -ggdb -m32 -c src/x86/VGA.c      -o VGA.o      -std=gnu99 -ffreestanding -O2 -Wall -Wextra &&
$CC -ggdb -m32 -c src/x86/GDT.c      -o GDT.o      -std=gnu99 -ffreestanding -O2 -Wall -Wextra &&
$CC -ggdb -m32 -T scripts/linker.ld -o glu.bin -ffreestanding -O2 -nostdlib boot.o kernel.o io.o mem.o teletype.o VGA.o GDT.o &&

cp glu.bin isodir/boot/glu.bin &&
grub-mkrescue -o glu.iso isodir #&&
#qemu-system-i386 -cdrom glu.iso -s 1>/dev/null 2>/dev/null &
#sleep 0.5 && gdb -ex 'target remote localhost:1234'
#kill $?
