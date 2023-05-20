import cv2


def detectFace(grayFrame):
    faceCascade = cv2.CascadeClassifier(
        './model/haarcascade_frontalface_default.xml')
    return faceCascade.detectMultiScale(grayFrame, 1.1, 25)


def detectSmile(detectedFaces, grayFrame, frame):
    smileCascade = cv2.CascadeClassifier('./model/haarcascade_smile.xml')
    for (x, y, w, h) in detectedFaces:
        cv2.rectangle(frame, (x, y), (x + w, y + h),
                      (0, 255, 255), 3, cv2.LINE_AA)

        faceRoiGray = grayFrame[y:y+h:, x:x+w]
        faceRoi = frame[y:y+h:, x:x+w]

        detectedSmile = smileCascade.detectMultiScale(faceRoiGray, 2.4, 25)

        for (sx, yx, sw, sh) in detectedSmile:
            cv2.rectangle(faceRoi, (sx, yx), (sx + sw, yx + sh),
                          (0, 255, 255), 3, cv2.LINE_AA)
        return frame


cap = cv2.VideoCapture(0)

while True:
    ret, frame = cap.read()
    if not ret:
        break

    grayFrame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    detectedFaces = detectFace(grayFrame)

    detectSmile(detectedFaces, grayFrame, frame)

    cv2.imshow('Realtime Webcam', frame)

    if cv2.waitKey(1) >= 0:
        break

cap.release()
cv2.destroyAllWindows()
