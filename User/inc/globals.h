//----------------------------------------------------------------------------//
#ifndef INCGLOBALS_H
#define INCGLOBALS_H
//----------------------------------------------------------------------------//
#include "common_types.h"
#include <stdbool.h>
#include "HAL.h"
#include "MD5.h"
//----------------------------------------------------------------------------//
typedef struct{	//Счетчики
	u32 operatingTime;				//Время наработки
	u32 lastSessionOperatingTime;	//Время наработки последней сессии(периода)
	u32 starts;						//Cчетчик стартов программы
	u32 btn1;	//счетчики коротких нажатий кнопок(для цеха)
	u32 btn2;
}counters_t;
//----------------------------------------------------------------------------//
typedef struct{
	u16 MCU;	//Напряжение МК (+3.5 В)
	u16 Batt;	//Напряжение батарейки (до +3 В)
	u16 USB;
}voltage_t;
//----------------------------------------------------------------------------//
typedef struct{
	const char* firmwareVersion;	//Версия ПО
	const char* modelIdDevice;		//Модель устройства (РиМ 071.45)
}str_t;
//----------------------------------------------------------------------------//
typedef struct{
	bool powerOK;			//Сигнал PowerGood
	bool powerGoodInt;		//Флаг прерывания 
	bool powerBad;			//Пропало питание
	bool reboot;			//Флаг перезагрузки
	bool serviceMode;		//Сервисный режим
	bool logOn;				//Выдача лога
}flags_t;
//----------------------------------------------------------------------------//
typedef struct{
	u32 start;	//Начальный адрес
	u32 end;	//Конечный адрес
}range_t;
//----------------------------------------------------------------------------//
typedef struct{
	char mainProgramm[MD5_STR_LEN];	//MD5 основной программы
	char bootLoader[MD5_STR_LEN];	//MD5 загрузчика
	char userRange[MD5_STR_LEN];	//MD5 по пользовательскому диапазону
	range_t range;					//Диапазон
}md5_t;
//----------------------------------------------------------------------------//
typedef enum{
	RR_NONE,
	RR_POR,
	RR_IWDG,
	RR_HARDFAULT,
	RR_SOFTWARE,
	RR_LOWPOWER,
	RR_RSTPIN,
}rebootReason_t;
//----------------------------------------------------------------------------//
typedef enum{
	IMAGE_TRANSFER_NOT_INITIATED			= 0,
	IMAGE_TRANSFER_INITIATED				= 1,
	IMAGE_TRANSFER_VERIFICATION_INITIATED	= 2,
	IMAGE_TRANSFER_VERIFICATION_SUCCESSFUL	= 3,
	IMAGE_TRANSFER_VERIFICATION_FAILED		= 4,
	IMAGE_TRANSFER_ACTIVATEION_INITIATED	= 5,
	IMAGE_TRANSFER_ACTIVATEION_SUCCESSFUL	= 6,
	IMAGE_TRANSFER_ACTIVATEION_FAILED		= 7,
}imageTransferStatus_t;
//----------------------------------------------------------------------------//
typedef struct{
	u32 blockSize;
	u8 transferredBlockStatus[112];
	imageTransferStatus_t transferStatus;
	bool transferEnabled;
	u16 fStartVerified;	//Флаг запуска верификации(по сути номер блока)
	char calcMD5[MD5_STR_LEN];
	u32 firmareID;
}image_t;
//----------------------------------------------------------------------------//
typedef struct{
	u32 logOn;
	u32 serviceModeOn;
	u32 forcedReboot;
}timeouts_t;
//----------------------------------------------------------------------------//
typedef enum{
	JANUARY		= 1,
	FEBRUARY	= 2,
	MARCH		= 3,
	APRIL		= 4,
	MAY			= 5,
	JUNE		= 6,
	JULY		= 7,
	AUGUST		= 8,
	SEPTEMBER	= 9,
	OCTOBER		= 10,
	NOVEMBER	= 11,
	DECEMBER	= 12,
}month_t;
//----------------------------------------------------------------------------//
typedef enum{
	MONDAY		= 1,
	TUESDAY		= 2,
	WEDNESDAY	= 3,
	THURSDAY	= 4,
	FRIDAY		= 5,
	SATURDAY	= 6,
	SUNDAY		= 7,
}dayWeek_t;
//----------------------------------------------------------------------------//
typedef struct{
	u8 yearHiByte;
	u8 yearLowByte;
	month_t month;
	u8 dayOfMonth;
	dayWeek_t dayOfWeek;
}date_t;
//----------------------------------------------------------------------------//
typedef enum{
	V_BATT_STATUS_NONE,
	V_BATT_STATUS_FULL,
	V_BATT_STATUS_MID,
	V_BATT_STATUS_LOW,
	V_BATT_STATUS_LOWER,
}vBattStatus_t;
//----------------------------------------------------------------------------//
typedef enum{
	MODE_NONE,
	MODE_OFF,
	MODE_ON,
}mode_t;
//----------------------------------------------------------------------------//
typedef struct{
	vBattStatus_t vBatt;				//Статус напряжения батареи для отображения значка
	bool usbPwr;			//Питание от USB
	mode_t manufactoryMode;	//Режим калибровки(настройки)
	bool lcdDiag;
}Status_t;
//----------------------------------------------------------------------------//
typedef enum{
	BLE_STATE_DISCONNECT,
	BLE_STATE_SCAN,
	BLE_STATE_CONNECT,
}BLE_State_t;
//----------------------------------------------------------------------------//
typedef struct{
	u32 serialNumber;			//Серийный номер
	u32 numMeter;				//Серийный номер счетчика
	char numMeterStr[9];		//Серийный номер счетчика строкой
	u32 pinCode;				//Пинкод
	date_t dateOfProduction;	//Дата производства
	const u32*  firmwareID;		//идентификатор ПО, для удаленного обновления
	str_t str;					//Структура с указателями на строковые константы
	timeouts_t timeout;
	image_t image;				//Для удаленного обновления
	md5_t MD5;					//Hash-суммы
	counters_t cnt;				//Счетчики

	flags_t flag;				//Флаги
	rebootReason_t lastRebootReason;	//Причина последней перезргрузки
	//Измеряемые величины
	voltage_t Vcc;		//Все напряжения
	float temperature;	//Температура МК
	u16 pageNumber;
	
	Status_t status;
	BLE_State_t	BLE_State;
	char MAC_Address[18];
	char password[16];	//Пароль
}systemInfo_t;
//----------------------------------------------------------------------------//
extern systemInfo_t SystemInfo;
//----------------------------------------------------------------------------//
#endif
