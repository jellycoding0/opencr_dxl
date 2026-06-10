cd ~/ws/opencr_dxl/build && make -j4 && arm-none-eabi-objcopy -O binary opencr.elf opencr.bin

sudo dfu-util -a 0 -s 0x08000000:leave -D build/opencr.bin
