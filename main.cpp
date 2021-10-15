#include <QApplication>
#include "unitsmaster.h"
#include "unitsviewer.h"


int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  UnitsMaster unitsMaster;
  UnitsViewer unitsView;

  QObject::connect(&unitsView, &UnitsViewer::SignalGenerate,
                   &unitsMaster, &UnitsMaster::GenerateUnits);

  QObject::connect(&unitsView, &UnitsViewer::SignalCalculate,
                   &unitsMaster, &UnitsMaster::CalculateVisibility);

  QObject::connect(&unitsMaster, &UnitsMaster::UnitsGeometryUpdated,
                   &unitsView, &UnitsViewer::OnUnitsGeometryUpdated);

  QObject::connect(&unitsMaster, &UnitsMaster::UnitsVisibilityUpdated,
                   &unitsView, &UnitsViewer::OnUnitsVisibilityUpdated);

  QObject::connect(&unitsView, &UnitsViewer::SignalOpenUnitsFile,
                   &unitsMaster, &UnitsMaster::OpenUnitsFile);

  QObject::connect(&unitsView, &UnitsViewer::SignalSaveUnitsFile,
                   &unitsMaster, &UnitsMaster::SaveUnitsFile);

  unitsView.show();

  return a.exec();
}


