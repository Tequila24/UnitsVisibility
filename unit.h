#ifndef UNIT_H
#define UNIT_H

#include <QObject>
#include <QVector2D>

#define VIEW_ANGLE_SPREAD 135.5f
#define VIEW_ANGLE_SPREAD_RADIANS (VIEW_ANGLE_SPREAD*(M_PI / 180))
#define VIEW_DISTANCE 2
#define VIEW_DISTANCE_SQUARED (VIEW_DISTANCE*VIEW_DISTANCE)


class Unit
{

public:
  Unit(int number, QVector2D newPosition, QVector2D newDirection);

  const int number;
  const QVector2D position;
  const QVector2D direction;

  int visibleUnits;
};

#endif // UNIT_H
