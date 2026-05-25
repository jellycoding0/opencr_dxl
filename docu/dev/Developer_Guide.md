# OpenCR Pure STM32 Developer Guide (FINAL VERIFIED)

본 문서는 실측 및 검증을 완료한 OpenCR 보드의 실제 하드웨어 명세서입니다. **반드시 이 지침을 준수하여 개발하십시오.**

---

## 1. 하드웨어 핵심 규칙 (Confirmed)

### 🔴 LED 제어 로직: Active HIGH
이 보드의 LED는 표준 OpenCR과 달리 **Active HIGH** 방식으로 동작합니다.
- **`GPIO_PIN_SET (1)`**: LED 켜짐 (ON)
- **`GPIO_PIN_RESET (0)`**: LED 꺼짐 (OFF)
- **초기화**: `MX_GPIO_Init`에서 반드시 `RESET(0)`으로 초기화하여 부팅 시 소등 상태를 유지하십시오.

### 🔘 버튼 입력 로직: Pull-up (Active LOW)
모든 버튼은 누를 때 `LOW(0)` 신호가 발생합니다.
- **설정**: 반드시 내부 **`PULLUP`** 저항을 활성화하십시오.
- **상태**: `0` = 눌림 (Pressed), `1` = 떼짐 (Released)

---

## 2. 최종 확정 핀 매핑 (Ground Truth)

### 🤖 DYNAMIXEL (UART4)
| 신호명 | 핀 (GPIO) | 역할 |
| :--- | :--- | :--- |
| **TX** | **PC10** | 데이터 송신 |
| **RX** | **PC11** | 데이터 수신 |
| **DIR** | **PC9** | 방향 제어 (High: 송신, Low: 수신) |
| **PWR** | **PF15** | 모터 전원 (High: 공급) |

### 📟 버튼 & LED 통합 매핑 (Software Corrected)
물리적인 엇갈림을 소프트웨어에서 교정하여 아래와 같이 사용합니다.

| 입력 장치 | 핀 (GPIO) | 출력 LED | 핀 (GPIO) | 기능 |
| :--- | :--- | :--- | :--- | :--- |
| **PUSH SW1** | **PC12** | **LED 1** | **PG12** | 정회전 (Forward) |
| **PUSH PUSH SW2** | **PG3** | **LED 2** | **PE5** | 역회전 (Backward) |
| **SLIDE SW1** | **PG11** | **LED 3** | **PE4** | 슬라이드 1번 |
| **SLIDE SW2** | **PE6** | **LED 4** | **PG10** | 슬라이드 2번 |
| **-** | **-** | **Status** | **PG9** | 시스템 생존 신호 |

---

## 3. 소프트웨어 구현 가이드 (Protocol 2.0)

1.  **패킷 길이(Length)**: `Data Length + 5` 공식을 엄격히 준수하십시오.
    - Write 명령 예: `Addr(2) + Inst(1) + CRC(2) + Payload(N) = N + 5`
2.  **전송 완료 대기**: `HAL_UART_Transmit` 직후 반드시 `UART_FLAG_TC` 플래그가 `SET` 될 때까지 `while` 문으로 대기하십시오. 그렇지 않으면 반이중(Half-duplex) 스위칭 시 데이터가 유실됩니다.
3.  **데이터 순서**: 다이나믹셀은 **Little Endian**을 사용합니다. 4바이트 속도값 전송 시 하위 바이트부터 조립하십시오.
