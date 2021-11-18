import serial as s
import time
ser = s.Serial(port='COM9',baudrate=9600)
ser.write(b'Hello\r\n')
time.sleep(2)
ser.flush()
while True:
    h = (input("\nWhat to send : ")).encode('utf-8')
    print("SENDING: " + h.decode('utf-8'))
    ser.write(h)
    line=''
    time.sleep(.5)
    c = 0
    while len(line)==0:
        line = ser.readline().decode('utf-8').rstrip()
        time.sleep(.05)
    print("RETURNED: " + line)