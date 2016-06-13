

#include "stm32f4xx_it.h"
#include <stdio.h>
#include  <includes.h>
#define ERR_INFO "\r\nEnter HardFault_Handler, System Halt.\r\n"

unsigned int 	CountTimes    = 0;//计时
double 			OnFreq               = 0 ;//频率
unsigned char 	measureflag   = 0;//测量完毕的标志
unsigned char 	PulseNumFlag  = 0;//用于记录已经发射几个脉冲--控制个数
extern unsigned char PulseNUM ;//发射脉冲的个数

unsigned char 	PreRecvieData = 0;
unsigned char 	Reciv[20];
unsigned char 	RecivNum      = 0;
unsigned char 	IsCMDflag     = 0;
unsigned char 	UploadFlag    = 0;

/*
*********************************************************************************************************
*	Cortex-M3 内核异常中断服务程序
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*	函 数 名: NMI_Handler
*	功能说明: 不可屏蔽中断服务程序。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void NMI_Handler(void)
{
}

/*
*********************************************************************************************************
*	函 数 名: HardFault_Handler
*	功能说明: 硬件故障中断服务程序。其他异常处理被关闭，而又发生了异常，则触发。
*			  执行异常处理时，发生了异常，则触发。复位时默认使能。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void HardFault_Handler(void)
{
#if 1
  const char *pError = ERR_INFO;
  uint8_t i;

  for (i = 0; i < sizeof(ERR_INFO); i++)
  {
     USART1->DR = pError[i];
     /* 等待发送结束 */
     while ((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET);
  }
#endif

  /* 当硬件失效异常发生时进入死循环 */
  while (1)
  {
  }
}

/*
*********************************************************************************************************
*	函 数 名: MemManage_Handler
*	功能说明: 内存管理异常中断服务程序。违反MPU设定的存储器访问规则时触发。 复位时默认未使能
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void MemManage_Handler(void)
{
  /* 当内存管理异常发生时进入死循环 */
  while (1)
  {
  }
}

/*
*********************************************************************************************************
*	函 数 名: BusFault_Handler
*	功能说明: 总线访问异常中断服务程序。取指令、数据读写、堆栈操作出现异常。 复位时默认未使能
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void BusFault_Handler(void)
{
  /* 当总线异常时进入死循环 */
  while (1)
  {
  }
}

/*
*********************************************************************************************************
*	函 数 名: UsageFault_Handler
*	功能说明: 用法错误中断服务程序。执行未定义指令、非对齐操作、除零时触发。 复位时默认未使能
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void UsageFault_Handler(void)
{
  /* 当用法异常时进入死循环 */
  while (1)
  {
  }
}

/*
*********************************************************************************************************
*	函 数 名: SVC_Handler
*	功能说明: 通过SWI指令的系统服务调用中断服务程序。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void SVC_Handler(void)
{
}

/*
*********************************************************************************************************
*	函 数 名: DebugMon_Handler
*	功能说明: 调试监视器中断服务程序。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void DebugMon_Handler(void)
{
}

/*
*********************************************************************************************************
*	函 数 名: PendSV_Handler
*	功能说明: 可挂起的系统服务调用中断服务程序。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void PendSV_Handler(void)
{
}
























/*------------------------------------------测量频率-----------------------------------------------*/
void TIM4_IRQHandler(void)
{
	//ucoiii里面进入硬件中断服务函数需要以下函数
	// 外部脉冲达到100个
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		CountTimes = TIM2->CNT;//得到时间,结束计时
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		OnFreq = (double)30000000/(double)(CountTimes);//算出频率
		TIM_Cmd(TIM4,DISABLE);
		TIM4->CNT = 0;
		//printf("FINISHED MEASURE tim2 :%d\r\n",CountTimes);
		TIM_Cmd(TIM2,DISABLE);
		TIM2->CNT = 0;//关闭计时,等下一次测量
		measureflag = 1;
	}
	//第15个脉冲开始计时
	if(TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET)
	{	
		TIM2->CNT = 0;
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
		TIM_Cmd(TIM2,ENABLE);//开始计时了
	}
}


/*------------------------------------------激励脉冲-----------------------------------------------*/
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		PulseNumFlag++;
		if(PulseNumFlag == PulseNUM)
		{
			PulseNumFlag = 0;
			PulseNUM = 0;
			TIM_Cmd(TIM3,DISABLE);//达到脉冲个数之后就关闭定时器，停止发射
		}
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}


void EXTI9_5_IRQHandler(void)
{
		if(EXTI_GetITStatus(EXTI_Line9) != RESET)
		{
			EXTI_ClearITPendingBit(EXTI_Line9);
		}
}



