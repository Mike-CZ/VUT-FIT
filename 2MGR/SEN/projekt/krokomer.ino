/* 
 * Soubor:  krokomer.ino
 * Datum:   2017/11/19
 * Autor:   Bc. Lukas Pelanek, xpelan03@stud.fit.vutbr.cz
 * Projekt: SEN, Krokomer
 * Popis:   Jednoduchy krokomer, ktery je realizovan pomoci kitu Arduino a akcelerometru MMA7455. 
 *  
 * Pro zprovozneni aplikace je potreba nainstalovat knihovnu MMA_7455 (https://code.google.com/archive/p/mma-7455-arduino-library)
 * 
 * Zapojeni:
 * VCC - 5V
 * GND - GND 
 * SCL - A5
 * SDA - A4
*/
#include <Wire.h>
#include <MMA_7455.h>

// precision constant, may be changed
#define PRECISION 25

// delay before taking next sample
#define DELAY_MS 20

// timers
#define MIN_TIMEWIN 300
#define MAX_TIMEWIN 2000
float newTime = 0;
float oldTime = 0;

// max axis values
#define MAX_VALUE 255
float xMax = -MAX_VALUE;
float yMax = -MAX_VALUE;
float zMax = -MAX_VALUE;
float xMin = MAX_VALUE;
float yMin = MAX_VALUE;
float zMin = MAX_VALUE;

// dynamic threshold
float xThresh = 0;
float yThresh = 0;
float zThresh = 0;

// Smoothing filter
#define FILTER_SAMPLES 4
char xFilter[FILTER_SAMPLES] = {0};
char yFilter[FILTER_SAMPLES] = {0};
char zFilter[FILTER_SAMPLES] = {0};

// axis values
float xNew = 0;
float yNew = 0;
float zNew = 0;
float xOld = 0;
float yOld = 0;
float zOld = 0;

// counter
int counter = 0;

// number of steps
int steps = 0;

// sensor instance
MMA_7455 sensor = MMA_7455();

void printSteps() {
  Serial.print("Pocet kroku: ");
  Serial.println(steps, DEC);
}

// count step
void countStep() {
  oldTime = newTime;
  newTime = millis();
  float diff = newTime - oldTime;

  // count step if time is between timers
  if (diff >= MIN_TIMEWIN && diff <= MAX_TIMEWIN) {
    steps++;
    printSteps();
  }
  
}

// setup device
void setup() {
  // axis values
  char x, y, z, calX, calY, calZ;
    
  Serial.begin(9600);  
  delay(500);
  Serial.println("Inicializace krokomeru...");

  // set accelerometer sensitivity
  sensor.initSensitivity(2);

  // reset calibration offsets
  sensor.calibrateOffset(0, 0, 0);

  // start calibration
  Serial.println("Zahajeni kalibrace krokomeru...");
  delay(500);
  
  // read axis values
  x = sensor.readAxis('x');
  y = sensor.readAxis('y');
  z = sensor.readAxis('z');

  // get values for calibration
  calX = (-1) * x;
  calY = (-1) * y;
  calZ = 64 - z;

  // set calibration offsets. x = 0, y = 0, z = 64
  sensor.calibrateOffset(calX, calY, calZ);
  
  delay(500);
  Serial.println("Kalibrace dokoncena. Spoustim krokomer.");
  printSteps();
}

// main loop
void loop() {
  float xResult, yResult, zResult;
  
  // read data into filter registers
  for (int i = 0; i < FILTER_SAMPLES; i++) {
    xFilter[i] = sensor.readAxis('x');
    yFilter[i] = sensor.readAxis('y');
    zFilter[i] = sensor.readAxis('z');
  }

  // sum registers
  xResult = (xFilter[0] + xFilter[1] + xFilter[2] + xFilter[3]) / 4;
  yResult = (yFilter[0] + yFilter[1] + yFilter[2] + yFilter[3]) / 4;
  zResult = (zFilter[0] + zFilter[1] + zFilter[2] + zFilter[3]) / 4;

  // find max values
  if (xResult > xMax) xMax = xResult;
  if (yResult > yMax) yMax = yResult;
  if (zResult > zMax) zMax = zResult;

  // find min values
  if (xResult < xMin) xMin = xResult;
  if (yResult < yMin) yMin = yResult;
  if (zResult < zMin) zMin = zResult;

  // increase counter
  counter++;

  if (counter >= 50) {
    // reset counter
    counter = 0;

    // calculate new threshold
    xThresh = (xMax + xMin) / 2;
    yThresh = (yMax + yMin) / 2;
    zThresh = (zMax + zMin) / 2;

    // reinitialize max and min values
    xMax = -MAX_VALUE;
    yMax = -MAX_VALUE;
    zMax = -MAX_VALUE;
    xMin = MAX_VALUE;
    yMin = MAX_VALUE;
    zMin = MAX_VALUE;
  }

  xOld = xNew;
  yOld = yNew;
  zOld = zNew;
    
  if (abs(xResult - xNew) > PRECISION) {
    xNew = xResult;
  }
  if (abs(yResult - yNew) > PRECISION) {
    yNew = yResult;
  }
    
  if (abs(zResult - zNew) > PRECISION) {
    zNew = zResult;
  }
    
  if (abs(xThresh) > abs(yThresh)) {
    if (abs(xThresh) > abs(zThresh)) {
      // x axis is largest accel
      if (xOld > xThresh && xNew < xThresh) {
        countStep();
      }
    }
    else {
      // z axis is largest accel
      if (zOld > zThresh && zNew < zThresh){
        countStep();
      }
    }
  }
  else {
    if (abs(yThresh) > abs(zThresh)) {
      // y axis is the largest accel
      if (yOld > yThresh && yNew < yThresh) {
        countStep();
      }
    }
    else {
      // z axis is the largest accel
      if (zOld > zThresh && zNew < zThresh){
        countStep();
      }
    }
  }
  delay(DELAY_MS);
}

