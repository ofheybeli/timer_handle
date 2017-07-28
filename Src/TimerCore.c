#include "TimerCore.h"
#include "TimerMemory.h"


#define tMax 0xFFFF

extern Struct_Timer_Memory sTimerMemory;
Struct_Timer_Handle 	*sTimerCreateHandle;
Struct_Timer_Handle     *sTimerCreateHandleIter;

Struct_Timer_Message  	pMsg;

volatile uint16_t  	Timer_Tick;
uint16_t	Timer_Tick_Old;
uint16_t  	Timer_Tick_Dt;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Struct_Timer_Handle * Timer_FirstHandleAdress(void)
{
	uint8_t i=0;
	if(sTimerMemory.TimerHandleCount > 0)
	{
		sTimerCreateHandle = (Struct_Timer_Handle *) (sTimerMemory.pMemoryBaseAdress);

		while(1)
		{
			if( (sTimerCreateHandle->timerStatusFlag & FlgTimerEnable) != 0) 
			{
				i++; 
				sTimerCreateHandle = (Struct_Timer_Handle *) (sTimerMemory.pMemoryBaseAdress + sizeof(Struct_Timer_Handle)* (i)); 
			}
			else 
			{ return sTimerCreateHandle; }
		}
	} else return (Struct_Timer_Handle *) (sTimerMemory.pMemoryBaseAdress);
}


uint8_t Timer_SumHandle()
{
	uint8_t rToplam=0;
	sTimerCreateHandleIter = Timer_FirstHandleAdress();
	
	while( sTimerCreateHandleIter->pNextTimerHandle !=NULL )
	{		
		rToplam++;
		sTimerCreateHandleIter = (Struct_Timer_Handle *) sTimerCreateHandleIter->pNextTimerHandle;
	}
	return rToplam;
}

Struct_Timer_Handle * Timer_FindHandle(Time_Handle_Id TimerHandle)
{
	return  (Struct_Timer_Handle *)(sTimerMemory.pMemoryBaseAdress + sizeof(Struct_Timer_Handle)*(TimerHandle - TimerHandleOffset));
}

void Timer_ExecTickInt()
{
	uint8_t i;

	if(Timer_Tick_Old >  Timer_Tick)
	{
		Timer_Tick_Dt = ((tMax - Timer_Tick_Old) + Timer_Tick);
	}
	else
	{
		Timer_Tick_Dt = (Timer_Tick - Timer_Tick_Old);
	}

	Timer_Tick_Old = Timer_Tick;

	for(i=0; i < sTimerMemory.TimerHandleCount; i++)
	{
		sTimerCreateHandle = (Struct_Timer_Handle *)(sTimerMemory.pMemoryBaseAdress + sizeof(Struct_Timer_Handle)*i);

		if( (sTimerCreateHandle->timerStatusFlag & FlgCountEnable) != 0)	
		{		
			sTimerCreateHandle->timerCountDt += Timer_Tick_Dt;

			if(sTimerCreateHandle->timerCountDt  >= sTimerCreateHandle->timerCountMs)
			{
				sTimerCreateHandle->timerStatusFlag &= ~FlgCountEnable;
				sTimerCreateHandle->timerCountDt = 0;
				pMsg.MsgTimerId = TM_INTERVAL;
				pMsg.MsgTimerHandle = sTimerCreateHandle->timerHandle;
				pMsg.MsgTimerSrcHandle = 0;
				sTimerCreateHandle->func(&pMsg);
			}			
		}
	}		
}

void Timer_Exec()
{
	static unsigned char MsgShiftReg = TM_USER;
  
	if(sTimerCreateHandleIter->pNextTimerHandle != NULL)
	{
		if( (sTimerCreateHandleIter->timerStatusFlag & FlgTimerEnable) != 0)
		{	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
			if( (sTimerCreateHandleIter->timerStatusFlag & FlgMessageEnable ) != 0)
			{	
				if( (sTimerCreateHandleIter->MsgId & TM_CREATE) != 0 )	
				{
					sTimerCreateHandleIter->MsgId &= ~(TM_CREATE);
					pMsg.MsgTimerId = TM_CREATE;
					pMsg.MsgTimerHandle = sTimerCreateHandleIter->timerHandle;
					pMsg.MsgTimerSrcHandle = 0;
					sTimerCreateHandleIter->func(&pMsg);
				}
				else if((sTimerCreateHandleIter->MsgId & TM_DISABLE) != 0)
				{
					sTimerCreateHandleIter->MsgId &= ~(TM_DISABLE);
					sTimerCreateHandleIter->timerStatusFlag &= ~( FlgMessageEnable | FlgMessageEnable | FlgSetUserData | FlgTimerEnable);  
					pMsg.MsgTimerId = TM_DISABLE;
					pMsg.MsgTimerHandle = sTimerCreateHandleIter->timerHandle;					
					sTimerCreateHandleIter->func(&pMsg);
				}
				else if((sTimerCreateHandleIter->MsgId & TM_START) != 0)
				{
					sTimerCreateHandleIter->timerStatusFlag |= FlgCountEnable; 
					sTimerCreateHandleIter->MsgId &= ~(TM_START);
					pMsg.MsgTimerId = TM_START;
					pMsg.MsgTimerHandle = sTimerCreateHandleIter->timerHandle;
					pMsg.MsgTimerSrcHandle = 0;
					sTimerCreateHandleIter->func(&pMsg);			
				}
				else if((sTimerCreateHandleIter->MsgId & TM_STOP) != 0)
				{
					sTimerCreateHandleIter->timerStatusFlag &= ~FlgCountEnable;
					sTimerCreateHandleIter->MsgId &= ~(TM_STOP);
					pMsg.MsgTimerId = TM_STOP;
					pMsg.MsgTimerHandle = sTimerCreateHandleIter->timerHandle;
					pMsg.MsgTimerSrcHandle = 0;
					sTimerCreateHandleIter->func(&pMsg);
				}
				else if((sTimerCreateHandleIter->MsgId & TM_ENABLE) != 0)
				{
					sTimerCreateHandleIter->MsgId &= ~(TM_ENABLE);
					pMsg.MsgTimerId = TM_STOP;
					pMsg.MsgTimerHandle = sTimerCreateHandleIter->timerHandle;
					pMsg.MsgTimerSrcHandle = 0;
					sTimerCreateHandleIter->func(&pMsg);
				}
				//else if( sTimerCreateHandleIter->MsgId >= (1 << TM_USER) ) // 1 adet kullanýcý mesajý için yazýlmýþtýr
				//{	
                                                //            //sTimerCreateHandleIter->MsgId
				//	pMsg.MsgTimerHandle = sTimerCreateHandleIter->timerHandle;
				//	pMsg.MsgTimerId = sTimerCreateHandleIter->MsgId;
				//	pMsg.Data.p = sTimerCreateHandleIter->Data.p;
				//	pMsg.Data.v = sTimerCreateHandleIter->Data.v;
				//	sTimerCreateHandleIter->func(&pMsg);
				//}
				else
				{
					if((sTimerCreateHandleIter->MsgId >> MsgShiftReg) & 0x1)
					{  
						  pMsg.MsgTimerHandle = sTimerCreateHandleIter->timerHandle;
						  pMsg.MsgTimerId = sTimerCreateHandleIter->MsgId;
						  sTimerCreateHandleIter->MsgId &= ~(1 << MsgShiftReg);
						  pMsg.Data.p = sTimerCreateHandleIter->Data.p;
						  pMsg.Data.v = sTimerCreateHandleIter->Data.v;
						  sTimerCreateHandleIter->func(&pMsg);        
						  
					} 
					else if(sTimerCreateHandleIter->MsgId == 0)
					{
						  sTimerCreateHandleIter->timerStatusFlag &= ~FlgMessageEnable;
						  sTimerCreateHandleIter->MsgId = 0;
						  sTimerCreateHandleIter->Data.v = 0;
						  sTimerCreateHandleIter->Data.p = NULL;
					}
					if(++ MsgShiftReg > 15 ) 
					{
						MsgShiftReg = TM_USER;
					}
				}
			}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			
		}
		sTimerCreateHandleIter = (Struct_Timer_Handle *) sTimerCreateHandleIter->pNextTimerHandle;	
	}
	else 
	{ 
		sTimerCreateHandleIter = (Struct_Timer_Handle *)sTimerMemory.pMemoryBaseAdress; 
	}
	Timer_ExecTickInt();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
