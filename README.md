# CMake를 이용해 Makefile을 생성하는 단계
mkdir -p build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=cmake/gcc-arm-none-eabi.cmake

# 코드 수정 후 빌드 및 바이너리 생성
cd ~/ws/opencr_dxl/build && make -j4 && arm-none-eabi-objcopy -O binary opencr.elf opencr.bin


cd .. && sudo dfu-util -a 0 -s 0x08000000:leave -D build/opencr.bin