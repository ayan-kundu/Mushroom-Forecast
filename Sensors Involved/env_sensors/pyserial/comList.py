# import serial for python and arduino communication

import serial
import serial.tools.list_ports as ports 

portList=ports.comports()
port_list=[]

print(f"All available ports:")
for i in range(len(portList)):
  port_list.append(str(portList[i]))
  print(f"   {port_list[i]}")