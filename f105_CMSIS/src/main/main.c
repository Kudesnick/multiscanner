/********************************************************************
 * Основная программа
 ********************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include "stm32f10x.h"
#include "ifaces_types.h"
#include "console.h"

#include "Driver_CAN.h"
extern ARM_DRIVER_CAN Driver_CAN1;
extern ARM_DRIVER_CAN Driver_CAN2;

extern void console_send_str(char * str);

// Основная функция 
int main (void)
{
    static volatile uint8_t tmp = 0;
    
    iface_setting_t console_setting = console_get_init();
    iface_setting_t can_sett = {true, 500000, 0};
    
    const can_iface_specific_sett_t can_spec_sett = {5, 7, 5, 4, 0};
    msg_t msg = {0, IFACE_TX, 0x7E0, 8, {0, 1, 2, 3, 4, 5, 6, 7}, 0, 0};
    
    can_sett.sett = *(specific_setting_t*)&can_spec_sett;

// Вызывается из startup файла. Здесь не требуется.
//  SystemInit();   // CCLK2 = HCLK = SYSCLK = PLLCLK = HSE/2 * 9 = HSE_VALUE/2 * 9 = 16000000/2 * 9 = 72 MHz // CCLK1 = PLLCLK/2 = 72/2 = 36 MHz
    SystemCoreClockUpdate();
    
    console_init(console_setting);

    #if (CAN1_ENABLED == 1) || (CAN2_ENABLED == 1)

        can_pre_init();
        
    #endif
        
    #if (CAN1_ENABLED == 1)
        can_init(IFACE_CAN1, can_sett, NULL);
    #endif
        
    #if (CAN2_ENABLED == 1)
        can_init(IFACE_CAN2, can_sett, NULL);
    #endif

    #ifdef DEBUG
        can_send_msg(IFACE_CAN1, &msg);
        can_send_msg(IFACE_CAN2, &msg);
    #endif

    while(true)
    {
        tmp = ~tmp;
    }
}
