#include "Timer.h"
#include "TimerMemory.h"
#include "TimerCore.h"



Struct_Timer_Memory sTimerMemory;
extern Struct_Timer_Handle *sTimerCreateHandle;
extern Struct_Timer_Handle *sTimerCreateHandleIter;

void Timer_Memory_Assign(void *pAdress, uint16_t AdressCount)
{
	unsigned char *p;
	uint32_t i;
		
	if( (pAdress != NULL) && (AdressCount > 0) )
	{
		p = pAdress;
		
		for(i=0;i < AdressCount; i++)
		{
			*(p+i) = 0x00;	
		}
		
		sTimerMemory.pMemoryBaseAdress = p;
		sTimerMemory.pMemoryUsageAdress = p;
		sTimerMemory.pMemoryEndAdress = p + AdressCount;
		sTimerMemory.pMemoryExNumBytesAdress = sTimerMemory.pMemoryEndAdress;	
		
		sTimerCreateHandle				 = (Struct_Timer_Handle *)p;
		sTimerCreateHandleIter 		 = (Struct_Timer_Handle *)p;

		sTimerCreateHandle->pNextTimerHandle = NULL;
			
		sTimerMemory.TimerHandleCount = 0;		
		sTimerMemory.FlgMemoryEnabled = 1;
	}
	else { sTimerMemory.FlgMemoryEnabled = 0;}
	
	if(AdressCount < sizeof(Struct_Timer_Handle) ) sTimerMemory.FlgMemoryEnabled = 0;
	
}

uint16_t Timer_GetSizeMemory()
{
	if(sTimerMemory.FlgMemoryEnabled == 1)
	return (sTimerMemory.pMemoryEndAdress - sTimerMemory.pMemoryBaseAdress);
	else
	return 0;
}

uint16_t Timer_GetUsageMemory()
{
	if(sTimerMemory.FlgMemoryEnabled == 1)
	return(sTimerMemory.pMemoryEndAdress - sTimerMemory.pMemoryExNumBytesAdress + (sTimerMemory.pMemoryUsageAdress - sTimerMemory.pMemoryBaseAdress));
	else
	return 0;
}

uint16_t Timer_GetFreeMemory()
{
	if(sTimerMemory.FlgMemoryEnabled == 1)
	return(sTimerMemory.pMemoryExNumBytesAdress - sTimerMemory.pMemoryUsageAdress);
	else
	return 0;
}

