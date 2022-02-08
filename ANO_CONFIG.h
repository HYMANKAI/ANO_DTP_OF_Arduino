#ifndef _ANO_CONFIG_H_
#define _ANO_CONFIG_H_

#define Arduino
//#define Energia

#ifdef Arduino
 #define Only_One_Serial // Such as Arduino UNO、NANO、MINI...
// #define Multiple_Serial    // Such ad Aeduino Mega2560...

#ifdef Only_One_Serial
#define Dafault_SERIAL0
#endif

#ifdef Multiple_Serial
  #define Dafault_SERIAL0
  // #define USE_THE_SERIAL1
  // #define USE_THE_SERIAL2
  // #define USE_THE_SERIAL3
#endif

#endif

#ifdef Energia
  #define Dafault_SERIAL0
#endif

#endif /* _ANO_CONFIG_H_ */
