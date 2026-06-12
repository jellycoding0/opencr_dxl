# OpenCR 다이나믹셀 XL430-W250 제어 프로젝트

이 프로젝트는 무거운 공식 Dynamixel SDK를 사용하지 않고, 가벼운 맞춤형 **Dynamixel Protocol 2.0** 드라이버를 구현하여 OpenCR 1.0 보드(STM32F746)로 두 개의 다이나믹셀 XL430-W250 모터를 제어합니다.

## 하드웨어 설정 (Hardware Setup)
- **제어 보드:** OpenCR 1.0 (STM32F746)
- **모터:** 2x 다이나믹셀 XL430-W250 (기본 ID: 1, 2)
- **통신 설정:** UART4 (PC10: TX, PC11: RX) / 통신 속도: 1,000,000 bps
- **방향 제어 핀 (Direction):** PC9 (High: 송신 TX, Low: 수신 RX)
- **모터 전원 핀 (Power Enable):** PF15 (Active High)

## 주요 기능 및 구현 (Features)
1. **맞춤형 Protocol 2.0 드라이버 (`dynamixel.c`, `dynamixel.h`)**:
   - UART `TC (Transmission Complete)` 플래그를 사용하여 반이중(Half-duplex) 통신의 방향 전환 핀(PC9) 타이밍을 정밀하게 제어합니다.
   - Protocol 2.0 규격에 맞는 공식 CRC-16-IBM 계산 알고리즘이 포함되어 있습니다.
   - 필수 명령어인 `Ping`, `Read`, `WriteByte`, `WriteWord`를 지원합니다.

2. **초기화 시퀀스 (Initialization)**:
   - 전원 인가 후 모터 부팅에 필요한 지연 시간을 자동으로 처리합니다.
   - 모터를 무한 회전이 가능한 **속도 제어 모드 (Velocity Control Mode, 값: 1)** 로 자동 설정합니다.
   - 브로드캐스트 ID (0xFE)를 사용하여 연결된 모든 모터의 토크를 한 번에 활성화합니다.

3. **물리 버튼 제어 (Button Control)**:
   - 보드에 내장된 User Button 2개를 사용하여 모터를 제어합니다. (Active Low 방식)
   - **Button 1 (SW1 / PC12)**: 
     - 한 번 클릭: 두 모터가 **정방향**으로 회전합니다. (속도: 200)
     - 다시 클릭: 두 모터가 **정지**합니다.
   - **Button 2 (SW2 / PG3)**:
     - 한 번 클릭: 두 모터가 **역방향**으로 회전합니다. (속도: -200)
     - 다시 클릭: 두 모터가 **정지**합니다.

4. **상태 표시 (Status Indication)**:
   - User LED 1 (PG12)이 500ms 간격으로 깜빡이며 메인 제어 루프가 정상적으로 동작 중임을 알립니다.

## 빌드 및 업로드 (Building and Flashing)

1. `make` 명령어를 사용하여 프로젝트를 빌드합니다:
   ```bash
   cd build
   make -j4
   ```

2. 생성된 `.elf` 파일을 펌웨어 업로드를 위한 `.bin` 파일로 변환합니다:
   ```bash
   arm-none-eabi-objcopy -O binary opencr.elf opencr.bin
   ```

3. 생성된 `.bin` 파일을 STM32CubeProgrammer 등을 사용하여 OpenCR 보드에 업로드합니다.

## 주의 사항 (Limitations)
- 현재 드라이버는 `Read`나 `Ping` 동작 시 모터가 보내는 Status Packet 내부의 세부 에러 플래그(Error flag)를 깊게 검사하지 않습니다.
- 버튼 디바운싱(Debouncing) 로직과 제어 루프는 `while(1)` 문 안에서 `HAL_Delay()`를 이용한 간단한 방식으로 구현되어 있습니다. 더 복잡하고 정밀한 제어가 필요한 경우 타이머 인터럽트(Timer Interrupt) 기반으로 변경하는 것을 권장합니다.


cd ~/ws/opencr_dxl/build && make -j4 && arm-none-eabi-objcopy -O binary opencr.elf opencr.bin
cd .. && sudo dfu-util -a 0 -s 0x08000000:leave -D build/opencr.bin