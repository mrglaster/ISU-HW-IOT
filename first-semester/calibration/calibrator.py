import time

import numpy as np
import serial
from sklearn.linear_model import LinearRegression

COM_PORT = "COM9"
BRAUD_RATE = 9600
TIMEOUT = 0.3
connection = serial.Serial(timeout=5000, port=COM_PORT)

start_range = None
min_range = None
end_range = None

# data_uv = []
is_enough = False
is_learned = False
# data_ir = []
data_uv = [2, 4, 5, 6, 7, 8, 10, 11, 12, 13, 14, 16, 18]
data_ir = [191, 227, 419, 478, 504, 509, 499, 501, 510, 506,
            496, 501, 520]


def send_command(command):
    connection.write(command.encode())


def read_serial():
    if connection.inWaiting() > 0:
        line = connection.readline().decode('utf-8').rstrip()
        return line


def make_linear_regression(ir_data_got, is_data_got):
    reg = LinearRegression()
    reg.fit(np.array(ir_data_got).reshape(-1, 1), is_data_got)
    return reg


def enough_data_check(range_start, range_end, values, step=1):
    seen_values = set()
    for i in range(range_start, range_end + 1, step):
        if i in values and i not in seen_values:
            seen_values.add(i)
    return len(seen_values) / ((range_end - range_start + 1) * int(1 / step)) >= 0.9


def predict_value(reg, value):
    return reg.predict(value)


def collect_data():
    is_enough = False
    while True:
        time.sleep(0.5)
        data = read_serial()
        if data:
            if data.startswith("Got value"):
                val = int(data.split()[-2])
                last = 0
                arr = data_uv if not is_enough else data_ir
                if is_enough:
                    data_ir.append(int(data.split()[-2]))
                else:
                    if len(arr):
                        last = arr[-1]
                    if 0 < val - last < 3:
                        arr.append(int(data.split()[-2]))
                    else:
                        print(f"Error for value: {val}")
                print(data_uv)
                print(data_ir)
        if len(data_ir) != 0 and len(data_ir) == len(data_uv):
            print("Stop collecting")
            break
        if len(data_uv) > 0 and data_uv[-1] >= min_range or is_enough:
            print("Now collect the IR data. Input cmd")
            is_enough = True
            print(f"Value for distance: {data_uv[len(data_ir)]}")
        cmd = input()
        if len(cmd):
            if not is_enough:
                send_command('n')
            else:
                send_command('i')


if __name__ == "__main__":
    reg = None
    while True:
        print("Input")
        cmd = input()
        if cmd == "calibrate":
            data_ir = []
            data_uv = []
            print("Calibration mode")
            is_enough = False
            is_learned = False
            print("Start range:")
            start_range = int(input())
            print("End range:")
            end_range = int(input())
            min_range = (end_range - start_range + 1) * 0.9
            collect_data()
        else:
            if not is_learned:
                # print(data_ir, data_uv)
                reg = make_linear_regression(data_ir, data_uv)
                is_learned = True
            send_command('i')
            data = read_serial()
            # print(data)
            while not data:
                # print(data)
                data = read_serial()
            if data:
                # print(data)
                if data.startswith("Got value"):
                    val = int(data.split()[-2])
                    print(f"{predict_value(reg, [[val]])[0]} cm")
