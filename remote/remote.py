from time import sleep
from serial import Serial
import keyboard


def key_press(key):
    if(key.name == 'w'):
        ser.write(bytes('**w', 'utf-8'))
        print("up pressed")
    if(key.name == 'a'):
        ser.write(bytes('**w', 'utf-8'))
        print("left pressed")
    if(key.name == 's'):
        ser.write(bytes('**w', 'utf-8'))
        print("down pressed")
    if(key.name == 'd'):
        ser.write(bytes('**w', 'utf-8'))
        print("right pressed")
    if(key.name == 'q'):
        ser.write(bytes('**w', 'utf-8'))
        print("serong kiri pressed")
    if(key.name == 'e'):
        ser.write(bytes('**w', 'utf-8'))
        print("serong kanan pressed")


if __name__ == "__main__":
    keyboard.on_press(key_press)
    ser = Serial("COM8", 9600)
    if(ser.isOpen()):
        print("Serial is open")
    sleep(1)
    while True:
        print(ser.readall().decode("utf-8"))

    ser.write(bytes("*", "utf-8"))
    # if keyboard.is_pressed("w"):
    #     print("w pressed")
    #     ser.write(bytes("*", "utf-8"))
    #     ser.write(bytes("w", "utf-8"))
    # if keyboard.is_pressed("s"):
    #     print("s pressed")
    #     ser.write(bytes("*", "utf-8"))
    #     ser.write(bytes("s", "utf-8"))
    # if keyboard.is_pressed("a"):
    #     print("a pressed")
    #     ser.write(bytes("*", "utf-8"))
    #     ser.write(bytes("a", "utf-8"))
    # if keyboard.is_pressed("d"):
    #     print("d pressed")
    #     ser.write(bytes("*", "utf-8"))
    #     ser.write(bytes("d", "utf-8"))

    # if keyboard.is_pressed("q"):
    #     print("q pressed")
    #     ser.write(bytes("*", "utf-8"))
    #     ser.write(bytes("q", "utf-8"))
    # if keyboard.is_pressed("e"):
    #     print("e pressed")
    #     ser.write(bytes("*", "utf-8"))
    # ser.write(bytes("e", "utf-8"))
