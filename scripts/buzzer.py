import RPi.GPIO as GPIO
import time
import os
import atexit

GPIO.setmode(GPIO.BCM)

Buzzer_PIN = 17
GPIO.setup(Buzzer_PIN, GPIO.OUT, initial= GPIO.LOW)

def main():
    while True:
        print("Buzzer will be on for 4 seconds")
        GPIO.output(Buzzer_PIN,GPIO.HIGH) #Buzzer will be switched on
        time.sleep(4) #Waitmode for 4 seconds
        print("Buzzer wil be off for 4 seconds") 
        GPIO.output(Buzzer_PIN,GPIO.LOW) #Buzzer will be switched off 
        time.sleep(2) #Waitmode for another 2 seconds in which the buzzer will be off

def OnExitApp(user):
    GPIO.output(Buzzer_PIN,GPIO.LOW)
    GPIO.cleanup()
    sys.exit(0)

def exit_handler():
    GPIO.output(Buzzer_PIN,GPIO.LOW)
    sys.exit(0)

atexit.register(exit_handler)
#atexit.register(OnExitApp, 'system')
main()

#except KeyboardInterrupt:
#        GPIO.cleanup()
