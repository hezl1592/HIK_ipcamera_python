# -*- coding: utf-8 -*-
import os
import time
import cv2

from interface import HKIPCamera

hkcp = HKIPCamera(b"192.168.1.94", 8000, b"admin", b"zzz000000")
hkcp.start()
time.sleep(2)
while True:
    #cv2.imshow("display", hkcp.frame(rows=540, cols=960))
    cv2.imshow("display", hkcp.frame(rows=720, cols=1280))
    k = cv2.waitKey(1)
    if k == ord('q') or k == 27:
        break

'''
for i in range(20):
    cv2.imwrite("./%d.jpg" % i, hkcp.frame())
    # cv2.imshow("display", hkcp.frame(rows=540, cols=960))
    # cv2.waitKey(40)
    start_time = time.time()
    hkcp.frame()
    print(time.time() - start_time)

'''

hkcp.stop()
