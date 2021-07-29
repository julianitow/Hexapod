# coding:utf-8
# Utilisation d'un afficheur LCD 1602 sur Raspberry 3 B+
# Affichage de la date et l'heure


from RPLCD.gpio import CharLCD
from RPLCD.gpio import GPIO
import atexit
import time
import sys
import os

GPIO.setwarnings(False)

lcd = CharLCD(cols=16, rows=2, pin_rs=37, pin_e=35, pins_data=[33, 31, 29, 21])

message = ""

def main():
    message = ""

    for arg in sys.argv:
        if ".py" not in arg:
            message += " " + arg

    posMessage = 16/2-len(message)/2
    print("Displaying:" + message + " at position: " + str(posMessage))
    lcd.clear()
    while True:
        lcd.cursor_pos = (0, posMessage)
        lcd.write_string(message)
        lcd.cursor_pos = (1, 5)
        lcd.write_string("%s" %time.strftime("%H:%M:%S"))
        if "Status" in message:
            break

def OnExitApp(user):
    sys.exit(0)

try:
    atexit.register(OnExitApp, 'system')
    main()
except KeyboardInterrupt:
    os.system("python "+ sys.argv[0] + " Status: Online")
    sys.exit(0)
