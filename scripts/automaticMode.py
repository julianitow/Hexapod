import RPi.GPIO as GPIO
from time import sleep
import time

GPIO.setwarnings(False)

orientation = 1 # 1 -> forward -1 -> back

# Definition des pins
M1_En = 21
M1_In1 = 20
M1_In2 = 16

M2_En = 18
M2_In1 = 23
M2_In2 = 24

Trig = 11        
Echo = 8
TrigBack = 25
EchoBack = 15

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

#setup distance forward
GPIO.setup(Trig,GPIO.OUT)
GPIO.setup(Echo,GPIO.IN)
GPIO.output(Trig, False)

#setup distance back
GPIO.setup(TrigBack,GPIO.OUT)
GPIO.setup(EchoBack,GPIO.IN)
GPIO.output(TrigBack, False)


# Voir aide dans le tuto
M1_Vitesse = GPIO.PWM(M1_En, 100)
M2_Vitesse = GPIO.PWM(M2_En, 100)
M1_Vitesse.start(100)
M2_Vitesse.start(100)


def distanceFront():
    #while True:
    #time.sleep(1)       # On la prend toute les 1 seconde

    GPIO.output(Trig, True)
    time.sleep(0.00001)
    GPIO.output(Trig, False)

    while GPIO.input(Echo)==0:  ## Emission de l'ultrason
      debutImpulsion = time.time()

    while GPIO.input(Echo)==1:   ## Retour de l'Echo
      finImpulsion = time.time()

    distance = round((finImpulsion - debutImpulsion) * 340 * 100 / 2, 1)  ## Vitesse du son = 340 m/s

    return distance

def distanceBack():
    GPIO.output(TrigBack, True)
    time.sleep(0.00001)
    GPIO.output(TrigBack, False)

    while GPIO.input(EchoBack)==0:  ## Emission de l'ultrason
      debutImpulsion = time.time()

    while GPIO.input(EchoBack)==1:   ## Retour de l'Echo
      finImpulsion = time.time()

    distanceB = round((finImpulsion - debutImpulsion) * 340 * 100 / 2, 1)  ## Vitesse du son = 340 m/s

    return distanceB



def sens1(moteurNum) :
    GPIO.output(Pins[moteurNum - 1][1], GPIO.HIGH)
    GPIO.output(Pins[moteurNum - 1][2], GPIO.LOW)
    print("Moteur", moteurNum, "tourne dans le sens 1.")


def sens2(moteurNum) :
    GPIO.output(Pins[moteurNum - 1][1], GPIO.LOW)
    GPIO.output(Pins[moteurNum - 1][2], GPIO.HIGH)
    print("Moteur", moteurNum, "tourne dans le sens 2.")

def arret(moteurNum) :
    GPIO.output(Pins[moteurNum - 1][1], GPIO.LOW)
    GPIO.output(Pins[moteurNum - 1][2], GPIO.LOW)
    print("Moteur", moteurNum, "arret.")

def arretComplet() :
    GPIO.output(Pins[0][1], GPIO.LOW)
    GPIO.output(Pins[0][2], GPIO.LOW)
    GPIO.output(Pins[1][1], GPIO.LOW)
    GPIO.output(Pins[1][2], GPIO.LOW)
    print("Moteurs arretes.")

def forward():
    orientation = 1
    sens1(1)
    sens1(2)

def back():
    orientation = -1
    sens2(2)
    sens2(2)

while True :
    #if distance < 20 & orientation = 1
    #print(distanceBack())
    #sleep(1)
    distance = distanceFront()
    print(distance)
    forward()
    if distance < 20:
        arretComplet()
        back()
    sleep(1)
    #elif distanceBack() < 20 & orientation == -1:
    #    arret()
    #    forward()
    #sleep(1)

