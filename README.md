# OpenCR 다이나믹셀 XL430-W250 제어 프로젝트

이 프로젝트는 무거운 공식 Dynamixel SDK를 사용하지 않고, 가벼운 맞춤형 **Dynamixel Protocol 2.0** 드라이버를 구현하여 OpenCR 1.0 보드(STM32F746)로 모바일 로봇(터틀봇3 버거 사양)을 제어합니다.

## 하드웨어 설정 (Hardware Setup)
- **제어 보드:** OpenCR 1.0 (STM32F746)
- **모터:** 2x 다이나믹셀 XL430-W250 (ID 1: 왼쪽 바퀴, ID 2: 오른쪽 바퀴)
- **로봇 사양:** 터틀봇3 버거 (바퀴 간격: 160mm, 바퀴 지름: 66mm)
- **통신 설정:** UART4 (1,000,000 bps) / USB CDC (Virtual COM Port)

## 주요 기능 및 구현 (Features)
1. **차동 구동 기구학 (Differential Drive Kinematics)**:
   - ROS 2의 `/cmd_vel`과 호환되는 선속도(Linear) 및 각속도(Angular) 입력을 개별 바퀴 속도(RPM)로 변환합니다.
   - 100Hz 타이머 인터럽트 내에서 실시간 기구학 계산 및 모터 제어가 수행됩니다.

2. **PC 기반 원격 제어 (Teleoperation)**:
   - 전용 Python 스크립트(`teleop_opencr.py`)를 통해 키보드(`w, a, s, d, x`)로 로봇을 실시간 제어할 수 있습니다.
   - USB CDC(Virtual COM Port)를 통한 텍스트 기반 통신 프로토콜을 사용합니다.

3. **강력한 명령 파싱 (Robust Command Parsing)**:
   - 임베디드 환경에서 부동 소수점 라이브러리 의존성을 최소화하기 위해 수동 문자열 분해 및 `atof`를 이용한 안정적인 파싱 로직을 구현했습니다.

4. **안전 기능 (Safety Features)**:
   - **Emergency Stop:** 보드의 물리 버튼(Button 1)을 눌러 즉시 모든 속도를 0으로 초기화할 수 있습니다.
   - **Debug Feedback:** 명령 수신 시 보드의 파란색 LED(LED2)가 토글되어 통신 상태를 직관적으로 확인할 수 있습니다.

## 사용 방법 (Usage)

### 1. 빌드 및 업로드
```bash
cd build && make -j4 && arm-none-eabi-objcopy -O binary opencr.elf opencr.bin
sudo dfu-util -a 0 -s 0x08000000:leave -D opencr.bin
```

### 2. PC 제어 스크립트 실행
```bash
# Python 시리얼 라이브러리 설치 (필요시)
pip install pyserial

# 제어 스크립트 실행
python3 teleop_opencr.py
```

## 주요 개선 사항 (Improvements)
- **Non-blocking 구조:** 10ms 주기(100Hz)의 타이머 인터럽트 기반 설계로 통신 지연이나 시스템 멈춤 현상을 완전히 해결했습니다.
- **확장성:** USB CDC 인터페이스를 통해 ROS 2 노드와의 연동이 매우 용이합니다.
- **안정성:** `volatile` 속성과 전용 링 버퍼를 사용하여 USB 데이터 수신 누락을 방지했습니다.