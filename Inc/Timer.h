/***********************************************
*	Kael Mühendislik
	Ketamin Library
*	Timer-Callback
*	v2.0
*
************************************************/
#ifndef Timer_H_INCLUDE
#define Timer_H_INCLUDE


#define NULL		 0x0
//#define TimerPriHigh	3
//#define TimerPriMedium	2
//#define TimerPriLow		1

#define	TimerHandleOffset	1453

typedef unsigned char 	uint8_t;
typedef unsigned short  uint16_t;
typedef unsigned long 		uint32_t;
//typedef unsigned long int	uint64_t;

typedef signed char 		int8_t;
typedef signed short  	int16_t;
typedef signed long 			int32_t;
//typedef signed long int		int64_t;

typedef int16_t Time_Handle_Id;

/************Message Return Id *********/
#define TM_CREATE        (uint16_t)(1<<0)
#define TM_INTERVAL	 (uint16_t)(1<<1)
#define TM_START	 (uint16_t)(1<<2)
#define TM_STOP	 (uint16_t)(1<<3)
#define TM_DISABLE	 (uint16_t)(1<<4)
#define TM_ENABLE	 (uint16_t)(1<<5)
#define TM_USER	 8
/***************************************/

typedef struct{
	uint16_t			MsgTimerId; //Gelen Mesajýn Nosu
	Time_Handle_Id 		MsgTimerHandle; // Kendi Handle
	Time_Handle_Id 		MsgTimerSrcHandle; // Kaynak Handle
	union {
		void 		*p;
		uint32_t 	            v;
	}Data;
}Struct_Timer_Message;


/*Memory*/
void 				Timer_Memory_Assign(void *pAdress, uint16_t AdressCount);
uint16_t 				Timer_GetSizeMemory(void);
uint16_t 				Timer_GetUsageMemory(void);
uint16_t 				Timer_GetFreeMemory(void);
/*Core*/
void 				Timer_Exec(void);
/*User Function*/
Time_Handle_Id 			Timer_Create(uint16_t tMs, void (*foo)(Struct_Timer_Message *pMsg) , uint8_t  MsgTimerExNumBytesCount);
void 				Timer_Disable(Time_Handle_Id  TimerHandle);
int8_t 				Timer_Enable(Time_Handle_Id  TimerHandle,uint16_t tMs, void (*foo)(Struct_Timer_Message *pMsg));
void 				Timer_DefaultProc(Struct_Timer_Message *pMsg);
uint8_t 				Timer_SetUserData(Time_Handle_Id TimerHandle,Time_Handle_Id TimerDesHandle,void *pSrc,uint8_t NumBytes);
uint8_t 				Timer_GetUserData(Time_Handle_Id TimerHandle, void *pDest, uint8_t SizeOfBuffer);
void 				Timer_SendMessage(Time_Handle_Id TimerHandle , Struct_Timer_Message *pMsg);
void 				Timer_SendMessageNoPara(Time_Handle_Id TimerHandle, uint16_t MsgId);
uint8_t 				Timer_Stop(Time_Handle_Id TimerHandle);
uint8_t 				Timer_Start(Time_Handle_Id TimerHandle);
uint8_t 				Timer_Restart(Time_Handle_Id TimerHandle ,uint16_t tMs);
uint16_t 				Timer_GetCountMs(Time_Handle_Id TimerHandle);
uint8_t 				Timer_SetCallbackFunc(Time_Handle_Id TimerHandle , void (*foo)(Struct_Timer_Message *pMsg) );

#endif
