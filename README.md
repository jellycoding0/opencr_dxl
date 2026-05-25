# OpenCR 1.0 DYNAMIXEL & Odometry Project

이 프로젝트는 OpenCR 1.0 보드를 사용하여 다이나믹셀(DYNAMIXEL) 모터를 제어하고, 엔코더 및 IMU 데이터를 융합하여 정밀한 오도메트리(Odometry)를 구현하는 것을 목표로 합니다.

## 📂 프로젝트 문서 구조

모든 문서는 `/docu` 폴더에 체계적으로 정리되어 있습니다.

### 1. 하드웨어 및 사양 (`/docu`)
*   [Hardware Guide](./docu/Hardware_Guide.md): OpenCR 1.0 보드 사양 및 클럭/USB 설정
*   [Pinmap](./docu/Pinmap.md): 핀 할당표 (모터 전원, 방향 제어, 버튼 등)
*   [Robot Spec](./docu/Robot_Spec.md): 로봇 기구학 데이터 및 제원

### 2. 개발 가이드 (`/docu/dev`)
*   [Developer Guide](./docu/dev/Developer_Guide.md): 빌드/업로드 방법 및 코드 구조 설명

### 3. 프로젝트 규칙
*   [GEMINI Mandates](./GEMINI.md): AI 에이전트 작업 시 준수해야 할 핵심 제약 사항

## 🚀 진행 상태: Step 1 (USB 통신 복구)
현재 모든 기능을 초기화하고 가장 기초가 되는 **Native USB 통신**의 안정적인 인식을 위한 기초 공사를 진행 중입니다.

---
*Last Updated: 2026-05-25*
