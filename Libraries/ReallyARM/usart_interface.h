#ifndef USART_INTERFACE_H
#define USART_INTERFACE_H

#include "stm32f10x.h"

class USARTInterface
{
public:
  USARTInterface(USART_TypeDef *USARTx);
  void putCh(char ch) const;
  char getCh() const;
  void putString(const char *str, char separator) const;
  char * getString(char separator) const;

private:
  USART_TypeDef* _USART;
};

#endif // USART_INTERFACE_H
