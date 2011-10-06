#ifndef HOMER_SERVO_CONTROLLER_H
#define HOMER_SERVO_CONTROLLER_H

#include "ssc32_servo_controller.h"
#include <stdint.h>

class HOMERServoController
    : public SSC32ServoController
{
public:
  HOMERServoController(USART_TypeDef *USARTx);
  void setDefaultState();

private:
  static const uint8_t numServo = 16;
  static const uint16_t defaultPosition[];
  static const uint16_t defaultTime[];
};

#endif // HOMER_SERVO_CONTROLLER_H
