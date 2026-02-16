#include "mainwindow.h"
#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  // Set Surface Format for OpenGL 4.0 Core Profile
  QSurfaceFormat format;
  format.setVersion(4, 0);
  format.setProfile(QSurfaceFormat::CoreProfile);
  QSurfaceFormat::setDefaultFormat(format);

  MainWindow w;
  w.show();

  return a.exec();
}
