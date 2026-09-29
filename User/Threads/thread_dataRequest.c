//----------------------------------------------------------------------------//
#include "threads.h"
#include "dlmsListObj.h"
#include "utils.h"
#include <string.h>
#include <stdlib.h>
#include "messages.h"
#include "log.h"
#include "globals.h"
#include "HAL.h"
#include "calc_fsc.h"
#include "hdlc.h"
//----------------------------------------------------------------------------//
#define MAX_PDU_SIZE 4096
//----------------------------------------------------------------------------//
// Фиксированные байты заголовка DLMS (AARQ/AARE)
#define DLMS_HEADER_BYTE_0		0xE6
#define DLMS_HEADER_BYTE_1		0xE7
#define DLMS_HEADER_BYTE_2		0x00
#define DLMS_HEADER_BYTE_3		0xC4
#define DLMS_HEADER_BYTE_5		0x81

//Типы ответов (байт 4)
#define DLMS_RESP_TYPE_NORMAL	0x01	//Обычный ответ
#define DLMS_RESP_TYPE_BLOCK	0x02	//Блочный ответ

#define DLMS_OFFSET_TYPE			4	//Смещение байта типа ответа
#define DLMS_OFFSET_DATA_FLAG		6	//Смещение байта флага данных
#define DLMS_OFFSET_LAST_BLOCK_FLAG	6	//Смещение байта флага последнего блока
#define DLMS_OFFSET_BLOCK_NUM		7	// Смещение номера блока (4 байта)
#define DLMS_OFFSET_BLOCK_DATA_FLAG	11	// Смещение статуса блока
#define DLMS_OFFSET_LENGTH			12	// Смещение поля длины

// Статусы блока (байт 6 для блочного ответа)
#define DLMS_BLOCK_STATUS_NOT_LAST	0x00    // Блок корректен, но не последний
#define DLMS_BLOCK_STATUS_LAST		0x01    // Последний блок

// Флаги длины
#define DLMS_LEN_FLAG_EXTENDED		0x80	//Флаг расширенного поля длины
#define DLMS_LEN_MASK				0x7F	//sМаска для длины
//----------------------------------------------------------------------------//
pt_t		dataRequest_pt;
//----------------------------------------------------------------------------//
dlms_t dlms;
//----------------------------------------------------------------------------//
const u8 dlmsHeaderRelayConnect[5] = {0xC7, 0x01, 0x81, 0x00, 0x00};
//----------------------------------------------------------------------------//
bool read_relay_answer(u8* buff){	//Считываем ответ на включение реле
	if((0 == memcmp(buff, dlmsHeaderRelayConnect, sizeof(dlmsHeaderRelayConnect)))){	//если заголовок совпадает
		return true;
	}
	else	return false;	//Не правильный заголовок
}
//----------------------------------------------------------------------------//
const u8 dlmsHeaderReqDiagnosis[5] = {0xC4, 0x01, 0x81, 0x00, TAG_UINT32};
//----------------------------------------------------------------------------//
bool read_req_diagnosis(u8* buff){	//Считываем ответ на запрос диагностики
	if((0 == memcmp(buff, dlmsHeaderReqDiagnosis, sizeof(dlmsHeaderReqDiagnosis)))){	//если заголовок совпадает
		dlms.status.DiagLCD = (status_t)((buff[8] & 0x08) >> 3);
		return true;
	}
	else	return false;	//Не правильный заголовок
}
//----------------------------------------------------------------------------//
const u8 dlmsHeaderSetDiagnosis[4] = {0xC5, 0x01, 0x81, 0x00};
const u8 dlmsHeaderSetDiagnosisNoAccess[4] = {0xC5, 0x01, 0x81, 0x0D};	//Нет доступа, для классики
//----------------------------------------------------------------------------//
bool read_set_diagnosis(u8* buff){	//Считываем ответ на запись диагностики
	if((0 == memcmp(buff, dlmsHeaderSetDiagnosis, sizeof(dlmsHeaderSetDiagnosis)))){	//если заголовок совпадает
		return true;
	}
	else if((0 == memcmp(buff, dlmsHeaderSetDiagnosisNoAccess, sizeof(dlmsHeaderSetDiagnosisNoAccess)))){
		return true;
	}
	else	return false;	//Не правильный заголовок
}
//----------------------------------------------------------------------------//
const u8 dlmsHeaderInfoOrData[3] = {0x01, 0x01, 0x02};
//----------------------------------------------------------------------------//
//Тип данных | Значение		|	Описание

//	0F	(TAG_INT8)				02 				Статус RTC
//	16	(TAG_ENUM)				00 				Состояние реле
//	0F	(TAG_INT8)				B7 				Статус фаз
//	09	(TAG_OCTET_STRING)		01 01 			Текущий тариф
//	06	(TAG_UINT32)			11 4B 0B 00 	Статус пломб, ПКЭ, магнитное поле и прочее(см. ниже)

// TAG_UINT32:
// байты 0..3:
// байт 0: сборка бит - статус ПКЭ
// бит 0: состояние ПКЭ
// бит 4: флаг ПКЭ
// байт 1: сборка бит - статус пломб + версия ЖКИ
// бит 0: Tamper_Cover.State - замок 2
// бит 1: Tamper_Meter.State - замок 1
// бит 2: Tamper_Magnet.State
// бит 3: Tamper_Battery.State - замок 3
// бит 4: СВЧ
// бит 5: статус диагностики
// бит 6..7: версия ЖКИ (1..3)
// байт 2: сборка бит - флаги
// бит 0: Tamper_Cover.State - замок 2
// бит 1: Tamper_Meter.State - замок 1
// бит 2: Tamper_Magnet.State
// бит 3: Tamper_Battery.State - замок 3
// бит 4: СВЧ
// бит 5: статус диагностики
// бит 7: превышение лимита
// байт 3: сборка бит - квадрант + резерв
// бит 0: АЭ+
// бит 1: АЭ-
// бит 2: РЭ+
// бит 3: РЭ-
//----------------------------------------------------------------------------//
#define NUMB_STATUS_ELEMENTS	5
//----------------------------------------------------------------------------//
bool read_status(u8* buff){	//Считываем статус
	dlms.verLCD = (buff[11]& 0xC0) >> 6;	//Версия ЖКИ старшие 2 бита
	if(0 == dlms.verLCD){	//Версия ЖКИ 0	(до 890 постановления)
		if(0x00 == (buff[1] & 0x0B))	dlms.status.Rtc = STATUS_OK;
		else							dlms.status.Rtc  = STATUS_ERROR;
		
		dlms.status.relay = (RelayStatus_t)(buff[3] + 1);
		dlms.status.Phase = buff[5];
		dlms.curTariff = buff[8];
		
		if(0x00 == buff[10])	dlms.status.Eqi = STATUS_TAMPER_OK;
		else				dlms.status.Eqi = STATUS_TAMPER_ACTIVE;
		
		if((0x00 == buff[11]) && (0x00 == buff[12]) && (0x00 == buff[12]))	dlms.status.Magnet = STATUS_TAMPER_OK;
		else																dlms.status.Magnet = STATUS_TAMPER_ACTIVE;
	}
	else if(1 == dlms.verLCD){	//Версия ЖКИ 1	(890 постановление)
		if(0x00 == (buff[1] & 0x0B))	dlms.status.Rtc = STATUS_OK;
		else							dlms.status.Rtc  = STATUS_ERROR;
		
		dlms.status.relay = (RelayStatus_t)(buff[3] + 1);
		dlms.status.Phase = buff[5];
		dlms.curTariff = buff[8];
		
		if		((buff[10] & 0x01) == 0x01)	dlms.status.Eqi = STATUS_TAMPER_ACTIVE;
		else if	((buff[10] & 0x10) == 0x10)	dlms.status.Eqi = STATUS_TAMPER_FLAG;
		else 								dlms.status.Eqi = STATUS_TAMPER_OK;
		
		if		((buff[10] & 0x02) == 0x02)	dlms.status.Unbalance = STATUS_TAMPER_ACTIVE;
		else if	((buff[10] & 0x20) == 0x20)	dlms.status.Unbalance = STATUS_TAMPER_FLAG;
		else 								dlms.status.Unbalance = STATUS_TAMPER_OK;
		
		if		((buff[11] & 0x01) == 0x01)	dlms.status.Cover = STATUS_TAMPER_ACTIVE;
		else if	((buff[12] & 0x01) == 0x01)	dlms.status.Cover = STATUS_TAMPER_FLAG;
		else 								dlms.status.Cover = STATUS_TAMPER_OK;
	
		if		((buff[11] & 0x02) == 0x02)	dlms.status.Meter = STATUS_TAMPER_ACTIVE;
		else if	((buff[12] & 0x02) == 0x02)	dlms.status.Meter = STATUS_TAMPER_FLAG;
		else 								dlms.status.Meter = STATUS_TAMPER_OK;
		
		if		((buff[11] & 0x04) == 0x04)	dlms.status.Magnet = STATUS_TAMPER_ACTIVE;
		else if	((buff[12] & 0x04) == 0x04)	dlms.status.Magnet = STATUS_TAMPER_FLAG;
		else 								dlms.status.Magnet = STATUS_TAMPER_OK;
		
		if		((buff[11] & 0x08) == 0x08)	dlms.status.Battery = STATUS_TAMPER_ACTIVE;
		else if	((buff[12] & 0x08) == 0x08)	dlms.status.Battery = STATUS_TAMPER_FLAG;
		else 								dlms.status.Battery = STATUS_TAMPER_OK;
		
		if		((buff[11] & 0x10) == 0x10)	dlms.status.Uhf = STATUS_TAMPER_ACTIVE;
		else if	((buff[12] & 0x10) == 0x10)	dlms.status.Uhf = STATUS_TAMPER_FLAG;
		else 								dlms.status.Uhf = STATUS_TAMPER_OK;
		
		if		((buff[11] & 0x20) == 0x20)	dlms.status.Diag = STATUS_TAMPER_ACTIVE;
		else if	((buff[12] & 0x20) == 0x20)	dlms.status.Diag = STATUS_TAMPER_FLAG;
		else 								dlms.status.Diag = STATUS_TAMPER_OK;
		//Дублируется ли версия LCD
		if((buff[12] & 0x80) == 0x80)		dlms.status.LimPower = STATUS_TAMPER_ACTIVE;	//проверить
		else								dlms.status.LimPower = STATUS_TAMPER_OK;
		
		dlms.energyDir = (energyDir_t)(buff[13] & 0x0F);
	}
	else return false;
	return true;
}

s16 getElementLength(u8 *buff){
	u16 len = 1;
	s16 tmpLen;
	cosemTag_t tag = (cosemTag_t)buff[0];
	switch(tag) {
		case TAG_NULLDATA:
		case TAG_DONTCARE:
		case TAG_UTF8_STRING:		len += 0;	break;
		case TAG_BOOLEAN:			len += 1;	break;
		case TAG_INT8:
		case TAG_UINT8:
		case TAG_ENUM:				
		case TAG_BCD:				len +=  1;	break;
		case TAG_INT16:
		case TAG_UINT16:			len +=  2; break;
		case TAG_INT32:
		case TAG_UINT32:
		case TAG_FLOAT32:
		case TAG_FLOATING_POINT:	len +=  4;	break;
		case TAG_INT64:
		case TAG_UINT64:
		case TAG_FLOAT64:			len +=  8;	break;
		case TAG_OCTET_STRING:
		case TAG_VISIBLE_STRING:	len += buff[1] + 1; break;
		case TAG_DATETIME:			len += 12;
		case TAG_DATE:				len += 5;
		case TAG_TIME:				len += 4;
		case TAG_BITSTRING:{
			u16 lengthField = buff[1];
			if(lengthField & DLMS_LEN_FLAG_EXTENDED){	//если установлен флаг расширенного поля длины (BER length encoding)
				u8 fieldLen = lengthField & DLMS_LEN_MASK;
				
				switch(fieldLen){
					case 1:{
						lengthField = buff[2];
						len += (lengthField >> 3) + 2;
					}break;
					case 2:{
						lengthField = WORD(buff[2], buff[3]);;
						len += (lengthField >> 3) + 3;
					}break;
					default:{	// Ошибка: неподдерживаемая длина поля
						return -1; //Неизвестный тип
					}
				}
			}
			else{	//Короткое поле длины (1 байт)
				len += (lengthField >> 3) + 1;
			}
		}
		case TAG_STRUCTURE:
		case TAG_ARRAY:{
			u8 numbElements = buff[1];
			len++;
			for(u8 i = 0; i < numbElements; i ++){
				tmpLen = getElementLength(buff + len);
				if(-1 == tmpLen){
					return -1;
				}
				len += tmpLen;
			}
		}break;
		case TAG_COMPACT_ARRAY:{
			tmpLen = getElementLength(buff + len);
			if(-1 == tmpLen){
				return -1;
			}
		}
		default:	return -1; //Неизвестный тип
	}
	return	len;
}

//----------------------------------------------------------------------------//
bool read_data(u8* buff, u16 numElements, u16 len){	//Разбираем данные
	u16 offset = 0;
	s16 lenElement = 0;
	u8 OBIS_num;
	u8 obisCnt = 0;
	
	for(u8 i = 0; i < (DLMS_MAX_OBJ - 5); i++){
		dlms.obj[i].fShow = false;
	}
	for(u16 i = 0; i < numElements; i++){	//Основной цикл по элементам
		lenElement = getElementLength(buff + offset);
		if(-1 == lenElement) return false;
		
		if((buff[offset] == 0x02) && (buff[2 + offset] == 0x09) && (buff[3 + offset] == 0x06)){	//Если данные похожи на OBIS код
			OBIS_num = 255;
			for(u8 j = 0; j < DLMS_MAX_OBJ; j++){	//Поиск OBIS кода
				if(0 == memcmp(dlms.obj[j].pConst->OBIS, buff + offset + 4, 6)){
					OBIS_num = j;
					break;
				}
			}
			if(OBIS_num == 255){	//Если OBIS код не найден
				_SYSLOG_("OBIS Not Found: %u.%u.%u.%u.%u.%u", buff[4 + offset], buff[1 + offset], buff[6 + offset], buff[7 + offset], buff[8 + offset], buff[9 + offset]);
				dlms.error = DLMS_ERROR_OBIS_UNKNOWN;
			}
			else{
				obisCnt++;
				dlms.obj[OBIS_num].fShow = true;
				switch(buff[10 + offset]){	//Определяем тип данных
					case TAG_UINT32:{
						dlms.obj[OBIS_num].type = TAG_UINT32;
						dlms.obj[OBIS_num].value = DWORD(buff[11 + offset], buff[12 + offset], buff[13 + offset], buff[14 + offset]);
						dlms.obj[OBIS_num].scale = buff[18 + offset];
						dlms.obj[OBIS_num].unit = (unit_t)buff[20 + offset];
					}break;
					case TAG_INT32:{
						dlms.obj[OBIS_num].type = TAG_INT32;
						dlms.obj[OBIS_num].value = DWORD(buff[11 + offset], buff[12 + offset], buff[13 + offset], buff[14 + offset]);
						dlms.obj[OBIS_num].scale = buff[18 + offset];
						dlms.obj[OBIS_num].unit = (unit_t)buff[20 + offset];
					}break;
					case TAG_FLOAT32:{
						dlms.obj[OBIS_num].type = TAG_FLOAT32;
						dlms.obj[OBIS_num].value = DWORD(buff[11 + offset], buff[12 + offset], buff[13 + offset], buff[14 + offset]);
						dlms.obj[OBIS_num].scale = buff[18 + offset];
						dlms.obj[OBIS_num].unit = (unit_t)buff[20 + offset];
					}break;
					case TAG_UINT16:{
						dlms.obj[OBIS_num].type = TAG_UINT16;
						dlms.obj[OBIS_num].value = WORD(buff[11 + offset], buff[12 + offset]);
						dlms.obj[OBIS_num].scale = buff[16 + offset];
						dlms.obj[OBIS_num].unit = (unit_t)buff[18 + offset];
					}break;
					case TAG_INT16:{
						dlms.obj[OBIS_num].type = TAG_INT16;
						dlms.obj[OBIS_num].value = WORD(buff[11 + offset], buff[12 + offset]);
						dlms.obj[OBIS_num].scale = buff[16 + offset];
						dlms.obj[OBIS_num].unit = (unit_t)buff[18 + offset];
					}break;
					case TAG_INT64:{
						dlms.obj[OBIS_num].type = TAG_INT64;
						dlms.obj[OBIS_num].value = QWORD(buff[11 + offset], buff[12 + offset], buff[13 + offset], buff[14 + offset], buff[15 + offset], buff[16 + offset], buff[17 + offset], buff[18 + offset]);
						dlms.obj[OBIS_num].scale = buff[22 + offset];
						dlms.obj[OBIS_num].unit = (unit_t)buff[24 + offset];
					}break;
					case TAG_UINT64:{
						dlms.obj[OBIS_num].type = TAG_UINT64;
						dlms.obj[OBIS_num].value = QWORD(buff[11 + offset], buff[12 + offset], buff[13 + offset], buff[14 + offset], buff[15 + offset], buff[16 + offset], buff[17 + offset], buff[18 + offset]);
						dlms.obj[OBIS_num].scale = buff[22 + offset];
						dlms.obj[OBIS_num].unit = (unit_t)buff[24 + offset];
					}break;
					case TAG_OCTET_STRING:{	//Только дата и время
						dlms.obj[OBIS_num].type = TAG_OCTET_STRING;
						switch(OBIS_num){
							case DLMS_TIME:
							case DLMS_RDCH_TIME:{	//Время
								dlms.obj[OBIS_num].value = DWORD(buff[12 + offset], buff[13 + offset], buff[14 + offset], buff[15 + offset]);	//Собираем время в одну переменную
								dlms.obj[OBIS_num].unit = UNIT_NO_UNIT;
							}break;
							case DLMS_DATE:
							case DLMS_RDCH_DATE:{	//Дата
								dlms.obj[OBIS_num].value = DWORD(buff[12 + offset], buff[13 + offset], buff[14 + offset], buff[15 + offset]);	//Собираем дату в одну переменную
								dlms.obj[OBIS_num].unit = UNIT_NO_UNIT;
							}break;
							case DLMS_SERIAL_NUMBER_METER:{
								dlms.obj[OBIS_num].value = (buff[12 + offset] - 0x30)*10000000 + (buff[13 + offset] - 0x30)*1000000 + (buff[14 + offset] - 0x30)*100000 + (buff[15 + offset] - 0x30)*10000 + (buff[16 + offset] - 0x30)*1000 + (buff[17 + offset] - 0x30)*100 + (buff[18 + offset] - 0x30)*10 + (buff[19 + offset] - 0x30);
							}break;
							case DLMS_TYPE_METER:{
								if((buff[11 + offset]) > 10) break;
								u8 str[11];
								memset(str, 0, sizeof(str));
								memcpy(str, buff + 12 + offset, buff[11 + offset]);
								int type1, type2;
								u8 n = sscanf((char*)(buff + 12 + offset), "RIM %d.%d", &type1, &type2);
								if(n == 1){
									dlms.obj[OBIS_num].value = type1*100;
								}
								else if(n == 2){
									dlms.obj[OBIS_num].value = type1*100 + type2;
								}
							}break;
							case DLMS_FW_METER:{
								if((buff[11 + offset]) > 26) break;
								u8 str[25];
								memset(str, sizeof(str), 0);
								memcpy(str, buff + 12 + offset, buff[11 + offset]);
								int fw1, fw2;
								if(2 == sscanf((char*)(buff + 12 + offset), "%d.%d", &fw1, &fw2)){
									dlms.obj[OBIS_num].value = fw1*100 + fw2;
								}
							}break;
							default:{
								_SYSLOG_("DLMS Error - no date or time");
							}
						}
					}break;
					default:{
						_SYSLOG_("DLMS Error - unknown type of data");

					}
				}
				dlms.obj[DLMS_NO_DATA].fShow = false;
			}
		}
		
		offset += lenElement;
	}
	_SYSLOG_("%d", obisCnt);
	return true;
}
//----------------------------------------------------------------------------//
extern const list_dlms_OBIS_codes_t list_dlms_OBIS_codes[DLMS_MAX_OBJ];	//Массив с OBIS-кодами
//----------------------------------------------------------------------------//
u8 dlmsCosemFormingRequest(u8 type, u8 *buff, u32 numbBlock){
	switch(type){
		case REQ_DATA:{
			if(0xFFFFFFFF == numbBlock){
				buff[0] = 0xE6;
				buff[1] = 0xE6;
				buff[2] = 0x00;
				buff[3] = 0xC0;	//Get - запрос
				buff[4] = 0x01;	//нормальный запрос
				buff[5] = 0x81;	//идентификатор ретансляции
				buff[6] = 0x00;	//error
				buff[7] = 0x07;	//CLASS
				buff[8] = 0x00;	//
				buff[9] = 0x00;	//
				buff[10] = 0x15;	//OBIS
				buff[11] = 0x00;	//Standard readout definitions
				buff[12] = 0x02;	//
				buff[13] = 0xFF;	//
				buff[14] = 0x02;	//Attribute
				buff[15] = 0x00;	//нет параметров
				return 16;
			}
			else{
				buff[0] = 0xE6;
				buff[1] = 0xE6;
				buff[2] = 0x00;
				buff[3] = 0xC0;	//Get - запрос
				buff[4] = 0x02;	//блочный запрос
				buff[5] = 0x81;	//идентификатор ретансляции
				buff[6] = BYTE_3(numbBlock);	//
				buff[7] = BYTE_2(numbBlock);	//
				buff[8] = BYTE_1(numbBlock);	//
				buff[9] = BYTE_0(numbBlock);	//
				return 10;
			}
		}
		case REQ_DIAGNOSIS:{
			buff[0] = 0xE6;
			buff[1] = 0xE6;
			buff[2] = 0x00;
			buff[3] = 0xC0;	//Get - запрос
			buff[4] = 0x01;	//нормальный блочный запрос
			buff[5] = 0x81;	//идентификатор ретансляции
			buff[6] = 0x00;	//error
			buff[7] = 0x01;	//CLASS
			buff[8] = 0x00;	//
			buff[9] = 0x00;	//
			buff[10] = 0x60;	//OBIS
			buff[11] = 0x05;	//
			buff[12] = 0xAA;	//
			buff[13] = 0xFF;	//
			buff[14] = 0x02;	//Attribute
			buff[15] = 0x00;	//Нет параметров
			return 16;
		}
		case SET_DIAGNOSIS:{
			buff[0] = 0xE6;
			buff[1] = 0xE6;
			buff[2] = 0x00;
			buff[3] = 0xC1;	//Set - запрос
			buff[4] = 0x01;	//нормальный блочный запрос
			buff[5] = 0x81;	//идентификатор ретансляции
			buff[6] = 0x00;	//error
			buff[7] = 0x01;	//CLASS
			buff[8] = 0x00;	//
			buff[9] = 0x00;	//
			buff[10] = 0x60;	//OBIS
			buff[11] = 0x05;	//
			buff[12] = 0xAA;	//
			buff[13] = 0xFF;	//
			buff[14] = 0x02;	//Attribute
			buff[15] = 0x00;	//Нет параметров
			buff[16] = TAG_UINT32;
			buff[17] = 0x00;
			buff[18] = 0x00;
			buff[19] = 0x00;
			buff[20] = (STATUS_OK == dlms.status.DiagLCD) ? 0x08 : 0x00;	//Diag;
			return 21;
		}
		case CONNECT_RELAY:{
			buff[0] = 0xE6;
			buff[1] = 0xE6;
			buff[2] = 0x00;
			buff[3] = 0xC3;	//Set - запрос
			buff[4] = 0x01;	//нормальный блочный запрос
			buff[5] = 0x81;	//идентификатор ретансляции
			buff[6] = 0x00;	//error
			buff[7] = 0x46;	//CLASS
			buff[8] = 0x00;	//
			buff[9] = 0x00;	//
			buff[10] = 0x60;	//OBIS
			buff[11] = 0x03;	//
			buff[12] = 0x0A;	//
			buff[13] = 0xFF;	//
			buff[14] = 0x02;	//Attribute
			buff[15] = 0x01;	//Нет параметров
			buff[16] = 0x0F;
			buff[17] = 0x00;
			return 18;
		}
	}
	return 0;
}

u8 buff1[50];

u8 bleCntTx;
u8 bleCntRx;
u8 bleBuffTx[60];
u8 bleBuffRx[160];
u8 blePktRx[160];


#define HDLC_FLAG				0x7E
#define HDLC_FRAME_FORMAT		0xA0
#define HDLC_FRAME_FORMAT_MASK	0xF0
#define HDLC_DEST_ADDR			0x03
#define HDLC_SRC_ADDR			0x43
#define HDLC_MIN_LEN			9
#define HDLC_MAX_LEN			200
#define HDLC_FRAME_LEN_MASK		0x7FF
#define HDLC_HCS_OFFSET			6
#define HDLC_SEGMENTATION_MASK	0x08

typedef enum{
	HDLC_I,
	HDLC_RR,
}hdlcCmd_t;

u8 HDLC_Wrapper(u8* buff, u8 cnt, hdlcCmd_t cmd){
	switch(cmd){
		case HDLC_I:{
			memmove((buff + 8), buff, cnt);
			cnt += 9;
			buff[0] = HDLC_FLAG;
			buff[1] = BYTE_1(cnt) | HDLC_FRAME_FORMAT;
			buff[2] = BYTE_0(cnt);
			buff[3] = HDLC_DEST_ADDR;
			buff[4] = HDLC_SRC_ADDR;
			buff[5] = control.byte;
			control.bits.SSS++;
			u16 HCS = pppfcs16(PPPINITFCS16, (buff + 1), 5) ^ 0xffff;
			buff[6] = BYTE_0(HCS);
			buff[7] = BYTE_1(HCS);
			u16 FCS = pppfcs16(PPPINITFCS16, (buff + 1), (cnt - 2)) ^ 0xffff;
			buff[cnt - 1] = BYTE_0(FCS);
			buff[cnt] = BYTE_1(FCS);
			buff[cnt + 1] = HDLC_FLAG;
			cnt += 2;
		}break;
		case HDLC_RR:{
			cnt = 7;
			buff[0] = HDLC_FLAG;
			buff[1] = BYTE_1(cnt) | HDLC_FRAME_FORMAT;
			buff[2] = BYTE_0(cnt);
			buff[3] = HDLC_DEST_ADDR;
			buff[4] = HDLC_SRC_ADDR;
			control_t ctrl;
			ctrl.byte = control.byte;
			ctrl.bits.I = 1;
			ctrl.bits.SSS = 0;
			buff[5] = ctrl.byte;
			u16 FCS = pppfcs16(PPPINITFCS16, (buff + 1), (cnt - 2)) ^ 0xffff;
			buff[6] = BYTE_0(FCS);
			buff[7] = BYTE_1(FCS);
			buff[8] = HDLC_FLAG;
			cnt += 2;
		}break;
	}
	return cnt;
}
bool segmentation = false;
bool Check_HDLC_Frame(u8 *buff, u16 len){
	if((len < HDLC_MIN_LEN) || (len > HDLC_MAX_LEN) || (buff[0] != HDLC_FLAG) || (buff[len - 1] != HDLC_FLAG))	return false;
	if((buff[1] & HDLC_FRAME_FORMAT_MASK) != HDLC_FRAME_FORMAT) return false;
	u16 frameLen = WORD(buff[1], buff[2]) & HDLC_FRAME_LEN_MASK;
	if((frameLen + 2) != len)	return false;
	
	u16 HCS, FCS;
	HCS = pppfcs16(PPPINITFCS16, (buff + 1), 5) ^ 0xffff;
	FCS = pppfcs16(PPPINITFCS16, (buff + 1), (frameLen - 2)) ^ 0xffff;

	if((HCS != WORD(buff[HDLC_HCS_OFFSET + 1], buff[HDLC_HCS_OFFSET]) || (FCS != WORD(buff[frameLen], buff[frameLen - 1]))))	return false;
	
	if((HDLC_SRC_ADDR != buff[3]) || (HDLC_DEST_ADDR != buff[4])) return false;
	
	if((buff[1] & HDLC_SEGMENTATION_MASK) == HDLC_SEGMENTATION_MASK)	segmentation = true;	//Сегментация
	else	segmentation = false;	//Без сегментации
	
	control_t ctrl;
	ctrl.byte = buff[5];
	
	if(ctrl.bits.I == 0){	//I-кадр
		if(ctrl.bits.SSS == control.bits.RRR){
			control.bits.RRR++;
		}
		return true;
	}
	else{
		control.bits.RRR = 0;
		control.bits.SSS = 0;
		return false;
	}
}

struct{
	u8 hdlc[2048];
	u8 pdu[4096];
	u16 curLenHdlc;
	u16 curLenPdu;
}dataBuffRx;

void parsingDlms(u8 *buff, u16 len){
	dlms.obj[DLMS_LCD_DIAG].fShow = true;
	dlms.obj[DLMS_NO_DATA].fShow = true;
	dlms.obj[DLMS_SERIAL_NUMBER_DISPLAY].fShow = true;
	dlms.obj[DLMS_TYPE_DISPLAY].fShow = true;
	dlms.obj[DLMS_FW_DISPLAY].fShow = true;
	msg.set(MSG_DATA_UPDATE, NULL, 0);
	if(len < 10){
		_SYSLOG_("Error - COSEM len"); 
		return;
	}
	u8 *p = buff;
	if(0 != memcmp(p, dlmsHeaderInfoOrData, sizeof(dlmsHeaderInfoOrData))){	//если заголовок не совпадает
		_SYSLOG_("Error - COSEM Header");
		return;
	}
	p += 3;
	
	u8 lengthField = p[0];
	u16 numbElements = 0;
	
	if(lengthField & DLMS_LEN_FLAG_EXTENDED){	//если установлен флаг расширенного поля длины (BER length encoding)
		u8 fieldLen = lengthField & DLMS_LEN_MASK;
		switch(fieldLen){
			case 1:{
				numbElements = p[1];
				p++;
				len--;
			}break;
			case 2:{
				numbElements = WORD(p[1], p[2]);
				p++;
				p++;
				len--;
				len--;
			}break;
			default:{	// Ошибка: неподдерживаемая длина поля
				 _SYSLOG_("Unsupported length field size: %d", fieldLen);
				return;
			}
		}
	}
	else{	//Короткое поле длины (1 байт)
		numbElements = p[0];
	}
		
	
	if(numbElements < 5){
		_SYSLOG_("Error - DLMS elements less 5");
		return;
	}
	p++;
	if(!read_status(p)){
		_SYSLOG_("Error - DLMS Read Status");
		return;
	}
	p += 14;
	len -= 18;
	if(!read_data(p, (numbElements - 5), len)){
		_SYSLOG_("DLMS - Parsing Data Error");
	}
	else	_SYSLOG_("DLMS - Parsing Data Success");
	
	return;
}




u32 extractNumbBlock(u8 *buff, u16 len, u8 *buffOut, u16 *buffOutOffset, s8 *result){
	u32 numbBlock = 0;
	
	if(!buff || !buffOut || !buffOutOffset || !result || (len < 13)){	//Проверка корректности входных параметров
		if(result) *result = -1;
		return 0;
	}
	
	// Проверка заголовка DLMS
	if((DLMS_HEADER_BYTE_0 != buff[0]) || (DLMS_HEADER_BYTE_1 != buff[1]) || (DLMS_HEADER_BYTE_2 != buff[2]) || (DLMS_HEADER_BYTE_3 != buff[3]) || (DLMS_HEADER_BYTE_5 != buff[5])){
		*result = -1;    // Ошибка: неверный заголовок
		_SYSLOG_("DLMS header mismatch: %02X %02X %02X %02X %02X", buff[0], buff[1], buff[2], buff[3], buff[5]);
		return 0;
    }
	
	u8 responseType = buff[DLMS_OFFSET_TYPE];	//Проверка типа ответа
	
	
	
	if(DLMS_RESP_TYPE_NORMAL == responseType){	//Нормальный ответ
		if(0 != buff[DLMS_OFFSET_DATA_FLAG]){
			*result = -1;    // Ошибка в ответе
			_SYSLOG_("Normal response error - No data flag: %d", buff[DLMS_OFFSET_DATA_FLAG]);
			return 0;
		}
        u16 dataLen = len - 7;	//Копируем данные (начиная с 7 байта и до конца)
		if(*buffOutOffset + dataLen > MAX_PDU_SIZE){	//Проверка на переполнение выходного буфера
			*result = -1;
			_SYSLOG_("Buffer overflow in normal response");
			return 0;
        }
		memcpy((buffOut + *buffOutOffset), (buff + 7), dataLen);
		*buffOutOffset += dataLen;
		
		*result = 2;	//Обычный ответ, не блок
		return 0;		//Номер блока не актуален
	}
	else if(DLMS_RESP_TYPE_BLOCK == responseType){	//Блочный ответ
		if(len < 13){	//Проверка минимальной длины для блочного ответа
			*result = -1;
			_SYSLOG_("Block response too short: %d bytes", len);
			return 0;
		}
		u8 lastBlockStatus = buff[DLMS_OFFSET_LAST_BLOCK_FLAG];	//Анализ статуса блока
		
		if(DLMS_BLOCK_STATUS_NOT_LAST == lastBlockStatus)	*result = 0;	//не последний блок
		else												*result = 1;	//последний блок
//		else if(DLMS_BLOCK_STATUS_LAST == lastBlockStatus)	*result = 1;	//последний блок
//		else{
//			*result = -1;    // Ошибка
//            _SYSLOG_("Invalid block status: %d", lastBlockStatus);
//            return 0;
//		}
		
		numbBlock = DWORD(buff[DLMS_OFFSET_BLOCK_NUM], buff[DLMS_OFFSET_BLOCK_NUM +  1], buff[DLMS_OFFSET_BLOCK_NUM + 2], buff[DLMS_OFFSET_BLOCK_NUM + 3]);
		if (0 != buff[DLMS_OFFSET_BLOCK_DATA_FLAG]){	//Проверка статуса данных в блоке
			*result = -1;	//Ошибка: данные не готовы
			_SYSLOG_("Block data not ready, status: %d", buff[DLMS_OFFSET_BLOCK_DATA_FLAG]);
			return numbBlock;	//Возвращаем номер блока даже при ошибке
		}
		
		u16 dataLen = 0;
		u8 lengthField = buff[DLMS_OFFSET_LENGTH];
		u8 headerOffset  = 0;
		
		if(lengthField & DLMS_LEN_FLAG_EXTENDED){	//если установлен флаг расширенного поля длины (BER length encoding)
			u8 fieldLen = lengthField & DLMS_LEN_MASK;
			
			switch(fieldLen){
				case 1:{
					headerOffset = 1;
					dataLen = buff[DLMS_OFFSET_LENGTH + 1];
				}break;
				case 2:{
					headerOffset = 2;
					dataLen = WORD(buff[DLMS_OFFSET_LENGTH + 1], buff[DLMS_OFFSET_LENGTH + 2]);
				}break;
				default:{	// Ошибка: неподдерживаемая длина поля
					*result = -1;
					 _SYSLOG_("Unsupported length field size: %d", fieldLen);
					return numbBlock;
				}
			}
		}
		else{	//Короткое поле длины (1 байт)
            dataLen = lengthField;
            headerOffset = 0;
		}
		
		u16 dataStart = DLMS_OFFSET_LENGTH + 1 + headerOffset;
		if (dataStart + dataLen > len){	//Проверка, что данные не выходят за пределы буфера
			*result = -1;    // Ошибка: данные обрезаны
			_SYSLOG_("Data exceeds buffer: start=%d, len=%d, total=%d", dataStart, dataLen, len);
			return numbBlock;
		}
		
		if (*buffOutOffset + dataLen > MAX_PDU_SIZE){	//Проверка на переполнение выходного буфера
			*result = -1;
			_SYSLOG_("Buffer overflow in block response: %d + %d > %d", *buffOutOffset, dataLen, MAX_PDU_SIZE);
			return numbBlock;
		}
		
		memcpy((buffOut + *buffOutOffset), buff + dataStart, dataLen);
		*buffOutOffset += dataLen;
		_SYSLOG_("Block %lu extracted: type=%s, dataLen=%d, %s", numbBlock, (*result == 1) ? "LAST" : "MID", dataLen, (lastBlockStatus == DLMS_OFFSET_LAST_BLOCK_FLAG) ? "OK" : "LAST");
	}
	else{	// ===== Неизвестный тип ответа =====
		*result = -1;
		_SYSLOG_("Unknown response type: %02X", responseType);
		return 0;
	}
	return numbBlock;
}

PT_THREAD (thread_dataRequest(pt_t *pt)){
	static u32 numbBlock = 0xFFFFFFFF;
	static bool fError = false;
	static u8 noAnswer = 0;
	PT_BEGIN(pt);
		for(u8 i = 0; i < DLMS_MAX_OBJ; i++){	//Загружаем данные из Flash
			dlms.obj[i].pConst = &list_dlms_OBIS_codes[i];
			dlms.obj[i].fShow = false;
			dlms.obj[i].scale = 0;
			dlms.obj[i].type = TAG_NULLDATA;
			dlms.obj[i].unit = UNIT_NO_UNIT;
			dlms.obj[i].value = 0;
		}
		PT_WAIT_UNTIL(pt, msg.getVal(MSG_PULT_CONNECT_TO_METER));	//Запроса подключения пульта к счетчику из потока main
		
		control.bits.RRR = 0;
		control.bits.P_F = 1;
		control.bits.SSS = 0;
		control.bits.I = 0;
		
		while(1){
			dataBuffRx.curLenPdu = 0;
			do{	//Прием блоков данных
				dataBuffRx.curLenHdlc = 0;
				bleCntTx = dlmsCosemFormingRequest(REQ_DATA, bleBuffTx, numbBlock);
				bleCntTx = HDLC_Wrapper(bleBuffTx, bleCntTx, HDLC_I);
				do{	//Прием сегментированных данных
					tim.reset(TIMER_RF_ANSWER);
					msg.set(MSG_REQWEST, NULL, 0);
					PT_WAIT_UNTIL(pt, (msg.getVal(MSG_RADIO_ANSWER_OK) || (tim.get(TIMER_RF_ANSWER) > TIMEOUT_RADIO_ANSWER)));	//ожидаем ответ или таймаут
					if(tim.get(TIMER_RF_ANSWER) < TIMEOUT_RADIO_ANSWER){	//Если пришел ответ
						if(Check_HDLC_Frame(bleBuffRx, bleCntRx)){
							if(segmentation){
								u8 len = bleCntRx - 11;
								memcpy(dataBuffRx.hdlc + dataBuffRx.curLenHdlc, bleBuffRx + 8, len);
								dataBuffRx.curLenHdlc += len;
								bleCntTx = 0;
								bleCntTx = HDLC_Wrapper(bleBuffTx, bleCntTx, HDLC_RR);
								_SYSLOG_("HDLC - Segmentation");
							}
							else{
								u8 len = bleCntRx - 11;
								memcpy(dataBuffRx.hdlc + dataBuffRx.curLenHdlc, bleBuffRx + 8, len);
								dataBuffRx.curLenHdlc += len;
								bleCntTx = 0;
								_SYSLOG_("End Of Segmentation");
								fError = false;
								break;
							}
						}
						else{
							dlms.error = DLMS_ERROR_HDLC;
							_SYSLOG_("DLMS Error - HDLC Check");
							fError = true;
							break;
						}
					}
					else{
						_SYSLOG_("Radio Error - TimeOut Answer - REQ_DATA");	//Если таймаут
						noAnswer++;
						if(noAnswer > 10)	msg.set(MSG_NO_ANSWER, NULL, 0);
					}
					PT_YIELD(pt);
				}while(1);
				if(fError){
					numbBlock = 0xFFFFFFFF;
					break;
				}
				s8 result = -1;
				
				numbBlock = extractNumbBlock(dataBuffRx.hdlc, dataBuffRx.curLenHdlc, dataBuffRx.pdu, &dataBuffRx.curLenPdu, &result);
				
				
				if(-1 == result){
					numbBlock = 0xFFFFFFFF;
					break;
				}
				else if(2 == result){ //не блочный ответ
					parsingDlms(dataBuffRx.pdu, dataBuffRx.curLenPdu);
					numbBlock = 0xFFFFFFFF;
					break;
				}
				else if(0 == result){	//Блочный ответ и непоследний блок
					
				}
				else if(1 == result){	//Блочный ответ и последний блок
					parsingDlms(dataBuffRx.pdu, dataBuffRx.curLenPdu);
					numbBlock = 0xFFFFFFFF;
					break;
				}
			}while(1);
			
			PT_YIELD(pt);
			
			if(msg.getVal(MSG_REQWEST_RELAY_CONNECT)){
				bleCntTx = dlmsCosemFormingRequest(CONNECT_RELAY, bleBuffTx, 0xFF);
				bleCntTx = HDLC_Wrapper(bleBuffTx, bleCntTx, HDLC_I);
				do{
					if(msg.getVal(MSG_RELAY_CONNECT_BREAK)){
						_SYSLOG_("MSG_RELAY_CONNECT_BREAK");
						break;
					}
					tim.reset(TIMER_RF_ANSWER);
					msg.set(MSG_REQWEST, NULL, 0);
					PT_WAIT_UNTIL(pt, (msg.getVal(MSG_RADIO_ANSWER_OK) || (tim.get(TIMER_RF_ANSWER) > TIMEOUT_RADIO_ANSWER)));	//ожидаем ответ или таймаут
					if(tim.get(TIMER_RF_ANSWER) < TIMEOUT_RADIO_ANSWER){	//Если пришел ответ
						if(Check_HDLC_Frame(bleBuffRx, bleCntRx)){
							if(bleCntRx == 19){
								if(read_relay_answer(bleBuffRx + 11)){
									_SYSLOG_("DLMS - Recive Relay Connect");
									msg.set(MSG_RELAY_CONNECT_SUCCESS, NULL, 0);
								}
								else{
									_SYSLOG_("DLMS Error - Header Relay");
									msg.set(MSG_RELAY_CONNECT_ERROR, NULL, 0);
								}
							}
							else{
								_SYSLOG_("DLMS Error - Len");
								msg.set(MSG_RELAY_CONNECT_ERROR, NULL, 0);
							}
							break;
						}
						else{
							dlms.error = DLMS_ERROR_HDLC;
							_SYSLOG_("DLMS Error - HDLC Check");
							break;
						}
					}
					else{	//Если таймаут
						_SYSLOG_("Radio Error - TimeOut Answer - CONNECT_RELAY");
					}
					
				}while(1);
			}
			
			if(msg.getVal(MSG_SET_DIAGNOSIS)){
				bleCntTx = dlmsCosemFormingRequest(SET_DIAGNOSIS, bleBuffTx, 0xFF);
				bleCntTx = HDLC_Wrapper(bleBuffTx, bleCntTx, HDLC_I);
				do{
					tim.reset(TIMER_RF_ANSWER);
					msg.set(MSG_REQWEST, NULL, 0);
					PT_WAIT_UNTIL(pt, (msg.getVal(MSG_RADIO_ANSWER_OK) || (tim.get(TIMER_RF_ANSWER) > TIMEOUT_RADIO_ANSWER)));	//ожидаем ответ или таймаут
					if(tim.get(TIMER_RF_ANSWER) < TIMEOUT_RADIO_ANSWER){	//Если пришел ответ
						if(Check_HDLC_Frame(bleBuffRx, bleCntRx)){
							if(bleCntRx == 18){
								if(read_set_diagnosis(bleBuffRx + 11)){
									_SYSLOG_("DLMS - Recive Status Diagnosis LCD: %u", dlms.status.DiagLCD);
								}
								else	_SYSLOG_("DLMS Error - Header Set.Diagnosis");
							}
							else	_SYSLOG_("DLMS Error - Len");
							msg.set(MSG_RECEIVE_SET_DIAGNOSIS, NULL, 0);
							break;
						}
						else{
							dlms.error = DLMS_ERROR_HDLC;
							_SYSLOG_("DLMS Error - HDLC Check");
							break;
						}
					}
					else{	//Если таймаут
						_SYSLOG_("Radio Error - TimeOut Answer - SET_DIAGNOSIS");
					}
				}while(1);
			}
			
			bleCntTx = dlmsCosemFormingRequest(REQ_DIAGNOSIS, bleBuffTx, 0xFF);
			bleCntTx = HDLC_Wrapper(bleBuffTx, bleCntTx, HDLC_I);
			do{
				tim.reset(TIMER_RF_ANSWER);
				msg.set(MSG_REQWEST, NULL, 0);
				PT_WAIT_UNTIL(pt, (msg.getVal(MSG_RADIO_ANSWER_OK) || (tim.get(TIMER_RF_ANSWER) > TIMEOUT_RADIO_ANSWER)));	//ожидаем ответ или таймаут
				if(tim.get(TIMER_RF_ANSWER) < TIMEOUT_RADIO_ANSWER){	//Если пришел ответ
					if(Check_HDLC_Frame(bleBuffRx, bleCntRx)){
						if(bleCntRx == 23){
							if(read_req_diagnosis(bleBuffRx + 11)){
								_SYSLOG_("DLMS - Recive Status Diagnosis LCD: %u", dlms.status.DiagLCD);
							}
							else	_SYSLOG_("DLMS Error - Header Req.Diagnosis");
						}
						else	_SYSLOG_("DLMS Error - Len");
						break;
					}
					else{
						dlms.error = DLMS_ERROR_HDLC;
						_SYSLOG_("DLMS Error - HDLC Check");
						break;
					}
				}
				else{	//Если таймаут
					_SYSLOG_("Radio Error - TimeOut Answer - REQ_DIAGNOSIS");
				}
			}while(1);
			
		}
	PT_END(pt);
}
//----------------------------------------------------------------------------//
