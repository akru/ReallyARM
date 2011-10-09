/*******************************************************************************
* Project            : using C++ with the GNU tool-chain for
*                      ARM based controllers
* File Name          : main.cpp
* Author             : Martin Thomas
* Version            : see VERSION_STRING below
* Date               : see VERSION_STRING below
* Description        : Main program body
********************************************************************************
* License: 3BSD
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "stm32f10x.h"
#include "usart_interface.h"
#include "homer_servo_controller.h"

/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Global variables ----------------------------------------------------------*/

/* External function prototypes ----------------------------------------------*/
extern "C" char* get_heap_end( void );
extern "C" char* get_stack_top( void );

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration( void );
void GPIO_Configuration( void );
void NVIC_Configuration( void );

/* Public functions -- -------------------------------------------------------*/

/*******************************************************************************
* Function Name  : main
* Description    : Main program
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

int main( void )
{
  /* System Clocks Configuration */
  RCC_Configuration();

  /* NVIC configuration */
  NVIC_Configuration();

  /* Configure the GPIO ports */
  GPIO_Configuration();

  /* Enable Green LED */
  GPIO_SetBits( GPIOC, GPIO_Pin_9 );

  const uint8_t pos1[] =
  {
    140,  70,  60,  50, 150, // Left  hand (arm to claws)
     60, 250, 150, 240, 200, // Right hand (arm to claws)
    150, 140, 170, 230        // Body (Up to down)
  };
  const uint8_t pos2[] =
  {
    140,  70,  60,  50, 100, // Left  hand (arm to claws)
     60, 250, 150, 240, 200, // Right hand (arm to claws)
    150, 140, 170, 230        // Body (Up to down)
  };

  USARTInterface dbg(USART3);
  dbg.putString("READY", '\n');
  HOMERServoController homer(USART1);
  char *status = "HOMER: ready, initialisation complete.";
  while ( 1 )
  {
    while (dbg.getCh() != 'h');
    uint8_t cmd = dbg.getCh();
    uint8_t subcmd = dbg.getCh();
    if (cmd == 's')
    {
      if (subcmd == 'g')
      {
        dbg.putString(status, '\n');
      }
    }
    if (cmd == 'c')
    {
      if (subcmd == 's')
      {
        status = "HOMER: main program in process.";
        dbg.putString(status, '\n');
        homer.setPositionVector(pos1);
        homer.flush();
        bool set = homer.positionIsSet();
        while (!set)
        {
          for (uint32_t i = 0; i < 4000000; ++i)
            asm("nop");
          set = homer.positionIsSet();
        }
        homer.setPositionVector(pos2);
        homer.flush();
        set = homer.positionIsSet();
        while (!set)
        {
          for (uint32_t i = 0; i < 4000000; ++i)
            asm("nop");
          set = homer.positionIsSet();
        }
        status = "HOMER: main program is complete.";
      }
    }
  }
  return 0;
}


/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RCC_Configuration( void )
{
  /* Setup the microcontroller system. Initialize the Embedded Flash Interface,
     initialize the PLL and update the SystemFrequency variable. */
  SystemInit();

  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE );

  /* Enable GPIO_LED clock */
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE );
}

/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Configuration( void )
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Configure GPIO for LEDs as Output push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init( GPIOC, &GPIO_InitStructure );
}

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures Vector Table base location.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
#ifdef VECT_TAB_RAM
/* vector-offset (TBLOFF) from bottom of SRAM. defined in linker script */
extern uint32_t _isr_vectorsram_offs;
void NVIC_Configuration( void )
{
  /* Set the Vector Table base location at 0x20000000+_isr_vectorsram_offs */
  NVIC_SetVectorTable( NVIC_VectTab_RAM, ( uint32_t )&_isr_vectorsram_offs );
}

#else
extern uint32_t _isr_vectorsflash_offs;
void NVIC_Configuration( void )
{
  /* Set the Vector Table base location at 0x08000000+_isr_vectorsflash_offs */
  NVIC_SetVectorTable( NVIC_VectTab_FLASH, ( uint32_t )&_isr_vectorsflash_offs );
}

#endif /* VECT_TAB_RAM */


