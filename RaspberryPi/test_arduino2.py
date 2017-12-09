from __future__ import print_function
import sys
import serial
import time

def _main():
  arduino1 = serial.Serial('/dev/ttyACM0', 9600)
  arduino2 = serial.Serial('/dev/ttyACM1', 9600)
  time.sleep(2)

  prog = ["bun1 select 36\n",
          "bun1 arm -20 2000\n",
          "belt go 1200\n", 
          "sleep1 6000\n",
          "patty select 36\n",
          "sleep1 1500\n",
          "patty arm -20 2000\n",
          "sleep1 1500\n",
          "belt go 700\n",
          "sleep1 5000\n",
          "cheese 20 2000\n",
          "belt go 900\n",
          "sleep1 4500\n",
          "ketchup 90\n",
          "sleep1 2000\n",
          "ketchup 0\n",
          "belt go 1100\n",
          "sleep1 8000\n",
          "bun2 select 36\n",
          "sleep2 2000\n",
          "bun2 arm -20 2000\n"]

  arduino_tb = {"bun1": 1,
                "patty": 1,
                "sleep1": 1,
                "bun2": 2,
                "belt": 2,
                "cheese": 2,
                "ketchup": 2,
                "sleep2" : 2}
  """
  prog = ["cheese 20 1000\n",
          "belt go 800\n",
          "sleep 4000\n",
          "ketchup 90\n",
          "sleep 2000\n",
          "ketchup 0\n",
          "belt go 1200\n"]
  """
  for line in prog:
    part_name = line.split(' ')[0]
    machine_id = arduino_tb[part_name]
    if machine_id == 1:
      if part_name == "sleep1":
        t = line.split(' ')[1]
        line = "sleep {}".format(t)
      arduino1.write(line)
    elif machine_id == 2:
      if part_name == "sleep2":
        t = line.split(' ')[1]
        line = "sleep {}".format(t)
      arduino2.write(line)
    time.sleep(2)

if __name__ == "__main__":
  _main()
