#include "homer_servo_controller.h"
#include "utils.h"

const uint8_t HOMERServoController::defaultPosition[] =
{
  240,  70, 150,  50, 100, // Left  hand (arm to claws)
   60, 250, 150, 240, 200, // Right hand (arm to claws)
  150, 140, 170, 230        // Body (Up to down)
};

const uint16_t HOMERServoController::defaultTime[] =
{
  2000, 2000, 2000, 2000, 2000,
  2000, 2000, 2000, 2000, 2000,
  2000, 2000, 2000, 2000,
};

HOMERServoController::HOMERServoController(USART_TypeDef *USARTx)
  : SSC32ServoController(USARTx, numServo)
{
  setDefaultState();
  bool set = positionIsSet();
  while (!set)
  {
    for (uint32_t i = 0; i < 4000000; ++i)
      asm("nop");
    set = positionIsSet();
  }
}

void HOMERServoController::setDefaultState()
{
  setPositionVector(defaultPosition);
  setTimeVector(defaultTime);
  flush();
}
