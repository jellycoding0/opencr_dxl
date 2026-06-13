# OpenCR ROS 2 Mobile Robot LLC (Low-Level Controller)

이 프로젝트는 OpenCR 1.0(STM32F746) 보드를 사용하여 다이나믹셀 XL430-W250 기반 모바일 로봇(터틀봇3 버거 사양)을 위한 강력하고 정밀한 **하위 제어기(LLC)**를 구현합니다. ROS 2와 완벽하게 연동되며, 실시간 기구학 계산 및 오도메트리 피드백을 지원합니다.

## 하드웨어 사양 (Hardware Specification)
- **제어 보드:** OpenCR 1.0 (ARM Cortex-M7)
- **액추에이터:** 2x Dynamixel XL430-W250 (Protocol 2.0)
  - ID 1: 왼쪽 바퀴 (Left Wheel)
  - ID 2: 오른쪽 바퀴 (Right Wheel)
- **기구학 파라미터:**
  - 바퀴 간격 (Wheel Separation): 160mm
  - 바퀴 지름 (Wheel Diameter): 66mm
- **통신 포트:** 
  - 모터: UART4 (1,000,000 bps)
  - PC 연동: USB CDC (Virtual COM Port)

## 주요 기능 (Core Features)

### 1. 고성능 제어 루프 (Real-time Control)
- **100Hz 인터럽트 구동:** TIM3 타이머 인터럽트 기반의 비차단(Non-blocking) 설계를 통해 10ms 주기의 정밀한 속도 제어를 보장합니다.
- **안정적인 반이중 통신:** UART RX FIFO 자동 플러싱 및 방향 전환 타이밍 최적화를 통해 다이나믹셀과의 통신 신뢰성을 극대화했습니다.

### 2. ROS 2 통합 (ROS 2 Integration)
- **전용 시리얼 브릿지 (`ros2_opencr_bridge.py`):**
  - `/cmd_vel` (geometry_msgs/Twist) 구독: 선속도/각속도 명령 수신.
  - `/odom` (nav_msgs/Odometry) 발행: 실시간 위치/자세 데이터 전송.
  - `TF` 발행: `odom -> base_footprint` 좌표계 변환 실시간 전송.
- **차동 구동 기구학:** 터틀봇 표준 좌표계에 맞춘 전방향/회전 운동 변환 로직이 내장되어 있습니다.

### 3. 정밀 오도메트리 (Precise Odometry)
- **20Hz 피드백:** 초당 20회 엔코더 값을 읽어 PC로 전송합니다.
- **Rollover 처리:** 0~4095 엔코더 값의 오버플로우/언더플로우를 완벽하게 처리하여 데이터 튐 현상이 없습니다.
- **부호 보정:** 모터 장착 방향에 따른 부호를 자동으로 보정하여 ROS 2 표준 좌표계와 일치시킵니다.

### 4. 안전 및 진단 (Safety & Diagnostics)
- **비상 정지:** 보드의 물리 버튼(SW1)을 눌러 즉시 하드웨어 수준에서 정지할 수 있습니다.
- **상태 표시:** 명령 수신 시 파란색 LED(LED2) 토글, 시스템 동작 시 주황색 LED(LED1) 점멸.

## 사용 방법 (Usage)

### 1. 펌웨어 빌드 및 업로드
OpenCR 보드를 DFU 모드로 전환한 후 실행하세요.
```bash
cd build
make -j4
arm-none-eabi-objcopy -O binary opencr.elf opencr.bin
sudo dfu-util -a 0 -s 0x08000000:leave -D opencr.bin
```

### 2. ROS 2 브릿지 실행
PC에서 OpenCR과 시리얼 통신을 연결하고 ROS 2 토픽을 생성합니다.
```bash
# 의존성 설치
pip install pyserial

# 브릿지 노드 실행
python3 ros2_opencr_bridge.py
```

### 3. 로봇 조종 테스트
새 터미널에서 명령을 내려보세요.
```bash
# 직진 명령 테스트
ros2 topic pub /cmd_vel geometry_msgs/msg/Twist "{linear: {x: 0.1, y: 0.0, z: 0.0}, angular: {x: 0.0, y: 0.0, z: 0.0}}"

# 오도메트리 데이터 확인
ros2 topic echo /odom
```

## 프로젝트 구조 (Project Structure)
- `Core/Src/main.c`: 제어 루프 및 시리얼 명령 파싱.
- `Core/Src/dynamixel.c`: Protocol 2.0 드라이버 핵심 로직.
- `ros2_opencr_bridge.py`: ROS 2와 OpenCR 사이의 데이터 가교 역할.
- `teleop_opencr.py`: (옵션) 키보드 기반의 간단한 테스트 스크립트.
