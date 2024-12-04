# import serial for python and arduino communication
import time
import serial
from datetime import date
#from getch import getch 
#from msvcrt import getch as getch
import keyboard

# necessary var
sensor="Compost sensor"
serial_port='COM4' # needs checking 
baude_rate=9600

# path where the sesnor data would be stored 
storage_path="C:/Users/Staff/Documents/Arduino/Sensor data"
# encampulate access from Serial
ser= serial.Serial(serial_port,baude_rate,timeout=0.05)  # find right com 
if not ser.isOpen():
  ser.open()
  print('{serial_port} open {ser.isOpen()}')

#getVal=1
current_date=date.today()
hh,mm,ss=time.strftime('%H'),time.strftime('%M'),time.strftime('%S')
print(date)
with open(f'{storage_path}/{current_date} {sensor} data.txt','w') as s:
  # store data in a file 
  while True:
    # short delay 
    time.sleep(0.1) # needs to be sync/same with arduino samplling rate or comment out
    
    # read serial stream
    getVal=ser.readline()
    # show on console 
    print(getVal)

    if keyboard.is_pressed("a"):
      print("You pressed a and it is encounted!")
      # if 'r' is pressed, write getVal onto file 
      s.write(f'{hh}:{mm}:{ss}>>>')
      s.write(str(getVal));s.write('\n')
    if keyboard.is_pressed("q"):
      break
      print("You pressed q and Loop terminated!")

    time.sleep(0.1)