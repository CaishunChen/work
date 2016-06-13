#include "Serial.h"
#include "Dtu.h" 
#include "led.h"


enum
{
	IP_INTERVIEW,
	DOMAIN_INTERVIEW,
};


#define UPDATE_ADDRESS 	0X8000           //32k的位置

STATIC U8 g_szMachineType[] = "SCT100T15001-";

STATIC BOOL g_bUpdate = FALSE;

STATIC BOOL g_bUpdateCmd = FALSE;

STATIC BOOL g_bProtocolMode = TRUE;			//默认为协议模式

STATIC BOOL g_bLastDtuWork = FALSE;

STATIC U32 g_u32TimeOut = 50;

STATIC U32 g_u32WriteOffset = 0;

STATIC U8 g_u8DtuRetry = 0;

STATIC U32 g_u32TimeInternal = 6000;

STATIC U32 g_u32AppAddress = STM32_FLASH_APP_ADDR;
STATIC U32 g_u32HardEdition = HARDWARE_EDITION;
STATIC U32 g_u32SoftEdition = SOFTWARE_EDITION;

#define START_SYMBOL		0XFE
#define PROTOCOL_EDITION	0X01
#define DATA_DIRECTION		0X01
#define	MAINCMD_INTERNET 	0X06		//网络主命令
#define	SUBCMD_SENSOR	 	0X0001		//RD15002子命令
#define	FIRST_END_SYMBOL	 		0X0D
#define	SECOND_END_SYMBOL	 		0X0A

enum
{
	NENCRYPTION_NCOMPRESSION = 0X00,
	ENCRYPTION_NCOMPRESSION,
	NENCRYPTION_COMPRESSION,
	ENCRYPTION_COMPRESSION,
	
};

STATIC VOID HandleDataPackage(U8 *pBuf, U16 u16Length)
{
	
	if(g_bUpdateCmd == FALSE)	
	{
		if(g_bProtocolMode == TRUE)
		{
			U32 *pData;
			U32 u32Length;
			U32 u32TotalPackageNumber = 0;//总的包数
			U32 u32CurrentPackageNumber = 0;//当前包数
			U32 u32CrcValue;
			U16 u16Loop;
			U8 *szData;
			u32Length = u16Length + 19;
			
			pData = malloc(u32Length*4);
			
			if(pData != NULL)
			{
				for(u16Loop = 0; u16Loop < u32Length; u16Loop++)
				{
					pData[u16Loop] = pBuf[u16Loop];
				}
				u32CrcValue = HardGenCrc32(pData, u16Length);
				free(pData);
			}
			else
			{
				EnterException(ERR_MALLOC_FAIL);
			}
			szData = malloc(u16Length + 25);
			if(pData == NULL)
			{
				EnterException(ERR_MALLOC_FAIL);
			}
			szData[0] = START_SYMBOL;					//FE
			szData[1] = PROTOCOL_EDITION;				//01
			szData[2] = HIBYTE(HIWORD(u32Length));		//长度
			szData[3] = LOBYTE(HIWORD(u32Length));		//长度
			szData[4] = HIBYTE(LOWORD(u32Length));		//长度
			szData[5] = LOBYTE(LOWORD(u32Length));		//长度
			szData[6] = DATA_DIRECTION;					//方向
			szData[7] = MAINCMD_INTERNET;				//主命令
			szData[8] = HIBYTE(SUBCMD_SENSOR);			//子命令
			szData[9] = LOBYTE(SUBCMD_SENSOR);			//子命令
			szData[10] = NENCRYPTION_NCOMPRESSION;		//加密情况
			szData[11] = HIBYTE(HIWORD(u32TotalPackageNumber));//包总序号
			szData[12] = LOBYTE(HIWORD(u32TotalPackageNumber));//包总序号
			szData[13] = HIBYTE(LOWORD(u32TotalPackageNumber));//包总序号
			szData[14] = LOBYTE(LOWORD(u32TotalPackageNumber));//包总序号
			szData[15] = HIBYTE(HIWORD(u32CurrentPackageNumber));//当前包序号
			szData[16] = LOBYTE(HIWORD(u32CurrentPackageNumber));//当前包序号
			szData[17] = HIBYTE(LOWORD(u32CurrentPackageNumber));//当前包序号
			szData[18] = LOBYTE(LOWORD(u32CurrentPackageNumber));//当前包序号
			memcpy(szData + 19, pBuf, u16Length);
			
			szData[19 + u16Length] = HIBYTE(HIWORD(u32CrcValue));
			szData[20 + u16Length] = LOBYTE(HIWORD(u32CrcValue));
			szData[21 + u16Length] = HIBYTE(LOWORD(u32CrcValue));
			szData[22 + u16Length] = LOBYTE(LOWORD(u32CrcValue));
			
			szData[23 + u16Length] = FIRST_END_SYMBOL;			//结束符号
			
			szData[24 + u16Length] = SECOND_END_SYMBOL;			//结束符号
			
			UsartSend(USART2, szData, u32Length + 6);
			
			UsartSend(USART1, szData, u32Length + 6);
			
			free(szData);
		}
		else
		{
			UsartSend(USART2, pBuf, u16Length);

			UsartSend(USART1, pBuf, u16Length);
		}
	}
	else
	{

		UsartSend(USART2, pBuf, u16Length);
		
		UsartSend(USART1, pBuf, u16Length);
		
		g_bUpdateCmd = FALSE;
	
	}

}
STATIC BOOL SendAndWaitDtuDataProc(U8 *pBuf, U16 u16Length)
{
	U32 u32Tick;
	
	U32 u32Count;
	
	S32 s32Count;
	
	U8 szData[300];
	
	BOOL bRet = FALSE;
	
	Init(pUart1QueueInfo);
	
	UsartSend(USART1, pBuf, u16Length);
	
	u32Tick = GetTickCount();
	
	if(memcmp(pBuf, "AT+SAVEPARAM", 12) == 0)
	{
		u32Count = 5000;
	}
	else
	{
		u32Count = 1000;
	}
	
	do
	{
		if(GetQueueLength(pUart1QueueInfo))
		{
			s32Count = GetTickCount() - GetUartReceiveTime(USART1);
			if(s32Count > 50)
			{
				ReadUsartData(USART1, szData, &u16Length);
				
				UsartSend(USART1, szData, u16Length);
				
				if(strstr((char *)szData, "OK") != FALSE)
				{
					bRet = TRUE;
					g_u8DtuRetry = 0;
					break;
				}
			}
		}
		
		
	}while((GetTickCount() - u32Tick) < u32Count);
	
	DelayMs(200);
	
	if(bRet == FALSE)
	{
		g_u8DtuRetry++;
		if(g_u8DtuRetry > 20)
		{
			NVIC_SystemReset();
		}
	}
	
	return bRet;
}

STATIC VOID DtuConfigure(VOID)
{
	U8 szData[100] = {0};
	
	U8 u8InterviewType;
	
	memset(szData, 0, sizeof(szData));
	memcpy((char *)szData, "+++", 3);//enter AT mode
	while(SendAndWaitDtuDataProc(szData, strlen((char *)szData)) == FALSE);
	
	memset(szData, 0, sizeof(szData));
	sprintf((char *)szData, "%s\r\n", "AT+LOGIN=admin");//login
	while(SendAndWaitDtuDataProc(szData, strlen((char *)szData)) == FALSE);
	
	
	memset(szData, 0, sizeof(szData));
	sprintf((char *)szData, "%s\r\n", "AT+SETPARAM=RTUPMPL,1024");
	while(SendAndWaitDtuDataProc(szData, strlen((char *)szData)) == FALSE);
	
	memset(szData, 0, sizeof(szData));
	sprintf((char *)szData, "%s\r\n", "AT+SETPARAM=OPPDIO,OFF");
	while(SendAndWaitDtuDataProc(szData, strlen((char *)szData)) == FALSE);
	
	memset(szData, 0, sizeof(szData));
	sprintf((char *)szData, "%s\r\n", "AT+SETPARAM=RTUPDPTI,50");
	while(SendAndWaitDtuDataProc(szData, strlen((char *)szData)) == FALSE);
	
		
	u8InterviewType = IP_INTERVIEW;
	
	
	
		
	memset(szData, 0, sizeof(szData));
	sprintf((char *)szData, "%s\r\n", "AT+SETCHL=1");//set channel
	while(SendAndWaitDtuDataProc(szData, strlen((char *)szData)) == FALSE);
	
	memset(szData, 0, sizeof(szData));
	sprintf((char *)szData, "%s\r\n", "AT+SETPARAM=CHPCT,TCP");
	while(SendAndWaitDtuDataProc(szData, strlen((char *)szData)) == FALSE);
	
	memset(szData, 0, sizeof(szData));
	sprintf((char *)szData, "%s\r\n", "AT+SETPARAM=CHPDTUP,4003");//dtu port
	while(SendAndWaitDtuDataProc(szData, strlen((char *)szData)) == FALSE);
	
	if(u8InterviewType == IP_INTERVIEW)//铁四院的配置
	{
	
		memset(szData, 0, sizeof(szData));
		sprintf((char *)szData, "%s\r\n", "AT+SETPARAM=CHPDSCIP,59.172.252.243");//dsc ip
		while(SendAndWaitDtuDataProc(szData, strlen((char *)szData)) == FALSE);
				
		memset(szData, 0, sizeof(szData));
		sprintf((char *)szData, "%s\r\n", "AT+SETPARAM=CHPDSCD,");//dsc domain
		while(SendAndWaitDtuDataProc(szData, strlen((char *)szData)) == FALSE);
		
		memset(szData, 0, sizeof(szData));
		sprintf((char *)szData, "%s\r\n", "AT+SETPARAM=CHPDSCP,7005");//dsc port
		while(SendAndWaitDtuDataProc(szData, strlen((char *)szData)) == FALSE);
	}
	else
	{
		memset(szData, 0, sizeof(szData));
		sprintf((char *)szData, "%s\r\n", "AT+SETPARAM=CHPDSCIP,0.0.0.0");//dsc ip
		while(SendAndWaitDtuDataProc(szData, strlen((char *)szData)) == FALSE);
	
		memset(szData, 0, sizeof(szData));
		sprintf((char *)szData, "%s\r\n", "AT+SETPARAM=CHPDSCD,ever006.oicp.net");//dsc domain
		while(SendAndWaitDtuDataProc(szData, strlen((char *)szData)) == FALSE);
		
		memset(szData, 0, sizeof(szData));
		sprintf((char *)szData, "%s\r\n", "AT+SETPARAM=CHPDSCP,19085");//dsc port
		while(SendAndWaitDtuDataProc(szData, strlen((char *)szData)) == FALSE);
		


	}
	
	memset(szData, 0, sizeof(szData));
	sprintf((char *)szData, "%s\r\n", "AT+SAVEPARAM");
	while(SendAndWaitDtuDataProc(szData, strlen((char *)szData)) == FALSE);
	
	
	memset(szData, 0, sizeof(szData));
	sprintf((char *)szData, "%s\r\n", "AT+RESET");
	UsartSend(USART1, szData, strlen((char *)szData));
}


/************************************************************************************************/
STATIC VOID HandleCmdProc(U8 *pBuf, U16 u16Length)
{
	U8 szData[50];
	U8 szBuf[4] = {0};
	FLASH_Status FLASHStatus = FLASH_COMPLETE;
	if(u16Length == 7)//-------------------------升级命令
	{
		g_bUpdateCmd = TRUE;
		
		if((pBuf[0] == 0xfe) && (pBuf[1] == 0x00) && (pBuf[6] == 0xfe))//进入擦掉升级区域
		{
////			ExternFlashChipErase();
			FlashInit();//2016-2-24
			memset(szData, 0, sizeof(szData));
			sprintf((char *)szData, "app erase exflash over\r\n");
			HandleDataPackage(szData, strlen((char *)szData));
		}
		else if((pBuf[0] == 0xfe) && (pBuf[1] == 0x01) && (pBuf[6] == 0xfe))//进入升级模式
		{
			g_bUpdate = TRUE;
			g_u32TimeOut = 200;
			g_u32WriteOffset = 0;
			memset(szData, 0, sizeof(szData));
			sprintf((char *)szData, "app enter update mode\r\n");
			HandleDataPackage(szData, strlen((char *)szData));
		}
		else if((pBuf[0] == 0xfe) && (pBuf[1] == 0x02) && (pBuf[6] == 0xfe))//退出升级模式
		{
			g_bUpdate = FALSE;
			g_u32TimeOut = 20;
			memset(szData, 0, sizeof(szData));
			sprintf((char *)szData, "app exit update mode\r\n");
			HandleDataPackage(szData, strlen((char *)szData));
		}
		else if((pBuf[0] == 0xfe) && (pBuf[1] == 0x03) && (pBuf[6] == 0xfe))//写升级成功标志
		{
			FLASH_Unlock();///Flash解锁
			while(MCUFlashErase(FLASH_Sector_6) != 1);//直到擦除
			DelayMs(2);
			FLASH_ProgramWord(UPDATE_FLAG_ADDRESS, 0x55AA55AA);//写入成功标记
			FLASH_ProgramWord(UPDATE_FLAG_ADDRESS + 4, g_u32AppAddress);//写入应用地址标记
			FLASH_ProgramWord(UPDATE_FLAG_ADDRESS + 8, g_u32HardEdition);//写入硬件版本
			FLASH_ProgramWord(UPDATE_FLAG_ADDRESS + 12, g_u32SoftEdition);//写入软件版本
			FLASH_Lock();	
			
			memset(szData, 0, sizeof(szData));
			sprintf((char *)szData, "app set flag ok\r\n");
			HandleDataPackage(szData, strlen((char *)szData));
		}
		else if((pBuf[0] == 0xfe) && (pBuf[1] == 0x04) && (pBuf[6] == 0xfe))//应用地址
		{
			g_u32AppAddress = MAKELONG(MAKEWORD(pBuf[2], pBuf[3]), MAKEWORD(pBuf[4], pBuf[5]));
			memset(szData, 0, sizeof(szData));
			sprintf((char *)szData, "app set app address ok\r\n");
			HandleDataPackage(szData, strlen((char *)szData));
		}
		else if((pBuf[0] == 0xfe) && (pBuf[1] == 0x05) && (pBuf[6] == 0xfe))//硬件版本
		{
			g_u32HardEdition = MAKELONG(MAKEWORD(pBuf[2], pBuf[3]), MAKEWORD(pBuf[4], pBuf[5]));
			memset(szData, 0, sizeof(szData));
			sprintf((char *)szData, "app set hard edition ok\r\n");
			HandleDataPackage(szData, strlen((char *)szData));
		}
		else if((pBuf[0] == 0xfe) && (pBuf[1] == 0x06) && (pBuf[6] == 0xfe))//软件版本
		{
			g_u32SoftEdition = MAKELONG(MAKEWORD(pBuf[2], pBuf[3]), MAKEWORD(pBuf[4], pBuf[5]));
			memset(szData, 0, sizeof(szData));
			sprintf((char *)szData, "app set soft edition ok\r\n");
			HandleDataPackage(szData, strlen((char *)szData));
		}
		else if((pBuf[0] == 0xfe) && (pBuf[1] == 0x07) && (pBuf[6] == 0xfe))//重启
		{	
			memset(szData, 0, sizeof(szData));
			sprintf((char *)szData, "app restart\r\n");
			HandleDataPackage(szData, strlen((char *)szData));
			DelayMs(1500);
			UartDeinit(USART1);
			UartDeinit(USART2);
			NVIC_SystemReset();
		}
		else if((pBuf[0] == 0xfe) && (pBuf[1] == 0x08) && (pBuf[6] == 0xfe))//获取升级数据长度
		{
			memset(szData, 0, sizeof(szData));
			sprintf((char *)szData, "app update = %d Bytes\r\n", g_u32WriteOffset);
			HandleDataPackage(szData, strlen((char *)szData));
		}
		else if((pBuf[0] == 0xfe) && (pBuf[1] == 0x09) && (pBuf[6] == 0xfe))//擦除升级标志
		{
			FlashInit();
			
//			FlashReadSTM32(UPDATE_FLAG_ADDRESS, 4, szBuf);
			
			memset(szData, 0, sizeof(szData));
			
			if((szBuf[0] == 0xAA) && (szBuf[1] == 0x55) && (szBuf[2] == 0xAA) && (szBuf[3] == 0x55))
			{
				sprintf((char *)szData, "app erase flag error\r\n");
			}
			else
			{
				sprintf((char *)szData, "app erase flag ok\r\n");
			}
			HandleDataPackage(szData, strlen((char *)szData));
		}	
		else if((pBuf[0] == 0xfe) && (pBuf[1] == 0xf1) && (pBuf[6] == 0xfe))//设置时间间隔模式
		{
			g_u32TimeInternal = MAKELONG(MAKEWORD(pBuf[5], pBuf[4]), MAKEWORD(pBuf[3], pBuf[2]));
			g_u32TimeInternal = g_u32TimeInternal * 1000;
			g_bLastDtuWork = FALSE;//设置完之后强制发送所有数据
			memset(szData, 0, sizeof(szData));
			sprintf((char *)szData, "Set Interval mode success");
			HandleDataPackage(szData, strlen((char *)szData));
		}
		else if((pBuf[0] == 0xfe) && (pBuf[1] == 0x0B) && (pBuf[6] == 0xfe))//获取当前模式
		{
			memset(szData, 0, sizeof(szData));
			
			if(g_bProtocolMode == TRUE)
			{
				sprintf((char *)szData, "App Protocol Mode\r\n");
			}
			else
			{
				sprintf((char *)szData, "App Un-Protocol Mode\r\n");
			}
			
			HandleDataPackage(szData, strlen((char *)szData));
		}
		else if((pBuf[0] == 0xfe) && (pBuf[1] == 0x0C) && (pBuf[6] == 0xfe))//设置非协议模式
		{
			g_bProtocolMode = FALSE;
			memset(szData, 0, sizeof(szData));
			sprintf((char *)szData, "Un-Protocol Mode\r\n");
			HandleDataPackage(szData, strlen((char *)szData));
		}
		else if((pBuf[0] == 0xfe) && (pBuf[1] == 0x0D) && (pBuf[6] == 0xfe))//设置协议模式
		{
			g_bProtocolMode = TRUE;
			memset(szData, 0, sizeof(szData));
			sprintf((char *)szData, "Protocol Mode\r\n");
			HandleDataPackage(szData, strlen((char *)szData));
		}
//		
		else
		{
			if(g_bUpdate == TRUE) 
			{		
				Flash_w_Length(g_u32WriteOffset, szData, u16Length);
				g_u32WriteOffset = g_u32WriteOffset + u16Length;
				memset(szData, 0, sizeof(szData));
				sprintf((char *)szData, "app receive = %d\r\n",u16Length);
				HandleDataPackage(szData, strlen((char *)szData));			
			}
			else
			{
				g_bUpdateCmd = FALSE;
			}
		}
	}
	else if(u16Length == 10)//设置时间命令
	{
		g_bUpdateCmd = TRUE;
		
		if((pBuf[0] == 0xfe) && (pBuf[1] == 0xf2) && (pBuf[9] == 0xfe))//设置时间
		{
			U16 u16year;
			U8 u8month, u8day, u8hour, u8minute, u8second;
			u16year = MAKEWORD(pBuf[3], pBuf[2]);
			u8month = pBuf[4];
			u8day   = pBuf[5];
			u8hour  = pBuf[6];
			u8minute = pBuf[7];
			u8second = pBuf[8];
			set_time(pBuf[3],u8month,u8day,u8hour,u8minute,u8second,1,1);
			g_bLastDtuWork = FALSE;//设置完之后强制发送所有数据
			memset(szData, 0, sizeof(szData));
			sprintf((char *)szData, "Set time success");
			HandleDataPackage(szData, strlen((char *)szData));
		}
		else
		{
			if(g_bUpdate == TRUE) 
			{
				
				Flash_w_Length(g_u32WriteOffset, szData, u16Length);
				g_u32WriteOffset = g_u32WriteOffset + u16Length;
				memset(szData, 0, sizeof(szData));
				sprintf((char *)szData, "app receive = %d\r\n",u16Length);
				HandleDataPackage(szData, strlen((char *)szData));
				
			}
			else
			{
				g_bUpdateCmd = FALSE;
			}
		}
	}
	else
	{
		if(g_bUpdate == TRUE) 
		{
			Flash_w_Length(g_u32WriteOffset, szData, u16Length);
			g_u32WriteOffset = g_u32WriteOffset + u16Length;
			memset(szData, 0, sizeof(szData));
			sprintf((char *)szData, "app receive = %d\r\n",u16Length);
			HandleDataPackage(szData, strlen((char *)szData));
			
		}
		else
		{
			g_bUpdateCmd = FALSE;
		}
	}
}

VOID ReadLockId(U32 *pData)
{
	//获取CPU唯一ID
	pData[0]=*(vu32*)(0x1fff7a10);
	pData[1]=*(vu32*)(0x1fff7a14);
	pData[2]=*(vu32*)(0x1fff7a18);
}

STATIC VOID HandleBoardInfo(U8 *pBuf, U16 *pLength)
{
	U32 szId[3];
    U8 szData[50] = {0};
    U16 u16Length;
	ReadLockId(szId);
    sprintf((char *)szData, "#%13sID=0x%x%x%x\r\n", g_szMachineType, szId[2], szId[1], szId[0]);
    u16Length = strlen((char *)szData);
    memcpy(pBuf, szData, u16Length);
	*pLength = *pLength + u16Length; 
}

STATIC VOID HandleADCProc(U8 *pBuf, U16 *pLength)
{
    U8 szData[50] = {0};
    U16 u16Length;

    memset(szData, 0, sizeof(szData));
    sprintf((char *)szData, "#%13sCurrent voltage = %f V\r\n", g_szMachineType, get_dev_voltage(get_adc_value()));
    u16Length = strlen((char *)szData);
	memcpy(pBuf, szData, u16Length);
	*pLength = *pLength + u16Length;
}

STATIC VOID HandleModeProc(U8 *pBuf, U16 *pLength)
{
	U8 szData[50] = {0};
	U16 u16Length;
	U16 u16Time;
	u16Time = g_u32TimeInternal/1000;
	
    memset(szData, 0, sizeof(szData));
	sprintf((char *)szData, "#%13sInterval Mode = %d second\r\n", g_szMachineType, u16Time);
    u16Length = strlen((char *)szData);
	memcpy(pBuf, szData, u16Length);
	*pLength = *pLength + u16Length;
}

 STATIC VOID HandleRTCProc(U8 *pBuf, U16 *pLength)
{
    U8 szData[50] = {'\0'};
    U16 u16Length;
	memset(szData,'\0',50);
	strcat(szData,get_time2());
	strcat(szData,"\r\n");
    //sprintf((char *)szData, "%s\r\n",get_time());
    u16Length = strlen((char *)szData);
    memcpy(pBuf, szData, u16Length);
	*pLength = *pLength + u16Length;
}

STATIC VOID HandleTemperatureProc(U8 *pBuf, U16 *pLength)
{
	float Temperature,Humidity = 1;
	short temperature; 
	
	U8 szData[50] = {0};
	U16 u16Length;
	
//	temperature = DS18B20_Get_Temp();	

	temperature = 29;
	if(temperature<0)
	{										//显示负号
		temperature=-temperature;					//转为正数
		Temperature = temperature/10 + (temperature%10)/10.0;
		if((Temperature > 80) || (Temperature < -40) || (Humidity > 99) || (Humidity < 1))
		{
				sprintf((char *)szData, "#%13sHumidity&Temperature error,Please check", g_szMachineType);
		}
		else
		{
				sprintf((char *)szData, "#%13sHumidity = %.1f,Temperature =-%.1f",g_szMachineType, Humidity, Temperature);
		}
	}
	else
	{
		Temperature = temperature/10 + (temperature%10)/10.0;
		if((Temperature > 80) || (Temperature < -40) || (Humidity > 99) || (Humidity < 1))
		{
				sprintf((char *)szData, "#%13sHumidity&Temperature error,Please check\r\n", g_szMachineType);
		}
		else
		{
				sprintf((char *)szData, "#%13sHumidity = %.1f,Temperature =+%.1f\r\n",g_szMachineType, Humidity, Temperature);
		}
	}
	
	
	
  
	u16Length = strlen((char *)szData);
	memcpy(pBuf, szData, u16Length);
	*pLength = *pLength + u16Length;	
}

STATIC VOID HandleAds1248Proc(U8 *pBuf, U16 *pLength)
{
	U8 u8Channel;
	U8 szData[400] = {0};
	U16 u16Length = 0;
	U8 u8ADS_MUX0;
	float PT100Res;
	double PT100TempValue;
	for(u8Channel = 1; u8Channel < 5; u8Channel++)
	{
		//PT100TempValue = GetTemperature(LTC2402_GetResistance(u8Channel));
		PT100TempValue = GetFreq(u8Channel);
        memset(szData, 0, sizeof(szData));
        if((PT100TempValue > 80) || (PT100TempValue < -40))
        {
            sprintf((char *)szData, "#%13sChannel:%d Solid Temperature error,Please check\r\n", g_szMachineType, u8Channel);
        }
        else
        {
            sprintf((char *)szData, "#%13sChannel:%d Solid Temperature:%.2f\r\n",g_szMachineType, u8Channel, PT100TempValue);
        }
		
		memcpy(pBuf + u16Length, szData, strlen((char *)szData));
		u16Length = strlen((char *)szData) + u16Length;
	}
	
	*pLength = *pLength + u16Length;

}

typedef struct _tagLedTaskInfo
{
    U32 u32LedTick;
    BOOL bLed;
    U8 u8Count;
    
}CLedTaskInfo, *PLedTaskInfo;



VOID TestDtu(VOID)
{
	U8 *szData;
	U16 u16Length;
	U32 u32Tick;
    S32 s32Count;
	CLedTaskInfo cLedTaskInfo = {FALSE, 0, 0};
	
	szData = malloc(DTUUARTSIZE);
	if(szData == NULL)
	{
		EnterException(ERR_MALLOC_FAIL);
	}
    
	
	DtuInit();
	
	DtuConfigure();//配置GSM
	printf("DtuConfigure ok\r\n");
	u32Tick = GetTickCount();
	
	g_u32TimeOut = 20;
	
	
	while(1)
	{
        if(cLedTaskInfo.bLed == TRUE)
        {
        }
        
        //假如串口有数据，则进行串口数据解析
		if(GetQueueLength(pUart1QueueInfo))
		{
			s32Count = GetTickCount() - GetUartReceiveTime(USART1);
			if(s32Count > g_u32TimeOut)
			{
				ReadUsartData(USART1, szData, &u16Length);
				HandleCmdProc(szData, u16Length);			//处理命令
			}
		}
		
		
		if(DetectOnline() == TRUE)//上线了
		{
			if(g_bLastDtuWork == FALSE)//第一次链接成功即发送
			{
                cLedTaskInfo.bLed = TRUE;
                cLedTaskInfo.u32LedTick = GetTickCount();
				u32Tick = GetTickCount();
				
				u16Length = 0;
				
				printf("第一次链接成功即发送\r\n");
				
                HandleRTCProc(szData + u16Length, &u16Length);
				//printf("HandleBoardInfo\r\n");
                HandleBoardInfo(szData + u16Length, &u16Length);
				//printf("HandleModeProc\r\n");
				HandleModeProc(szData + u16Length, &u16Length);
				//printf("HandleADCProc\r\n");
                HandleADCProc(szData + u16Length, &u16Length);
				//printf("HandleTemperatureProc\r\n");
				HandleTemperatureProc(szData + u16Length, &u16Length);
				//printf("HandleAds1248Proc\r\n");
				HandleAds1248Proc(szData + u16Length, &u16Length);
				//printf("HandleDataPackage\r\n");
				HandleDataPackage(szData,u16Length);//重连成功后发送状态数据
				
                g_bLastDtuWork = TRUE;
			}
			
			
			s32Count = GetTickCount() - u32Tick;
			if(s32Count > g_u32TimeInternal)  //定时发送一组数据给上位机
			{
                cLedTaskInfo.bLed = TRUE;
                cLedTaskInfo.u32LedTick = GetTickCount();
				u32Tick = GetTickCount();
				u16Length = 0;
                HandleRTCProc(szData + u16Length, &u16Length);
                HandleBoardInfo(szData + u16Length, &u16Length);
				HandleModeProc(szData + u16Length, &u16Length);
                HandleADCProc(szData + u16Length, &u16Length);
				HandleTemperatureProc(szData + u16Length, &u16Length);
				HandleAds1248Proc(szData + u16Length, &u16Length);
				
				HandleDataPackage(szData,u16Length);
			}
			
		}
		else // 断线了
		{
			s32Count = GetTickCount() - u32Tick;
			if(s32Count > 1000)
			{
				memset(szData, 0, DTUUARTSIZE);
				sprintf((char *)szData, "app off line");
				UsartSend(USART1, (U8 *)szData, strlen((char *)szData));
				u32Tick = GetTickCount();
			}
		}
	}

}


