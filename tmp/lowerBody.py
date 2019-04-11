from picamera.array import PiRGBArray
from picamera import PiCamera
import time
import cv2

camera = PiCamera()
camera.resolution = (640, 480)
camera.framerate = 5
rawCapture = PiRGBArray(camera, size=(640, 480))

def findfaces(image):

    #cascPath = os.path.join(haarcascadeFolder, "haarcascade_upperbody.xml")
    #cascPath = os.path.join(haarcascadeFolder, "haarcascade_fullbody.xml")
    #cascPath = os.path.join(haarcascadeFolder, "haarcascade_russian_plate_number.xml")

    # Create the haar cascade
    faceCascade = cv2.CascadeClassifier("./haar3.xml")

    # Read the image
    height, width, depth = image.shape
    scale = 1
    if (width > 1024):
        scale = 1024.0/width
        image = cv2.resize(image, None, fx=scale, fy=scale)
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

    # Detect faces in the image
    faces = faceCascade.detectMultiScale(
        gray,
        scaleFactor=1.05,
        minNeighbors=5,
        minSize=(30, 30),
    )

    return faces 

def detect_face(image):
    faceCascade = cv2.CascadeClassifier('./haar2.xml')
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    faces = faceCascade.detectMultiScale(
        gray,
        scaleFactor=1.3,
        minNeighbors=3,
        minSize=(30, 30),
    )
    return faces

def detectFace(image):
    cascadePath = "/home/pi/tmp/haar.xml"
    FACE_SHAPE = 0.45
    result = image.copy()
    imageGray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    cascade = cv2.CascadeClassifier(cascadePath)
    faceRect = cascade.detectMultiScale(imageGray, scaleFactor=1.1, minNeighbors=1, minSize=(1,1))
    if len(faceRect) <= 0:
        return False
    else:
        # confirm face
        imageSize = image.shape[0] * image.shape[1]
        #print("d1")
        filteredFaceRects = []
        for faceR in faceRect:
            faceSize = faceR[2]*faceR[3]
            if FACE_SHAPE > min(faceR[2], faceR[3])/max(faceR[2], faceR[3]):
                break
            filteredFaceRects.append(faceR)

        if len(filteredFaceRects) > 0:
            return True
        else:
            return False


# allow the camera to warmup
time.sleep(0.1)

# capture frames from the camera
for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):
	image = frame.array
	faces = findfaces(image)
	print(len(faces))
	key = cv2.waitKey(1) & 0xFF

        # clear the stream in preparation for the next frame
	rawCapture.truncate(0)

        # if the `q` key was pressed, break from the loop
	if key == ord("q"):
		break

