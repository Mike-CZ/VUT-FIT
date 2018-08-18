import cv2
import numpy as np

def main():
    # load image
    img = cv2.imread('image.png', cv2.IMREAD_COLOR)

    # convert to gray
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    # median blur
    blur = cv2.medianBlur(gray, 5)

    # canny edge detection
    edges = cv2.Canny(blur, 18, 32)

    # find contours
    image, contours, hierarchy = cv2.findContours(edges, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

    # draw contours
    outimg = np.zeros((img.shape[0], img.shape[1]), dtype=np.uint8)
    for x in range(len(contours)):
        if len(contours[x]) < 30:
            cv2.drawContours(outimg, contours, x, 255, cv2.FILLED)

    # morphological opening
    structuringElement = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (4, 4))
    opening = cv2.morphologyEx(outimg, cv2.MORPH_OPEN, structuringElement)

    # draw result into image
    w, h, _ = img.shape
    for i in range(0, w):
        for j in range(0, h):
            if opening[i, j] != 0:
                img[i, j] = opening[i, j]

    cv2.imwrite('microaneurysms.png', img)

if __name__ == '__main__':
    main()
