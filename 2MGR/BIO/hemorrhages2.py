import cv2
import numpy as np

from random import randint

# get near points
def getPoints(image, x, y, eightPixels):
    points = []
    if x - 1 >= 0:
        points.append((x-1, y))
    if x + 1 < image.shape[0] - 1:
        points.append((x+1, y))
    if y - 1 >= 0:
        points.append((x, y-1))
    if y + 1 < image.shape[1] - 1:
        points.append((x, y+1))

    if eightPixels == True:
        if x - 1 >= 0 and y - 1 >= 0:
            points.append((x-1, y-1))
        if x - 1 >= 0 and y + 1 < image.shape[1] - 1:
            points.append((x-1, y+1))
        if x + 1 < image.shape[0] - 1 and y - 1 >= 0:
            points.append((x+1, y-1))
        if x + 1 < image.shape[0] - 1 and y + 1 < image.shape[1] - 1:
            points.append((x+1, y+1))

    return points


# region growing
def regionGrowing(image, threshold=0, maxColor=None, minRegionSize=None, maxRegionSize=None, eightPixels=False):
    visited = np.zeros((image.shape[0], image.shape[1]), dtype=np.uint8)
    outimg = np.zeros((image.shape[0], image.shape[1]), dtype=np.uint8)
    regions = []
    queue = []

    if maxColor is None:
        for x in range(0, image.shape[0]):
            for y in range(0, image.shape[1]):
                pixel = image[x, y]
                # check only points those are not black and have not been visited yet
                if image[x, y] != 0 and visited[x, y] == 0:
                    region = []
                    region.append((x, y))    # points of actual region
                    queue.append((x, y))    # put point into queue
                    while len(queue) > 0:
                        # check neighbors
                        for coord in getPoints(image, queue[0][0], queue[0][1], eightPixels):
                            if visited[coord] == 0:
                                if image[coord] != 0:
                                    if abs(int(pixel) - int(image[coord])) == 0:
                                        queue.append(coord)
                                        region.append(coord)
                                        visited[coord] = 1
                        queue.pop(0)
                    regions.append(region)
                visited[x, y] = 1           # set point as visited to optimize

    if maxColor is not None:
        for x in range(0, image.shape[0]):
            for y in range(0, image.shape[1]):
                pixel = image[x, y]
                # check only points those are not black and have not been visited yet
                if image[x, y] != 0 and visited[x, y] == 0 and image[x, y] <= maxColor:
                    region = []
                    region.append((x, y))  # points of actual region
                    queue.append((x, y))  # put point into queue
                    while len(queue) > 0:
                        # check neighbors
                        for coord in getPoints(image, queue[0][0], queue[0][1], eightPixels):
                            if visited[coord] == 0:
                                if image[coord] != 0:
                                    if abs(int(pixel) - int(image[coord])) == 0:
                                        queue.append(coord)
                                        region.append(coord)
                                        visited[coord] = 1
                        queue.pop(0)
                    regions.append(region)
                visited[x, y] = 1  # set point as visited to optimize

    # print regions into output image
    for region in regions:
        if (minRegionSize is None or len(region) >= minRegionSize) and (maxRegionSize is None or len(region) <= maxRegionSize):
            for coords in region:
                outimg[coords] = 255

    return outimg

def main():

    # load image
    img = cv2.imread('image.png', cv2.IMREAD_COLOR)
    #img = cv2.resize(img, (640, 480))

    # gray scale
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    # blur gray scale image
    blurred = cv2.blur(gray, (5, 5))

    # get background mask by thresholding
    ret, backgroundMask = cv2.threshold(blurred, 5, 255, cv2.THRESH_BINARY)

    # apply mask
    masked = cv2.bitwise_and(img, img, mask=backgroundMask)

    # extract green channel
    _, green, _ = cv2.split(masked)

    clahe = cv2.createCLAHE(clipLimit=3.0, tileGridSize=(8, 8))
    equ = clahe.apply(green)

    thresholded = cv2.adaptiveThreshold(equ, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 51, 1)

    inverted = cv2.bitwise_not(thresholded)
    structuringElement = cv2.getStructuringElement(cv2.MORPH_RECT, (5, 5))
    opening = cv2.morphologyEx(inverted, cv2.MORPH_OPEN, structuringElement)

    growing = regionGrowing(opening, maxRegionSize=2000, eightPixels=True)
    bloodVessles = cv2.subtract(opening, growing)

    threshold = np.amax(equ) / 2
    maxcolor = np.amax(equ) / 5

    blurred = equ

    # thresholding
    _, thresh = cv2.threshold(blurred, threshold, np.amax(blurred), cv2.THRESH_TOZERO_INV)

    detection = regionGrowing(thresh, threshold=10, maxRegionSize=70, maxColor=maxcolor)

    res = cv2.subtract(detection, bloodVessles)

    # mark extracted circles into image
    w, h, _ = img.shape
    for i in range(0, w):
        for j in range(0, h):
            if res[i, j] != 0:
                img[i, j] = res[i, j]

    cv2.imwrite('result.png', img)

if __name__ == '__main__':
    main()