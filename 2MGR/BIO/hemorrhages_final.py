import cv2
import numpy as np

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
def regionGrowing(image, minRegionSize=None, maxRegionSize=None, eightPixels=False):
    visited = np.zeros((image.shape[0], image.shape[1]), dtype=np.uint8)
    outimg = np.zeros((image.shape[0], image.shape[1]), dtype=np.uint8)
    regions = []
    queue = []

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

    # print regions into output image
    for region in regions:
        if (minRegionSize is None or len(region) >= minRegionSize) and (maxRegionSize is None or len(region) <= maxRegionSize):
            for coords in region:
                outimg[coords] = 255

    return outimg

def main():

    # load image
    img = cv2.imread('image.png', cv2.IMREAD_COLOR)

    # extract red channel
    _, green, _ = cv2.split(img)

    clahe = cv2.createCLAHE(clipLimit=3.0, tileGridSize=(8, 8))
    cl1 = clahe.apply(green)

    ret, thresh1 = cv2.threshold(cl1, 40, 255, cv2.THRESH_BINARY_INV)

    # morphological opening
    structuringElement = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (4, 4))
    hemo = cv2.morphologyEx(thresh1, cv2.MORPH_OPEN, structuringElement)

    # find contours
    image, contours, hierarchy = cv2.findContours(hemo, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

    # find elements
    outimg = np.zeros((img.shape[0], img.shape[1]), dtype=np.uint8)
    for x in range(len(contours)):
        _, _, w, h = cv2.boundingRect(contours[x])
        aspect_ratio = float(w) / h
        # find elements with aspect ration between 0.6 to 1.4
        if aspect_ratio >= 0.60 and aspect_ratio <= 1.4:
            # find elements with height between 15 to 70 points
            if h >= 15 and h <= 70:
                # find elements with width between 15 to 70 points
                if w >= 15 and w <= 70:
                    cv2.drawContours(outimg, contours, x, 255, cv2.FILLED)

    # adaptive gauss threshold
    thresholded = cv2.adaptiveThreshold(green, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 71, 1)

    # invert colors
    inverted = cv2.bitwise_not(thresholded)

    # morphological opening
    structuringElement = cv2.getStructuringElement(cv2.MORPH_RECT, (5, 5))
    opening = cv2.morphologyEx(inverted, cv2.MORPH_OPEN, structuringElement)

    # get blood vessels
    growing = regionGrowing(opening, maxRegionSize=2000, eightPixels=True)
    bloodVessles = cv2.subtract(opening, growing)

    # subtract blood vessels from hemorrhages image
    result = cv2.subtract(outimg, bloodVessles)

    # draw result into image
    w, h, _ = img.shape
    for i in range(0, w):
        for j in range(0, h):
            if result[i, j] != 0:
                img[i, j] = result[i, j]

    cv2.imwrite('hemorrhages.png', img)


if __name__ == '__main__':
    main()

