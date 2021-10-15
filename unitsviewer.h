#ifndef UNITSVIEWER_H
#define UNITSVIEWER_H

#include <QWidget>
#include <QSpinBox>
#include <QPushButton>
#include <QGraphicsView>
#include <QLabel>
#include <QVector>
#include "unit.h"
#include <QGraphicsPixmapItem>
#include <QtMath>
#include <QPainter>
#include <QWheelEvent>
#include <QTextEdit>


class UnitsViewer : public QWidget
{
  Q_OBJECT
public:
  explicit UnitsViewer(QWidget *parent = nullptr);

private:
  QSpinBox* UnitsCounter;
  QSpinBox* AreaMultiplier;
  QPushButton* GenerateButton;
  QPushButton* OpenFileButton;
  QPushButton* SaveFileButton;
  QPushButton* CalculateButton;
  QGraphicsView* SceneView;
  QTextEdit* tEdit;
  void keyPressEvent(QKeyEvent* event);
  void ZoomSceneIn();
  void ZoomSceneOut();
  float fitScale;

signals:
  void SignalGenerate(int unitsCount, int areaSize);
  void SignalCalculate();
  void SignalOpenUnitsFile(QString fileName);
  void SignalSaveUnitsFile(QString fileName);

public slots:
  void OnGenerateClicked();
  void OnOpenFileClicked();
  void OnSaveFileClicked();
  void OnCalculateClicked();

  void OnUnitsGeometryUpdated(const QList<Unit*> units);
  void OnUnitsVisibilityUpdated(const QList<Unit*> units, qint64 timeElapsed);
};

#endif // UNITSVIEWER_H
