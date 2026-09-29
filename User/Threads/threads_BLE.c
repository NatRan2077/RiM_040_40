//----------------------------------------------------------------------------//
#include "threads.h"
#include "common_types.h"
#include "messages.h"
#include "log.h"
#include "cmt453x.h"
#include "hp_sleep.h"
#include "app_ble.h"
#include "utils.h"
//----------------------------------------------------------------------------//
pt_t		ble_pt;
//----------------------------------------------------------------------------//
bleData_t bleData;
//----------------------------------------------------------------------------//
void turnOnRxEnTxEn(void){
	if(ip_diagcntl_get() != 0x80 + 0x03){
		RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB, ENABLE);
		GPIO_InitType GPIO_InitStructure;
		GPIO_InitStruct(&GPIO_InitStructure);
		GPIO_InitStructure.Pin              = GPIO_PIN_0 | GPIO_PIN_3;
		GPIO_InitStructure.GPIO_Mode        = GPIO_MODE_AF_PP;
		GPIO_InitStructure.GPIO_Pull        = GPIO_PULL_DOWN;
		GPIO_InitStructure.GPIO_Alternate     = GPIO_AF6;
		GPIO_InitPeripheral(GPIOB,  &GPIO_InitStructure);

		//afec block clk is on in active mode
		REG32(0x40011000 + 0x0) |= 0x01 << 28; //open dbg port
		ip_diagcntl_set(0x80 + 0x03); //enable dbg port 0 and set DIAG0[6:0] as 0x28
	}
}
//----------------------------------------------------------------------------//
void user_send_ble_data(const uint8_t *data, uint16_t length);
extern u8 bleCntTx;
extern u8 bleCntRx;
extern u8 bleBuffTx[60];
extern u8 bleBuffRx[160];
//----------------------------------------------------------------------------//
PT_THREAD (thread_BLE(pt_t *pt)){
	u32 numMeter;
  	PT_BEGIN(pt);
		u8 *p_mac = SystemGetMacAddr();
		u8 mac_address[6];
		if (p_mac != NULL){
			memcpy(mac_address, p_mac, 6);	//  опируем MAC-адрес
			// ‘орматируем в читаемый вид "AA:BB:CC:DD:EE:FF"
			snprintf(SystemInfo.MAC_Address, sizeof(SystemInfo.MAC_Address), "%02X:%02X:%02X:%02X:%02X:%02X", mac_address[0], mac_address[1], mac_address[2], mac_address[3], mac_address[4], mac_address[5]);
		}
		else memcpy(SystemInfo.MAC_Address, "  :  :  :  :  :  ", sizeof(SystemInfo.MAC_Address));
	
		SystemInfo.BLE_State = BLE_STATE_DISCONNECT;
		PT_WAIT_UNTIL(pt, msg.getVal(MSG_BLE_START));
		numMeter = SystemInfo.numMeter;
		for(s8 i = 7; i >= 0; i--){
			SystemInfo.numMeterStr[i] = (numMeter%10) + '0';
			numMeter /= 10;
		}
		SystemInfo.numMeterStr[8] = 0;
		app_ble_init();
		hp_ble_radio_power_set(TX_POWER_Pos6_DBM);
		turnOnRxEnTxEn();
	
		while(1){
			rwip_schedule();
			//hp_sleep(); подозрение что из этого зависает
			if(msg.getVal(MSG_REQWEST)){
				if(BLE_STATE_CONNECT == SystemInfo.BLE_State){
					_SYSLOG_DATA_("[Data BLE TX]", bleBuffTx, bleCntTx);
					user_send_ble_data(&bleBuffTx[0], bleCntTx);
				}
			}
			PT_YIELD(pt);
		}
	PT_END(pt);
}
//----------------------------------------------------------------------------//
