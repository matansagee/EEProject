#include <stdio.h>
#include <wiringPi.h>

// if we want to set manualy we will use the syntax:
//wiringPiSetupGpio()
//pinMode(17, INPUT);
//pinMode (23, OUTPUT);

const int pwmPin = 18; //PWM LED
const int ledPin = 23; // Regular LED
const int butPin = 17; // Active-low button

const int pwmValue = 75; // LED brightness

int main(void)
{
//wiringPiSetup();
wiringPiSetupGpio();  //Initialize wiringPi using the broadcom GPIO pin numbers

pinMode (pwmPin, PWM_OUTPUT);
pinMode (ledPin, OUTPUT);
pinMode (butPin, INPUT);
pullUpDnControl (butPin, PUD_UP);

printf("Blinker is running! (are you excited?) Press CTRL+C to stop that thing.\n");

while(1)
{
       if (digitalRead(butPin)) // Button is released if this returns 1
        {
            pwmWrite(pwmPin, pwmValue); // PWM LED at bright setting
            digitalWrite(ledPin, LOW);     // Regular LED off
        }
        else // If digitalRead returns 0, button is pressed
        {
            pwmWrite(pwmPin, 1024 - pwmValue); // PWM LED at dim setting
            // Do some blinking on the ledPin:
            digitalWrite(ledPin, HIGH); // Turn LED ON
            delay(75); // Wait 75ms
             digitalWrite(ledPin, LOW); // Turn LED OFF
            delay(75); // Wait 75ms again
        }
}

return 0;
}

