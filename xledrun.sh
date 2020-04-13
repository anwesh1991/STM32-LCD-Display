export PATH="/opt/gcc-arm/bin:$PATH"
make
arm-none-eabi-objcopy -Obinary ledtest.elf ledtest.bin
st-flash write ledtest.bin 0x8000000

