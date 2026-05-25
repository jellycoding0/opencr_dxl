# OpenCR 1.0 하드웨어 및 통신 설정 가이드

본 문서는 수차례의 시행착오 끝에 검증된 OpenCR 1.0의 하드웨어 설정 정석을 담고 있습니다. **향후 모든 작업은 이 설정을 기준으로 합니다.**

## 1. 클럭 설정 (Clock Configuration) - 정밀 48MHz 확보
USB CDC 장치 인식 실패를 방지하기 위해 24MHz 외부 크리스탈(HSE)을 사용한 정밀 클럭 세팅이 필수입니다.

*   **HSE Input**: 24 MHz
*   **PLLM**: / 24
*   **PLLN**: 432
*   **PLLP**: / 2 (SYSCLK: 216MHz)
*   **PLLQ**: / 9 (USB: 48MHz - **오차 0%**)
*   **APB1/APB2**: 각각 / 4, / 2 (주변장치 클럭 제한 준수)

## 2. USB 물리적 활성화 (PC14)
OpenCR 1.0 보드는 `PC14` (User Label: `USB_SW`) 핀을 **HIGH**로 설정해야 PC가 USB 연결을 감지할 수 있습니다.

```c
/* USER CODE BEGIN 2 */
// OpenCR 1.0 보드의 USB VBUS / Pull-up 라인을 활성화합니다.
HAL_GPIO_WritePin(USB_SW_GPIO_Port, USB_SW_Pin, GPIO_PIN_SET);
HAL_Delay(100); 
/* USER CODE END 2 */
```
*   **주의**: 반드시 `MX_USB_DEVICE_Init()` 호출 **직전**에 실행되어야 함.

## 3. 다이나믹셀 통신 (DYNAMIXEL)
모터 제어 시 아래의 검증된 설정을 절대 변경하지 마십시오.

*   **핀맵**:
    *   **전원(DXL_PWR_EN)**: `PF15` (High=ON)
    *   **방향(DXL_DIR)**: `PC9` (High=TX, Low=RX)
    *   **UART**: UART4 (1,000,000 bps)
*   **CRC**: Protocol 2.0용 **256칸 전체 테이블** 필수 사용. (약소화 버전 금지)

## 4. 사용자 인터페이스
*   **SW1 (사용자 버튼 1)**: `PC12` (Active LOW)
*   **Status LED**: `PG9` (Blink 확인용)
