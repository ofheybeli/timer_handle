#ifndef TimerCore_H_INCLUDE
#define TimerCore_H_INCLUDE

#include "Timer.h"

#define FlgTimerEnable 			(uint8_t)(1<<0)
#define FlgCountEnable			(uint8_t)(1<<1)
#define FlgMessageEnable 		(uint8_t)(1<<2)
#define FlgSetUserData			(uint8_t)(1<<3)


typedef struct{
	Time_Handle_Id 			timerHandle; // Herbir Timer Handle 
	uint8_t				timerStatusFlag;
	uint16_t				timerCountMs; // Timer Kaç ms de bir yenilenecek
	uint16_t				timerCountDt;
	uint16_t				MsgId; //mesaj Id si eklenir
	//Time_Handle_Id 			MsgSrcHandle;	
	uint8_t				timerExNumBytes; // Timer Ex Kaç byte taþýyacak
	uint8_t		 		timerUsageExNumBytes; // Timer Ex Kaç byte taþýyacak		
	uint8_t * 				pTimerExNumBytes; // Timer Ex byte baþlangýç adresi	
  union {
		void 	*p;
		uint32_t 	v;
	}Data;
	void (*func)(Struct_Timer_Message *tMsg); // Timer Callback fonksiyonu
	void *pNextTimerHandle; 
}Struct_Timer_Handle;


Struct_Timer_Handle * 	Timer_FirstHandleAdress		(void);
uint8_t 	            Timer_SumHandle	            (void);
void 		Timer_ExecTickInt		(void);
Struct_Timer_Handle * 	Timer_FindHandle		(Time_Handle_Id TimerHandle);


#endif
