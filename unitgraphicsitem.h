#ifndef UNITGRAPHICSITEM_H
#define UNITGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QVector2D>
#include <QPainter>
#include <QPointF>


class UnitGraphicsItem : public QGraphicsItem
{

public:
  UnitGraphicsItem(int newNumber, QVector2D newPos, QVector2D newDir);

  QRectF boundingRect() const override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
  int number;
  QPointF position;
  float rotationAngle;
};

#endif // UNITGRAPHICSITEM_H
