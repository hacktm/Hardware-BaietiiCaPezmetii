#include<math.h>

#define ANALOG0 14

class Accelerometer
{
    int p[3]; // which analog pins
    int a[3]; // acceleration, zero-based
    int b[3]; // acceleration bias/calibration information
    int g, t, r; // cached copies of calculations
    int scale; // scaling factor between ADC and gravity


public:
    Accelerometer(int pinX, int pinY, int pinZ)
    {
        pinMode((p[0] = pinX) + ANALOG0, INPUT);
        pinMode((p[1] = pinY) + ANALOG0, INPUT);
        pinMode((p[2] = pinZ) + ANALOG0, INPUT);
        for (int i = 0; i < 3; i++)
            b[i] = 512;
        g = t = r = 0;
        scale = 100;
    }

    void update()
    {
        for (int i = 0; i < 3; i++)
            a[i] = analogRead(p[i]) - b[i];
        g = t = r = 0;
    }

    void dump()
    {
        Serial.print("x"); Serial.print(a[0]);
        Serial.print("y"); Serial.print(a[1]);
        Serial.print("z"); Serial.print(a[2]);
        Serial.print("mg"); Serial.print(milligee());
        Serial.print("pitch"); Serial.print(pitch());
        Serial.print("roll"); Serial.print(roll());
        Serial.println();
    }

    void calibrate()
    {
        for (int i = 0; i < 3; i++)
            b[i] = analogRead(p[i]);
        b[2] -= scale;
        update();
    }

    int milligee()
    {
        if (g != 0) return g;
        long squared = 0.0;
        for (int i = 0; i < 3; i++)
            squared += (long)a[i] * (long)a[i];
        g = squared * 1000 / (scale*scale);
        return g;
    }

    int accel(int axis)
    {
        if (axis < 0 || axis > 3) return 0;
        return a[axis];
    }

    int roll()
    {
        if (r != 0) return r;
        r = (int)(atan2(a[0], a[2]) * 180. / M_PI);
        return r;
    }

    int pitch()
    {
        if (t != 0) return t;
        t = (int)(acos(a[1] / (float)scale) * 180. / M_PI);
        return t;
    }

    void loop()
    {
        update();
    }
};

void loop() { ; } 

void setup()
{
  Serial.begin(9600);
  
  Accelerometer accel = Accelerometer(0,1,2);
  int div = 0;
  while(1)
  {
    delay(20);
    accel.loop();
    accel.calibrate();
    if (--div <= 0) { div = 1; accel.dump(); } // print every 8th loop
  } 
}


