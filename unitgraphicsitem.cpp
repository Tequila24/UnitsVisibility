#include "unitgraphicsitem.h"
#include "unit.h"
#include <QDebug>
#include <QtMath>
#include <QVector3D>


UnitGraphicsItem::UnitGraphicsItem(int newNumber, QVector2D newPos, QVector2D newDir)
{
  number = newNumber;
  position = QPointF(newPos.x(), -newPos.y());
  float dotProduct = QVector2D::dotProduct(newDir.normalized(), QVector2D(1, 0).normalized());  // dot product
  float angleInRadians = qAcos(dotProduct); // angle in radians
  float cross = (QVector3D::crossProduct(newDir.normalized().toVector3D(), QVector3D(1, 0, 0))).z(); // angle sign
  rotationAngle = qRadiansToDegrees(angleInRadians ) * (cross>0 ? 1 : -1);
}


QRectF UnitGraphicsItem::boundingRect() const
{
	return QRectF(-50, -50, 100, 100);
}


void UnitGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  Q_UNUSED(option);
  Q_UNUSED(widget);

  painter->save();
  painter->translate(position);
  painter->scale(0.04f, 0.04f);
  painter->drawText(0, 0, QString::number(number));
  painter->rotate(rotationAngle);
  painter->drawPie(-50, -50, 100, 100, (-VIEW_ANGLE_SPREAD/2)*16, VIEW_ANGLE_SPREAD*16);
  painter->restore();
}
