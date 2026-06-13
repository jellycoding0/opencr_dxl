# ros2 topic echo /odom --field pose.pose
# ros2 topic echo /odom --field twist.twist
# ros2 run teleop_twist_keyboard teleop_twist_keyboard --ros-args -p speed:=0.1 -p turn:=0.5

import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Twist
from nav_msgs.msg import Odometry
from geometry_msgs.msg import Quaternion, TransformStamped
import tf2_ros
import serial
import threading
import math
import time

class OpenCRBridge(Node):
    def __init__(self):
        super().__init__('opencr_bridge')
        
        # Parameters
        self.declare_parameter('port', '/dev/ttyACM0')
        self.declare_parameter('baud', 115200)
        self.declare_parameter('wheel_separation', 0.160) # m
        self.declare_parameter('wheel_radius', 0.033)     # m
        self.declare_parameter('ticks_per_rev', 4096)    # XL430-W250
        
        port = self.get_parameter('port').value
        baud = self.get_parameter('baud').value
        self.wheel_sep = self.get_parameter('wheel_separation').value
        self.wheel_rad = self.get_parameter('wheel_radius').value
        self.ticks_per_rev = self.get_parameter('ticks_per_rev').value

        # Robot State
        self.x = 0.0
        self.y = 0.0
        self.th = 0.0
        self.last_l_tick = None
        self.last_r_tick = None
        self.last_time = self.get_clock().now()

        # Serial setup
        try:
            self.ser = serial.Serial(port, baud, timeout=0.1)
            self.get_logger().info(f"Connected to OpenCR on {port}")
        except Exception as e:
            self.get_logger().error(f"Failed to connect to OpenCR: {e}")
            raise e

        # ROS Publishers/Subscribers
        self.odom_pub = self.create_publisher(Odometry, 'odom', 10)
        self.cmd_vel_sub = self.create_subscription(Twist, 'cmd_vel', self.cmd_vel_callback, 10)
        self.tf_broadcaster = tf2_ros.TransformBroadcaster(self)

        # Start serial read thread
        self.read_thread = threading.Thread(target=self.serial_read_loop, daemon=True)
        self.read_thread.start()

    def cmd_vel_callback(self, msg):
        # Convert Twist to "V[lin],A[ang]\n"
        cmd = f"V{msg.linear.x:.3f},A{msg.angular.z:.3f}\n"
        self.ser.write(cmd.encode())

    def serial_read_loop(self):
        while rclpy.ok():
            if self.ser.in_waiting > 0:
                try:
                    line = self.ser.readline().decode('utf-8', errors='ignore').strip()
                    if line.startswith('O'):
                        # Parse "O[L],[R]"
                        data = line[1:].split(',')
                        if len(data) == 2:
                            l_tick, r_tick = map(int, data)
                            self.update_odometry(l_tick, r_tick)
                except Exception as e:
                    self.get_logger().warn(f"Serial read error: {e}")

    def update_odometry(self, l_tick, r_tick):
        now = self.get_clock().now()
        dt = (now - self.last_time).nanoseconds / 1e9
        
        if self.last_l_tick is None:
            self.last_l_tick = l_tick
            self.last_r_tick = r_tick
            self.last_time = now
            return

        # Diff in ticks
        # Based on main.c, both ID 1 and ID 2 are sent negated commands for forward.
        # This means for forward motion, both encoder values decrease.
        # To get a positive distance for forward, we do: -(current - last)
        dl_tick = -(l_tick - self.last_l_tick)
        dr_tick = -(r_tick - self.last_r_tick)

        # Handle encoder rollover (4096 to 0 or vice versa)
        if dl_tick > 2048: dl_tick -= 4096
        elif dl_tick < -2048: dl_tick += 4096
        
        if dr_tick > 2048: dr_tick -= 4096
        elif dr_tick < -2048: dr_tick += 4096

        # Ticks to meters
        dist_l = (dl_tick / self.ticks_per_rev) * (2 * math.pi * self.wheel_rad)
        dist_r = (dr_tick / self.ticks_per_rev) * (2 * math.pi * self.wheel_rad)

        dist_center = (dist_l + dist_r) / 2.0
        d_th = (dist_r - dist_l) / self.wheel_sep

        # Update pose
        self.x += dist_center * math.cos(self.th + d_th / 2.0)
        self.y += dist_center * math.sin(self.th + d_th / 2.0)
        self.th += d_th

        # Broadcast TF and Publish Odom
        self.publish_odom(now)

        self.last_l_tick = l_tick
        self.last_r_tick = r_tick
        self.last_time = now

    def publish_odom(self, now):
        # Quaternion from yaw
        q = Quaternion()
        q.x = 0.0
        q.y = 0.0
        q.z = math.sin(self.th / 2.0)
        q.w = math.cos(self.th / 2.0)

        # 1. TF
        t = TransformStamped()
        t.header.stamp = now.to_msg()
        t.header.frame_id = 'odom'
        t.child_frame_id = 'base_footprint'
        t.transform.translation.x = self.x
        t.transform.translation.y = self.y
        t.transform.translation.z = 0.0
        t.transform.rotation = q
        self.tf_broadcaster.sendTransform(t)

        # 2. Odom Message
        odom = Odometry()
        odom.header.stamp = now.to_msg()
        odom.header.frame_id = 'odom'
        odom.child_frame_id = 'base_footprint'
        odom.pose.pose.position.x = self.x
        odom.pose.pose.position.y = self.y
        odom.pose.pose.orientation = q
        self.odom_pub.publish(odom)

def main(args=None):
    rclpy.init(args=args)
    bridge = OpenCRBridge()
    try:
        rclpy.spin(bridge)
    except KeyboardInterrupt:
        pass
    finally:
        # Stop robot on exit
        bridge.ser.write("V0.00,A0.00\n".encode())
        bridge.ser.close()
        bridge.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()
