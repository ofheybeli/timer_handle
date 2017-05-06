#ifndef TimerMemory_H_INCLUDE
#define TimerMemory_H_INCLUDE


typedef struct{
	uint8_t 		 	FlgMemoryEnabled; 				//Hafýza aktif edilmiþ bilgisini tutar
	uint16_t	 		TimerHandleCount;				  //Üretilen Timer sayýsýný tutar
	uint8_t 			*pMemoryBaseAdress; 			//Baþlangýç Adresi
	uint8_t 			*pMemoryEndAdress; 				//Son Adres
	uint8_t 			*pMemoryExNumBytesAdress; //Son Adres
	uint8_t 			*pMemoryUsageAdress; 			//Baþlangýçtan þimdiye kadar kullanýlan Alanýn adresi	
}Struct_Timer_Memory;

uint16_t Timer_GetSizeMemory		(void);
uint16_t Timer_GetUsageMemory		(void);
uint16_t Timer_GetFreeMemory		(void);



#endif
