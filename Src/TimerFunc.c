#include "Timer.h"
#include "TimerMemory.h"
#include "TimerCore.h"

extern Struct_Timer_Memory sTimerMemory;
extern Struct_Timer_Handle *sTimerCreateHandle;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Time_Handle_Id Timer_Create(uint16_t tMs, void (*foo)(Struct_Timer_Message *pMsg) , uint8_t  MsgTimerExNumBytesCount)
{
	Time_Handle_Id  tHandle = -1;

	if( sTimerMemory.FlgMemoryEnabled == 0 ) return -1;
	if((sTimerMemory.pMemoryEndAdress <= (sTimerMemory.pMemoryBaseAdress + sizeof(Struct_Timer_Handle)*sTimerMemory.TimerHandleCount)))   return -1;
	
		sTimerCreateHandle = (Struct_Timer_Handle *)(sTimerMemory.pMemoryBaseAdress + sizeof(Struct_Timer_Handle)*sTimerMemory.TimerHandleCount ); //Di�er d���me ge�ildi ve ba�lang�� de�eri null olarak atand�
		
	 	if(sTimerCreateHandle->pNextTimerHandle == NULL)
		{
			tHandle = sTimerCreateHandle->timerHandle = TimerHandleOffset + sTimerMemory.TimerHandleCount;	//Timer Handle Id 
	 	
			sTimerCreateHandle->timerStatusFlag &= ~(FlgCountEnable); //Saya� Pasif Durumda
			sTimerCreateHandle->timerCountMs = tMs; // Timer Restart De�eri Tutulur
			sTimerCreateHandle->timerCountDt = 0; //Timer Say�c� timerCountMs ye e�it olunca callback ger�ekle�ir
			
			sTimerCreateHandle->func = foo; //Callback fonksiyon adresi
			
			sTimerCreateHandle->timerStatusFlag |= FlgMessageEnable; //Timer Mesaj olu�tu�unda flag set edilir
			sTimerCreateHandle->MsgId |= (uint8_t)(TM_CREATE); //Callback Mesaj numaralar� tutulur
//			sTimerCreateHandle->MsgSrcHandle = 0; //Kaynak Timer Handle yok
						
			sTimerCreateHandle->timerStatusFlag |= FlgTimerEnable;//Timer Handle Aktif durumda
			
			 if(MsgTimerExNumBytesCount != 0)	// Ex haberle�me alan� olu�turulur
			 {
					sTimerMemory.pMemoryExNumBytesAdress = (uint8_t *)(sTimerMemory.pMemoryExNumBytesAdress - MsgTimerExNumBytesCount );
				  sTimerCreateHandle->pTimerExNumBytes = (uint8_t *)(sTimerMemory.pMemoryExNumBytesAdress + 1 );
					sTimerCreateHandle->timerExNumBytes = MsgTimerExNumBytesCount;
					sTimerCreateHandle->timerUsageExNumBytes = 0;
			 }
			 
			sTimerMemory.TimerHandleCount++; //Ka� tane Timer olu�turuldu�unu tutar		 	
			sTimerCreateHandle->pNextTimerHandle = (Struct_Timer_Handle *)( sTimerMemory.pMemoryBaseAdress + sizeof(Struct_Timer_Handle)*	sTimerMemory.TimerHandleCount ); //Bi sonraki Yeni d���m�n adresi
			sTimerMemory.pMemoryUsageAdress = (uint8_t *)sTimerCreateHandle->pNextTimerHandle;
			 
		} else return -1;
	return tHandle;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Timer_Disable(Time_Handle_Id  TimerHandle)
{
	sTimerCreateHandle = Timer_FindHandle(TimerHandle);
	
	sTimerCreateHandle->MsgId = TM_DISABLE;
	sTimerCreateHandle->timerStatusFlag |= FlgMessageEnable;
}

int8_t Timer_Enable(Time_Handle_Id  TimerHandle,uint16_t tMs, void (*foo)(Struct_Timer_Message *pMsg))
{
	sTimerCreateHandle = Timer_FindHandle(TimerHandle);
	
	if( (sTimerCreateHandle->timerStatusFlag & FlgTimerEnable ) == 0) //Timer Disable Edilmisse
	{
		sTimerCreateHandle->timerStatusFlag &= ~(FlgCountEnable); //Saya� Pasif Durumda
		sTimerCreateHandle->timerCountMs = tMs; // Timer Restart De�eri Tutulur
		sTimerCreateHandle->timerCountDt = 0; //Timer Say�c� timerCountMs ye e�it olunca callback ger�ekle�ir
			
		sTimerCreateHandle->func = foo; //Callback fonksiyon adresi
			
		sTimerCreateHandle->timerStatusFlag |= FlgMessageEnable; //Timer Mesaj olu�tu�unda flag set edilir
		sTimerCreateHandle->MsgId |= (uint8_t)(TM_ENABLE); //Callback Mesaj numaralar� tutulur
//		sTimerCreateHandle->MsgSrcHandle = 0; //Kaynak Timer Handle yok
				
		sTimerCreateHandle->timerStatusFlag |= FlgTimerEnable;//Timer Handle Aktif durumda
	} else return 0;
	return 1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
