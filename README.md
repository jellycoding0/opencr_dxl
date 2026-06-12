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

3. **타이머 인터럽트 기반 비차단 제어 (Timer-based Non-blocking Control)**:
   - 기존의 `while(1)` 루프와 `HAL_Delay()` 기반의 제어 방식을 **TIM3 타이머 인터럽트(100Hz, 10ms 주기)** 기반으로 완전히 재설계했습니다.
   - 모든 제어 로직이 인터럽트 서비스 루틴(ISR) 내에서 동작하므로, 메인 루프는 비워져 있으며 다른 비동기 작업에 CPU 자원을 할당할 수 있습니다.

4. **정밀한 버튼 제어 및 디바운싱 (Button Control & Debouncing)**:
   - **소프트웨어 디바운싱:** `HAL_Delay` 없이 10ms 주기의 타이머 카운터를 사용하여 50ms 동안 버튼 상태가 유지될 때만 입력을 인정하는 정밀한 상태 머신 방식을 사용합니다.
   - **Button 1 (SW1 / PC12)**: 
     - 토글 동작: 정방향 회전(속도: 200) ↔ 정지.
   - **Button 2 (SW2 / PG3)**:
     - 토글 동작: 역방향 회전(속도: -200) ↔ 정지.

5. **시스템 상태 표시 (System Heartbeat)**:
   - User LED 1 (PG12)이 타이머 카운터를 통해 500ms 간격으로 깜빡이며 시스템이 정상 동작 중임을 나타냅니다.

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

3. `dfu-util`을 사용하여 펌웨어를 업로드합니다:
   ```bash
   sudo dfu-util -a 0 -s 0x08000000:leave -D build/opencr.bin
   ```

## 주요 개선 사항 (Improvements)
- **안정성 향상:** 인터럽트 내에서 `HAL_Delay()`를 사용하지 않는 Non-blocking 설계를 통해 시스템이 멈추는(Hang) 현상을 방지했습니다.
- **CPU 효율성:** 제어 루프를 특정 주기로 고정(100Hz)하여 불필요한 UART 통신 부하를 줄이고 전력 효율을 높였습니다.
- **확장성:** 메인 루프가 자유로워짐에 따라 향후 USB 통신이나 센서 데이터 처리 등 추가 기능을 병렬로 구현하기 용이합니다.



cd ~/ws/opencr_dxl/build && make -j4 && arm-none-eabi-objcopy -O binary opencr.elf opencr.bin
cd .. && sudo dfu-util -a 0 -s 0x08000000:leave -D build/opencr.bin