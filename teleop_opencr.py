import serial
import sys
import termios
import tty
import select

# 터틀봇3 버거 사양 기준 임계값
MAX_LIN_VEL = 0.22  # m/s
MAX_ANG_VEL = 2.84  # rad/s
LIN_VEL_STEP = 0.01
ANG_VEL_STEP = 0.1

msg = """
OpenCR Mobile Robot Teleop
---------------------------
Control Your Robot!
---------------------------
Moving around:
        w
   a    s    d
        x

w/x : increase/decrease linear velocity
a/d : increase/decrease angular velocity
space, s : force stop

CTRL-C to quit
"""

def get_key():
    fd = sys.stdin.fileno()
    old_settings = termios.tcgetattr(fd)
    try:
        tty.setraw(sys.stdin.fileno())
        rlist, _, _ = select.select([sys.stdin], [], [], 0.1)
        if rlist:
            key = sys.stdin.read(1)
        else:
            key = ''
    finally:
        termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
    return key

def main():
    # USB CDC Port (Change if necessary, e.g., /dev/ttyACM1)
    port = '/dev/ttyACM0'
    try:
        ser = serial.Serial(port, 115200, timeout=1)
    except:
        print(f"Error: Could not open port {port}")
        return

    lin_vel = 0.0
    ang_vel = 0.0
    print(msg)

    try:
        while True:
            key = get_key()
            if key == 'w':
                lin_vel = min(lin_vel + LIN_VEL_STEP, MAX_LIN_VEL)
            elif key == 'x':
                lin_vel = max(lin_vel - LIN_VEL_STEP, -MAX_LIN_VEL)
            elif key == 'a':
                ang_vel = min(ang_vel + ANG_VEL_STEP, MAX_ANG_VEL)
            elif key == 'd':
                ang_vel = max(ang_vel - ANG_VEL_STEP, -MAX_ANG_VEL)
            elif key == ' ' or key == 's':
                lin_vel = 0.0
                ang_vel = 0.0
            elif key == 't':
                ser.write("T\n".encode())
                print("\rSent TEST command to OpenCR", end="")
            elif key == '\x03':  # CTRL-C
                break
            
            # Send command even if no key is pressed to keep it alive (optional)
            # Or only send on change. Here we send only on key press for simplicity.
            if key != '':
                cmd = f"V{lin_vel:.2f},A{ang_vel:.2f}\n"
                ser.write(cmd.encode())
                sys.stdout.write(f"\rCurrent Velocity: Linear {lin_vel:.2f} m/s, Angular {ang_vel:.2f} rad/s    ")
                sys.stdout.flush()

    except Exception as e:
        print(e)
    finally:
        # Final stop command
        ser.write("V0.00,A0.00\n".encode())
        ser.close()
        print("\nTeleop stopped.")

if __name__ == "__main__":
    main()
