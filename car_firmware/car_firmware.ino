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

#define  btBufferLength 32
char btBuffer[btBufferLength];
//uint8_t  btBuffer[btBufferLength];
uint8_t  btBufferCount;
char     btByte;
char     delimiter = 59; //';'
char     ack = 33; // '!'
int      waitTime;

float data[3];
int   intdata[3];

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
        waitTime = 10;
}

void loop()
{
        receiveBuffer();
}

/*void receiveBufferXbee()
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
}*/

bool receiveBuffer()
{
	uint8_t lastByte;
	boolean timeout = false;
	while (!timeout) {
		while (xbeeRemote.available() > 0) {
			lastByte = xbeeRemote.read();
      
			if (lastByte == ack) {
				Move();
				clearBuffer(btBuffer, btBufferCount, btBufferLength);
			} else if (btBufferCount < btBufferLength) {
				btBuffer[btBufferCount] = lastByte;
				btBufferCount++;
			} else {
				return false;
			}
		}
    
		/*if (xbeeRemote.available() <= 0 && !timeout) {
			if (waitTime > 0) delayMicroseconds(waitTime);
			if (xbeeRemote.available() <= 0) timeout = true;
		}*/
	}
	return timeout;
}


void getFloatVal(float values[], char m_buffer[], uint8_t m_bufferCount)
{
  int t = 0;
  int pos = 0;
  
  int start = 1;
  for (int endpos = 1; endpos<m_bufferCount; endpos++) {
    if (btBuffer[endpos] == delimiter) {
      char b[(endpos-start)+1];
      t=0;
      for (int i = start; i<endpos; i++) {
        b[t++] = (char)btBuffer[i];
      }
      b[t] = '\0';
      values[pos++] = atof(b);
      start = endpos+1;
    }
  }
  char b[(m_bufferCount-start)+1];
  t=0;
  for (int i = start; i<m_bufferCount; i++) {
    b[t++] = (char)btBuffer[i];
  }
  b[t] = '\0';
  values[pos] = atof(b);
}

void clearBuffer(char m_buffer[], uint8_t m_bufferCount, int m_bufferLength)
{
	for (uint8_t i=0; i < m_bufferLength; i++)
	{
	  btBuffer[i] = 0;
	}
	//m_buffer[0] = 0; //clear the buffer
	//m_bufferCount = 0;
	btBufferCount = 0;
}

void Move()
{
	getFloatVal(data, btBuffer, btBufferCount);


	Serial.print("data0: ");
	Serial.println(data[0]);
	Serial.print("data1: ");
	Serial.println(data[1]);

  
	int throtle = int(data[1]);
	int steering = int(data[0]);
  
	if (throtle > 1 && throtle < 10)
        {
		//GoForward(throtle*28);
                GoBackward(abs(throtle)*(28));
        }
	else if (throtle < 0 && throtle > -10)
        {
		//GoBackward(throtle*(-28));
                GoForward(abs(throtle)*28);
        }
	else if (throtle == 0)
		Stop();
    
	if (steering == 0)
		Neutral();
	else if (steering < -1)
		TurnLeft();
	else if (steering > 1)
		TurnRight();
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
