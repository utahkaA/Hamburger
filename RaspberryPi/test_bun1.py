from __future__ import print_function
import sys
import serial
import time

def _main():
  connection = serial.Serial('/dev/ttyACM0', 9600)
  time.sleep(2)
  print(connection.portstr)

  prog = ["bun1 select 36\n",
          "bun1 arm -20 1000\n",
          "belt go 800\n"]

  for line in prog:
    connection.write(line)
    time.sleep(2)

if __name__ == "__main__":
  _main()
