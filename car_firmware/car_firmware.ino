#define DEBUG     1
#define VERSION   2.00
#define BUILDTYPE "BETA"

#include <SoftwareSerial.h>

#ifdef DEBUG  //debuging mode
char *serialBuffer;
char serialByte;
int  iByteNo = 0;
#endif

const int MOTOR2_PIN1 = 3;
const int MOTOR2_PIN2 = 5;
const int MOTOR1_PIN1 = 6;
const int MOTOR1_PIN2 = 9;

char recvCmd;

const int xbeeRxPin = 4;
const int xbeeTxPin = 2;
SoftwareSerial xbeeRemote(xbeeRxPin, xbeeTxPin);

void setup()
{
#ifdef DEBUG
	Serial.begin(115200);
	Serial.println("DEBUG MODE ENABLED");
#endif
  
	//set the MOTOR pins
	pinMode(MOTOR2_PIN1, OUTPUT);
	pinMode(MOTOR2_PIN2, OUTPUT);
	pinMode(MOTOR1_PIN1, OUTPUT);
	pinMode(MOTOR1_PIN2, OUTPUT);

        xbeeRemote.begin(9600);
}

void loop()
{
        receiveBufferXbee();
}

void receiveBufferXbee()
{
  while (xbeeRemote.available() > 0) {
    recvCmd = xbeeRemote.read();

    if (recvCmd == '1')
    {  
      GoForward(250);
      //Serial.println(recvCmd);   
    }  
    if (recvCmd == '2')
    {
      GoBackward(250);
    }
    if (recvCmd == '6')
    {
      Stop();
    }
      
    if (recvCmd == '4')
      TurnLeft();
    else if (recvCmd == '3')
      TurnRight();
    else if (recvCmd == '5')
      Neutral();
  }
}

void GoForward(int speed)
{
  analogWrite(MOTOR2_PIN1, speed);
  analogWrite(MOTOR2_PIN2, 0);
}

void GoBackward(int speed)
{
  analogWrite(MOTOR2_PIN1, 0);
  analogWrite(MOTOR2_PIN2, speed);
}

void Stop()
{
  analogWrite(MOTOR2_PIN1, 0);
  analogWrite(MOTOR2_PIN2, 0);
}

void TurnLeft()
{
  analogWrite(MOTOR1_PIN1, 255);
  analogWrite(MOTOR1_PIN2, 0);
}

void TurnRight()
{
  analogWrite(MOTOR1_PIN1, 0);
  analogWrite(MOTOR1_PIN2, 255);
}

void Neutral()
{
  analogWrite(MOTOR1_PIN1, 0);
  analogWrite(MOTOR1_PIN2, 0);
}
