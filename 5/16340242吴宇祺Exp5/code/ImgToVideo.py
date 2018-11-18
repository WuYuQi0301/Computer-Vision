import cv2
import os
#图片路径
img_root = "../result/"
#视频的FPS
fps = 24
#可以用(*'DVIX')或(*'X264')
fourcc = cv2.VideoWriter_fourcc(*'MJPG')
#saveVideo.avi是要生成的视频名称，（384,288）是图片尺寸
videoWriter = cv2.VideoWriter('../saveVideo.avi',fourcc,fps,(408,408))
#生成视频
for i in range(10):
    frame = cv2.imread(img_root+str(i)+'.bmp')
    videoWriter.write(frame)
videoWriter.release()