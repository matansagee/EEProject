#include <stdio.h>
#include <wiringPi.h>


void SetupGpios()
{
wiringPiSetupGpio();  //Initialize wiringPi using the broadcom GPIO pin numbers

const int device_0 = 18; 
const int device_1 = 23; 
const int device_2 = 17;
const int device_3 = 22;

pinMode (device_0, OUTPUT);
pinMode (device_1, OUTPUT);
pinMode (device_2, OUTPUT);
pinMode (device_3, OUTPUT);

}

int StartDevicemain(int device_num)
{
 
  switch (device_num)
 {
   case (0):
  {
    if (!digitalRead(device_0))       // if off will ruturn 0
       digitalWrite(device_0, HIGH);
    if (digitalRead(device_3)) return 0;
    else return 1;  
  }
   case (1):
  {
   if (!digitalRead(device_1))       // if off will ruturn 0
      digitalWrite(device_1, HIGH);
   if (digitalRead(device_1)) return 0;
   else return 1;  
  }
   case (2):
  {
   if (!digitalRead(device_2))       // if off witll ruturn 0
      digitalWrite(device_2, HIGH);
   if (digitalRead(device_2)) return 0;
   else return 1;  
  }
   case (3):
  {
   if (!digitalRead(device_3))       // if off witll ruturn 0
      else digitalWrite(device_3, HIGH);
   if (digitalRead(device_3)) return 0;
   else return 1;  
  }
  }
}

int StopDevicemain(int device_num)
{
 
  switch (device_num) 
 {
   case (0):
  {
    if (digitalRead(device_0))       // if on will ruturn 1
       digitalWrite(device_0, LOW);
    if (!digitalRead(device_3)) return 0;
    else return 1;  
  }
   case (1):
  {
   if (digitalRead(device_1))       // if on will ruturn 1
      digitalWrite(device_1, LOW);
   if (!digitalRead(device_1)) return 0;
   else return 1;  
  }
   case (2):
  {
   if (digitalRead(device_2))       // if off witll ruturn 0
      digitalWrite(device_2, LOW);
   if (!digitalRead(device_2)) return 0;
   else return 1;  
  }
   case (3):
  {
   if (!digitalRead(device_3))       // if on witll ruturn 0
      digitalWrite(device_3, LOW);
   if (!digitalRead(device_3)) return 0;
   else return 1;  
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

