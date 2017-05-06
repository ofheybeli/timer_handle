#include "Timer.h"
#include "TimerMemory.h"
#include "TimerCore.h"


extern Struct_Timer_Memory sTimerMemory;
extern Struct_Timer_Handle *sTimerCreateHandle;
extern Struct_Timer_Handle *sTimerCreateHandleIter;

/***************************************Timer User Function**************************************/

void Timer_DefaultProc(Struct_Timer_Message *pMsg)// Mesajý sýfýrlamak için kullanýlýr
{
	sTimerCreateHandle = (Struct_Timer_Handle *)Timer_FindHandle(pMsg->MsgTimerHandle);
	sTimerCreateHandle->MsgId &= ~(pMsg->MsgTimerId);	
}


uint8_t Timer_SetUserData(Time_Handle_Id TimerHandle,Time_Handle_Id TimerDesHandle,void *pSrc,uint8_t NumBytes)
{
	uint8_t i;
	uint8_t *p = pSrc;
	sTimerCreateHandle =  Timer_FindHandle(TimerDesHandle);
	if( (sTimerCreateHandle->timerStatusFlag & FlgTimerEnable ) == 0) return 0;
	
		if(sTimerCreateHandle->timerExNumBytes >= NumBytes)
		{
			  sTimerCreateHandle->timerUsageExNumBytes = NumBytes;
				sTimerCreateHandle->timerStatusFlag |= FlgSetUserData;
				for(i=0; i < sTimerCreateHandle->timerUsageExNumBytes ; i++)
				{
						*(sTimerCreateHandle->pTimerExNumBytes + i) = *(uint8_t *)(p+i);
				}
			
		}else return 0;
	return 1;
}

uint8_t Timer_GetUserData(Time_Handle_Id TimerHandle, void *pDest, uint8_t SizeOfBuffer)
{
	uint8_t i;
	uint8_t *p = pDest;
	sTimerCreateHandle =  Timer_FindHandle(TimerHandle);
	if( (sTimerCreateHandle->timerStatusFlag & FlgTimerEnable ) == 0) return 0;
	if( sTimerCreateHandle->timerUsageExNumBytes < SizeOfBuffer) return 0;
	else { SizeOfBuffer = sTimerCreateHandle->timerUsageExNumBytes;}
		
		if( (sTimerCreateHandle->timerStatusFlag & FlgSetUserData) != 0)
		{
			sTimerCreateHandle->timerUsageExNumBytes = 0;
			sTimerCreateHandle->timerStatusFlag &= ~FlgSetUserData;
			for(i = 0; i < SizeOfBuffer ; i++)
			{
                                        *(uint8_t *)(p + i) = *(sTimerCreateHandle->pTimerExNumBytes + i);
			}
		}
		else return 0;
		
	return SizeOfBuffer;
}

void Timer_SendMessage(Time_Handle_Id TimerHandle , Struct_Timer_Message *pMsg)
{
		sTimerCreateHandle =  Timer_FindHandle(TimerHandle);
		if( (sTimerCreateHandle->timerStatusFlag & FlgTimerEnable ) != 0)
		{
			  sTimerCreateHandle->timerStatusFlag |= FlgMessageEnable;
			  sTimerCreateHandle->MsgId |= pMsg->MsgTimerId;
			  sTimerCreateHandle->Data.p = pMsg->Data.p;
			  sTimerCreateHandle->Data.v = pMsg->Data.v;
		}			
}

void Timer_SendMessageNoPara(Time_Handle_Id TimerHandle, uint16_t MsgId)
{
		sTimerCreateHandle =  Timer_FindHandle(TimerHandle);
		if( (sTimerCreateHandle->timerStatusFlag & FlgTimerEnable ) != 0)
		{
			sTimerCreateHandle->timerStatusFlag |= FlgMessageEnable;
			sTimerCreateHandle->MsgId |= MsgId;
		}
}

uint8_t Timer_Stop(Time_Handle_Id TimerHandle)
{		
	sTimerCreateHandle =  Timer_FindHandle(TimerHandle);
	if( (sTimerCreateHandle->timerStatusFlag & FlgTimerEnable ) == 0) return 0;
	
		 
		sTimerCreateHandle->timerStatusFlag |= FlgMessageEnable;
		sTimerCreateHandle->MsgId |= TM_STOP;

	return 1;
}

uint8_t Timer_Start(Time_Handle_Id TimerHandle)
{
	sTimerCreateHandle =  Timer_FindHandle(TimerHandle);
	if( (sTimerCreateHandle->timerStatusFlag & FlgTimerEnable ) == 0) return 0;
	
		sTimerCreateHandle->timerStatusFlag |= FlgMessageEnable;
		sTimerCreateHandle->MsgId |= TM_START;
	
	return 1;
}

uint8_t Timer_Restart(Time_Handle_Id TimerHandle ,uint16_t tMs)
{
	sTimerCreateHandle =  Timer_FindHandle(TimerHandle);
	if( (sTimerCreateHandle->timerStatusFlag & FlgTimerEnable ) == 0) return 0;
	
		sTimerCreateHandle->timerStatusFlag |= FlgCountEnable;  
		sTimerCreateHandle->timerCountMs = tMs;
	
	return 1;
}

uint8_t Timer_SetCallbackFunc(Time_Handle_Id TimerHandle , void (*foo)(Struct_Timer_Message *pMsg) )
{
	
	sTimerCreateHandle =  Timer_FindHandle(TimerHandle);
	if( (sTimerCreateHandle->timerStatusFlag & FlgTimerEnable ) == 0) return 0;
	
		sTimerCreateHandle->func = foo;
	
	return 1;
}

uint16_t Timer_GetCountMs(Time_Handle_Id TimerHandle)
{
		sTimerCreateHandle =  Timer_FindHandle(TimerHandle);
		if( (sTimerCreateHandle->timerStatusFlag & FlgTimerEnable ) == 0) return 0;
	
	return sTimerCreateHandle->timerCountMs;
}
