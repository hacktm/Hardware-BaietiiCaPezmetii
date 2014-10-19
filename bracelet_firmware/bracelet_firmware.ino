#include <math.h>

const int xPin = A6;
const int yPin = A5;
const int zPin = A4;
const int b1Pin = 5;
const int b2Pin = 3; 
const int b3Pin = 4;

int xVal;
int yVal;
int zVal;
float xAccel;
float prefxAccel;
float fXg = 0;
float yAccel;
float prefyAccel;
float fYg = 0;
float zAccel;
float fZg = 0;
float cancelScale;

int b1Val;
int b2Val;
int b3Val;

int dt;
unsigned long oldTime;
int old_xAccel;

float vx;
float old_vx;
float dx;

// Val Ranges:
// initialize to mid-range and allow calibration to
// find the minimum and maximum for each axis
int xValMin = 240;
int xValMax = 760;

int yValMin = 290;
int yValMax = 790;

int zValMin = 156;
int zValMax = 660;

// Take multiple samples to reduce noise
const int sampleSize = 10;

void setup() 
{
  analogReference(EXTERNAL);
  Serial.begin(9600);
  unsigned long time = millis();
//  while(millis()-time < 1000)
//  {
//    xVal = ReadAxis(xPin);
//    yVal = ReadAxis(yPin);
//    zVal = ReadAxis(zPin);    
//    AutoCalibrate(xVal,yVal,zVal);
//  }
  Serial.print("Val Ranges: X: ");
    Serial.print(xValMin);
    Serial.print("-");
    Serial.print(xValMax);
    
    Serial.print(", Y: ");
    Serial.print(yValMin);
    Serial.print("-");
    Serial.print(yValMax);
    
    Serial.print(", Z: ");
    Serial.print(zValMin);
    Serial.print("-");
    Serial.print(zValMax);
    Serial.println();
    Serial.print(xVal);
    Serial.print(", ");
    Serial.print(yVal);
    Serial.print(", ");
    Serial.print(zVal);
    
    // Convert raw values to 'milli-Gs"
    long xScaled = map(xVal, xValMin, xValMax, -1000, 1000);
    long yScaled = map(yVal, yValMin, yValMax, -1000, 1000);
    long zScaled = map(zVal, zValMin, zValMax, -1000, 1000);
  
    // re-scale to fractional Gs
    float xAccel = xScaled / 1000.0;
    float yAccel = yScaled / 1000.0;
    float zAccel = zScaled / 1000.0;
  
    Serial.print(" :: ");
    Serial.print(xAccel);
    Serial.print("m/s2, ");
    Serial.print(yAccel);
    Serial.print("m/s2, ");
    Serial.print(zAccel);
    Serial.println("m/s2");
    
    pinMode(b1Pin, INPUT);
    pinMode(b2Pin, INPUT);
    pinMode(b3Pin, INPUT);
}

void loop() 
{
    b1Val = digitalRead(b1Pin);
    b2Val = digitalRead(b2Pin);
    b3Val = digitalRead(b3Pin);
    
    unsigned long time = millis();
    dt = time - oldTime;
    xVal = ReadAxis(xPin);
    yVal = ReadAxis(yPin);
    zVal = ReadAxis(zPin);
  
    long xScaled = map(xVal, xValMin, xValMax, -1000, 1000);
    long yScaled = map(yVal, yValMin, yValMax, -1000, 1000);
    long zScaled = map(zVal, zValMin, zValMax, -1000, 1000);
    
    fXg = xVal * 0.5 + (fXg * (1.0 - 0.5));
    fYg = yVal * 0.5 + (fYg * (1.0 - 0.5));
    fZg = zVal * 0.5 + (fZg * (1.0 - 0.5));
  
    // re-scale to fractional Gs
    xAccel = xScaled / 1000.0 * 9.8;
    yAccel = yScaled / 1000.0 * 9.8;
    zAccel = zScaled / 1000.0 * 9.8;
    if(xAccel > 3 || xAccel < -3)
    {
      vx += (xAccel * (dt/1000.0));
      dx += (vx * (dt/1000.0));
    }
  
//    Serial.print(" :: ");
//    Serial.print(xAccel);
//    Serial.print("G, ");
//    Serial.print(yAccel);
//    Serial.print("G, ");
//    Serial.print(zAccel);
//    Serial.print("G");
//    Serial.println();
    old_vx = vx;
    old_xAccel = xAccel;
    oldTime = time;
    /*Serial.print("vx=  ");
    Serial.print(vx);
    Serial.print("xAccel    ");
    Serial.print(xAccel);
    Serial.print("roll  ");
    Serial.print(Roll());
    Serial.println();*/ 
    
    if (Roll() < -0.3)
    {
      Serial.write('4');
    } else if(Roll() > 0.3)
    {
      Serial.write('3');
    } else if (Roll() > -0.3 && Roll() < 0.3) {
      Serial.write('5');
    }
    if (b2Val == 1)
    {
      Serial.write('2');
    }else if (b3Val == 1)
    {
      Serial.write('1');
    }
    if (b1Val == 1)
    {
      Serial.write('6');
    }
    
    delay(100);   
}

double Roll()
{
  return atan2(-xAccel,zAccel);
}

//
// Read "sampleSize" samples and report the average
//
int ReadAxis(int axisPin)
{
  long reading = 0;
  analogRead(axisPin);
  delay(1);
  for (int i = 0; i < sampleSize; i++)
  {
    reading += analogRead(axisPin);
  }
  return reading/sampleSize;
}

//
// Find the extreme raw readings from each axis
//
void AutoCalibrate(int xVal, int yVal, int zVal)
{
  Serial.println("Calibrate");
  if (xVal < xValMin)
  {
    xValMin = xVal;
  }
  if (xVal > xValMax)
  {
    xValMax = xVal;
  }
  
  if (yVal < yValMin)
  {
    yValMin = yVal;
  }
  if (yVal > yValMax)
  {
    yValMax = yVal;
  }

  if (zVal < zValMin)
  {
    zValMin = zVal;
  }
  if (zVal > zValMax)
  {
    zValMax = zVal;
  }
}
