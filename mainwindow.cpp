#include "mainwindow.h"
#include "mainview.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  // Create MainView
  mainView = new MainView(this);
  setCentralWidget(mainView);

  // Create "Part A" Menu
  QMenu *partAMenu = menuBar()->addMenu("Menu for Part A");

  // 1. Rotation Action
  actRotation = new QAction("Set Rotation", this);
  connect(actRotation, SIGNAL(triggered()), this, SLOT(onRotationTriggered()));
  partAMenu->addAction(actRotation);

  // 2. View Position Action
  actViewPosition = new QAction("Set View Position", this);
  connect(actViewPosition, SIGNAL(triggered()), this,
          SLOT(onViewPositionTriggered()));
  partAMenu->addAction(actViewPosition);

  // 3. Reset Action
  actReset = new QAction("Reset View", this);
  connect(actReset, SIGNAL(triggered()), this, SLOT(onResetTriggered()));
  partAMenu->addAction(actReset);

  // 4. Animation Action
  actAnimation = new QAction("Toggle Animation", this);
  actAnimation->setCheckable(true);
  connect(actAnimation, SIGNAL(triggered()), this,
          SLOT(onAnimationTriggered()));
  partAMenu->addAction(actAnimation);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::onRotationTriggered() {
  bool ok;
  double angle = QInputDialog::getDouble(
      this, "Rotation", "Angle (degrees):", 0, -360, 360, 1, &ok);
  if (!ok)
    return;

  double dirX = QInputDialog::getDouble(this, "Rotation Axis",
                                        "Direction X:", 0, -1, 1, 1, &ok);
  if (!ok)
    return;
  double dirY = QInputDialog::getDouble(this, "Rotation Axis",
                                        "Direction Y:", 1, -1, 1, 1, &ok);
  if (!ok)
    return;
  double dirZ = QInputDialog::getDouble(this, "Rotation Axis",
                                        "Direction Z:", 0, -1, 1, 1, &ok);
  if (!ok)
    return;

  double pointX = QInputDialog::getDouble(this, "Rotation Point", "Point X:", 0,
                                          -10, 10, 1, &ok);
  if (!ok)
    return;

  mainView->setRotation((float)angle, glm::vec3((float)pointX, 0.0f, 0.0f),
                        glm::vec3((float)dirX, (float)dirY, (float)dirZ));
}

void MainWindow::onViewPositionTriggered() {
  bool ok;
  double eyeX = QInputDialog::getDouble(this, "View Position", "Eye X:", 2.0,
                                        -10, 10, 1, &ok);
  if (!ok)
    return;
  double eyeY = QInputDialog::getDouble(this, "View Position", "Eye Y:", 2.0,
                                        -10, 10, 1, &ok);
  if (!ok)
    return;
  double eyeZ = QInputDialog::getDouble(this, "View Position", "Eye Z:", 2.0,
                                        -10, 10, 1, &ok);
  if (!ok)
    return;

  mainView->setViewPosition(glm::vec3((float)eyeX, (float)eyeY, (float)eyeZ),
                            glm::vec3(0.0f, 0.0f, 0.0f));
}

void MainWindow::onResetTriggered() {
  mainView->resetView();
  actAnimation->setChecked(false);
}

void MainWindow::onAnimationTriggered() {
  mainView->toggleAnimation(actAnimation->isChecked());
}
