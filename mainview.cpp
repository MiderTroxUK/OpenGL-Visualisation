#include "mainview.h"
#include "glutils.h"
#include <iostream>

// Fix for GLM experimental extension error
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

MainView::MainView(QWidget *parent) : QOpenGLWidget(parent) {
  // Part A Requirement: Animation Timer
  timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(onTimerUpdate()));
  timer->setInterval(16); // ~60 FPS

  // Initialize state variables
  currentAngle = 0.0f;
  rotationPoint = glm::vec3(0.0f);
  rotationDirection = glm::vec3(0.0f, 1.0f, 0.0f);

  eyePos = glm::vec3(0.0f, 0.0f, 3.0f);
  targetPos = glm::vec3(0.0f, 0.0f, 0.0f);
}

MainView::~MainView() {
  delete scene;
  // No need to delete timer, it is a child of 'this'
}

void MainView::initializeGL() {
  // 1. Initialize GLEW (The standard way)
  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (GLEW_OK != err) {
    std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
  }

  // 2. Create the Scene
  scene = new SceneBasic();

  // 3. Initialize the Scene (Compiles shaders, loads buffers)
  scene->initScene();
}

void MainView::paintGL() {
  if (scene) {
    scene->render();
  }
}

void MainView::resizeGL(int w, int h) {
  if (scene) {
    scene->resize(w, h);
  }
}

void MainView::onTimerUpdate() {
  // Increment angle for animation
  currentAngle += 1.0f;
  if (currentAngle > 360.0f)
    currentAngle -= 360.0f;

  // Apply rotation with new angle
  setRotation(currentAngle, rotationPoint, rotationDirection);
}

// Part A: Public methods

void MainView::setRotation(float angle, glm::vec3 point, glm::vec3 direction) {
  // Store state for animation
  currentAngle = angle;
  rotationPoint = point;
  rotationDirection = direction;

  // 1. Calculate Translation T (-point)
  glm::mat4 T_inv = glm::translate(glm::mat4(1.0f), -point);

  // 2. Calculate Rotation R (angle about direction)
  glm::mat4 R = glm::rotate(glm::mat4(1.0f), glm::radians(angle), direction);

  // 3. Calculate Translation T_back (+point)
  glm::mat4 T = glm::translate(glm::mat4(1.0f), point);

  // 4. Combine M = T * R * T_inv
  // Order: Translate to origin -> Rotate -> Translate back
  glm::mat4 model = T * R * T_inv;

  if (scene) {
    scene->setModel(model);
    update(); // Request repaint
  }
}

void MainView::setViewPosition(glm::vec3 eye, glm::vec3 target) {
  eyePos = eye;
  targetPos = target;

  glm::mat4 view = glm::lookAt(eye, target, glm::vec3(0.0f, 1.0f, 0.0f));

  if (scene) {
    scene->setView(view);
    update();
  }
}

void MainView::resetView() {
  // Reset state
  currentAngle = 0.0f;
  rotationPoint = glm::vec3(0.0f);
  rotationDirection = glm::vec3(0.0f, 1.0f, 0.0f);
  eyePos = glm::vec3(0.0f, 0.0f, 3.0f);
  targetPos = glm::vec3(0.0f, 0.0f, 0.0f);

  // Reset Matrices
  if (scene) {
    scene->setModel(glm::mat4(1.0f));
    scene->setView(glm::lookAt(eyePos, targetPos, glm::vec3(0.0f, 1.0f, 0.0f)));
    update();
  }

  timer->stop();
}

void MainView::toggleAnimation(bool animate) {
  if (animate)
    timer->start();
  else
    timer->stop();
}
