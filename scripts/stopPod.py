import RPi.GPIO as GPIO

# Definition des pins
M1_En = 21
M1_In1 = 20
M1_In2 = 16

M2_En = 18
M2_In1 = 23
M2_In2 = 24


# Creation d'une liste des pins pour chaque moteur pour compacter la suite du code
Pins = [[M1_En, M1_In1, M1_In2], [M2_En, M2_In1, M2_In2]]


# Setup
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

GPIO.setup(M1_En, GPIO.OUT)
GPIO.setup(M1_In1, GPIO.OUT)
GPIO.setup(M1_In2, GPIO.OUT)

GPIO.setup(M2_En, GPIO.OUT)
GPIO.setup(M2_In1, GPIO.OUT)
GPIO.setup(M2_In2, GPIO.OUT)

GPIO.output(Pins[0][1], GPIO.LOW)
GPIO.output(Pins[0][2], GPIO.LOW)
GPIO.output(Pins[1][1], GPIO.LOW)
GPIO.output(Pins[1][2], GPIO.LOW)
print("Moteurs arretes.")

