from __future__ import print_function
import sys
import serial
import time

def _main():
  connection = serial.Serial('/dev/ttyACM0', 9600)
  time.sleep(2)
  print(connection.portstr)

  msg = " ".join(sys.argv[1:])

  connection.write(msg)
  """
  idx = 0
  switch = ['a', 'b']
  while 1:
    connection.write(switch[idx]) 
    time.sleep(1)
    idx = 1 if idx == 0 else 0
  """

if __name__ == "__main__":
  _main()
