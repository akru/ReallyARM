#include "homer_servo_controller.h"

const uint16_t HOMERServoController::defaultPosition[] =
{
  2400,  700, 1500,  500, 1000, // Left  hand (arm to claws)
   600, 2500, 1500, 2400, 2000, // Right hand (arm to claws)
  1500, 1500,                   // Track drive
  1500, 1400, 1700, 2300        // Body (Up to down)
};

const uint16_t HOMERServoController::defaultTime[] =
{
  2000, 2000, 2000, 2000, 2000,
  2000, 2000, 2000, 2000, 2000,
  2000, 2000,
  2000, 2000, 2000, 2000
};

HOMERServoController::HOMERServoController(USART_TypeDef *USARTx)
  : SSC32ServoController(USARTx, numServo)
{
  setDefaultState();
}

void HOMERServoController::setDefaultState()
{
  setPositionVector(defaultPosition);
  setTimeVector(defaultTime);
  flush();
}
