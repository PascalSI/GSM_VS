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

MB_Master MB_Master_List[] = {};

//	InField MenuGSM_Modem_State[]	= { {Enum, &GSM_Modem, GSM_ModemList, 0}, {Byte, &GSM_State, NULL, 0} };

// ~~~~~~~~~~~~~~~
	MenuPage MenuModem = {SetMenu(ListGSM), 0, &Menu0,
};

	MenuLine Menu0List[] = {
		{" -GSM-сигнализатор- ", .InnPage=&Menu0},
		{" Служебное          ", .InnPage=&MenuSyst},
		{" Модем              ", .InnPage=&MenuModem},
	};
	MenuPage Menu0 = {SetMenu(Menu0List), 0, &Menu0,
};



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// ~~~~~~~~~~~
void
ApplInit(void)
{
	

}

// ~~~~~~~~~~~~
void
ApplCycle(void)
{

}
// ~~~~~~~~~~~
void
StartKey(void)
{	
	GSM_StartCalling(0,12);
}

// ~~~~~~~~~~
void
StopKey(void)
{
	
}
