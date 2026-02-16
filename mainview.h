#ifndef MAINVIEW_H
#define MAINVIEW_H

// GLEW must be included before any Qt OpenGL headers
#include <GL/glew.h>

#include "scenebasic.h"
#include <QOpenGLWidget>
#include <QTimer>

class MainView : public QOpenGLWidget {
  Q_OBJECT

public:
  MainView(QWidget *parent = nullptr);
  ~MainView();

  // Part A: Public methods to control the scene from the Menu
  void setRotation(float angle, glm::vec3 point, glm::vec3 direction);
  void setViewPosition(glm::vec3 eye, glm::vec3 target);
  void resetView();
  void toggleAnimation(bool animate);

protected:
  // Standard Qt OpenGL Overrides
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

private slots:
  void onTimerUpdate();

private:
  SceneBasic *scene; // The engine provided by Cranfield
  QTimer *timer;     // For the animation requirement

  // State variables for Part A
  float currentAngle;
  glm::vec3 rotationPoint;
  glm::vec3 rotationDirection;

  glm::vec3 eyePos;
  glm::vec3 targetPos;
};

#endif // MAINVIEW_H
