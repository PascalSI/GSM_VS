//#define BOOTLOADER_SUPPORT
#include "plc_type.h"
#include <stdint.h>

//#define vmd2_1
//#define vmd2_2
#define vmd2_3

//#define HIGH_SPEED_COUNTER	//Объявить до #define sys_plc

#define sys_plc "sys_vm2.h"

#define ADC_Ref_Chan

#define InitThermType {TSM100,TSM100,TSM100,TSM100,TSM100,TSM100};

//#define NOT_USE_LCD_RETURN_HOME_CMD	//для ЖКИ Fordata

// ~~~~~~~~~~~~~~~
/*#define DataFlash_Mbit 16
#define DataFlash_Size DataFlash_Mbit*131072
#define DataFlash_PageSize 512				//размер буфера микросхемы DataFlash может быть 256,512,1024,2048 байт
// ~~~~~~~~~~~~~~~
*/

uint8_t		MB_Coil[4];
uint8_t		MB_Input[4];
uint16_t	MB_HoldReg[4];
uint16_t	MB_InReg[80];

#define Modbus_Map_List {				\
	{									\
		MB_Coil,		040,			\
		MB_Input,		040,			\
		MB_HoldReg,		4,				\
		MB_InReg,		80				\
	},									\
}

#define Modbus_Qt 1
#define MB_Port_List {Port(1) }
#define Init_Modbus_Param { {EvenParity, 1, MBBR19200} }
#define MB_Role_List {Role_Slave }
//#define Init_Modbus_M_Param {[1]={100, 20, 5, 1}}		// Period; Timeout, Try, TimeOff;
#define Init_MB_NativeAddr {1}

#define RTYP (MB_InReg+38)
#define RMON (MB_InReg+39)
#define RKEY (MB_Coil+3)

// ~~~~~~~~~~~~~~~
//#define UDP_EASY
// ~~~~~~~~~~~~~~~

// ~~~~~~~~~~~~~~~

#define InitMenu Menu0

// ~~~~~~~~~~~~~~~
// Divisible by 8!
#define Timer8ApplManuNumber	040
#define Timer16ApplManuNumber	060
#define Timer32ApplManuNumber	020
#define Timer8ApplNumber	(Timer8ApplManuNumber  + 040)
#define Timer16ApplNumber	(Timer16ApplManuNumber + 030)
#define Timer32ApplNumber	(Timer32ApplManuNumber + 020)

#define GSM "gsm_Alarm_system.h"

#include "system.h"
static uint8_t Caller_state = 0;


#define DEBOUNCE_TIMEOUT_INIT 	 { 2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2		}
#define CALLING_DELAY_INIT 		 { 5, 	5, 	 5,   5,   5, 	5, 	 5,   5,   5,   5,   5,   5,   5,   5,   5,   5		}
#define CALLING_RETRY_DELAY_INIT { 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300	}

uint16_t debounceTimeout[16] 		EEMEM = DEBOUNCE_TIMEOUT_INIT;
uint16_t calling_delay[16] 			EEMEM = CALLING_DELAY_INIT;
uint16_t calling_retry_delay[16] 	EEMEM = CALLING_RETRY_DELAY_INIT;

typedef struct{
	uint8_t number;
	uint8_t DI_State;
	uint8_t Alarm_State;
	uint8_t timer;
}Channel;
// ~~~~~~~~~~~

Channel Channels[16];

MB_Master MB_Master_List[] = {};

//	InField MenuGSM_Modem_State[]	= { {Enum, &GSM_Modem, GSM_ModemList, 0}, {Byte, &GSM_State, NULL, 0} };

// ~~~~~~~~~~~~~~~
InField Channel0_Name_InField[]			= { {Byte, &Channels[0].number, NULL, 0} };
InField Channel0_DI_State_InField[]		= { {Byte, &Channels[0].DI_State, NULL, 0} };
InField Channel0_Alarm_State_InField[]	= { {Byte, &Channels[0].Alarm_State, NULL, 0} };
OutField Channel0_debounceTimeout_OutField[]		= { {EE_Word, &(debounceTimeout[0]), 0, 65535, NULL, NULL, 0} };
OutField Channel0_calling_delay_OutField[]		= { {EE_Word, &calling_delay[0], 0, 65535, NULL, NULL, 0} };
OutField Channel0_calling_retry_delay_OutField[]	= { {EE_Word, &calling_retry_delay[0], 0, 65535, NULL, NULL, 0} };

MenuLine Channel0[] = {
		{" Номер канала    {{{", Channel0_Name_InField, 				NULL,	NULL},
		{" Состояние входа {{{", Channel0_DI_State_InField, 			NULL,	NULL},
		{" Состояние канала{{{", Channel0_Alarm_State_InField, 			NULL,	NULL},
		{" Тантидребезг  }}}}}", NULL,									Channel0_debounceTimeout_OutField,	NULL},
		{" Тдозвона      }}}}}", NULL,									Channel0_calling_delay_OutField,	NULL},
		{" Тповт.дозвона }}}}}", NULL,									Channel0_calling_retry_delay_OutField,		NULL},
		
};
MenuPage Channel0_Page = {SetMenu(Channel0), 0, NULL};

InField Channel1_Name_InField[]			= { {Byte, &Channels[1].number, NULL, 0} };
InField Channel1_DI_State_InField[]		= { {Byte, &Channels[1].DI_State, NULL, 0} };
InField Channel1_Alarm_State_InField[]	= { {Byte, &Channels[1].Alarm_State, NULL, 0} };
OutField Channel1_debounceTimeout_OutField[]		= { {EE_Word, &(debounceTimeout[1]), 0, 65535, NULL, NULL, 0} };
OutField Channel1_calling_delay_OutField[]			= { {EE_Word, &calling_delay[1], 0, 65535, NULL, NULL, 0} };
OutField Channel1_calling_retry_delay_OutField[]	= { {EE_Word, &calling_retry_delay[1], 0, 65535, NULL, NULL, 0} };

MenuLine Channel1[] = {
		{" Номер канала    {{{", Channel1_Name_InField, 				NULL,	NULL},
		{" Состояние входа {{{", Channel1_DI_State_InField, 			NULL,	NULL},
		{" Состояние канала{{{", Channel1_Alarm_State_InField, 			NULL,	NULL},
		{" Тантидребезг  }}}}}", NULL,									Channel1_debounceTimeout_OutField,	NULL},
		{" Тдозвона      }}}}}", NULL,									Channel1_calling_delay_OutField,	NULL},
		{" Тповт.дозвона }}}}}", NULL,									Channel1_calling_retry_delay_OutField,		NULL},
		
};
MenuPage Channel1_Page = {SetMenu(Channel1), 0, NULL};

InField Channel2_Name_InField[]			= { {Byte, &Channels[2].number, NULL, 0} };
InField Channel2_DI_State_InField[]		= { {Byte, &Channels[2].DI_State, NULL, 0} };
InField Channel2_Alarm_State_InField[]	= { {Byte, &Channels[2].Alarm_State, NULL, 0} };
OutField Channel2_debounceTimeout_OutField[]		= { {EE_Word, &(debounceTimeout[2]), 0, 65535, NULL, NULL, 0} };
OutField Channel2_calling_delay_OutField[]			= { {EE_Word, &calling_delay[2], 0, 65535, NULL, NULL, 0} };
OutField Channel2_calling_retry_delay_OutField[]	= { {EE_Word, &calling_retry_delay[2], 0, 65535, NULL, NULL, 0} };

MenuLine Channel2[] = {
		{" Номер канала    {{{", Channel2_Name_InField, 				NULL,	NULL},
		{" Состояние входа {{{", Channel2_DI_State_InField, 			NULL,	NULL},
		{" Состояние канала{{{", Channel2_Alarm_State_InField, 			NULL,	NULL},
		{" Тантидребезг  }}}}}", NULL,									Channel2_debounceTimeout_OutField,	NULL},
		{" Тдозвона      }}}}}", NULL,									Channel2_calling_delay_OutField,	NULL},
		{" Тповт.дозвона }}}}}", NULL,									Channel2_calling_retry_delay_OutField,		NULL},
		
};
MenuPage Channel2_Page = {SetMenu(Channel2), 0, NULL};

InField Channel3_Name_InField[]			= { {Byte, &Channels[3].number, NULL, 0} };
InField Channel3_DI_State_InField[]		= { {Byte, &Channels[3].DI_State, NULL, 0} };
InField Channel3_Alarm_State_InField[]	= { {Byte, &Channels[3].Alarm_State, NULL, 0} };
OutField Channel3_debounceTimeout_OutField[]		= { {EE_Word, &(debounceTimeout[3]), 0, 65535, NULL, NULL, 0} };
OutField Channel3_calling_delay_OutField[]			= { {EE_Word, &calling_delay[3], 0, 65535, NULL, NULL, 0} };
OutField Channel3_calling_retry_delay_OutField[]	= { {EE_Word, &calling_retry_delay[3], 0, 65535, NULL, NULL, 0} };

MenuLine Channel3[] = {
		{" Номер канала    {{{", Channel3_Name_InField, 				NULL,	NULL},
		{" Состояние входа {{{", Channel3_DI_State_InField, 			NULL,	NULL},
		{" Состояние канала{{{", Channel3_Alarm_State_InField, 			NULL,	NULL},
		{" Тантидребезг  }}}}}", NULL,									Channel3_debounceTimeout_OutField,	NULL},
		{" Тдозвона      }}}}}", NULL,									Channel3_calling_delay_OutField,	NULL},
		{" Тповт.дозвона }}}}}", NULL,									Channel3_calling_retry_delay_OutField,		NULL},
		
};
MenuPage Channel3_Page = {SetMenu(Channel3), 0, NULL};


InField Channel4_Name_InField[]			= { {Byte, &Channels[4].number, NULL, 0} };
InField Channel4_DI_State_InField[]		= { {Byte, &Channels[4].DI_State, NULL, 0} };
InField Channel4_Alarm_State_InField[]	= { {Byte, &Channels[4].Alarm_State, NULL, 0} };
OutField Channel4_debounceTimeout_OutField[]		= { {EE_Word, &(debounceTimeout[4]), 0, 65535, NULL, NULL, 0} };
OutField Channel4_calling_delay_OutField[]			= { {EE_Word, &calling_delay[4], 0, 65535, NULL, NULL, 0} };
OutField Channel4_calling_retry_delay_OutField[]	= { {EE_Word, &calling_retry_delay[4], 0, 65535, NULL, NULL, 0} };

MenuLine Channel4[] = {
		{" Номер канала    {{{", Channel4_Name_InField, 				NULL,	NULL},
		{" Состояние входа {{{", Channel4_DI_State_InField, 			NULL,	NULL},
		{" Состояние канала{{{", Channel4_Alarm_State_InField, 			NULL,	NULL},
		{" Тантидребезг  }}}}}", NULL,									Channel4_debounceTimeout_OutField,	NULL},
		{" Тдозвона      }}}}}", NULL,									Channel4_calling_delay_OutField,	NULL},
		{" Тповт.дозвона }}}}}", NULL,									Channel4_calling_retry_delay_OutField,		NULL},
		
};
MenuPage Channel4_Page = {SetMenu(Channel4), 0, NULL};

InField Channel5_Name_InField[]			= { {Byte, &Channels[5].number, NULL, 0} };
InField Channel5_DI_State_InField[]		= { {Byte, &Channels[5].DI_State, NULL, 0} };
InField Channel5_Alarm_State_InField[]	= { {Byte, &Channels[5].Alarm_State, NULL, 0} };
OutField Channel5_debounceTimeout_OutField[]		= { {EE_Word, &(debounceTimeout[5]), 0, 65535, NULL, NULL, 0} };
OutField Channel5_calling_delay_OutField[]			= { {EE_Word, &calling_delay[5], 0, 65535, NULL, NULL, 0} };
OutField Channel5_calling_retry_delay_OutField[]	= { {EE_Word, &calling_retry_delay[5], 0, 65535, NULL, NULL, 0} };

MenuLine Channel5[] = {
		{" Номер канала    {{{", Channel5_Name_InField, 				NULL,	NULL},
		{" Состояние входа {{{", Channel5_DI_State_InField, 			NULL,	NULL},
		{" Состояние канала{{{", Channel5_Alarm_State_InField, 			NULL,	NULL},
		{" Тантидребезг  }}}}}", NULL,									Channel5_debounceTimeout_OutField,	NULL},
		{" Тдозвона      }}}}}", NULL,									Channel5_calling_delay_OutField,	NULL},
		{" Тповт.дозвона }}}}}", NULL,									Channel5_calling_retry_delay_OutField,		NULL},
		
};
MenuPage Channel5_Page = {SetMenu(Channel5), 0, NULL};


MenuLine ChannelSettingsList[] = {
		{" Канал 1            ", .InnPage=&Channel0_Page},
/*		{" Канал 2            ", .InnPage=&Channel1_Page},
		{" Канал 3            ", .InnPage=&Channel2_Page},
		{" Канал 4            ", .InnPage=&Channel3_Page},
		{" Канал 5            ", .InnPage=&Channel4_Page},
		{" Канал 6            ", .InnPage=&Channel5_Page},
		{" Канал 7            ", .InnPage=&Channel6_Page},
		{" Канал 8            ", .InnPage=&Channel7_Page},
		{" Канал 9            ", .InnPage=&Channel8_Page},
		{" Канал 10           ", .InnPage=&Channel9_Page},
		{" Канал 11           ", .InnPage=&Channel10_Page},
		{" Канал 12           ", .InnPage=&Channel11_Page},
		{" Канал 13           ", .InnPage=&Channel12_Page},
		{" Канал 14           ", .InnPage=&Channel13_Page},
		{" Канал 15           ", .InnPage=&Channel14_Page},
		{" Канал 16           ", .InnPage=&Channel15_Page},*/
	};
//	MenuPage Menu0 = {SetMenu(Menu0List), 0, &Menu0,


	MenuPage MenuModem = {SetMenu(ListGSM), 0, &Menu0,};
	InField Field_TransmitterState[]			= { {Byte, &Channels[0].DI_State, NULL, 0} };

	MenuLine Menu0List[] = {
		{" -GSM-сигнализатор- ", .InnPage=&Menu0},
		{" Служебное          ", .InnPage=&MenuSyst},
		{" Модем              ", .InnPage=&MenuModem},
		{" Настройка каналов  ", .InnPage=&Channel0_Page},
		{" Сост.oбзвона    {{{",  Field_TransmitterState},
		
	};
	MenuPage Menu0 = {SetMenu(Menu0List), 0, &Menu0,
};



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
enum{
	CALLER_IDLE, CHECK_PHONE_BUSY, CALLER_ARBITR
};
uint8_t CallerState;
uint8_t CallerBusyState;
uint8_t CallerMessage[2];
uint8_t CurrentAbonent;
uint8_t CurrentCall;
void Caller_StartCalling(uint8_t inputN){
	CallerMessage[0]=1;
	CallerMessage[1]=inputN;
	CallerBusyState = 1;
}

void CallerCycle(){
	
	switch(Caller_state){
		case CALLER_IDLE:
			if(0 != CallerMessage[0]){
				CallerMessage[0] = 0;
				CurrentAbonent = 0;
				CurrentCall = 0;
				CallerBusyState = 1;
				Caller_state = CHECK_PHONE_BUSY;
				break;
			}
			CallerBusyState = 0;
			break;
		case CHECK_PHONE_BUSY:
			if(0 == TransmitterState){
				Caller_state = CALLER_ARBITR;
				break;
			}else 
			{
				if(1 == GSM_GetConfirmState()){
					Caller_state = CALLER_IDLE;
					break;
				}
			}
			break;
		case CALLER_ARBITR:
			GSM_StartCalling(CurrentAbonent,CallerMessage[1]);
			if(CurrentCall<2){
				CurrentCall++;
				Caller_state = CHECK_PHONE_BUSY;
				break;
			}
			else{
			 if(CurrentAbonent<MaxTelephDirSz-1){
			 	CurrentAbonent++;
				CurrentCall = 0;
				Caller_state = CHECK_PHONE_BUSY;
				break;
			 }
			 else{
			 	Caller_state = CALLER_IDLE;
				break;
			 }
			}			
			break;
	}
}

enum{
	CHANNEL_PINSCAN,        //0
	CHANNEL_WAIT_DEBOUNCE,	//1
	CHANNEL_WRITE_LOG,		//2
	CHANNEL_WAIT_CALL,		//3
	CHANNEL_CALLING,		//4
	CHANNEL_WAIT_RECALL		//5
};
void ChannelProcessing(uint8_t chNumber){
	switch(Channels[chNumber].Alarm_State){
		case CHANNEL_PINSCAN:
			if(0 == Channels[chNumber].DI_State)
			{
				Channels[chNumber].Alarm_State = CHANNEL_WAIT_DEBOUNCE;
				StartTimer16(Channels[chNumber].timer, erw(&debounceTimeout[chNumber])*100);
				break;
			}
			break;
		case CHANNEL_WAIT_DEBOUNCE:
			if(1 == Channels[chNumber].DI_State)
			{
				Channels[chNumber].Alarm_State = CHANNEL_PINSCAN;
				break;				
			} 
			if(Timer16Stopp(Channels[chNumber].timer)){
				Channels[chNumber].Alarm_State = CHANNEL_WRITE_LOG;
			}
			break;
		case CHANNEL_WRITE_LOG:
				Channels[chNumber].Alarm_State = CHANNEL_WAIT_CALL;
				StartTimer16(Channels[chNumber].timer, erw(&calling_delay[chNumber])*100);
			break;
		case CHANNEL_WAIT_CALL:
			if(1 == Channels[chNumber].DI_State)
			{
				Channels[chNumber].Alarm_State = CHANNEL_PINSCAN;
				break;				
			} 
			if(Timer16Stopp(Channels[chNumber].timer)){
				Channels[chNumber].Alarm_State = CHANNEL_CALLING;
			}
			break;
		case CHANNEL_CALLING:
			if(0 == CallerBusyState){
				Caller_StartCalling(Channels[chNumber].number);
				Channels[chNumber].Alarm_State = CHANNEL_WAIT_RECALL;
				StartTimer16(Channels[chNumber].timer, erw(&calling_retry_delay[chNumber])*100);
			}
			break;
		case CHANNEL_WAIT_RECALL:
			if(1 == Channels[chNumber].DI_State)
			{
				Channels[chNumber].Alarm_State = CHANNEL_PINSCAN;
				break;				
			} 
			if(Timer16Stopp(Channels[chNumber].timer)){
				Channels[chNumber].Alarm_State = CHANNEL_CALLING;
			}			
			break;

	}

}

void
ApplInit(void)
{
	for(int i=0; i<16; i++){
		Channels[i].number = i+1;
		Channels[i].timer = Timer16Alloc();
	}

}

// ~~~~~~~~~~~~
void
ApplCycle(void)
{
	for(uint8_t i=0; i<16; i++){
			Channels[i].DI_State = DI_State(i);
			ChannelProcessing(i);
	}
	CallerCycle();
}
// ~~~~~~~~~~~
void
StartKey(void)
{	
	
}

// ~~~~~~~~~~
void
StopKey(void)
{
	
}
