#include "unitsmaster.h"
#include <QDebug>
#include <QtMath>
#include <QTime>
#include <QFile>
#include <QtConcurrent/QtConcurrentRun>


#define CELLSIZE 4


UnitsMaster::UnitsMaster() : QObject()
{
  connect(this, &UnitsMaster::ChunkDone,
          this, &UnitsMaster::OnChunkDone);
}


void UnitsMaster::OpenUnitsFile(const QString fileName)
{
  units.clear();

  float biggestCoord = 0;

  QFile file(fileName);
  if (file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    QTextStream stream(&file);

    while (!stream.atEnd()) {
      QString line = stream.readLine();
      QStringList argsList = line.split(',');
      Unit* newUnit = new Unit( argsList[0].toInt(),
                                QVector2D(argsList[1].toFloat(), argsList[2].toFloat()),
                                QVector2D(argsList[3].toFloat(), argsList[4].toFloat()));
      AddUnit( newUnit );
      if (abs(newUnit->position.x()) > biggestCoord)
        biggestCoord = abs(newUnit->position.x());
      else if (abs(newUnit->position.y()) > biggestCoord)
        biggestCoord = abs(newUnit->position.y());
    }
  }
  file.close();

  emit UnitsGeometryUpdated(units.values());
}


void UnitsMaster::SaveUnitsFile(const QString fileName)
{
  QFile file(fileName);
  if (file.open(QIODevice::WriteOnly | QIODevice::Text))
  {
    QTextStream stream(&file);

    foreach (Unit* unit, units)
    {
      stream << QString("%0,%1,%2,%3,%4").arg(unit->number)
                                             .arg((unit->position.x()))
                                             .arg(unit->position.y())
                                             .arg(unit->direction.x())
                                             .arg(unit->direction.y())
             << "\n";
    }
  }
  file.close();
}


void UnitsMaster::GenerateUnits(const int unitsCount, const int areaSize)
{
  ClearUnits();

  QRandomGenerator rnd(QTime::currentTime().msecsSinceStartOfDay());
  for (int counter = 0; counter < unitsCount; ++counter)
  {
    QVector2D randomPos(rnd.generateDouble() * areaSize - areaSize/2,
                        rnd.generateDouble() * areaSize - areaSize/2);
    QVector2D randomDir(rnd.generateDouble() - 0.5f,
                        rnd.generateDouble() - 0.5f);
    randomDir.normalize();
    Unit* newUnit = new Unit(counter, randomPos, randomDir);
    AddUnit( newUnit );
  }

  emit UnitsGeometryUpdated(units.values());
}


void UnitsMaster::CalculateVisibility()
{
  eTimer.start();

  chunksRunning = 0;
  foreach (int chunk, units.uniqueKeys())
  {
    chunksRunning++;
    QtConcurrent::run(this, &UnitsMaster::ProcessChunk, units.values(chunk));
  }
}


void UnitsMaster::OnChunkDone()
{
  chunksRunning--;

  if (chunksRunning == 0) {
    qint64 timeElapsed = eTimer.elapsed();
    emit UnitsVisibilityUpdated(units.values(), timeElapsed);
  }
}


void UnitsMaster::ProcessChunk(QList<Unit*> subList)
{
  QList<Unit*> allNeighbors = GetAllNeighbors(subList.first()->position);

  foreach (Unit* currentUnit, subList)
  {
    currentUnit->visibleUnits = 0;
    foreach (Unit* anotherUnit, allNeighbors )
    {
      if (currentUnit == anotherUnit) {
        continue;
      }

      QVector2D localPositionAnother = (anotherUnit->position - currentUnit->position);
      if ( localPositionAnother.lengthSquared() < VIEW_DISTANCE_SQUARED)
      {
        float dotProduct = QVector2D::dotProduct(currentUnit->direction.normalized(),
                                                 localPositionAnother.normalized()); // dot
        float angleInRadians = qAcos(dotProduct); // angle in radians
        if (angleInRadians < (VIEW_ANGLE_SPREAD_RADIANS/2))
        {
          currentUnit->visibleUnits++;
        }
      }
    }
  }

  emit ChunkDone();
}


void UnitsMaster::ClearUnits()
{
  foreach (Unit* unit, units) {
    delete unit;
  }
  units.clear();
}

QList<Unit *> UnitsMaster::GetAllNeighbors(QVector2D position)
{
  QSet<int> neighborKeys;

  float margin = CELLSIZE;
  neighborKeys.insert(CellNumber(position));
  neighborKeys.insert(CellNumber(position + QVector2D(-margin, margin)));
  neighborKeys.insert(CellNumber(position + QVector2D(0, margin)));
  neighborKeys.insert(CellNumber(position + QVector2D(margin, margin)));
  neighborKeys.insert(CellNumber(position + QVector2D(margin, 0)));
  neighborKeys.insert(CellNumber(position + QVector2D(margin, -margin)));
  neighborKeys.insert(CellNumber(position + QVector2D(0, -margin)));
  neighborKeys.insert(CellNumber(position + QVector2D(-margin, -margin)));
  neighborKeys.insert(CellNumber(position + QVector2D(-margin, 0)));

  QList<Unit*> neighborUnits;

  foreach (int hashKey, neighborKeys)
  {
    neighborUnits += units.values(hashKey);
  }

  return neighborUnits;
}

void UnitsMaster::AddUnit(Unit* newUnit)
{
  units.insert(CellNumber(newUnit->position), newUnit);
}

int UnitsMaster::CellNumber(QVector2D position)
{
  return (qFloor(position.x() / CELLSIZE) * 73856093) ^
         (qFloor(position.y() / CELLSIZE) * 19349663);
}
