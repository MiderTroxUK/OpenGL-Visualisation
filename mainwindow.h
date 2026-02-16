#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>


class MainView; // Forward declaration

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void onRotationTriggered();
  void onViewPositionTriggered();
  void onResetTriggered();
  void onAnimationTriggered();

private:
  Ui::MainWindow *ui;
  MainView *mainView;

  QAction *actRotation;
  QAction *actViewPosition;
  QAction *actReset;
  QAction *actAnimation;
};
#endif // MAINWINDOW_H
