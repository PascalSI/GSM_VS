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


InField Channel6_Name_InField[]			= { {Byte, &Channels[6].number, NULL, 0} };
InField Channel6_DI_State_InField[]		= { {Byte, &Channels[6].DI_State, NULL, 0} };
InField Channel6_Alarm_State_InField[]	= { {Byte, &Channels[6].Alarm_State, NULL, 0} };
OutField Channel6_debounceTimeout_OutField[]		= { {EE_Word, &(debounceTimeout[6]), 0, 65535, NULL, NULL, 0} };
OutField Channel6_calling_delay_OutField[]			= { {EE_Word, &calling_delay[6], 0, 65535, NULL, NULL, 0} };
OutField Channel6_calling_retry_delay_OutField[]	= { {EE_Word, &calling_retry_delay[6], 0, 65535, NULL, NULL, 0} };

MenuLine Channel6[] = {
		{" Номер канала    {{{", Channel6_Name_InField, 				NULL,	NULL},
		{" Состояние входа {{{", Channel6_DI_State_InField, 			NULL,	NULL},
		{" Состояние канала{{{", Channel6_Alarm_State_InField, 			NULL,	NULL},
		{" Тантидребезг  }}}}}", NULL,									Channel6_debounceTimeout_OutField,	NULL},
		{" Тдозвона      }}}}}", NULL,									Channel6_calling_delay_OutField,	NULL},
		{" Тповт.дозвона }}}}}", NULL,									Channel6_calling_retry_delay_OutField,		NULL},
		
};
MenuPage Channel6_Page = {SetMenu(Channel6), 0, NULL};


InField Channel7_Name_InField[]			= { {Byte, &Channels[7].number, NULL, 0} };
InField Channel7_DI_State_InField[]		= { {Byte, &Channels[7].DI_State, NULL, 0} };
InField Channel7_Alarm_State_InField[]	= { {Byte, &Channels[7].Alarm_State, NULL, 0} };
OutField Channel7_debounceTimeout_OutField[]		= { {EE_Word, &(debounceTimeout[7]), 0, 65535, NULL, NULL, 0} };
OutField Channel7_calling_delay_OutField[]			= { {EE_Word, &calling_delay[7], 0, 65535, NULL, NULL, 0} };
OutField Channel7_calling_retry_delay_OutField[]	= { {EE_Word, &calling_retry_delay[7], 0, 65535, NULL, NULL, 0} };

MenuLine Channel7[] = {
		{" Номер канала    {{{", Channel7_Name_InField, 				NULL,	NULL},
		{" Состояние входа {{{", Channel7_DI_State_InField, 			NULL,	NULL},
		{" Состояние канала{{{", Channel7_Alarm_State_InField, 			NULL,	NULL},
		{" Тантидребезг  }}}}}", NULL,									Channel7_debounceTimeout_OutField,	NULL},
		{" Тдозвона      }}}}}", NULL,									Channel7_calling_delay_OutField,	NULL},
		{" Тповт.дозвона }}}}}", NULL,									Channel7_calling_retry_delay_OutField,		NULL},
		
};
MenuPage Channel7_Page = {SetMenu(Channel7), 0, NULL};


InField Channel8_Name_InField[]			= { {Byte, &Channels[8].number, NULL, 0} };
InField Channel8_DI_State_InField[]		= { {Byte, &Channels[8].DI_State, NULL, 0} };
InField Channel8_Alarm_State_InField[]	= { {Byte, &Channels[8].Alarm_State, NULL, 0} };
OutField Channel8_debounceTimeout_OutField[]		= { {EE_Word, &(debounceTimeout[8]), 0, 65535, NULL, NULL, 0} };
OutField Channel8_calling_delay_OutField[]			= { {EE_Word, &calling_delay[8], 0, 65535, NULL, NULL, 0} };
OutField Channel8_calling_retry_delay_OutField[]	= { {EE_Word, &calling_retry_delay[8], 0, 65535, NULL, NULL, 0} };

MenuLine Channel8[] = {
		{" Номер канала    {{{", Channel8_Name_InField, 				NULL,	NULL},
		{" Состояние входа {{{", Channel8_DI_State_InField, 			NULL,	NULL},
		{" Состояние канала{{{", Channel8_Alarm_State_InField, 			NULL,	NULL},
		{" Тантидребезг  }}}}}", NULL,									Channel8_debounceTimeout_OutField,	NULL},
		{" Тдозвона      }}}}}", NULL,									Channel8_calling_delay_OutField,	NULL},
		{" Тповт.дозвона }}}}}", NULL,									Channel8_calling_retry_delay_OutField,		NULL},
		
};
MenuPage Channel8_Page = {SetMenu(Channel8), 0, NULL};


InField Channel9_Name_InField[]			= { {Byte, &Channels[9].number, NULL, 0} };
InField Channel9_DI_State_InField[]		= { {Byte, &Channels[9].DI_State, NULL, 0} };
InField Channel9_Alarm_State_InField[]	= { {Byte, &Channels[9].Alarm_State, NULL, 0} };
OutField Channel9_debounceTimeout_OutField[]		= { {EE_Word, &(debounceTimeout[9]), 0, 65535, NULL, NULL, 0} };
OutField Channel9_calling_delay_OutField[]			= { {EE_Word, &calling_delay[9], 0, 65535, NULL, NULL, 0} };
OutField Channel9_calling_retry_delay_OutField[]	= { {EE_Word, &calling_retry_delay[9], 0, 65535, NULL, NULL, 0} };

MenuLine Channel9[] = {
		{" Номер канала    {{{", Channel9_Name_InField, 				NULL,	NULL},
		{" Состояние входа {{{", Channel9_DI_State_InField, 			NULL,	NULL},
		{" Состояние канала{{{", Channel9_Alarm_State_InField, 			NULL,	NULL},
		{" Тантидребезг  }}}}}", NULL,									Channel9_debounceTimeout_OutField,	NULL},
		{" Тдозвона      }}}}}", NULL,									Channel9_calling_delay_OutField,	NULL},
		{" Тповт.дозвона }}}}}", NULL,									Channel9_calling_retry_delay_OutField,		NULL},
		
};
MenuPage Channel9_Page = {SetMenu(Channel9), 0, NULL};


InField Channel10_Name_InField[]			= { {Byte, &Channels[10].number, NULL, 0} };
InField Channel10_DI_State_InField[]		= { {Byte, &Channels[10].DI_State, NULL, 0} };
InField Channel10_Alarm_State_InField[]	= { {Byte, &Channels[10].Alarm_State, NULL, 0} };
OutField Channel10_debounceTimeout_OutField[]		= { {EE_Word, &(debounceTimeout[10]), 0, 65535, NULL, NULL, 0} };
OutField Channel10_calling_delay_OutField[]			= { {EE_Word, &calling_delay[10], 0, 65535, NULL, NULL, 0} };
OutField Channel10_calling_retry_delay_OutField[]	= { {EE_Word, &calling_retry_delay[10], 0, 65535, NULL, NULL, 0} };

MenuLine Channel10[] = {
		{" Номер канала    {{{", Channel10_Name_InField, 				NULL,	NULL},
		{" Состояние входа {{{", Channel10_DI_State_InField, 			NULL,	NULL},
		{" Состояние канала{{{", Channel10_Alarm_State_InField, 		NULL,	NULL},
		{" Тантидребезг  }}}}}", NULL,									Channel10_debounceTimeout_OutField,	NULL},
		{" Тдозвона      }}}}}", NULL,									Channel10_calling_delay_OutField,	NULL},
		{" Тповт.дозвона }}}}}", NULL,									Channel10_calling_retry_delay_OutField,		NULL},
		
};
MenuPage Channel10_Page = {SetMenu(Channel10), 0, NULL};


InField Channel11_Name_InField[]			= { {Byte, &Channels[11].number, NULL, 0} };
InField Channel11_DI_State_InField[]		= { {Byte, &Channels[11].DI_State, NULL, 0} };
InField Channel11_Alarm_State_InField[]	= { {Byte, &Channels[11].Alarm_State, NULL, 0} };
OutField Channel11_debounceTimeout_OutField[]		= { {EE_Word, &(debounceTimeout[11]), 0, 65535, NULL, NULL, 0} };
OutField Channel11_calling_delay_OutField[]			= { {EE_Word, &calling_delay[11], 0, 65535, NULL, NULL, 0} };
OutField Channel11_calling_retry_delay_OutField[]	= { {EE_Word, &calling_retry_delay[11], 0, 65535, NULL, NULL, 0} };

MenuLine Channel11[] = {
		{" Номер канала    {{{", Channel11_Name_InField, 				NULL,	NULL},
		{" Состояние входа {{{", Channel11_DI_State_InField, 			NULL,	NULL},
		{" Состояние канала{{{", Channel11_Alarm_State_InField, 			NULL,	NULL},
		{" Тантидребезг  }}}}}", NULL,									Channel11_debounceTimeout_OutField,	NULL},
		{" Тдозвона      }}}}}", NULL,									Channel11_calling_delay_OutField,	NULL},
		{" Тповт.дозвона }}}}}", NULL,									Channel11_calling_retry_delay_OutField,		NULL},
		
};
MenuPage Channel11_Page = {SetMenu(Channel11), 0, NULL};


InField Channel12_Name_InField[]			= { {Byte, &Channels[12].number, NULL, 0} };
InField Channel12_DI_State_InField[]		= { {Byte, &Channels[12].DI_State, NULL, 0} };
InField Channel12_Alarm_State_InField[]		= { {Byte, &Channels[12].Alarm_State, NULL, 0} };
OutField Channel12_debounceTimeout_OutField[]		= { {EE_Word, &(debounceTimeout[12]), 0, 65535, NULL, NULL, 0} };
OutField Channel12_calling_delay_OutField[]			= { {EE_Word, &calling_delay[12], 0, 65535, NULL, NULL, 0} };
OutField Channel12_calling_retry_delay_OutField[]	= { {EE_Word, &calling_retry_delay[12], 0, 65535, NULL, NULL, 0} };

MenuLine Channel12[] = {
		{" Номер канала    {{{", Channel12_Name_InField, 				NULL,	NULL},
		{" Состояние входа {{{", Channel12_DI_State_InField, 			NULL,	NULL},
		{" Состояние канала{{{", Channel12_Alarm_State_InField, 			NULL,	NULL},
		{" Тантидребезг  }}}}}", NULL,									Channel12_debounceTimeout_OutField,	NULL},
		{" Тдозвона      }}}}}", NULL,									Channel12_calling_delay_OutField,	NULL},
		{" Тповт.дозвона }}}}}", NULL,									Channel12_calling_retry_delay_OutField,		NULL},
		
};
MenuPage Channel12_Page = {SetMenu(Channel12), 0, NULL};


InField Channel13_Name_InField[]			= { {Byte, &Channels[13].number, NULL, 0} };
InField Channel13_DI_State_InField[]		= { {Byte, &Channels[13].DI_State, NULL, 0} };
InField Channel13_Alarm_State_InField[]		= { {Byte, &Channels[13].Alarm_State, NULL, 0} };
OutField Channel13_debounceTimeout_OutField[]		= { {EE_Word, &(debounceTimeout[13]), 0, 65535, NULL, NULL, 0} };
OutField Channel13_calling_delay_OutField[]			= { {EE_Word, &calling_delay[13], 0, 65535, NULL, NULL, 0} };
OutField Channel13_calling_retry_delay_OutField[]	= { {EE_Word, &calling_retry_delay[13], 0, 65535, NULL, NULL, 0} };

MenuLine Channel13[] = {
		{" Номер канала    {{{", Channel13_Name_InField, 				NULL,	NULL},
		{" Состояние входа {{{", Channel13_DI_State_InField, 			NULL,	NULL},
		{" Состояние канала{{{", Channel13_Alarm_State_InField, 			NULL,	NULL},
		{" Тантидребезг  }}}}}", NULL,									Channel13_debounceTimeout_OutField,	NULL},
		{" Тдозвона      }}}}}", NULL,									Channel13_calling_delay_OutField,	NULL},
		{" Тповт.дозвона }}}}}", NULL,									Channel13_calling_retry_delay_OutField,		NULL},
		
};
MenuPage Channel13_Page = {SetMenu(Channel13), 0, NULL};


InField Channel14_Name_InField[]			= { {Byte, &Channels[14].number, NULL, 0} };
InField Channel14_DI_State_InField[]		= { {Byte, &Channels[14].DI_State, NULL, 0} };
InField Channel14_Alarm_State_InField[]		= { {Byte, &Channels[14].Alarm_State, NULL, 0} };
OutField Channel14_debounceTimeout_OutField[]		= { {EE_Word, &(debounceTimeout[14]), 0, 65535, NULL, NULL, 0} };
OutField Channel14_calling_delay_OutField[]			= { {EE_Word, &calling_delay[14], 0, 65535, NULL, NULL, 0} };
OutField Channel14_calling_retry_delay_OutField[]	= { {EE_Word, &calling_retry_delay[14], 0, 65535, NULL, NULL, 0} };

MenuLine Channel14[] = {
		{" Номер канала    {{{", Channel14_Name_InField, 				NULL,	NULL},
		{" Состояние входа {{{", Channel14_DI_State_InField, 			NULL,	NULL},
		{" Состояние канала{{{", Channel14_Alarm_State_InField, 			NULL,	NULL},
		{" Тантидребезг  }}}}}", NULL,									Channel14_debounceTimeout_OutField,	NULL},
		{" Тдозвона      }}}}}", NULL,									Channel14_calling_delay_OutField,	NULL},
		{" Тповт.дозвона }}}}}", NULL,									Channel14_calling_retry_delay_OutField,		NULL},
		
};
MenuPage Channel14_Page = {SetMenu(Channel14), 0, NULL};


InField Channel15_Name_InField[]			= { {Byte, &Channels[15].number, NULL, 0} };
InField Channel15_DI_State_InField[]		= { {Byte, &Channels[15].DI_State, NULL, 0} };
InField Channel15_Alarm_State_InField[]		= { {Byte, &Channels[15].Alarm_State, NULL, 0} };
OutField Channel15_debounceTimeout_OutField[]		= { {EE_Word, &(debounceTimeout[15]), 0, 65535, NULL, NULL, 0} };
OutField Channel15_calling_delay_OutField[]			= { {EE_Word, &calling_delay[15], 0, 65535, NULL, NULL, 0} };
OutField Channel15_calling_retry_delay_OutField[]	= { {EE_Word, &calling_retry_delay[15], 0, 65535, NULL, NULL, 0} };

MenuLine Channel15[] = {
		{" Номер канала    {{{", Channel15_Name_InField, 				NULL,	NULL},
		{" Состояние входа {{{", Channel15_DI_State_InField, 			NULL,	NULL},
		{" Состояние канала{{{", Channel15_Alarm_State_InField, 			NULL,	NULL},
		{" Тантидребезг  }}}}}", NULL,									Channel15_debounceTimeout_OutField,	NULL},
		{" Тдозвона      }}}}}", NULL,									Channel15_calling_delay_OutField,	NULL},
		{" Тповт.дозвона }}}}}", NULL,									Channel15_calling_retry_delay_OutField,		NULL},
		
};
MenuPage Channel15_Page = {SetMenu(Channel15), 0, NULL};

MenuLine ChannelSettingsList[] = {
		{" Канал 1            ", .InnPage=&Channel0_Page},
		{" Канал 2            ", .InnPage=&Channel1_Page},
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
		{" Канал 16           ", .InnPage=&Channel15_Page},
	};
	MenuPage ChannelSettingsMenu = {SetMenu(ChannelSettingsList), 0, &Menu0,};


	MenuPage MenuModem = {SetMenu(ListGSM), 0, &Menu0,};
	InField Field_TransmitterState[]			= { {Byte, &Channels[0].DI_State, NULL, 0} };
	OutField CallNum1_OutField[] = { {EE_Text, &CALL_Number[0], DIGIT, UKR_CAPS, NULL, NULL, 0} };
	OutField CallNum2_OutField[] = { {EE_Text, &CALL_Number[1], DIGIT, UKR_CAPS, NULL, NULL, 0} };
	OutField CallNum3_OutField[] = { {EE_Text, &CALL_Number[2], DIGIT, UKR_CAPS, NULL, NULL, 0} };

	MenuLine PhoneNums[] = {
		{" -Номера телефонов- ", .InnPage=&Menu0},
		{" N1:+}}}}}}}}}}}}   ", NULL, CallNum1_OutField, NULL},
		{" N2:+}}}}}}}}}}}}   ", NULL, CallNum2_OutField, NULL},
		{" N3:+}}}}}}}}}}}}   ", NULL, CallNum3_OutField, NULL},
	};
	MenuPage PhoneNums_Page = {SetMenu(PhoneNums), 0, NULL};



	MenuLine Menu0List[] = {
		{" -GSM-сигнализатор- ", .InnPage=&Menu0},
		{" Служебное          ", .InnPage=&MenuSyst},
		{" Модем              ", .InnPage=&MenuModem},
		{" Номера телефонов   ", .InnPage=&PhoneNums_Page},
		{" Настройка каналов  ", .InnPage=&ChannelSettingsMenu},
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

void WriteDigOut(uint8_t DO_Num, uint8_t DO_Level)
{
	if(1 == DO_Level){
		SetDigOut(DO_Num);
	}else
	{
		ResDigOut(DO_Num);	
	}
}

void ModbusAppCycle(){
	MB_Input[0] = (DI_State(0)<<0)|(DI_State(1)<<1)|(DI_State(2)<<2)|(DI_State(3)<<3)|(DI_State(4)<<4)|(DI_State(5)<<5)|(DI_State(6)<<6)|(DI_State(7)<<7);
	MB_Input[1] = (DI_State(8)<<0)|(DI_State(9)<<1)|(DI_State(10)<<2)|(DI_State(11)<<3)|(DI_State(12)<<4)|(DI_State(13)<<5)|(DI_State(14)<<6)|(DI_State(15)<<7);
	WriteDigOut(0, (MB_Coil[0])&1);
	WriteDigOut(1, ((MB_Coil[0])&(1<<1))>>1);
	WriteDigOut(2, ((MB_Coil[0])&(1<<2))>>2);
	WriteDigOut(3, ((MB_Coil[0])&(1<<3))>>3);
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
	ModbusAppCycle();
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
