/*_-----------------MCU外设驱动------------------*/
#include "kernel.h"             //里面包含了板子的各种配置,以及一些系统的小函数，比如延时CRC校验等....
#include "msp430f149_uart.h"
#include "msp430f149_gpio.h"
#include "msp430f149_timer.h"
#include "msp430f149_adc.h"
#include "msp430f149_spi.h"
#include "msp430f149_flash.h"
/*_----------------器件驱动---------------------*/
#include "Vibration.h"
#include "Power.h"
#include "JN5139.h"
#include "LTC2402.h"
#include "Beep.h"
/*_----------------功能函数---------------------*/
#include "Freq.h"
#include "Voltage.h"
#include "Temperature.h"
#include "DTprotocol.h"
#include "LoopRamFunc.h"
/*_----------------测试函数---------------------*/
#include "test.h"


void zigbeeInitialization(void);


void testzigbee(void)
{
  
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  SystemInitialization();

  if(ZigbeeInit() == 1)
  {printf("Zigbee Init ok....\r\n");}
  

  while(1)
  {
  }
  
}












/**********************************sct各个模块的初始化等操作************************************/
void zigbeeInitialization(void)
{
  RCC_HSECLKInit();
/*****************************电源控制*******************************/
  PowerIoInit();        //电源控制IO引脚初始化
  POWER_MAIN_ON;        //主电源开关(是否进行降压)
  POWER_MCU_ON;         //MCU电源开关
  POWER_ZIGBEE_ON;      //ZIGBEE电源开关
  POWER_ADC_ON;         //LTC2402以及其基准源电源开关
  DRIVER_EN_OFF;        //振弦电路的发射部分的开关
  
/*****************************串口输出******************************/
  
#ifdef SERIAL_USE_PORT0
  UART_Init(UART_0,UART_SouceClk_SMCLK,UART_SMCLKBaud_9600);
#endif
  
#ifdef SERIAL_USE_PORT1
  UART_Init(UART_1,UART_SouceClk_SMCLK,UART_SMCLKBaud_9600);
#endif
  
  
  
  
}






///*----------------看门狗中断函数--------------------*/
// //Watchdog Timer interrupt service routine
//#pragma vector=WDT_VECTOR
//__interrupt void watchdog_timer(void)
//{
// LPM3_EXIT;
//}

