#include "ssc32_servo_controller.h"
#include "utils.h"

SSC32ServoController::SSC32ServoController(
    USART_TypeDef *USARTx, unsigned char numServo)
  : USARTInterface(USARTx), _numServo(numServo),
    _posVect(new uint8_t[numServo]), _timeVect(new uint16_t[numServo])
{
  putString("VER", '\r');
}

SSC32ServoController::~SSC32ServoController()
{
  delete [] _posVect;
  delete [] _timeVect;
}

void SSC32ServoController::setPositionVector(const uint8_t *vect)
{
  for (uint8_t i = 0; i < _numServo; ++i)
    _posVect[i] = *vect++;
}

void SSC32ServoController::setTimeVector(const uint16_t *vect)
{
  for (uint8_t i = 0; i < _numServo; ++i)
    _timeVect[i] = *vect++;
}

void SSC32ServoController::flush() const
{
  for (uint8_t i = 0; i < _numServo; ++i)
  {
    //-- Select servo -------------------
    putCh('#');
    char *str = String::number(i);
    putString(str, ' ');
    delete [] str;
    //-----------------------------------

    //-- Select position ----------------
    putCh('P');
    str = String::number(_posVect[i] * 10);
    putString(str, ' ');
    delete [] str;
    //-----------------------------------

    //-- Select time --------------------
    putCh('T');
    str = String::number(_timeVect[i]);
    putString(str, ' ');
    delete [] str;
    //-----------------------------------
  }
  putCh('\r'); // Terminate cmd string
}

uint8_t * SSC32ServoController::getRealState() const
{
  uint8_t *realState = new uint8_t[_numServo];
  uint8_t *p = realState;
  eraseReadBuffer();
  for (uint8_t i = 0; i < _numServo; ++i)
  {
    //-- Query servo pulse --------------
    char *arg = String::number(i);
    putString("QP", ' ');
    putString(arg, '\r');
    *p++ = getCh();
    delete [] arg;
    //-----------------------------------
  }
  return realState;
}

bool SSC32ServoController::positionIsSet() const
{
  bool res;
  uint8_t *realPosition = getRealState();
  res = Math::vectEqu(_posVect, realPosition, _numServo, 3);
  delete [] realPosition;
  return res;
}
