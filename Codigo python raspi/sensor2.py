from signal import signal, SIGTERM, SIGHUP, pause
from time import sleep
from gpiozero import DistanceSensor
from threading import Thread
import serial

reading = True
sensor = DistanceSensor(echo=24, trigger=23)
mensajeApagar = 'A'
mensajeEncender = 'E'

arduino = serial.Serial('/dev/ttyACM0', 9600)


def safe_exit(signum, frame):
	exit(1)

def read_distance():
	while reading:
		leido = sensor.value
		message = f"Distance: {leido:1.2f} m"
		print(message)
		        
		if(leido < 0.30):
			arduino.write(mensajeApagar.encode())
		else:
			arduino.write(mensajeEncender.encode())
		sleep(0.25)

try:
	signal(SIGTERM, safe_exit)
	signal(SIGHUP,safe_exit)
	
	reader = Thread(target=read_distance, daemon=True)
	reader.start()
	pause()

except KeyboardInterrupt:
	pass
	
finally:
	reading=False
	reader.join()
	sensor.close()
