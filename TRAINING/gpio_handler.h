#ifndef GPIO_HANDLER
#define GPIO_HANDLER

void SetupGpios();
int StartDevice(int deviceNumber);
int StopDevice(int deviceNumber);
int StopAllDevices ();

#endif
