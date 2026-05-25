# OpenCR (STM32F7xx) Pin Mapping Table
본 문서는 업로드된 OpenCR 회로도 및 핀 매핑 엑셀 데이터를 기반으로 정리한 핀 정보 일람입니다.

참고
https://github.com/ROBOTIS-GIT/OpenCR-Hardware/blob/master/Schematic/OpenCR_REVH.pdf
OpenCR_REVH.png
---

## 1. GPIO Port별 상세 매핑

### █ PORT A (PA)
| 핀 번호 (Pin No.) | 포트/핀 이름 (GPIO) | 신호/넷 이름 (Signal / Net) | 비고 |
| :---: | :--- | :--- | :--- |
| 34 | PA0-WKUP | ARD_A0 | |
| 35 | PA1 | ICM_SPI_INT | |
| 36 | PA2 | ARD_D6 | |
| 37 | PA3 | ARD_D9 | |
| 40 | PA4 | ICM_SPI_CS | |
| 41 | PA5 | ICM_SPI_CLK | |
| 42 | PA6 | ICM_SPI_MISO | |
| 43 | PA7 | QLLO_S3 | |
| 100 | PA8 | ARD_D5 | |
| 101 | PA9 | ARD_D13 | |
| 102 | PA10 | OTG_FS_ID | |
| 103 | PA11 | OTG_FS_N | |
| 104 | PA12 | OTG_FS_P | |
| 105 | PA13 | JTMS_SWDIO | |
| 109 | PA14 | JTCK_SWCLK | |
| 110 | PA15 | JTDI | |

### █ PORT B (PB)
| 핀 번호 (Pin No.) | 포트/핀 이름 (GPIO) | 신호/넷 이름 (Signal / Net) | 비고 |
| :---: | :--- | :--- | :--- |
| 46 | PB0 | QLLO_S5 | |
| 47 | PB1 | QLLO_S6 | |
| 48 | PB2 | - | |
| 133 | PB3 | JTDO_SWO | |
| 134 | PB4 | ARD_D3 | |
| 135 | PB5 | ICM_SPI_SDI | |
| 136 | PB6 | - | |
| 137 | PB7 | ARD_D14 | |
| 139 | PB8 | ARD_D15 | |
| 140 | PB9 | ARD_D10 | |
| 69 | PB10 | TEST_PIN1 | |
| 70 | PB11 | TEST_PIN2 | |
| 73 | PB12 | CAN_RX | |
| 74 | PB13 | CAN_TX | |
| 75 | PB14 | ARD_D12 | |
| 76 | PB15 | ARD_D11 | |

### █ PORT C (PC)
| 핀 번호 (Pin No.) | 포트/핀 이름 (GPIO) | 신호/넷 이름 (Signal / Net) | 비고 |
| :---: | :--- | :--- | :--- |
| 26 | PC0 | ADC_BAT_PWR_IN | |
| 27 | PC1 | ARD_D7 | |
| 28 | PC2 | ARD_D8 | |
| 29 | PC3 | TEST_PIN4 | |
| 44 | PC4 | QLLO_S8 | |
| 45 | PC5 | QLLO_S10 | |
| 96 | PC6 | ARD_D1 | |
| 97 | PC7 | ARD_D0 | |
| 98 | PC8 | QLLO_S1 | |
| 99 | PC9 | DXL_DIR | |
| 111 | PC10 | DXL_TXD | |
| 112 | PC11 | DXL_RXD | |
| 113 | PC12 | BUT_USER1 | |
| 7 | PC13-ANTI_TAMP | TEST_PIN3 | |

### █ PORT D (PD)
| 핀 번호 (Pin No.) | 포트/핀 이름 (GPIO) | 신호/넷 이름 (Signal / Net) | 비고 |
| :---: | :--- | :--- | :--- |
| 114 | PD0 | - | |
| 115 | PD1 | - | |
| 116 | PD2 | TEST_PIN4 | |
| 117 | PD3 | OTG_FS_VBUS | |
| 118 | PD4 | OTG_FS_OC | |
| 119 | PD5 | USER_UART2_TX | |
| 122 | PD6 | USER_UART2_RX | |
| 123 | PD7 | OTG_FS_PWR | |
| 77 | PD8 | TEST_PIN16 | |
| 78 | PD9 | TEST_PIN18 | |
| 79 | PD10 | QLLO_SLEEP | |
| 80 | PD11 | - | |
| 81 | PD12 | - | |
| 82 | PD13 | - | |
| 85 | PD14 | - | |
| 86 | PD15 | - | |

### █ PORT E (PE)
| 핀 번호 (Pin No.) | 포트/핀 이름 (GPIO) | 신호/넷 이름 (Signal / Net) | 비고 |
| :---: | :--- | :--- | :--- |
| 141 | PE0 | USER_UART8_RX | |
| 142 | PE1 | USER_UART8_TX | |
| 1 | PE2 | - | |
| 2 | PE3 | TEST_PIN5 | |
| 3 | PE4 | SYS_USER_LED3 | |
| 4 | PE5 | SYS_USER_LED2 | |
| 5 | PE6 | SW_USER2 | |
| 58 | PE7 | - | |
| 59 | PE8 | QLLO_S16 | |
| 60 | PE9 | QLLO_S20 | |
| 63 | PE10 | TEST_PIN7 | |
| 64 | PE11 | TEST_PIN8 | |
| 65 | PE12 | TEST_PIN9 | |
| 66 | PE13 | TEST_PIN10 | |
| 67 | PE14 | TEST_PIN11 | |
| 68 | PE15 | TEST_PIN12 | |

### █ PORT F (PF)
| 핀 번호 (Pin No.) | 포트/핀 이름 (GPIO) | 신호/넷 이름 (Signal / Net) | 비고 |
| :---: | :--- | :--- | :--- |
| 10 | PF0 | TEST_PIN13 | |
| 11 | PF1 | TEST_PIN14 | |
| 12 | PF2 | TEST_PIN15 | |
| 13 | PF3 | QLLO_S18 | |
| 14 | PF4 | TEST_PIN17 | |
| 15 | PF5 | QLLO_S13 | |
| 18 | PF6 | ARD_A5 | |
| 19 | PF7 | ARD_A4 | |
| 20 | PF8 | ARD_A3 | |
| 21 | PF9 | ARD_A2 | |
| 22 | PF10 | ARD_A1 | |
| 49 | PF11 | QLLO_S11 | |
| 50 | PF12 | QLLO_S15 | |
| 53 | PF13 | - | |
| 54 | PF14 | BUZZER_SIG | |
| 55 | PF15 | DXL_PWR_EN | |

### █ PORT G (PG)
| 핀 번호 (Pin No.) | 포트/핀 이름 (GPIO) | 신호/넷 이름 (Signal / Net) | 비고 |
| :---: | :--- | :--- | :--- |
| 56 | PG0 | - | |
| 57 | PG1 | - | |
| 87 | PG2 | TEST_PIN6 | |
| 88 | PG3 | BUT_USER2 | |
| 89 | PG4 | - | |
| 90 | PG5 | ARD_D2 | |
| 91 | PG6 | ARD_D4 | |
| 92 | PG7 | - | |
| 93 | PG8 | - | |
| 124 | PG9 | SYS_STS_LED | |
| 125 | PG10 | SYS_USER_LED4 | |
| 126 | PG11 | SW_USER1 | |
| 127 | PG12 | SYS_USER_LED1 | |
| 128 | PG13 | - | |
| 129 | PG14 | - | |
| 132 | PG15 | - | |

### █ 시스템 및 클럭 핀 (System & Clocks)
| 핀 번호 (Pin No.) | 포트/핀 이름 (Name) | 신호/넷 이름 (Signal) | 비고 |
| :---: | :--- | :--- | :--- |
| 8 | PC14-OSC32_IN | - | |
| 9 | PC15-OSC32_OUT | - | |
| 23 | PH0-OSC_IN | - | |
| 24 | PH1-OSC_OUT | - | |
| 25 | NRST | - | |
| 138 | BOOT0 | - | |
| 143 | PDR_ON | - | |

---

## 2. 주요 기능별 분류 (주요 신호 요약)

### 🔗 다이나믹셀 (Dynamixel) 제어 관련
* **PC9**: DXL_DIR (방향 제어)
* **PC10**: DXL_TXD (데이터 송신)
* **PC11**: DXL_RXD (데이터 수신)
* **PF15**: DXL_PWR_EN (다이나믹셀 전원 인에이블)

### 📡 아두이노 호환 핀 (Arduino Headers)
* **아날로그 입력**: PA0-WKUP (ARD_A0), PF10 (ARD_A1), PF9 (ARD_A2), PF8 (ARD_A3), PF7 (ARD_A4), PF6 (ARD_A5)
* **디지털 IO / PWM**: PC7 (ARD_D0), PC6 (ARD_D1), PG5 (ARD_D2), PB4 (ARD_D3), PG6 (ARD_D4), PA8 (ARD_D5), PA2 (ARD_D6), PC1 (ARD_D7), PC2 (ARD_D8), PA3 (ARD_D9), PB9 (ARD_D10), PB15 (ARD_D11), PB14 (ARD_D12), PA9 (ARD_D13), PB7 (ARD_D14), PB8 (ARD_D15)

### 🚨 시스템 LED 및 사용자 스위치
* **사용자 LED**: 
  * PG12 (SYS_USER_LED1)
  * PE4 (SYS_USER_LED2)
  * PE3 (SYS_USER_LED3)
  * PG10 (SYS_USER_LED4)
  * PG9 (SYS_STS_LED - 상태 표시)
* **사용자 스위치**: 
  * PC12 (BUT_USER1)
  * PG3 (BUT_USER2)
  * PG11 (SW_USER1)
  * PE6 (SW_USER2)

### 🔄 통신 인터페이스 (CAN, UART, USB, SPI)
* **CAN**: PB12 (CAN_RX), PB13 (CAN_TX)
* **IMU SPI (ICM-20648 등)**: PA1 (ICM_SPI_INT), PA4 (ICM_SPI_CS), PA5 (ICM_SPI_CLK), PA6 (ICM_SPI_MISO), PB5 (ICM_SPI_SDI)
* **USB OTG**: PA10 (OTG_FS_ID), PA11 (OTG_FS_N), PA12 (OTG_FS_P), PD3 (OTG_FS_VBUS), PD4 (OTG_FS_OC), PD7 (OTG_FS_PWR)
* **사용자 UART**: 
  * UART2: PD5 (USER_UART2_TX), PD6 (USER_UART2_RX)
  * UART8: PE1 (USER_UART8_TX), PE0 (USER_UART8_RX)

### 🔊 기타 인터페이스
* **부저**: PF14 (BUZZER_SIG)
* **배터리 전원 전압 체크 ADC**: PC0 (ADC_BAT_PWR_IN)
