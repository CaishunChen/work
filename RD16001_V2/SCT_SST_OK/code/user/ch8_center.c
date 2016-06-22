#include <msp430.h>
#include "test.h"
#include "cfg.h"
#include "gpio.h"
#include "freq.h"
#include "uart.h"
#include "kernel.h"
#include "clk.h"
#include "timer.h"
#include "power.h"
#include "flash.h"
#include "rtc.h"
#include "s485.h"
#include "level.h"
#include "rain.h"
#include "cmd.h"
#include "ramloop.h"
#include "key.h"
#include "ecc.h"
#include "dtu.h"
#include "rf.h"
#include <stdio.h>
#include <string.h>
#include "includes.h"


extern LoopRAMType Uart0loop;//����0���λ���(���ڴ˴�Ϊʲô����ָ���˼����ָ�벻�����ڴ棬����һ���ṹ�����ʹ���ڴ�)
extern long Uart0time;



//SCT16001
//��ȡ���豸���豸ID
void GetSubDeviceID(unsigned char ch, char *IDstring)
{
  char DeviceID[20] = {'\0'};
  char temString[20] = {'\0'};
  unsigned char ID = 0;
  
  strcat(DeviceID,"SCT16");
  
  switch(ch)
  {
    case 1: ID = *(unsigned char*)(SUB_DEVICE_ID+0);
            if(ID > 99)
              {sprintf(temString, "%d", ID);}
            else if( 99 > ID && ID >9)
              {sprintf(temString, "0%d", ID);}
            else
              {sprintf(temString, "00%d", ID);}
            break;
    case 2: ID = *(unsigned char*)(SUB_DEVICE_ID+1);
            if(ID > 99)
              {sprintf(temString, "%d", ID);}
            else if( 99 > ID && ID >9)
              {sprintf(temString, "0%d", ID);}
            else
              {sprintf(temString, "00%d", ID);}
            break;
    case 3: ID = *(unsigned char*)(SUB_DEVICE_ID+2);
            if(ID > 99)
              {sprintf(temString, "%d", ID);}
            else if( 99 > ID && ID >9)
              {sprintf(temString, "0%d", ID);}
            else
              {sprintf(temString, "00%d", ID);}
            break;
    case 4: ID = *(unsigned char*)(SUB_DEVICE_ID+3);
            if(ID > 99)
              {sprintf(temString, "%d", ID);}
            else if( 99 > ID && ID >9)
              {sprintf(temString, "0%d", ID);}
            else
              {sprintf(temString, "00%d", ID);}
            break;
    case 5: ID = *(unsigned char*)(SUB_DEVICE_ID+4);
            if(ID > 99)
              {sprintf(temString, "%d", ID);}
            else if( 99 > ID && ID >9)
              {sprintf(temString, "0%d", ID);}
            else
              {sprintf(temString, "00%d", ID);}
            break;
    case 6: ID = *(unsigned char*)(SUB_DEVICE_ID+5);
            if(ID > 99)
              {sprintf(temString, "%d", ID);}
            else if( 99 > ID && ID >9)
              {sprintf(temString, "0%d", ID);}
            else
              {sprintf(temString, "00%d", ID);}
            break;
    case 7: ID = *(unsigned char*)(SUB_DEVICE_ID+6);
            if(ID > 99)
              {sprintf(temString, "%d", ID);}
            else if( 99 > ID && ID >9)
              {sprintf(temString, "0%d", ID);}
            else
              {sprintf(temString, "00%d", ID);}
            break;
    case 8: ID = *(unsigned char*)(SUB_DEVICE_ID+7);
            if(ID > 99)
              {sprintf(temString, "%d", ID);}
            else if( 99 > ID && ID >9)
              {sprintf(temString, "0%d", ID);}
            else
              {sprintf(temString, "00%d", ID);}
            break;
  default:sprintf(temString, "000");break;
  }
  strcat(DeviceID,temString);
  strcat(IDstring,DeviceID);
}




//$DTLOG,010001
//��ȡ��������ID
void GetSensor485ID(unsigned char ch, char *IDstring)
{
  char DeviceID[20]  = {'\0'};
  char temString[20]  = {'\0'};
  unsigned char ID   = 0;
  strcat(DeviceID,"$DTLOG,010");
  
  switch(ch)
  {
    case 1: ID = *(unsigned char*)(SENSOR_ID_ADDR+0);
            if(ID > 99)
              {sprintf(temString, "%d", ID);}
            else if( 99 > ID && ID >9)
              {sprintf(temString, "0%d", ID);}
            else
              {sprintf(temString, "00%d", ID);}
            break;
    case 2: ID = *(unsigned char*)(SENSOR_ID_ADDR+1);
            if(ID > 99)
              {sprintf(temString, "%d", ID);}
            else if( 99 > ID && ID >9)
              {sprintf(temString, "0%d", ID);}
            else
              {sprintf(temString, "00%d", ID);}
            break;
    case 3: ID = *(unsigned char*)(SENSOR_ID_ADDR+2);
            if(ID > 99)
              {sprintf(temString, "%d", ID);}
            else if( 99 > ID && ID >9)
              {sprintf(temString, "0%d", ID);}
            else
              {sprintf(temString, "00%d", ID);}
            break;
    case 4: ID = *(unsigned char*)(SENSOR_ID_ADDR+3);
            if(ID > 99)
              {sprintf(temString, "%d", ID);}
            else if( 99 > ID && ID >9)
              {sprintf(temString, "0%d", ID);}
            else
              {sprintf(temString, "00%d", ID);}
            break;
    case 5: ID = *(unsigned char*)(SENSOR_ID_ADDR+4);
            if(ID > 99)
              {sprintf(temString, "%d", ID);}
            else if( 99 > ID && ID >9)
              {sprintf(temString, "0%d", ID);}
            else
              {sprintf(temString, "00%d", ID);}
            break;
    case 6: ID = *(unsigned char*)(SENSOR_ID_ADDR+5);
            if(ID > 99)
              {sprintf(temString, "%d", ID);}
            else if( 99 > ID && ID >9)
              {sprintf(temString, "0%d", ID);}
            else
              {sprintf(temString, "00%d", ID);}
            break;
    case 7: ID = *(unsigned char*)(SENSOR_ID_ADDR+6);
            if(ID > 99)
              {sprintf(temString, "%d", ID);}
            else if( 99 > ID && ID >9)
              {sprintf(temString, "0%d", ID);}
            else
              {sprintf(temString, "00%d", ID);}
            break;
    case 8: ID = *(unsigned char*)(SENSOR_ID_ADDR+7);
            if(ID > 99)
              {sprintf(temString, "%d", ID);}
            else if( 99 > ID && ID >9)
              {sprintf(temString, "0%d", ID);}
            else
              {sprintf(temString, "00%d", ID);}
            break;
  default:sprintf(temString, "000");break;
  }
  strcat(DeviceID,temString);
  strcat(DeviceID,"\r\n");
  strcat(IDstring,DeviceID);
}


//485����������
void s485(unsigned char ch, char *dat)
{
  char retData[100] ={'\0'};
  unsigned char measure485Times = 2;//�������Դ���
  char value485[100] = {'\0'};//485�������Ĳ�������
  char cmd485[30] = {'\0'};//485������������
  
  
  GetSensor485ID(ch,cmd485);
  //printf("cmd485:%s\r\n",cmd485);

  CH_Select(ch);
  while(1)
  {
    if( Get485(cmd485,value485) == 1 )
      {
        sprintf(retData,"#%s-CH%d:%s",MACHINE_ID, ch, value485);
        memcpy(dat,retData,sizeof(retData));
        memset(value485,'\0',100);
        break;
      }
    else
     {
        measure485Times--;
        if(measure485Times == 0)
          {
            sprintf(retData,"#%s-CH%d:%s",MACHINE_ID, ch, value485);
            memcpy(dat,retData,sizeof(retData));
            memset(value485,'\0',100);
            break;
          }
     }
  }
}
//��������
void Freq(unsigned char ch, char *dat)
{
  char retData[100] ={'\0'};
  unsigned char measureVWTime = 2;//���Խ��в����Ĵ���
  float measureFreq = 0.0;//VW�������
  float VWtemp = 0.0;

  
  while(1)
  {
    CH_Select(ch);
    measureFreq = GetFreq();
    
    VWtemp = Get_VW_Voltage();//��ѹ
    //printf("vol:%f\r\n",VWtemp);
    VWtemp = 9950/VWtemp - 21290;//����
    //printf("res:%f\r\n",VWtemp);
    VWtemp = GetNTCTemperature(VWtemp);//�¶�
    //printf("temp:%f\r\n",VWtemp);
    
    if( measureFreq != 0 )
      {
        sprintf(retData,"#%s-CH%d:F-%.2f T-%.2f",MACHINE_ID,ch, measureFreq, VWtemp);
        memcpy(dat,retData,sizeof(retData));
        break;
      }
    else
     {
        measureVWTime--;
        if(measureVWTime == 0)
          {
            sprintf(retData,"#%s-CH%d:F-%.2f T-%.2f",MACHINE_ID,ch, measureFreq, VWtemp);
            memcpy(dat,retData,sizeof(retData));
            break;
          }
     }
    
    
  }
}





void Tilt(unsigned char ch, char *dat)
{
  char retData[150] ={'\0'};
  unsigned char measure485Times = 2;//�������Դ���
  char value485[100] = {'\0'};//485�������Ĳ�������
  char cmd1[] = {0x01,0x03,0x00,0x64,0x00,0x03,0x44,0x14};
  char cmd2[] = {0x01,0x03,0x00,0x69,0x00,0x0D,0x54,0x13};
  
  unsigned char length = 0;
  unsigned char length485 = 0;

  CH_Select(ch);
  while(1)
  {
    if( Get485(cmd1,value485) == 1 )
      {
        sprintf(retData,"#%s-CH%d:",MACHINE_ID, ch);
        length = strlen(retData);
        
        memcpy(dat,retData,length);
        length485 = getDataLength(value485,100);//���ص����ݵĳ���
        memcpy(dat+length,value485,length485);
        
        length = length+length485;
        length485 = 0;
        memset(value485,'\0',100);
        break;
      }
    else
     {
        measure485Times--;
        if(measure485Times == 0)
          {
            sprintf(retData,"#%s-CH%d:",MACHINE_ID, ch);
            memcpy(dat,retData,strlen(retData));
            memset(value485,'\0',100);
            break;
          }
     }
  }
  
  while(1)
  {
    if( Get485(cmd2,value485) == 1 )
      {
        length = getDataLength(value485,100);
        length485 = getDataLength(value485,100);
        
        memcpy(dat,retData,sizeof(retData));
        memset(value485,'\0',100);
        break;
      }
    else
     {
        measure485Times--;
        if(measure485Times == 0)
          {
            sprintf(retData,"#%s-CH%d:%s",MACHINE_ID, ch, value485);
            memcpy(dat,retData,sizeof(retData));
            memset(value485,'\0',100);
            break;
          }
     }
  }
}





//�ȴ���ȡ���豸������
unsigned char GetSubDevData(char *dat)
{
  char recivedDat[150] = {'\0'};
  unsigned char ret = 0;
  unsigned char retry = 0;
  while(1)
  {
    if( ( Uart0loop.LoopDatLength != 0 ) && ( (GetTick()- Uart0time) > 5 ) )
        {
          LoopRAMReadLength(&Uart0loop, recivedDat, Uart0loop.LoopDatLength);
          ret = 1;
          break;
        }
    else
        {
          retry++;
          if(retry == 20)
            { 
              retry = 0;
              ret = 0;
              break; 
            }
        }
    delay_ms(1000);
  }
  
  memcpy(dat, recivedDat, strlen(recivedDat));
  return ret;
}

//��ȡ���豸��ͬ����Ӧ
unsigned char GetSubDevResponse(void)
{
  char dat[200] = {'\0'};
  unsigned char ret = 0;
  unsigned char retry = 0;
  while(1)
  {
    if( (Uart0loop.LoopDatLength != 0) && ( (GetTick()- Uart0time) > 5))
        {
            LoopRAMReadLength(&Uart0loop, dat, Uart0loop.LoopDatLength);
            if(strstr(dat, "ok") != 0)
              {
                ret = 1;
                memset(dat,0,200);
                break;
              }
            else
              {
               memset(dat,0,200);
              }
        }
    else
        {
          if(retry == 3)
            { 
              retry = 0;
              ret = 0;
              memset(dat,0,200);
              break;
            }
          retry++;
        }
    delay_ms(500);
  }
  return ret;
}








//ͬ�����豸subNum����ȡ�ش������ݵ�responsdata
void syncSub(unsigned char subNum , char *responsdata)
{
    unsigned int responseTimes = 0;
    char IDstring[20] = {'\0'};//ͬ��ʱ���õ���ID
    char Timestring[30] = {'\0'};//��ǰʱ��
    char cmdSub[30] = {'\0'};//ͬ��ʱ��ʹ�õ�����

    
    GetSubDeviceID(subNum,IDstring);
    
    while(GetSubDevResponse() == 0)
    {
      /*ͬ��ʱ��*/
      GetRtcHEX(Timestring);
      cmdSub[0] = 0xfe;
      cmdSub[1] = IDstring[0];//s
      cmdSub[2] = IDstring[1];//c
      cmdSub[3] = IDstring[2];//t
      cmdSub[4] = IDstring[3];//1
      cmdSub[5] = IDstring[4];//6
      cmdSub[6] = IDstring[5];//0
      cmdSub[7] = IDstring[6];//0
      cmdSub[8] = IDstring[7];//x
      cmdSub[9] = 0x0f;//sybc time
      cmdSub[10] = 0x20;//year
      cmdSub[11] = Timestring[0];//mon
      cmdSub[12] = Timestring[1];//day
      cmdSub[13] = Timestring[2];//dow
      cmdSub[14] = Timestring[3];//hour
      cmdSub[15] = Timestring[4];//min
      cmdSub[16] = Timestring[5];//sec
      cmdSub[17] = Timestring[6];//3
      cmdSub[18] = 0xfe;
      
      
    
      responseTimes++;
      if(responseTimes == 1200)
        {responseTimes = 0; break;}
      UartStringLength(UART_0,cmdSub,19);
      
      memset(cmdSub,0,30);
      memset(Timestring,0,30);
    }
    
    memset(cmdSub,0,30);
    /*ͬ�����豸ʱ����*/
    cmdSub[0] = 0xfe;
    cmdSub[1] = IDstring[0];//s
    cmdSub[2] = IDstring[1];//c
    cmdSub[3] = IDstring[2];//t
    cmdSub[4] = IDstring[3];//1
    cmdSub[5] = IDstring[4];//6
    cmdSub[6] = IDstring[5];//0
    cmdSub[7] = IDstring[6];//0
    cmdSub[8] = IDstring[7];//x
    cmdSub[9] = 0x0e;//
    cmdSub[10] = *(unsigned char*)(INTERVAL_TIME_ADDR+0);
    cmdSub[11] = *(unsigned char*)(INTERVAL_TIME_ADDR+1) + 6;
    cmdSub[12] = *(unsigned char*)(INTERVAL_TIME_ADDR+2);
    cmdSub[13] = *(unsigned char*)(INTERVAL_TIME_ADDR+3);
    cmdSub[14] = *(unsigned char*)(INTERVAL_TIME_ADDR+4);
    cmdSub[15] = *(unsigned char*)(INTERVAL_TIME_ADDR+5);
    cmdSub[16] = *(unsigned char*)(INTERVAL_TIME_ADDR+6);
    cmdSub[17] = *(unsigned char*)(INTERVAL_TIME_ADDR+7);
    cmdSub[18] = 0xfe;
    while(GetSubDevResponse() == 0)
    {
      responseTimes++;
      if(responseTimes == 1200)
        {responseTimes = 0; break;}
      UartStringLength(UART_0,cmdSub,19);
    }
    
    
    /*ͬ�����豸ͨ������*/
    memset(cmdSub,0,30);
    cmdSub[0] = 0xfe;
    cmdSub[1] = IDstring[0];//s
    cmdSub[2] = IDstring[1];//c
    cmdSub[3] = IDstring[2];//t
    cmdSub[4] = IDstring[3];//1
    cmdSub[5] = IDstring[4];//6
    cmdSub[6] = IDstring[5];//0
    cmdSub[7] = IDstring[6];//0
    cmdSub[8] = IDstring[7];//x
    cmdSub[9] = 0x10;//
    cmdSub[10] = *(unsigned char*)(SUB_DEVICE_TYPE_ADDR+0);
    cmdSub[11] = *(unsigned char*)(SUB_DEVICE_TYPE_ADDR+1);
    cmdSub[12] = *(unsigned char*)(SUB_DEVICE_TYPE_ADDR+2);
    cmdSub[13] = *(unsigned char*)(SUB_DEVICE_TYPE_ADDR+3);
    cmdSub[14] = *(unsigned char*)(SUB_DEVICE_TYPE_ADDR+4);
    cmdSub[15] = *(unsigned char*)(SUB_DEVICE_TYPE_ADDR+5);
    cmdSub[16] = *(unsigned char*)(SUB_DEVICE_TYPE_ADDR+6);
    cmdSub[17] = *(unsigned char*)(SUB_DEVICE_TYPE_ADDR+7);
    cmdSub[18] = 0xfe;
    while(GetSubDevResponse() == 0)
    {
      responseTimes++;
      if(responseTimes == 1200)
        {responseTimes = 0; break;}
      UartStringLength(UART_0,cmdSub,19);
    }
    
    /*ͬ�����豸485 ID*/
    memset(cmdSub,0,30);
    cmdSub[0] = 0xfe;
    cmdSub[1] = IDstring[0];//s
    cmdSub[2] = IDstring[1];//c
    cmdSub[3] = IDstring[2];//t
    cmdSub[4] = IDstring[3];//1
    cmdSub[5] = IDstring[4];//6
    cmdSub[6] = IDstring[5];//0
    cmdSub[7] = IDstring[6];//0
    cmdSub[8] = IDstring[7];//x
    cmdSub[9] = 0x12;//
    cmdSub[10] = *(unsigned char*)(SUB_DEVICE_SENSOR_ID_ADDR+0);
    cmdSub[11] = *(unsigned char*)(SUB_DEVICE_SENSOR_ID_ADDR+1);
    cmdSub[12] = *(unsigned char*)(SUB_DEVICE_SENSOR_ID_ADDR+2);
    cmdSub[13] = *(unsigned char*)(SUB_DEVICE_SENSOR_ID_ADDR+3);
    cmdSub[14] = *(unsigned char*)(SUB_DEVICE_SENSOR_ID_ADDR+4);
    cmdSub[15] = *(unsigned char*)(SUB_DEVICE_SENSOR_ID_ADDR+5);
    cmdSub[16] = *(unsigned char*)(SUB_DEVICE_SENSOR_ID_ADDR+6);
    cmdSub[17] = *(unsigned char*)(SUB_DEVICE_SENSOR_ID_ADDR+7);
    cmdSub[18] = 0xfe;
    while(GetSubDevResponse() == 0)
    {
      responseTimes++;
      if(responseTimes == 1200)
        {responseTimes = 0; break;}
      UartStringLength(UART_0,cmdSub,19);
    }
    
    
    //�ȴ��豸���ݻش�
    while(GetSubDevData(responsdata) == 0)
    {
      delay_ms(1000);
      responseTimes++;
      if(responseTimes == 300)
        {responseTimes = 0; break;}
    }
    
    
    memset(cmdSub,0,30);
    memset(IDstring,0,20);
    memset(Timestring,0,30);
}































/********************************************************************************************

                ��Ҫ������
*********************************************************************************************
*/
void ch8_center_Task(void)
{
    char UpLoadData[800] = {'\0'};//��Ҫ�ϴ�������
    char dataTemp[150] = {'\0'};//�ݴ���
    char PROTOCOL_UpLoadData[900] = {'\0'};//������Э��Ľ�Ҫ�ϴ�����
    unsigned char detectTime = 0;//dtu����ʱ��
    char deviceID[8]={0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};//Ĭ�ϵ����豸��
    float vol = 0.0;//�ڲ���Դ��ѹ
    unsigned int wakeTime = 0;//�������
    unsigned char i=0;
    
    
    unsigned char nextInterval_H;
    unsigned char nextInterval_L;
    
    
    WDTCTL = WDTPW + WDTHOLD;
    ClkInit();
    SysTickInit();
   //Ĭ��ϵͳʱ��
    SetRtc(0x2016,0x05,0x06,0x05,0x09,0x42,0x48);
    if( 0xAA != *(unsigned char *)DEVICE_FIRST_TIME_POWERON_ADDR)
    {
      /*�Ƿ��ǵ�һ�ο���*/
      FlashInit();
      FlashEreaseSector(DEVICE_FIRST_TIME_POWERON_ADDR);
      FlashWriteByte(DEVICE_FIRST_TIME_POWERON_ADDR,0xAA);
      /*�ɼ�ʱ����*/
      FlashInit();
      FlashEreaseSector(INTERVAL_TIME_ADDR);
      FlashWriteByte(INTERVAL_TIME_ADDR,  (unsigned char)( INTERVAL_TIME/256 ) );//�߰�λ
      FlashWriteByte(INTERVAL_TIME_ADDR+1,(unsigned char)(INTERVAL_TIME % 256) ); //�Ͱ�λ
      /*cmd�ȴ�ʱ��*/
      FlashInit();
      FlashEreaseSector(CMD_TIME_ADDR);
      FlashWriteByte(CMD_TIME_ADDR, CMD_TIME );
      //Ĭ��ͨ�����ͣ�0X00:vw  0X01:485
      FlashInit();
      FlashEreaseSector(CHANNEL_TYPE_ADDR);
      FlashWriteByte(CHANNEL_TYPE_ADDR+0,0x00);
      FlashWriteByte(CHANNEL_TYPE_ADDR+1,0x00);
      FlashWriteByte(CHANNEL_TYPE_ADDR+2,0x00);
      FlashWriteByte(CHANNEL_TYPE_ADDR+3,0x00);
      FlashWriteByte(CHANNEL_TYPE_ADDR+4,0x00);
      FlashWriteByte(CHANNEL_TYPE_ADDR+5,0x00);
      FlashWriteByte(CHANNEL_TYPE_ADDR+6,0x00);
      FlashWriteByte(CHANNEL_TYPE_ADDR+7,0x00);
      //Ĭ��ͨ������485�������ı��
      FlashInit();
      FlashEreaseSector(SENSOR_ID_ADDR);
      FlashWriteByte(SENSOR_ID_ADDR+0,0x00);
      FlashWriteByte(SENSOR_ID_ADDR+1,0x00);
      FlashWriteByte(SENSOR_ID_ADDR+2,0x00);
      FlashWriteByte(SENSOR_ID_ADDR+3,0x00);
      FlashWriteByte(SENSOR_ID_ADDR+4,0x00);
      FlashWriteByte(SENSOR_ID_ADDR+5,0x00);
      FlashWriteByte(SENSOR_ID_ADDR+6,0x00);
      FlashWriteByte(SENSOR_ID_ADDR+7,0x00);
      //Ĭ���ϴ��Ƿ��Э�� 0x00:unprotocol 0x01:protocol
      FlashInit();
      FlashEreaseSector(PROTOCOL_TYPE_ADDR);
      FlashWriteByte(PROTOCOL_TYPE_ADDR,0x00);  
      //Ĭ�����ӵ����豸�ı��ID
      FlashInit();
      FlashEreaseSector(SUB_DEVICE_ID);
      FlashWriteAddr(SUB_DEVICE_ID,deviceID,8);
      //Ĭ�����豸ͨ�����ͣ�0X00:vw  0X01:485
      FlashInit();
      FlashEreaseSector(SUB_DEVICE_TYPE_ADDR);
      FlashWriteByte(SUB_DEVICE_TYPE_ADDR+0,0x00);
      FlashWriteByte(SUB_DEVICE_TYPE_ADDR+1,0x00);
      FlashWriteByte(SUB_DEVICE_TYPE_ADDR+2,0x00);
      FlashWriteByte(SUB_DEVICE_TYPE_ADDR+3,0x00);
      FlashWriteByte(SUB_DEVICE_TYPE_ADDR+4,0x00);
      FlashWriteByte(SUB_DEVICE_TYPE_ADDR+5,0x00);
      FlashWriteByte(SUB_DEVICE_TYPE_ADDR+6,0x00);
      FlashWriteByte(SUB_DEVICE_TYPE_ADDR+7,0x00);
      //Ĭ�������ͨ������485�������ı��
      FlashInit();
      FlashEreaseSector(SUB_DEVICE_SENSOR_ID_ADDR);
      FlashWriteByte(SUB_DEVICE_SENSOR_ID_ADDR+0,0x00);
      FlashWriteByte(SUB_DEVICE_SENSOR_ID_ADDR+1,0x00);
      FlashWriteByte(SUB_DEVICE_SENSOR_ID_ADDR+2,0x00);
      FlashWriteByte(SUB_DEVICE_SENSOR_ID_ADDR+3,0x00);
      FlashWriteByte(SUB_DEVICE_SENSOR_ID_ADDR+4,0x00);
      FlashWriteByte(SUB_DEVICE_SENSOR_ID_ADDR+5,0x00);
      FlashWriteByte(SUB_DEVICE_SENSOR_ID_ADDR+6,0x00);
      FlashWriteByte(SUB_DEVICE_SENSOR_ID_ADDR+7,0x00);
    }

    

    
    
    
    
    
    
    
  /*������Դ*/
  PowerInit();
  PWR_ON;
  COLL_PWR_OFF;
  TRAN_PWR_OFF;
  while(1)
  {
/*----------------------�Ƿ�������ʱ����------------------*/
    if( *(unsigned char *)SET_INTERVAL_TIME_FLAG_ADDR == 0x55 )
      {
          nextInterval_H = *(unsigned char *)NEXT_INTERVAL_TIME_ADDR;
          nextInterval_L = *(unsigned char *)(NEXT_INTERVAL_TIME_ADDR+1);
          
          FlashInit();
          FlashEreaseSector(SET_INTERVAL_TIME_FLAG_ADDR);//������ü���ı�־
          
          FlashEreaseSector(INTERVAL_TIME_ADDR);
          FlashWriteByte(INTERVAL_TIME_ADDR,         nextInterval_H);
          FlashWriteByte(INTERVAL_TIME_ADDR+1,       nextInterval_L);
      }
 
/*------------------------ͬ�����豸------------------------*/        
      PWR_ON;
      COLL_PWR_OFF;
      TRAN_PWR_ON;

      RFInit();
      delay_ms(1000);

      syncSub(1,dataTemp);
      strcat(UpLoadData, dataTemp);
      memset(dataTemp,0,150);
      syncSub(2,dataTemp);
      strcat(UpLoadData, dataTemp);
      memset(dataTemp,0,150);

      
/*--------------------��ȡ����ͨ���ǲɼ�����----------------*/
      PWR_ON;
      COLL_PWR_ON;
      TRAN_PWR_ON;
      sprintf(dataTemp,"#%s-time:",MACHINE_ID);
      strcat(UpLoadData, dataTemp);
      memset(dataTemp,0,150);
      GetRtc(dataTemp);
      strcat(UpLoadData, dataTemp);
      memset(dataTemp,0,150);
      
      SDInit();
      delay_ms(1000);
      vol = Get_INNER_Voltage();
      SDOff();
      sprintf(dataTemp,"#%s-voltage:%.2fV",MACHINE_ID,vol);
      strcat(UpLoadData, dataTemp);
      memset(dataTemp,0,150);
      
      wakeTime = *(unsigned char*)INTERVAL_TIME_ADDR * 256 + *(unsigned char*)(INTERVAL_TIME_ADDR+1)+6;
      sprintf(dataTemp,"#%s-interval:%d Min",MACHINE_ID,wakeTime);
      strcat(UpLoadData, dataTemp);
      memset(dataTemp,0,150);

      for(i=0; i<8; i++)
      {
        if( *(unsigned char*)(CHANNEL_TYPE_ADDR + i) == 1 )//485
          {
            Init485();
            delay_ms(1000);
            s485(1+i,dataTemp);
            strcat(UpLoadData, dataTemp);
            memset(dataTemp,0,150);
          }
        if( *(unsigned char*)(CHANNEL_TYPE_ADDR + i) == 0 )//vw
          {
            FreqInit();
            Freq(1+i,dataTemp);
            strcat(UpLoadData, dataTemp);
            memset(dataTemp,0,150);
          }
      }
  
/*------------------�ȴ����ӵ�DTU---------------------------*/      
      PWR_ON;
      COLL_PWR_OFF;
      TRAN_PWR_ON;
      LED(1);
      DtuInit();
      LED(0);
      while(1)
      {
        if(DtuStatu() == 1)//���Ϸ�����
          {
             LED(1);delay_ms(5000);break;
          }
        if(DtuStatu() == 0)//û�����Ϸ�����
          {
            LED(0);
            delay_ms(1000);
            detectTime++;
            if(detectTime == 180)
            {
              detectTime = 0; 
              goto DISCONNECT;
            }
          }
      }
      
      
/*---------------------------�ϴ�����----------------------*/
      if(*(unsigned char*)(PROTOCOL_TYPE_ADDR+0) == 1)//��Э�鴫��
        {
          packData(UpLoadData, getDataLength(UpLoadData,800), PROTOCOL_UpLoadData);
          UartStringLength(UART_0, PROTOCOL_UpLoadData, getDataLength(PROTOCOL_UpLoadData,900));
          memset(UpLoadData ,'\0',800);
          memset(PROTOCOL_UpLoadData ,'\0',900);
        }
      if(*(unsigned char*)(PROTOCOL_TYPE_ADDR+0) == 0)//����Э�鴫��
        {
          UartStringLength(UART_0, UpLoadData, getDataLength(UpLoadData,800));
          memset( UpLoadData , '\0', 800 );
        }
      delay_ms(5000);
      
      cmdTask();
      
      
      PWR_OFF;
      COLL_PWR_OFF;
      TRAN_PWR_OFF;
      
DISCONNECT:
      /*�̶�һ��ʱ���ϴ����ݣ�����һ���ж�һ���Ƿ��Ѿ������ϴ����ݵ�ʱ��*/
      LowperMeneagement();//�͹���
      delay_ms(500);
  }

}
