//----------------------------------------------------------------------------//
#include "utils.h"
#include "log.h"
#include "HAL.h"
#include "threads.h"
#include "memory.h"
//----------------------------------------------------------------------------//
int main(void){
	remap_vector_table();
	__disable_irq();
	
	SupplyEn();				//Включение питания
	Init_Core();

	ReadSystemInfoAndSettings();
	Threads_Init();

	_SYSLOG_("START");
	
	__enable_irq();
	
    while (1){
		thread_systemInfo(&systemInfo_pt);
		thread_lcd(&lcd_pt);
		thread_btn(&btn_pt);
		thread_main(&main_pt);
		thread_status(&status_pt);
		thread_usbUart(&usbUart_pt);
		thread_dataRequest(&dataRequest_pt);
		thread_dataRouting(&dataRouting_pt);
		
		thread_BLE(&ble_pt);
		
		thread_disabling(&disabling_pt);
		IWDG_Refresh();
	}
}
//----------------------------------------------------------------------------//
