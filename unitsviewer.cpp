#include "unitsviewer.h"
#include "unitgraphicsitem.h"
#include <QFileDialog>
#include <QDebug>
#include <QScrollBar>

#define BUTTON_WIDTH 100
#define BUTTON_HEIGHT BUTTON_WIDTH/4


UnitsViewer::UnitsViewer(QWidget *parent) : QWidget(parent)
{
  this->setFixedSize(1140, 620);

  GenerateButton = new QPushButton("Generate Units", this);
  GenerateButton->setGeometry(10,
                              10,
                              BUTTON_WIDTH,
                              BUTTON_HEIGHT);

  OpenFileButton = new QPushButton("Open Units File", this);
  OpenFileButton->setGeometry(GenerateButton->pos().x(),
                              GenerateButton->pos().y() + BUTTON_HEIGHT*2,
                              BUTTON_WIDTH,
                              BUTTON_HEIGHT);

  SaveFileButton = new QPushButton("Save Units File", this);
  SaveFileButton->setGeometry(OpenFileButton->pos().x(),
                              OpenFileButton->pos().y() + BUTTON_HEIGHT*2,
                              BUTTON_WIDTH,
                              BUTTON_HEIGHT);

  CalculateButton = new QPushButton("Calculate Visibility", this);
  CalculateButton->setGeometry(SaveFileButton->pos().x(),
                               SaveFileButton->pos().y() + BUTTON_HEIGHT*2,
                               BUTTON_WIDTH,
                               BUTTON_HEIGHT);

  UnitsCounter = new QSpinBox(this);
  UnitsCounter->setButtonSymbols(QAbstractSpinBox::PlusMinus);
  UnitsCounter->setGeometry(CalculateButton->pos().x(),
                           CalculateButton->pos().y() + BUTTON_HEIGHT*3,
                           BUTTON_WIDTH,
                           BUTTON_HEIGHT);
  UnitsCounter->setMinimum(1);
  UnitsCounter->setMaximum(10000);
  UnitsCounter->setSingleStep(1000);
  UnitsCounter->setValue(1000);
  QLabel* UnitsCounterLabel = new QLabel("Units count", this);
  UnitsCounterLabel->setGeometry(UnitsCounter->x(),
                                 UnitsCounter->y() - BUTTON_HEIGHT,
                                 BUTTON_WIDTH,
                                 BUTTON_HEIGHT);

  AreaMultiplier = new QSpinBox(this);
  AreaMultiplier->setButtonSymbols(QAbstractSpinBox::PlusMinus);
  AreaMultiplier->setGeometry(UnitsCounter->pos().x(),
                           UnitsCounter->pos().y() + BUTTON_HEIGHT*2,
                           BUTTON_WIDTH,
                           BUTTON_HEIGHT);
  AreaMultiplier->setMinimum(1);
  AreaMultiplier->setMaximum(100);
  AreaMultiplier->setSingleStep(1);
  AreaMultiplier->setValue(10);
  QLabel* AreaMultiplierLabel = new QLabel("Area size", this);
  AreaMultiplierLabel->setGeometry(AreaMultiplier->x(),
                                   AreaMultiplier->y() - BUTTON_HEIGHT,
                                   BUTTON_WIDTH,
                                   BUTTON_HEIGHT);

  QLabel* buttonsLabel = new QLabel("Home to zoom in, \nEnd to zoom out", this);
  buttonsLabel->move(AreaMultiplier->pos().x(),
                     AreaMultiplier->pos().y() + BUTTON_HEIGHT*3);

  SceneView = new QGraphicsView(new QGraphicsScene(), this);
  SceneView->setGeometry(120, 10, 600, 600);
  SceneView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  SceneView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

  tEdit = new QTextEdit(this);
  tEdit->setGeometry(SceneView->geometry().topRight().x() + 10,
                     SceneView->rect().topRight().y()+10,
                     400, 600);

  connect(GenerateButton, &QPushButton::clicked,
          this, &UnitsViewer::OnGenerateClicked);

  connect(OpenFileButton, &QPushButton::clicked,
          this, &UnitsViewer::OnOpenFileClicked);

  connect(SaveFileButton, &QPushButton::clicked,
          this, &UnitsViewer::OnSaveFileClicked);

  connect(CalculateButton, &QPushButton::clicked,
          this, &UnitsViewer::OnCalculateClicked);
}

void UnitsViewer::OnGenerateClicked()
{
  emit SignalGenerate(UnitsCounter->value(), AreaMultiplier->value());
}

void UnitsViewer::OnOpenFileClicked()
{
  QString fileName = QFileDialog::getOpenFileName(this, "Open units file", "/", "TXT files (*.txt)");
  if (!fileName.isEmpty())
    emit SignalOpenUnitsFile(fileName);
}

void UnitsViewer::OnSaveFileClicked()
{
  QString fileName = QFileDialog::getSaveFileName(this, "Save units file", "/", "TXT files (*.txt)");
  if (!fileName.isEmpty())
    emit SignalSaveUnitsFile(fileName);
}

void UnitsViewer::OnCalculateClicked()
{
  emit SignalCalculate();
}

void UnitsViewer::OnUnitsGeometryUpdated(const QList<Unit*> units)
{
  SceneView->scene()->clear();
  float biggestCoord = 0;

  auto iter = units.cbegin();
  while (iter != units.cend())
  {
	QGraphicsItem* item =  new UnitGraphicsItem((*iter)->number, (*iter)->position, (*iter)->direction );
	SceneView->scene()->addItem(item);

    if (abs((*iter)->position.x()) > biggestCoord)
      biggestCoord = abs((*iter)->position.x());
    else if (abs((*iter)->position.y()) > biggestCoord)
      biggestCoord = abs((*iter)->position.y());

    iter++;
  }

  SceneView->scene()->setSceneRect(-biggestCoord,
                                   -biggestCoord,
                                   biggestCoord*2,
                                   biggestCoord*2);

  SceneView->resetMatrix();
  fitScale = (float)SceneView->size().width() / ((float)AreaMultiplier->value()*1.2f);
  SceneView->scale(fitScale, fitScale);
}

void UnitsViewer::OnUnitsVisibilityUpdated(const QList<Unit*> units, qint64 timeElapsed)
{
  tEdit->clear();
  auto iter = units.cbegin();
  while (iter != units.cend())
  {
    tEdit->append(QString("Unit #%0 sees %1 another units").arg((*iter)->number).arg((*iter)->visibleUnits));
    iter++;
  }
  tEdit->append(" ");
  tEdit->append(QString("Visibility calculated in %0 msecs").arg(timeElapsed));
}

void UnitsViewer::keyPressEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_Home)
    SceneView->setMatrix(SceneView->matrix().scale(1.1, 1.1));
  else if (event->key() == Qt::Key_End)
    SceneView->setMatrix(SceneView->matrix().scale(0.9, 0.9));

  event->accept();
}
