import keyboard
import time

while True:
    print('Reading!')
    if keyboard.is_pressed("a"):
        print("You pressed a and it is encounted!")
    if keyboard.is_pressed("q"):
        break
        print("You pressed q and Loop terminated!")

    time.sleep(0.1)



'''
while True:
    if keyboard.read_key() == "a":
        print("You pressed a and it is encounted!")
    keyboard.wait("q")       
    print("Waited to get out of while loop and You pressed q")
    
    keyboard.record(until="p")# return the recording
    print("You pressed p and it is recorded!")
    break
'''
#Reference
#Link:https://stackoverflow.com/question  s/24072790/how-to-detect-key-presses