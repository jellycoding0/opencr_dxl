

# 라이다 LDS-02   (장애물 감지)
ros2 topic echo /scan --field ranges 

- Baudrate 115200
360도 돌며 1도 간격으로 거리[m] 데이터 출력

ros2 run lds_02_driver lds_02_driver_node --ros-args -p port:=/dev/ttyUSB0
ros2 topic echo /scan



# Rviz2 사용법
1. 로봇 중심(base_link)과 라이다 센서 중심(base_scan)을 연결해 주는 가짜 좌표(Static TF) 노드를 먼저 하나 실행
ros2 run tf2_ros static_transform_publisher "0" "0" "0.12" "0" "0" "0" "base_link" "base_scan"

2. Rviz2 실행 및 라이다 토픽 추가하기
- rviz2
- 왼쪽 패널 맨 위에 Global Options ➡️ Fixed Frame 항목 ➡️ base_scan
- Rviz2 왼쪽 하단의 [Add] ➡️ By topic 탭을 ➡️ 선택 목록에서 /scan ➡️ LaserScan





# LLC OpenCR 
/odom 

# SLAM 노드 
/scan과 /odom을 융합하여 지도(map) 제작

# Nav2 패키지 
지도를 기반으로 목적지까지의 경로를 계산하여 로봇 브릿지 노드에게 /cmd_vel 명령 하강