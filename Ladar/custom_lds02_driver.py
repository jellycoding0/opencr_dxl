#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from sensor_msgs.msg import LaserScan
import serial
import threading
import math
import struct
import sys
import time

class CustomLds02Driver(Node):
    def __init__(self):
        super().__init__('custom_lds02_driver')
        
        self.port = '/dev/ttyUSB0'
        self.ser = None
        self.baudrate = 115200  # 완벽히 검증된 하드웨어 통신 속도
        
        if not self.connect_and_wake_lidar():
            self.get_logger().error("Lidar 센서 포트 개방 실패")
            sys.exit(1)

        self.scan_pub = self.create_publisher(LaserScan, '/scan', 10)
        
        # 360도 데이터 버퍼
        self.scan_ranges = [0.0] * 360
        self.scan_intensities = [0.0] * 360

        self.running = True
        self.read_thread = threading.Thread(target=self.packet_parser_loop, daemon=True)
        self.read_thread.start()

    def connect_and_wake_lidar(self):
        try:
            self.get_logger().info(f"Connecting to {self.port} at {self.baudrate} bps...")
            self.ser = serial.Serial(self.port, self.baudrate, timeout=1)
            
            # 하드웨어 안정화 신호 공급
            self.ser.dtr = True
            self.ser.rts = True
            time.sleep(0.5)
            return True
        except Exception as e:
            self.get_logger().error(f"Port open error: {e}")
            return False

    def packet_parser_loop(self):
        self.get_logger().info("LDS-02 (0x54 Protocol) Parser Thread started.")
        sync_logged = False
        
        while self.running:
            try:
                if self.ser.in_waiting == 0:
                    time.sleep(0.002)
                    continue
                
                # 1. 이 라이다 패킷의 진짜 시작 바이트는 0x54 입니다.
                b = self.ser.read(1)
                if b == b'\x54':
                    if not sync_logged:
                        self.get_logger().info("🎯 [SUCCESS] Sync locked with Lidar Packet Header (0x54)!")
                        sync_logged = True
                    
                    # 0x54 프로토콜은 한 패킷이 총 47바이트입니다. (Header 1바이트 포함)
                    # 남은 46바이트를 읽어서 패킷을 완성합니다.
                    packet_remain = self.ser.read(46)
                    if len(packet_remain) < 46:
                        continue
                    
                    packet = b'\x54' + packet_remain
                    
                    # 2. 패킷 헤더 정보 파싱
                    # 패킷 길이(보통 0x2C=44), 현재 패킷의 회전 속도(RPM)
                    packet_len = packet[1]
                    speed_rpm = struct.unpack('<H', packet[2:4])[0]
                    
                    # 시작 각도와 종료 각도 파싱 (단위: 0.01도)
                    start_angle_raw = struct.unpack('<H', packet[4:6])[0] / 100.0
                    end_angle_raw = struct.unpack('<H', packet[42:44])[0] / 100.0
                    
                    # 3. 데이터 포인트 파싱 (한 패킷에 12개의 측정 포인트가 들어있음)
                    # 각 포인트는 3바이트 구조: [Distance Low, Distance High, Intensity]
                    for i in range(12):
                        offset = 6 + (i * 3)
                        dist_mm = struct.unpack('<H', packet[offset:offset+2])[0]
                        intensity = packet[offset+2]
                        
                        # 12개 포인트 사이의 각도를 선형 보간(Linear Interpolation) 처리
                        if end_angle_raw >= start_angle_raw:
                            angle = start_angle_raw + ((end_angle_raw - start_angle_raw) / 11.0) * i
                        else:
                            angle = start_angle_raw + (((end_angle_raw + 360.0) - start_angle_raw) / 11.0) * i
                        
                        # 360도 인덱스 범위 안으로 클램핑
                        idx = int(round(angle)) % 360
                        
                        # mm 단위를 ROS 표준인 미터(m) 단위로 변환
                        dist_m = dist_mm / 1000.0
                        
                        # LDS-02 물리 측정 한계치 필터링
                        if dist_m > 3.5 or dist_m < 0.12:
                            dist_m = 0.0
                            
                        self.scan_ranges[idx] = dist_m
                        self.scan_intensities[idx] = float(intensity)
                    
                    # 스캔 한바퀴가 거의 끝나는 지점(340도 통과 시)마다 ROS2 토픽 일괄 발행
                    if start_angle_raw > 340.0:
                        self.publish_laser_scan()
                        
            except Exception as e:
                self.get_logger().error(f"Parser Error: {e}")

    def publish_laser_scan(self):
        scan_msg = LaserScan()
        scan_msg.header.stamp = self.get_clock().now().to_msg()
        scan_msg.header.frame_id = 'base_scan'  # 터틀봇 내비게이션 표준 호환 프레임 명칭
        
        scan_msg.angle_min = 0.0
        scan_msg.angle_max = 2.0 * math.pi
        scan_msg.angle_increment = (2.0 * math.pi) / 360.0
        
        scan_msg.time_increment = 0.0
        scan_msg.scan_time = 0.2
        scan_msg.range_min = 0.12
        scan_msg.range_max = 3.5
        
        scan_msg.ranges = list(self.scan_ranges)
        scan_msg.intensities = list(self.scan_intensities)
        
        self.scan_pub.publish(scan_msg)

    def stop(self):
        self.running = False
        try:
            if self.ser:
                self.ser.close()
            self.get_logger().info("Lidar 포트가 안전하게 닫혔습니다.")
        except:
            pass

def main(args=None):
    rclpy.init(args=args)
    node = CustomLds02Driver()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.stop()
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()