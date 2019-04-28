#include "bsp_update.h"
/**************************************************
                  分区划分
model :  stm32f103c8t6       flash :   64K

bootloader :   5K          0x8000000-0x8001400     0-5K

APP1       :   25K         0x8001800-0x8008800    6k-34k

APP2       :   25K         0x8008c00-0X800FC00    35k-63K     

***************************************************/

/***************************************************
                  参数地址分配  (0X800FC00-0x8010000 64K)
uint8_t APP1.jump_flag                          0X800FC00            2
uint8_t APP1.upgrade_flag                       0X800FC06                      
uint8_t APP1.upgrade_version                    0X800FC10           2 
uint8_t APP1.Total_blocks                       0X800FC20             2  
uint8_t APP1.local_version                      0X800FC30             2
uint16_t APP1.NOW_blocks                        0X800FC40
uint16_t APP1.flash_blocks                      0X800FC60 

*****************************************************/
#define             App2_Start_Addr            0x8008c00