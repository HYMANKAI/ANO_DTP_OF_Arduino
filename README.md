## 基于Arduino 框架匿名上位机（ver.7）通讯协议

**Version: 1.0**  
**Release date: 2022 Feb 8**

**写在前面**：

为什么我不自己开发上位机？

​		在写了在写了... 进度0.01%（新建文件夹（雾））

为什么我要重复造轮子呢？

​		因为有朋友用Arduino Mega 2560做Mini Fly（会飞的PCB）。采用了匿名科技（ANO）团队的控制方案和上位机。为了方便（偷懒），所以想着把通讯协议移植到Arduino框架上。这样一来无论是小灰机还是小破车都可以采用，而且基于Arduino框架可以让代码简单很多。

*废话到此结束，下面开始使用轮子*

这个轮子有三个文件

> `ANO_CONFIG.h`			配置文件，根据板子型号选择

> `ANO_DTP.h`					头文件，请include进 .ino文件里

> `ANO_DTP.cpp`				源程序，不用管，用就完事了，觉得不爽可以自己改

把项目Github上拉下来，把拉下来的zip文件导入到Arduino IDE里就完事了。至于怎么导入，我相信Very NB 的 you 肯定会注意到Arduino IDE上的项目栏：

然后，点击它。顺序是 项目-->加载库-->增加.zip库，选择刚才压缩的压缩包就完事了。

导入完后，这个库就会到下面这个路径里。Very NB 的 you 肯定能找到

> `C:\Users\username\Documents\Arduino\libraries`

之后打开文件找到里面的`ANO_CONFIG.h`

针对你的需求和芯片类型进行选择注释或取消注释宏定义

然后你就可以快乐的用轮子了

**EX:**

当单片机仅有一个串口时候例如Arduino UNO,系统只能调用默认串口（Serial）

请在`void setup()`函数里把`Serial.begin(115200)；`加上

假如有多串口的，例如Mega 2560有四个串口，那么使用除默认串口外的三个串口（Serial1、Serial2、Serial3）时，可以不加`Serial(1/2/3).begin`，但保险起见，还是加上

使用姨妈红的Energia IDE，就必须加上串口初始化函数，不然没法用，原因我也不知道。

当你使用Arduino IDE时请参考例子里的ArduinoUNO的代码示例

当你使用Energia IDE时请参考例子里的MSP430G2ET的代码示例

**注意**：

第一，代码基于Arduino框架，跨平台，除了原谅绿的Arduino IDE、VSCODE的PlatformIO插件也可以用，当然TI发行的姨妈红Energia IDE也可以使用，当然主控芯片是MSP430、MSP432和 TM4C系列，现在代码仅支持MSP4302553的TI官方火箭板（MSP-EXP430G2ET），因为我手中只有这块板子。

第二，请 NB 的 you 先去看看匿名科技的v7上位机基本操作怎么用再使用此代码。

### 函数说明

`ANO(uint32_t baudrate)`

构造函数，使用Energia IDE时不被编译，不可使用。Arduino IDE正常编译

`Report(uint8_t fun, uint8_t *data, uint8_t len)`

传送数据(内部调用OR单独使用)；fun:功能字，data:数据缓存区,最多30字节!!，len:data区有效数据个数

`Raw_Data(int16_t aacx, int16_t aacy, int16_t aacz, int16_t gyrox, int16_t gyroy, int16_t gyroz)`

姿态等基本信息；aac  X Y Z；gyro X Y Z

`Angle_Data(float roll_o, float pitch_o, float yaw_o)`

欧拉角/ROL、 PIT、YAW

`Quaternion_Data(int16_t V0, int16_t V1, int16_t V2, int16_t V3)`

四元数 V0、V1、V2、V3

`Height_Data(int32_t ALT_FU, int32_t ALT_ADD)`

ALT_FU：融合后对地高度，单位厘米。
ALT_ADD：附加高度传感高度数据，如超声波、激光测距，单位厘米。

`CAR_Inductance(int16_t L1, int16_t L2, int16_t L3, int16_t L4, int16_t L5, int16_t L6, int16_t L7, int16_t L8)`

发送八个电感ADC原始数据,兼容麦克纳姆轮多对电感
参数说明：L1L8:ADC原始数据；使用灵活格式帧0xF1

`CAR_MotorPID(float KP1, float KI1, float KD1, float KP2, float KI2, float KD2)`

发送电机PID参数，使用灵活格式帧0xF2

`CAR_ServoPID(float SKP, float SKI, float SKD)`

发送舵机PID参数，使用灵活格式帧0xF3

`CAR_PWM_ENCODER(int16_t LPWM, int16_t LENCODE, int16_t RPWM, int16_t RENCODE, int16_t SPWM)`

发送左右电机PWM，舵机PWM和编码器数据，使用灵活格式帧0xF4

