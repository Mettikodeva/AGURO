import serial
import cv2 as cv
import numpy as np
import time
import matplotlib.pyplot as plt

base_img = np.zeros((480, 640, 3), np.uint8)
if __name__ == "__main__":
    ser = serial.Serial("/dev/ttyUSB0", 115200)
    calibrating = True
    distance = 0
    pid = [0, 0, 0]  # err dl dr
    sensor = [0, 0, 0, 0, 0, 0, 0, 0]
    time_list = []
    data_err = []
    try:
        while True:
            img = base_img.copy()
            key = ""
            while key != "*":
                try:
                    key = ser.read().decode("utf-8")
                except UnicodeDecodeError:
                    continue

                print(key, end="")
            try:
                type = ser.read().decode("utf-8")
            except UnicodeDecodeError:
                continue
            print(type)
            # if type == "C":
            # calibrating = True
            # try:
            #     data = ser.read_until("#").decode("utf-8")
            # except UnicodeDecodeError:
            #     continue
            # data = data.split(",")
            # print(data)
            # cv.putText(
            #     img,
            #     "Calibrating",
            #     (10, 30),
            #     cv.FONT_HERSHEY_SIMPLEX,
            #     1,
            #     (255, 255, 255),
            #     2,
            # )
            # for i in range(8):
            #     cv.putText(
            #         img,
            #         str(data[i]),
            #         (10, 60 + i * 20),
            #         cv.FONT_HERSHEY_SIMPLEX,
            #         1,
            #         (255, 255, 255),
            #         2,
            #     )
            # else:
            if type == "P":
                print("pid")
                try:
                    data = ser.read_until("#".encode("utf-8")).decode("utf-8")
                except UnicodeDecodeError:
                    continue
                data = data.replace("#", "")
                pid = data.split(",")
                data_err.append(pid[0])
                time_list.append(time.time())
                print("pid ", pid)

            elif type == "S":
                print("sensor")
                try:
                    data = ser.read_until(",#".encode("utf-8")).decode("utf-8")
                except UnicodeDecodeError:
                    continue
                sensor = data.split(",")
                print("sensor", sensor)

            elif type == "D":
                print("distance")
                try:
                    distance = ser.read_until("#".encode("utf-8")).decode("utf-8")
                except UnicodeDecodeError:
                    continue
                print("dist ", distance)

            cv.putText(
                img,
                "Distance: " + str(distance),
                (10, 30),
                cv.FONT_HERSHEY_SIMPLEX,
                1,
                (255, 255, 255),
                2,
            )
            cv.putText(
                img,
                "err: " + str(pid[0]) + "  dl: " + str(pid[1]) + "  dr: " + str(pid[2]),
                (10, 60),
                cv.FONT_HERSHEY_SIMPLEX,
                1,
                (255, 255, 255),
                2,
            )
            for i in range(8):
                cv.putText(
                    img,
                    str(sensor[i]),
                    (10 + i * 50, 120),
                    cv.FONT_HERSHEY_SIMPLEX,
                    1,
                    (255, 255, 255),
                    2,
                )

            cv.imshow("Aguro", img)
            if cv.waitKey(1) & 0xFF == ord("q"):
                break

    except KeyboardInterrupt:
        ser.close()
        print("Serial closed")
        # cv.destroyAllWindows()
        # print("All windows closed")
        exit()
    plt.title("err")
    # plt.axis([0, len(time), -1, 1])
    plt.plot(time_list, data_err)
    plt.show()
