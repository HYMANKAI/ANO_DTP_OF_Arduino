#include "Arduino.h"
#include "ANO_DTP.h"
#include "ANO_CONFIG.h"

#ifdef Arduino
ANO::ANO(uint32_t baudrate)
#ifdef Dafault_SERIAL0
    : function(&Serial)
#elif defined USE_THE_SERIAL1
    : function(&Serial1)
#elif defined USE_THE_SERIAL2
    : function(&Serial2)
#elif defined USE_THE_SERIAL3
    : function(&Serial3)
#endif
{
  function->begin(baudrate);
}
#elif defined Energia

#endif
//*************************************************************************
//传送数据(内部调用OR单独使用)
// fun:功能字.
// data:数据缓存区,最多30字节!!
// len:data区有效数据个数
//*************************************************************************
void ANO::Report(uint8_t fun, uint8_t *data, uint8_t len)
{
  uint8_t send_buf[35];
  uint8_t i;
  if (len > 30)
    return;

  send_buf[len + 4] = 0;
  send_buf[len + 5] = 0;
  send_buf[0] = 0XAA;
  send_buf[1] = 0XFF;
  send_buf[2] = fun;
  send_buf[3] = len;

  for (i = 0; i < len; i++)
    send_buf[4 + i] = data[i];

  for (i = 0; i < len + 4; i++)
  {
    send_buf[len + 4] += send_buf[i];
    send_buf[len + 5] += send_buf[len + 4];
  }
  for (i = 0; i < len + 6; i++)
#ifdef Arduino
    function->write(send_buf[i]);
#elif defined Energia
    Serial.write(send_buf[i]);
#endif
}
//*************************************************************************
//飞机姿态等基本信息
// aac  X Y Z
// gyro X Y Z
//*************************************************************************
void ANO::Raw_Data(int16_t aacx, int16_t aacy, int16_t aacz, int16_t gyrox, int16_t gyroy, int16_t gyroz)
{
  uint8_t tbuf[13];

  tbuf[0] = aacx & 0XFF;
  tbuf[1] = (aacx >> 8) & 0XFF;

  tbuf[2] = aacy & 0XFF;
  tbuf[3] = (aacy >> 8) & 0XFF;
  tbuf[4] = aacz & 0XFF;
  tbuf[5] = (aacz >> 8) & 0XFF;

  tbuf[6] = gyrox & 0XFF;
  tbuf[7] = (gyrox >> 8) & 0XFF;
  tbuf[8] = gyroy & 0XFF;
  tbuf[9] = (gyroy >> 8) & 0XFF;
  tbuf[10] = gyroz & 0XFF;
  tbuf[11] = (gyroz >> 8) & 0XFF;

  tbuf[12] = 0xFF;

  Report(0x01, tbuf, 13);
}
//*************************************************************************
//飞机姿态等基本信息(欧拉角)
// ROL
// PIT
// YAW
//*************************************************************************
void ANO::Angle_Data(float roll_o, float pitch_o, float yaw_o)
{
  uint8_t tbuf[7];

  int16_t roll = 100 * roll_o;
  int16_t pitch = 100 * pitch_o;
  int16_t yaw = 100 * yaw_o;

  tbuf[0] = roll & 0XFF;
  tbuf[1] = (roll >> 8) & 0XFF;

  tbuf[2] = pitch & 0XFF;
  tbuf[3] = (pitch >> 8) & 0XFF;

  tbuf[4] = yaw & 0XFF;
  tbuf[5] = (yaw >> 8) & 0XFF;

  tbuf[6] = 0x00;

  Report(0x03, tbuf, 7);
}
//*************************************************************************
//飞机姿态等基本信息（四元数）
// V0
// V1
// V2
// V3
//*************************************************************************
void ANO::Quaternion_Data(int16_t V0, int16_t V1, int16_t V2, int16_t V3)
{
  uint8_t tbuf[9];

  V0 *= 10000 * V0;
  V1 *= 10000 * V1;
  V2 *= 10000 * V2;
  V3 *= 10000 * V3;

  tbuf[0] = V0 & 0XFF;
  tbuf[1] = (V0 >> 8) & 0XFF;

  tbuf[2] = V1 & 0XFF;
  tbuf[3] = (V1 >> 8) & 0XFF;

  tbuf[4] = V2 & 0XFF;
  tbuf[5] = (V2 >> 8) & 0XFF;

  tbuf[6] = V3 & 0XFF;
  tbuf[7] = (V3 >> 8) & 0XFF;

  tbuf[8] = 0x01;

  Report(0x04, tbuf, 9);
}
//*************************************************************************
//飞机高度数据
// ALT_FU：融合后对地高度，单位厘米。
// ALT_ADD：附加高度传感高度数据，如超声波、激光测距，单位厘米。
//*************************************************************************
void ANO::Height_Data(int32_t ALT_FU, int32_t ALT_ADD)
{
  uint8_t tbuf[9];
  uint8_t i;

  tbuf[0] = ALT_FU & 0XFF;
  tbuf[1] = (ALT_FU >> 8) & 0XFF;
  tbuf[2] = (ALT_FU >> 16) & 0XFF;
  tbuf[3] = (ALT_FU >> 24) & 0XFF;

  tbuf[4] = ALT_ADD & 0XFF;
  tbuf[5] = (ALT_ADD >> 8) & 0XFF;
  tbuf[6] = (ALT_ADD >> 16) & 0XFF;
  tbuf[7] = (ALT_ADD >> 24) & 0XFF;

  tbuf[8] = 0x01;

  Report(0x05, tbuf, 9);
}

//***********************************通用智能车部分********************************************************//

///*********************************************************************************************************
//*  函数名称：void CAR_Inductance(int16_t L1, int16_t L2, int16_t L3,int16_t L4, int16_t L5, int16_t L6,
//*				int16_t L7, int16_t L8, uint8_t Frame)
//*  功能说明：发送八个电感ADC原始数据,兼容麦克纳姆轮多对电感
//*  参数说明：L1~L8:ADC原始数据；Frame：帧ID:可以直接使用灵活格式帧0xF1~0xFA
//*  函数返回：无
//*  修改时间：2022年2月8日
//*  备    注：此处省略frame参量，直接使用灵活格式帧0xF1
//**********************************************************************************************************/
void ANO::CAR_Inductance(int16_t L1, int16_t L2, int16_t L3, int16_t L4, int16_t L5, int16_t L6, int16_t L7, int16_t L8)
{
  uint8_t tbuf[16];

  tbuf[0] = L1 & 0XFF;
  tbuf[1] = (L1 >> 8) & 0XFF;

  tbuf[2] = L2 & 0XFF;
  tbuf[3] = (L2 >> 8) & 0XFF;

  tbuf[4] = L3 & 0XFF;
  tbuf[5] = (L3 >> 8) & 0XFF;

  tbuf[6] = L4 & 0XFF;
  tbuf[7] = (L4 >> 8) & 0XFF;

  tbuf[8] = L5 & 0XFF;
  tbuf[9] = (L5 >> 8) & 0XFF;

  tbuf[10] = L6 & 0XFF;
  tbuf[11] = (L6 >> 8) & 0XFF;

  tbuf[12] = L7 & 0XFF;
  tbuf[13] = (L7 >> 8) & 0XFF;

  tbuf[14] = L8 & 0XFF;
  tbuf[15] = (L8 >> 8) & 0XFF;

  Report(0xF1, tbuf, 16);
}
///*********************************************************************************************************
//*  函数名称：void CAR_MotorPID(float KP1, float KI1, float KD1,float KP2, float KI2, float KD2)
//*  功能说明：发送电机PID参数
//*  参数说明：两组PID参数
//*  函数返回：无
//*  修改时间：2022年2月8日
//*  备    注：使用灵活格式帧0xF2
//**********************************************************************************************************/
void ANO::CAR_MotorPID(float KP1, float KI1, float KD1, float KP2, float KI2, float KD2)
{
  uint8_t tbuf[12];

  int16_t kp1 = 100 * KP1;
  int16_t ki1 = 100 * KI1;
  int16_t kd1 = 100 * KD1;

  int16_t kp2 = 100 * KP2;
  int16_t ki2 = 100 * KI2;
  int16_t kd2 = 100 * KD2;

  tbuf[0] = kp1 & 0XFF;
  tbuf[1] = (kp1 >> 8) & 0XFF;

  tbuf[2] = ki1 & 0XFF;
  tbuf[3] = (ki1 >> 8) & 0XFF;

  tbuf[4] = kd1 & 0XFF;
  tbuf[5] = (kd1 >> 8) & 0XFF;

  tbuf[6] = kp2 & 0XFF;
  tbuf[7] = (kp2 >> 8) & 0XFF;

  tbuf[8] = ki2 & 0XFF;
  tbuf[9] = (ki2 >> 8) & 0XFF;

  tbuf[10] = kd2 & 0XFF;
  tbuf[11] = (kd2 >> 8) & 0XFF;

  Report(0xF2, tbuf, 12);
}
///*********************************************************************************************************
//*  函数名称：CAR_ServoPID(float SKP, float SKI, float SKD)
//*  功能说明：发送舵机PID参数
//*  参数说明：舵机PID参数
//*  函数返回：无
//*  修改时间：2022年2月8日
//*  备    注：使用灵活格式帧0xF3
//**********************************************************************************************************/
void ANO::CAR_ServoPID(float SKP, float SKI, float SKD)
{
  uint8_t tbuf[6];

  int16_t skp = 100 * SKP;
  int16_t ski = 100 * SKI;
  int16_t skd = 100 * SKD;

  tbuf[0] = skp & 0XFF;
  tbuf[1] = (skp >> 8) & 0XFF;

  tbuf[2] = ski & 0XFF;
  tbuf[3] = (ski >> 8) & 0XFF;

  tbuf[4] = skd & 0XFF;
  tbuf[5] = (skd >> 8) & 0XFF;

  Report(0xF3, tbuf, 6);
}
///*********************************************************************************************************
//*  函数名称：void CAR_PWM_ENCODER(int16_t LPWM, int16_t LENCODE, int16_t RPWM, int16_t RENCODE, int16_t SPWM)
//*  功能说明：发送左右电机PWM，舵机PWM和编码器数据
//*  参数说明：电机舵机编码器参数
//*  函数返回：无
//*  修改时间：2022年2月8日
//*  备    注：使用灵活格式帧0xF4
//**********************************************************************************************************/
void ANO::CAR_PWM_ENCODER(int16_t LPWM, int16_t LENCODE, int16_t RPWM, int16_t RENCODE, int16_t SPWM)
{
  uint8_t tbuf[10];

  tbuf[0] = LPWM & 0XFF;
  tbuf[1] = (LPWM >> 8) & 0XFF;

  tbuf[2] = LENCODE & 0XFF;
  tbuf[3] = (LENCODE >> 8) & 0XFF;

  tbuf[4] = RPWM & 0XFF;
  tbuf[5] = (RPWM >> 8) & 0XFF;

  tbuf[6] = RENCODE & 0XFF;
  tbuf[7] = (RENCODE >> 8) & 0XFF;

  tbuf[8] = SPWM & 0XFF;
  tbuf[9] = (SPWM >> 8) & 0XFF;

  Report(0xF4, tbuf, 10);
}
