import numpy as np
import time
import cv2
import socket

cap = cv2.VideoCapture(0)


def findfaces(image):
    # cascPath = os.path.join(haarcascadeFolder, "haarcascade_upperbody.xml")
    # cascPath = os.path.join(haarcascadeFolder, "haarcascade_fullbody.xml")
    # cascPath = os.path.join(haarcascadeFolder, "haarcascade_russian_plate_number.xml")

    # Create the haar cascade
    faceCascade = cv2.CascadeClassifier("./haar3.xml")

    # Read the image
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

    # Detect faces in the image
    faces = faceCascade.detectMultiScale(
        gray,
        scaleFactor=1.10,
        minNeighbors=5,
        minSize=(30, 30),
    )

    return faces


# allow the camera to warmup
time.sleep(0.1)

# capture frames from the camera
s = socket.socket()
s.connect(('raspberrypi.local',12348))
try:
    newX = 0
    while True:
        ret, image = cap.read()
        faces = findfaces(image)
        for (x, y, w, h) in faces:
            newX = x+(w/2)-310
        s.send(str(newX).encode())

except KeyboardInterrupt or AttributeError:
    s.send('!'.encode())
    s.close()
    cap.release()
    cv2.destroyAllWindows()
