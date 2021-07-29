import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BCM)

Buzzer_PIN = 17
GPIO.setup(Buzzer_PIN, GPIO.OUT, initial= GPIO.LOW)

def main():
    GPIO.output(Buzzer_PIN,GPIO.LOW) #Buzzer will be switched off

main()
