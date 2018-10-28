import urllib
import cv2
import numpy as np
import time

url='http://192.168.0.13:8080/shot.jpg'
#url='http://192.168.0.13:8080/video/jpeg'
#cv2.namedWindow('Frame', cv2.WINDOW_NORMAL)
#cv2.resizeWindow('image', 640,480)
i=0
f = open("rgb.txt", "a")
while True:
    # Use urllib to get the image from the IP camera
    imgResp = urllib.urlopen(url)
    
    # Numpy to convert into a array
    imgNp = np.array(bytearray(imgResp.read()),dtype=np.uint8)
    
    # Finally decode the array to OpenCV usable format ;) 
    img = cv2.imdecode(imgNp,-1)
    cv2.imwrite("rgb/image%04i.jpg" %i, img)
    f.write("%04i rgb/image%04i.jpg\n" %(i,i)) 
    i+=1
	# put the image on screen
    cv2.imshow('Frame',img)

    #To give the processor some less stress
    #time.sleep(0.1) 

    # Quit if q is pressed
    if cv2.waitKey(1) == 27:
        break


