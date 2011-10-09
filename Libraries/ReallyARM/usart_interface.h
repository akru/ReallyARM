#ifndef USART_INTERFACE_H
#define USART_INTERFACE_H

#include "stm32f10x.h"
#include <stdint.h>

class USARTInterface
{
public:
  USARTInterface(USART_TypeDef *USARTx);
  void putCh(uint8_t ch) const;
  uint8_t getCh() const;
  bool readyGet() const;
  bool readyPut() const;
  void eraseReadBuffer() const;
  void putString(const char *str, char separator) const;
  char * getString(char separator) const;

private:
  USART_TypeDef* _USART;
};

#endif // USART_INTERFACE_H
