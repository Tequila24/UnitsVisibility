#ifndef UNITSMASTER_H
#define UNITSMASTER_H

#include <QObject>
#include "unit.h"
#include <QVector>
#include <QMultiMap>
#include <QRandomGenerator>
#include <QElapsedTimer>
#include "unitsviewer.h"


class UnitsMaster : public QObject
{
  Q_OBJECT

public:
  explicit UnitsMaster();

private:
  void AddUnit(Unit* newUnit);
  void ClearUnits();
  int CellNumber(QVector2D position);
  QList<Unit*> GetAllNeighbors(QVector2D position);

  void ProcessUnit(Unit* currentUnit);
  void ProcessChunk(QList<Unit*> subList);
  QMultiHash<int, Unit*> units;  // SPH
  uint chunksRunning;
  uint fieldSize;

  QElapsedTimer eTimer;

signals:
  void UnitsGeometryUpdated(const QList<Unit*> units);
  void UnitsVisibilityUpdated(const QList<Unit*> units, qint64 timeElapsed);
  void ChunkDone();

public slots:
  void OpenUnitsFile(const QString fileName);
  void SaveUnitsFile(const QString fileName);
  void GenerateUnits(const int unitsCount, const int areaSize);
  void CalculateVisibility();
  void OnChunkDone();
};

#endif // UNITSMASTER_H
