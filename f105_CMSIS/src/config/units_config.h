//------------------------------------------------------------------------------
// Настройки интерфейсов
//------------------------------------------------------------------------------

#include "misc.h"

//-------- <<< Use Configuration Wizard in Context Menu >>> --------------------

#ifndef _UNIT_CONFIG_H_
#define _UNIT_CONFIG_H_

// Поддерживаемые классы хардварных интерфейсов
typedef enum
{
    UNIT_CLASS_TYPE_UART,
//  UNIT_CLASS_TYPE_CAN,
    UNIT_CLASS_TYPE_COUNT
}unit_class_type_t;

// Поддерживаемые типы протоколов
typedef enum
{
//  PROTOCOL_TYPE_UART,
    PROTOCOL_TYPE_UART_STRING,
//  PROTOCOL_TYPE_LIN,
//  PROTOKOL_TYPE_CAN,
//  PROTOCOL_TYPE_CAN_TP,
    PROTOCOL_TYPE_COUNT
}protocol_type_t;

#ifndef NULL
	#define NULL 0
#endif

#define GPIO_PORT(num) \
 ((num == 0) ? GPIOA : \
  (num == 1) ? GPIOB : \
  (num == 2) ? GPIOC : \
  (num == 3) ? GPIOD : \
  (num == 4) ? GPIOE : \
  (num == 5) ? GPIOF : \
  (num == 6) ? GPIOG : \
  NULL)

#define LIN_DRIVER(num) \
 ((num == 1) ? Driver_USART1 : \
  (num == 2) ? Driver_USART2 : \
  (num == 3) ? Driver_USART3 : \
  (num == 4) ? Driver_USART4 : \
  (num == 5) ? Driver_USART5 : \
  NULL)
  
#define CONSOLE_DRIVER(num) \
 ((num == 1) ? Driver_USART1 : \
  (num == 2) ? Driver_USART2 : \
  (num == 3) ? Driver_USART3 : \
  (num == 4) ? Driver_USART4 : \
  (num == 5) ? Driver_USART5 : \
  NULL)

#define PARITY_TYPE(num) \
 ((num == 0) ? USART_Parity_No   : \
  (num == 1) ? USART_Parity_Even : \
  (num == 2) ? USART_Parity_Odd  : \
  NULL)

//	<e0>CAN1 enabled
//      <o1>can physics port num <1=>CAN1 <2=>CAN2
//		<e2>strobe port enabled
//			<o3>Port <0=>GPIOA <1=>GPIOB <2=>GPIOC <3=>GPIOD <4=>GPIOE <5=>GPIOF <6=>GPIOG
//    		<o4>Pin <0=>0 <1=>1 <2=>2 <3=>3 <4=>4 <5=>5 <6=>6 <7=>7 <8=>8 <9=>9 <10=>10 <11=>11 <12=>12 <13=>13 <14=>14 <15=>15
//			<o5>Active level <0=>0 <1=>1
//		</e>
//	</e>
#define CAN1_ENABLED 1
#define CAN1_Driver_NUM 1
#define CAN1_STB_AVAILABLE 1
#define CAN1_STB_PORT GPIO_PORT(1)
#define CAN1_STB_PIN_NUM 15
#define CAN1_STB_ON 0

#define CAN1_STB_OFF (CAN1_STB_ON ^ 1)
#define CAN1_STB_PIN  CAN1_STB_PORT,CAN1_STB_PIN_NUM
#if   (CAN1_Driver_NUM == 1)
    #define CAN1_Driver Driver_CAN1
#elif (CAN1_Driver_NUM == 2)
    #define CAN1_Driver Driver_CAN2
#endif

//	<e0>CAN2 enabled
//      <o1>can physics port num <1=>CAN1 <2=>CAN2
//		<e2>strobe port enabled
//			<o3>Port <0=>GPIOA <1=>GPIOB <2=>GPIOC <3=>GPIOD <4=>GPIOE <5=>GPIOF <6=>GPIOG
//    		<o4>Pin <0=>0 <1=>1 <2=>2 <3=>3 <4=>4 <5=>5 <6=>6 <7=>7 <8=>8 <9=>9 <10=>10 <11=>11 <12=>12 <13=>13 <14=>14 <15=>15
//			<o5>Active level <0=>0 <1=>1
//		</e>
#define CAN2_ENABLED 1
#define CAN2_Driver_NUM 2
#define CAN2_STB_AVAILABLE 1
#define CAN2_STB_PORT GPIO_PORT(1)
#define CAN2_STB_PIN_NUM 0
#define CAN2_STB_ON 0

#define CAN2_STB_OFF (CAN2_STB_ON ^ 1)
#define CAN2_STB_PIN  CAN2_STB_PORT,CAN2_STB_PIN_NUM
#if   (CAN2_Driver_NUM == 1)
    #define CAN2_Driver Driver_CAN1
#elif (CAN2_Driver_NUM == 2)
    #define CAN2_Driver Driver_CAN2
#endif
//	</e>


//	<e0>LIN1 enabled
//      <o1>lin physics port num <1=>UART1 <2=>UART2 <3=>UART3 <4=>UART4 <5=>UART5
//		<e2>strobe port enabled
//			<o3>Port <0=>GPIOA <1=>GPIOB <2=>GPIOC <3=>GPIOD <4=>GPIOE <5=>GPIOF <6=>GPIOG
//    		<o4>Pin <0=>0 <1=>1 <2=>2 <3=>3 <4=>4 <5=>5 <6=>6 <7=>7 <8=>8 <9=>9 <10=>10 <11=>11 <12=>12 <13=>13 <14=>14 <15=>15
//			<o5>Active level <0=>0 <1=>1
//		</e>
#define LIN1_ENABLED 0
#define LIN1_Driver_NUM 2
#define LIN1_STB_AVAILABLE 1
#define LIN1_STB_PORT GPIO_PORT(1)
#define LIN1_STB_PIN_NUM 0
#define LIN1_STB_ON 0

#define LIN1_STB_OFF (LIN1_STB_ON ^ 1)
#define LIN1_STB_PIN  LIN1_STB_PORT,LIN1_STB_PIN_NUM

//		<e0>pullUp master (1k)
//			<o1>Port <0=>GPIOA <1=>GPIOB <2=>GPIOC <3=>GPIOD <4=>GPIOE <5=>GPIOF <6=>GPIOG
//    		<o2>Pin <0=>0 <1=>1 <2=>2 <3=>3 <4=>4 <5=>5 <6=>6 <7=>7 <8=>8 <9=>9 <10=>10 <11=>11 <12=>12 <13=>13 <14=>14 <15=>15
//			<o3>Active level <0=>0 <1=>1
//		</e>
#define LIN1_PULL_UP_MASTER_AVAILABLE 1
#define LIN1_PULL_UP_MASTER_PORT GPIO_PORT(1)
#define LIN1_PULL_UP_MASTER_PIN_NUM 0
#define LIN1_PULL_UP_MASTER_ON 0

#define LIN1_PULL_UP_MASTER_OFF (LIN1_PULL_UP_MASTER_ON ^ 1)
#define LIN1_PULL_UP_MASTER_PIN  LIN1_PULL_UP_MASTER_PORT,LIN1_PULL_UP_MASTER_PIN_NUM

//		<e0>pullUp slave (10k)
//			<o1>Port <0=>GPIOA <1=>GPIOB <2=>GPIOC <3=>GPIOD <4=>GPIOE <5=>GPIOF <6=>GPIOG
//    		<o2>Pin <0=>0 <1=>1 <2=>2 <3=>3 <4=>4 <5=>5 <6=>6 <7=>7 <8=>8 <9=>9 <10=>10 <11=>11 <12=>12 <13=>13 <14=>14 <15=>15
//			<o3>Active level <0=>0 <1=>1
//		</e>
#define LIN1_PULL_UP_SLAVE_AVAILABLE 1
#define LIN1_PULL_UP_SLAVE_PORT GPIO_PORT(1)
#define LIN1_PULL_UP_SLAVE_PIN_NUM 0
#define LIN1_PULL_UP_SLAVE_ON 0

#define LIN1_PULL_UP_SLAVE_OFF (LIN1_PULL_UP_SLAVE_ON ^ 1)
#define LIN1_PULL_UP_SLAVE_PIN  LIN1_PULL_UP_SLAVE_PORT,LIN1_PULL_UP_SLAVE_PIN_NUM

#if   (LIN1_Driver_NUM == 1)
    #define LIN1_Driver Driver_USART1
#elif (LIN1_Driver_NUM == 2)
    #define LIN1_Driver Driver_USART2
#elif (LIN1_Driver_NUM == 3)
    #define LIN1_Driver Driver_USART3
#elif (LIN1_Driver_NUM == 4)
    #define LIN1_Driver Driver_USART4
#elif (LIN1_Driver_NUM == 5)
    #define LIN1_Driver Driver_USART5
#endif
//	</e>


//	<e0>LIN2 enabled
//      <o1>lin physics port num <1=>UART1 <2=>UART2 <3=>UART3 <4=>UART4 <5=>UART5
//		<e2>strobe port enabled
//			<o3>Port <0=>GPIOA <1=>GPIOB <2=>GPIOC <3=>GPIOD <4=>GPIOE <5=>GPIOF <6=>GPIOG
//    		<o4>Pin <0=>0 <1=>1 <2=>2 <3=>3 <4=>4 <5=>5 <6=>6 <7=>7 <8=>8 <9=>9 <10=>10 <11=>11 <12=>12 <13=>13 <14=>14 <15=>15
//			<o5>Active level <0=>0 <1=>1
//		</e>
#define LIN2_ENABLED 0
#define LIN2_Driver_NUM 2
#define LIN2_STB_AVAILABLE 1
#define LIN2_STB_PORT GPIO_PORT(1)
#define LIN2_STB_PIN_NUM 0
#define LIN2_STB_ON 0

#define LIN2_STB_OFF (LIN2_STB_ON ^ 1)
#define LIN2_STB_PIN  LIN2_STB_PORT,LIN2_STB_PIN_NUM

//		<e0>pullUp master (1k)
//			<o1>Port <0=>GPIOA <1=>GPIOB <2=>GPIOC <3=>GPIOD <4=>GPIOE <5=>GPIOF <6=>GPIOG
//    		<o2>Pin <0=>0 <1=>1 <2=>2 <3=>3 <4=>4 <5=>5 <6=>6 <7=>7 <8=>8 <9=>9 <10=>10 <11=>11 <12=>12 <13=>13 <14=>14 <15=>15
//			<o3>Active level <0=>0 <1=>1
//		</e>
#define LIN2_PULL_UP_MASTER_AVAILABLE 1
#define LIN2_PULL_UP_MASTER_PORT GPIO_PORT(1)
#define LIN2_PULL_UP_MASTER_PIN_NUM 0
#define LIN2_PULL_UP_MASTER_ON 0

#define LIN2_PULL_UP_MASTER_OFF (LIN2_PULL_UP_MASTER_ON ^ 1)
#define LIN2_PULL_UP_MASTER_PIN  LIN2_PULL_UP_MASTER_PORT,LIN2_PULL_UP_MASTER_PIN_NUM

//		<e0>pullUp slave (10k)
//			<o1>Port <0=>GPIOA <1=>GPIOB <2=>GPIOC <3=>GPIOD <4=>GPIOE <5=>GPIOF <6=>GPIOG
//    		<o2>Pin <0=>0 <1=>1 <2=>2 <3=>3 <4=>4 <5=>5 <6=>6 <7=>7 <8=>8 <9=>9 <10=>10 <11=>11 <12=>12 <13=>13 <14=>14 <15=>15
//			<o3>Active level <0=>0 <1=>1
//		</e>
#define LIN2_PULL_UP_SLAVE_AVAILABLE 1
#define LIN2_PULL_UP_SLAVE_PORT GPIO_PORT(1)
#define LIN2_PULL_UP_SLAVE_PIN_NUM 0
#define LIN2_PULL_UP_SLAVE_ON 0

#define LIN2_PULL_UP_SLAVE_OFF (LIN2_PULL_UP_SLAVE_ON ^ 1)
#define LIN2_PULL_UP_SLAVE_PIN  LIN2_PULL_UP_SLAVE_PORT,LIN2_PULL_UP_SLAVE_PIN_NUM
//	</e>

//	<e0>LIN Contactor enabled
//		<o1>Port <0=>GPIOA <1=>GPIOB <2=>GPIOC <3=>GPIOD <4=>GPIOE <5=>GPIOF <6=>GPIOG
//  	<o2>Pin <0=>0 <1=>1 <2=>2 <3=>3 <4=>4 <5=>5 <6=>6 <7=>7 <8=>8 <9=>9 <10=>10 <11=>11 <12=>12 <13=>13 <14=>14 <15=>15
//		<o3>Active level <0=>0 <1=>1
#define LIN2LIN_AVAILABLE 0
#define LIN2LIN_PORT GPIO_PORT(1)
#define LIN2LIN_PIN_NUM 0
#define LIN2LIN_ON 0

#define LIN2LIN_OFF (LIN2LIN_ON ^ 1)
#define LIN2LIN_PIN  LIN2LIN_PORT,LIN2LIN_PIN_NUM

#if   (LIN2_Driver_NUM == 1)
    #define LIN2_Driver Driver_USART1
#elif (LIN2_Driver_NUM == 2)
    #define LIN2_Driver Driver_USART2
#elif (LIN2_Driver_NUM == 3)
    #define LIN2_Driver Driver_USART3
#elif (LIN2_Driver_NUM == 4)
    #define LIN2_Driver Driver_USART4
#elif (LIN2_Driver_NUM == 5)
    #define LIN2_Driver Driver_USART5
#endif
//  </e>


//	<h>Console
//      <o0>Console physics port num <1=>UART1 <2=>UART2 <3=>UART3 <4=>UART4 <5=>UART5
//		<e1>standby port enabled
//			<o2>Port <0=>GPIOA <1=>GPIOB <2=>GPIOC <3=>GPIOD <4=>GPIOE <5=>GPIOF <6=>GPIOG
//    		<o3>Pin <0=>0 <1=>1 <2=>2 <3=>3 <4=>4 <5=>5 <6=>6 <7=>7 <8=>8 <9=>9 <10=>10 <11=>11 <12=>12 <13=>13 <14=>14 <15=>15
//			<o4>Active level <0=>0 <1=>1
//		</e>
//      <o5>Baudrate bit/s <1-2000000>
//      <o6>Parity <0=>none <1=>even <2=>odd
//      <o7>Byte length <8=>8 <9=>9
//      <o8>Stop bits <0=>1 <1=>2 <2=>1.5 <3=>0.5 
#define CON_PORT_NUM 2
#define CON_STB_AVAILABLE 0
#define CON_STB_PORT GPIO_PORT(1)
#define CON_STB_PIN_NUM 0
#define CON_STB_ON 0

#define CON_BAUD 9600
#define CON_PARITY PARITY_TYPE(0)
#define CON_BYTE_LENGTH 8
#define CON_STOP_BITS 0

#define CON_STB_OFF (CON_STB_ON ^ 1)
#define CON_STB_PIN  CON_STB_PORT,CON_STB_PIN_NUM
#if   (CON_PORT_NUM == 1)
    #define CON_UNIT USART1
#elif (CON_PORT_NUM == 2)
    #define CON_UNIT USART2
#elif (CON_PORT_NUM == 3)
    #define CON_UNIT USART3
#elif (CON_PORT_NUM == 4)
    #define CON_UNIT UART4
#elif (CON_PORT_NUM == 5)
    #define CON_UNIT UART5
#endif
//	</h>

#endif /* _UNIT_CONFIG_H_ */
