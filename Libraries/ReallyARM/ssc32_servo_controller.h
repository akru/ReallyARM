#ifndef SSC32_SERVO_CONTROLLER_H
#define SSC32_SERVO_CONTROLLER_H

#include "usart_interface.h"
#include <stdint.h>

class SSC32ServoController
    : public USARTInterface
{
public:
  SSC32ServoController(USART_TypeDef *USARTx, uint8_t numServo);
  ~SSC32ServoController();
  void setPositionVector(const uint16_t *vect);
  void setTimeVector(const uint16_t *vect);
  void flush() const;

private:
  const uint8_t _numServo;
  uint16_t *_posVect;
  uint16_t *_timeVect;
};

#endif // SSC32_SERVO_CONTROLLER_H
