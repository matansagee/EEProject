#include <stdio.h>
#include <wiringPi.h>

const int device_0 = 18; 
const int device_1 = 23; 
const int device_2 = 17;
const int device_3 = 22;

void SetupGpios()
{
wiringPiSetupGpio();  //Initialize wiringPi using the broadcom GPIO pin numbers

pinMode (device_0, OUTPUT);
pinMode (device_1, OUTPUT);
pinMode (device_2, OUTPUT);
pinMode (device_3, OUTPUT);

}

int StartDevice(int device_num)
{
 
  switch (device_num)
 {
   case (0):
  {
    if (!digitalRead(device_0))       // if off will ruturn 0
       digitalWrite(device_0, HIGH);
    return 0;
  }
   case (1):
  {
   if (!digitalRead(device_1))       // if off will ruturn 0
      digitalWrite(device_1, HIGH);
   return 0;
  }
   case (2):
  {
   if (!digitalRead(device_2))       // if off witll ruturn 0
      digitalWrite(device_2, HIGH);
   return 0;
  }
   case (3):
  {
   if (!digitalRead(device_3))       // if off witll ruturn 0
      digitalWrite(device_3, HIGH);
   return 0;
  }
  }
}

int StopDevice(int device_num)
{
 
  switch (device_num) 
 {
   case (0):
  {
    if (digitalRead(device_0))       // if on will ruturn 1
       digitalWrite(device_0, LOW);
    return 0;
  }
   case (1):
  {
   if (digitalRead(device_1))       // if on will ruturn 1
      digitalWrite(device_1, LOW);
   return 0;
  }
   case (2):
  {
   if (digitalRead(device_2))       // if off witll ruturn 0
      digitalWrite(device_2, LOW);
   return 0;
  }
   case (3):
  {
   if (!digitalRead(device_3))       // if on witll ruturn 0
      digitalWrite(device_3, LOW);
   return 0;
  }
 }
}

int StopAllDevices()
{
   digitalWrite(device_0, LOW);
   digitalWrite(device_1, LOW);
   digitalWrite(device_2, LOW);
   digitalWrite(device_3, LOW);

}

