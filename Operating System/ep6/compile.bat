cd app

nasm -o stoneUL.com stoneUL.asm
nasm -o stoneUR.com stoneUR.asm
nasm -o stoneDL.com stoneDL.asm
nasm -o stoneDR.com stoneDR.asm

nasm -f elf32 string.asm -o stringASM.o
gcc -c -O3 -m16 -march=i386 -ffreestanding -fno-PIE -nostdlib -lgcc -fno-asynchronous-unwind-tables -fno-zero-initialized-in-bss -fno-common -mpreferred-stack-boundary=2 -masm=intel string.c -O3 -o stringC.o
gcc -c -O3 -m16 -march=i386 -ffreestanding -fno-PIE -nostdlib -lgcc -fno-asynchronous-unwind-tables -fno-zero-initialized-in-bss -fno-common -mpreferred-stack-boundary=2 -masm=intel -S string.c -O3 -o stringC.asm
ld3 --entry=_start -T ld.lds -o string.tmp stringASM.o stringC.o
objcopy -O binary string.tmp string.img

rm stringASM.o stringC.o string.tmp

cd ../kernel

nasm -o loader.com loader.asm
nasm -f elf32 kernel.asm -o kernelASM.o
gcc -c -O3 -m16 -march=i386 -ffreestanding -fno-PIE -nostdlib -lgcc -fno-asynchronous-unwind-tables -fno-zero-initialized-in-bss -fno-common -mpreferred-stack-boundary=2 -masm=intel kernel.c -O3 -o kernelC.o
gcc -c -O3 -m16 -march=i386 -ffreestanding -fno-PIE -nostdlib -lgcc -fno-asynchronous-unwind-tables -fno-zero-initialized-in-bss -fno-common -mpreferred-stack-boundary=2 -masm=intel -S kernel.c -O3 -o kernelC.asm
ld3 --entry=set_seg -T ld_kernel.lds -o kernel.tmp kernelASM.o kernelC.o
objcopy -O binary kernel.tmp kernel.img

cd ../lib

nasm -f elf32 lib.asm -o libASM.o
gcc -c -O3 -m16 -march=i386 -ffreestanding -fno-PIE -nostdlib -lgcc -fno-asynchronous-unwind-tables -fno-zero-initialized-in-bss -fno-common -mpreferred-stack-boundary=2 -masm=intel lib.c -O3 -o libC.o
gcc -c -O3 -m16 -march=i386 -ffreestanding -fno-PIE -nostdlib -lgcc -fno-asynchronous-unwind-tables -fno-zero-initialized-in-bss -fno-common -mpreferred-stack-boundary=2 -masm=intel -S lib.c -O3 -o libC.asm
gcc -c -O3 -m16 -march=i386 -ffreestanding -fno-PIE -nostdlib -lgcc -fno-asynchronous-unwind-tables -fno-zero-initialized-in-bss -fno-common -mpreferred-stack-boundary=2 -masm=intel test.c -O3 -o test.o
gcc -c -O3 -m16 -march=i386 -ffreestanding -fno-PIE -nostdlib -lgcc -fno-asynchronous-unwind-tables -fno-zero-initialized-in-bss -fno-common -mpreferred-stack-boundary=2 -masm=intel -S test.c -O3 -o test.asm
ld3 --entry=_start -T ld_test.lds -o test.tmp test.o libC.o libASM.o
objcopy -O binary test.tmp test.img