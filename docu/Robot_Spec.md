# Robot Specification & Kinematics Data

이 문서는 로봇의 물리적 제원과 오도메트리(Odometry) 계산에 필요한 기구학 상수를 정의합니다.

## 1. 구동계 사양 (Actuator)
*   **모델**: DYNAMIXEL XM430-W210-T (또는 호환 모델)
*   **프로토콜**: Protocol 2.0
*   **엔코더 분해능**: 4,096 [pulses/rev] (12-bit)
*   **기어비**: 1 : 1 (직구동 기준)

## 2. 기구학 상수 (Kinematic Constants)
오도메트리 계산 시 아래 상수값을 사용합니다.

| 항목 | 기호 | 값 | 단위 | 비고 |
| :--- | :---: | :--- | :--- | :--- |
| 바퀴 반지름 | $R$ | 0.033 | m | 66mm 바퀴 기준 |
| 바퀴 사이 거리 | $L$ | 0.160 | m | Wheel Track |
| 회전당 펄스 수 | $CPR$ | 4,096 | pulses | |
| 미터당 펄스 수 | - | 약 19,752 | pulses/m | $CPR / (2 \pi R)$ |

## 3. 좌표계 정의 (Coordinate System)
*   **기준점 (Origin)**: 두 바퀴의 중심점
*   **X축**: 전진 방향 (Forward)
*   **Y축**: 왼쪽 방향 (Left)
*   **Theta ($\theta$)**: 반시계 방향 회전 (Counter-Clockwise, CCW)

## 4. 오도메트리 공식 (Differential Drive)
10ms($\Delta t$) 주기로 아래 수식을 계산합니다.

1.  **각 바퀴의 이동 거리**: $\Delta s_L, \Delta s_R$
2.  **중심 이동 거리**: $\Delta s = (\Delta s_R + \Delta s_L) / 2$
3.  **회전각 변화량**: $\Delta \theta = (\Delta s_R - \Delta s_L) / L$
4.  **현재 위치 업데이트**:
    *   $x_{new} = x + \Delta s \cdot \cos(\theta + \Delta \theta / 2)$
    *   $y_{new} = y + \Delta s \cdot \sin(\theta + \Delta \theta / 2)$
    *   $\theta_{new} = \theta + \Delta \theta$

---
*참고: 위 수치는 TurtleBot3 Burger 표준 사양이며, 실제 로봇 제작 시 측정값으로 보정해야 합니다.*
