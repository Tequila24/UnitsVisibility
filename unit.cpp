#include "unit.h"


Unit::Unit(int number, QVector2D newPosition, QVector2D newDirection) :
  number(number),
  position(newPosition),
  direction(newDirection)
{
  visibleUnits = 0;
}
