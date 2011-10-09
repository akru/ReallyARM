#include "usart_interface.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "utils.h"
#include <stdint.h>

USARTInterface::USARTInterface(USART_TypeDef *USARTx)
  : _USART(USARTx)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  if ( USARTx == USART1 )
  {
    /* Enable USART1, GPIOA and AFIO clocks */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 |
                           RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    /* Configure USART1 TX (PA.09) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    /* Configure USART1 RX (PA.10) as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
  }
  else
  {
    if (USARTx == USART2)
    {
      /* Enable USART2, GPIOA and AFIO clocks */
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
      /* Configure USART2 TX (PA.02) as alternate function push-pull */
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
      GPIO_Init(GPIOA, &GPIO_InitStructure);
      /* Configure USART2 RX (PA.03) as input floating */
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
      GPIO_Init(GPIOA, &GPIO_InitStructure);
    }
    else
      if (USARTx == USART3)
      {
        /* Enable USART3, GPIOB and AFIO clocks */
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
        /* Configure USART3 TX (PB.10) as alternate function push-pull */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
        /* Configure USART3 RX (PB.11) as input floating */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
      }
  }

  USART_InitTypeDef USART_InitStructure;
  /* USART configured as follow:
   - BaudRate = 115000 baud
   - Word Length = 8 Bits
   - One Stop Bit
   - No parity
   - Hardware flow control disabled (RTS and CTS signals)
   - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  /* Configure USART */
  USART_Init(USARTx, &USART_InitStructure);

  /* Enable the USART */
  USART_Cmd(USARTx, ENABLE);
}

void USARTInterface::putCh(uint8_t ch) const
{
  while (!readyPut());
  USART_SendData(_USART, ch);
}

uint8_t USARTInterface::getCh() const
{
  while (!readyGet());
  return USART_ReceiveData(_USART);
}

bool USARTInterface::readyPut() const
{
  if (USART_GetFlagStatus(_USART, USART_FLAG_TXE) == RESET)
    return false;
  return true;
}

bool USARTInterface::readyGet() const
{
  if (USART_GetFlagStatus(_USART, USART_FLAG_RXNE) == RESET)
    return false;
  return true;
}

void USARTInterface::eraseReadBuffer() const
{
  while (readyGet())
    getCh();
}

void USARTInterface::putString(const char *str, char separator) const
{
  while (*str)
    putCh(*str++);
  putCh(separator);
}

char * USARTInterface::getString(char separator) const
{
  const uint8_t bufSize = 255;
  char buf[bufSize];
  uint8_t i = 0;
  while (char ch = getCh() != separator && i < bufSize)
    buf[i++] = ch;
  if (i == bufSize)
    return 0;
  buf[i] = 0;
  char *out = new char[++i];
  String::strcpy(buf, out);
  return out;
}
