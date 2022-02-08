#include "ANO_DTP.h"

ANO SEND(115200);

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  delay(300);
  SEND.Angle_Data(20,30,40);
  SEND.Quaternion_Data(1000,1000,0,0);
  SEND.Raw_Data(100,200,300,400,500,600);
}
