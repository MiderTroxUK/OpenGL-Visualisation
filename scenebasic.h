#ifndef SCENEBASIC_H
#define SCENEBASIC_H

#include "glslprogram.h"
#include "scene.h"
#include <GL/glew.h>
#include <glm/glm.hpp>

using glm::mat4;

class SceneBasic : public Scene {
private:
  int width, height;
  GLuint vboHandles[3];
  GLuint vaoHandle;
  GLSLProgram prog;

  glm::mat4 model;
  glm::mat4 view;
  glm::mat4 projection;

  void readData(const char *fname);
  void CreateVBO();
  void setMatrices();

  float positionData[24];
  float colorData[24];
  unsigned int indexData[36];

public:
  SceneBasic();
  virtual ~SceneBasic() {}

  void initScene();
  void render();
  void resize(int, int);
  void update(float t);

  void setModel(glm::mat4 m) { model = m; }
  void setView(glm::mat4 v) { view = v; }
  void setProjection(glm::mat4 p) { projection = p; }

  void printActiveUniforms(GLuint programHandle);
  void printActiveAttribs(GLuint programHandle);
};

#endif // SCENEBASIC_H
