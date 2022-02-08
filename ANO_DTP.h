#ifndef _ANO_DTP_H_
#define _ANO_DTP_H_

#include "ANO_CONFIG.h"

class ANO
{
public:
    void Report(uint8_t fun, uint8_t *data, uint8_t len);
    void Angle_Data(float roll_o, float pitch_o, float yaw_o);
    void Quaternion_Data(int16_t V0, int16_t V1, int16_t V2, int16_t V3);
    void Raw_Data(int16_t aacx, int16_t aacy, int16_t aacz, int16_t gyrox, int16_t gyroy, int16_t gyroz);
    void Height_Data(int32_t ALT_FU,int32_t ALT_ADD);
    void CAR_Inductance(int16_t L1, int16_t L2, int16_t L3, int16_t L4, int16_t L5, int16_t L6,int16_t L7, int16_t L8);
    void CAR_MotorPID(float KP1, float KI1, float KD1, float KP2, float KI2,float KD2);
    void CAR_ServoPID(float SKP, float SKI, float SKD);
    void CAR_PWM_ENCODER(int16_t LPWM, int16_t LENCODE, int16_t RPWM, int16_t RENCODE,int16_t SPWM);
#ifdef Arduino
    ANO(uint32_t baudrate);
#elif defined Energia
#endif

private:
#ifdef Arduino
    HardwareSerial *function;
#elif defined Energia
#endif
};

#endif /* _ANO_DTP_H_ */
