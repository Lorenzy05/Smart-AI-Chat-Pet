import serial
import time

# 最简单的控制程序
try:
    # 请根据你的系统修改端口号
    # Windows: COM3, COM4, COM5 等
    # Linux: /dev/ttyUSB0, /dev/ttyUSB1 等  
    # Mac: /dev/tty.usbserial-*
    
    ser = serial.Serial('COM11', 115200)  # 修改为你的实际端口
    time.sleep(2)  # 等待连接
    
    print("开始控制机器人！输入 f,b,l,r 控制移动，q退出")
    
    while True:
        cmd = input("命令: ").strip()
        
        if cmd == 'q':
            break
        elif cmd in ['f', 'b', 'l', 'r']:
            ser.write((cmd + '\n').encode())
            print(f"发送: {cmd}")
        else:
            print("无效命令")
            
except Exception as e:
    print(f"错误: {e}")
    print("请检查端口号是否正确")