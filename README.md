1. LED

2. 버튼

3. 시리얼

4. IMU

5. 모터




gemini


mkdir -p build && cd build && cmake…




 DFU 모드로 전환(Boot 버튼 누른 채로
  Reset)

sudo dfu-util -a 0 -s 0x08000000:leave -D build/opencr.bin


GPIO_PIN_RESET 0 켜짐

GPIO_PIN_SET 1 꺼짐

Active-Low(0일 때 켜짐)


