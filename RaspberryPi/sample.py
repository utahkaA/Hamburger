from __future__ import print_function
import sys
import serial
import time

def _main():
  port_name = sys.argv[1] 
  connection = serial.Serial(port_name, 9600)
  time.sleep(2)
  print(connection.portstr)

  msg = " ".join(sys.argv[2:])

  connection.write(msg)

if __name__ == "__main__":
  _main()
