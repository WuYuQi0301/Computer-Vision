import cv2 as cv
def edge_detect(img):
    blurred = blurred = cv.GaussianBlur(img, (3, 3), 0)
    gray = cv.cvtColor(blurred, cv.COLOR_RGB2GRAY)
    edge_output = cv.Canny(gray, 50, 150)
    dst = cv.bitwise_and(img, img, mask= edge_output)
    cv.imshow("Color Edge", edge_output)
    cv.imwrite('cannyed.bmp', edge_output);
    
src = cv.imread('./dataset/1.bmp')
cv.namedWindow('img in', cv.WINDOW_NORMAL)
cv.imshow('img in', src)
edge_detect(src)

cv.waitKey(0)
cv.destroyAllWindows()
