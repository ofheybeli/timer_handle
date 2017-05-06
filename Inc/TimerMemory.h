#ifndef TimerMemory_H_INCLUDE
#define TimerMemory_H_INCLUDE


typedef struct{
	uint8_t 		 	FlgMemoryEnabled; 				//Haf�za aktif edilmi� bilgisini tutar
	uint16_t	 		TimerHandleCount;				  //�retilen Timer say�s�n� tutar
	uint8_t 			*pMemoryBaseAdress; 			//Ba�lang�� Adresi
	uint8_t 			*pMemoryEndAdress; 				//Son Adres
	uint8_t 			*pMemoryExNumBytesAdress; //Son Adres
	uint8_t 			*pMemoryUsageAdress; 			//Ba�lang��tan �imdiye kadar kullan�lan Alan�n adresi	
}Struct_Timer_Memory;

uint16_t Timer_GetSizeMemory		(void);
uint16_t Timer_GetUsageMemory		(void);
uint16_t Timer_GetFreeMemory		(void);



#endif
