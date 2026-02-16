#include "scenebasic.h"

#include <cstdio>
#include <cstdlib>

#include <iostream>

#include <fstream>
using std::ifstream;

#include <sstream>
using std::ostringstream;

#include <glm/gtc/matrix_transform.hpp>

SceneBasic::SceneBasic() {
  readData("shader/scenebasic.dat");

  // Initialize matrices
  model = glm::mat4(1.0f);

  // Default Camera Position (Eye at 0,0,3 looking at 0,0,0)
  // This ensures the object is visible on startup
  view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                     glm::vec3(0.0f, 1.0f, 0.0f));

  projection = glm::mat4(1.0f);
}

void SceneBasic::readData(const char *fname) {
  ifstream ifs(fname);

  if (!ifs) {
    std::cout << "data file not found\n";
    exit(1);
  } else {
    // 8 vertices * 3 components = 24
    for (int i = 0; i < 24; i++)
      ifs >> positionData[i];
    for (int i = 0; i < 24; i++)
      ifs >> colorData[i];
    // 12 triangles * 3 indices = 36
    for (int i = 0; i < 36; i++)
      ifs >> indexData[i];
  }
}

void SceneBasic::CreateVBO() {
  // Create and populate the buffer objects
  glGenBuffers(3, vboHandles);

  GLuint positionBufferHandle = vboHandles[0];
  GLuint colorBufferHandle = vboHandles[1];

  // bind positionBufferHandle to GL_ARRAY_BUFFER buffer object target
  glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
  // creates and initializes GL_ARRAY_BUFFER buffer object's data store
  glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), positionData,
               GL_STATIC_DRAW);

  // bind colorBufferHandle to GL_ARRAY_BUFFER target
  glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
  glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), colorData, GL_STATIC_DRAW);

  // bind vboHandles[2] to GL_ELEMENT_ARRAY_BUFFER target
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboHandles[2]);
  // creates and initializes GL_ELEMENT_ARRAY_BUFFER buffer object's data
  // store
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(unsigned int), indexData,
               GL_STATIC_DRAW);

  // Create and set-up the vertex array object
  glGenVertexArrays(1, &vaoHandle);
  // bind the vertex array object
  glBindVertexArray(vaoHandle);

  /* enables the generic vertex attribute array
     the values in the generic vertex attribute array will be accessed
     and used for rendering when calls are made to vertex array commands
     such as glDrawArrays */
  glEnableVertexAttribArray(0); // Vertex position
  glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
  /* define an array of generic vertex attribute data
     specifies the location and data format of the array of generic
     vertex attributes at index 0 to use when rendering */
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

  glEnableVertexAttribArray(1); // Vertex color
  glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
}

void SceneBasic::initScene() {
  //////////////////////////////////////////////////////
  /////////// Vertex shader //////////////////////////
  //////////////////////////////////////////////////////

  /* The OpenGL Extension Wrangler Library (GLEW) is a cross-platform
   open-source C/C++ extension loading library. GLEW provides efficient
   run-time mechanisms for determining which OpenGL extensions are supported
   on the target platform.

   if you link with some function - it must exist on target platform,
   or your program wouldn't launch. GLEW handles that - you're not linking
   with GL functions directly, but instead getting function pointers after
   initialization phase. It allows you to check at runtime which extensions
   are present and which functions may be used.

   GLEW obtains information on the supported extensions from the graphics
   driver. Experimental or pre-release drivers, however, might not report every
   available extension through the standard mechanism, in which case GLEW will
   report it unsupported. To circumvent this situation, the glewExperimental
   global switch can be turned on by setting it to GL_TRUE before calling
   glewInit(), which ensures that all extensions with valid entry points will be
   exposed. */

  glewExperimental = GL_TRUE;
  glewInit();

  // Create the vertex shader object
  GLuint vertShader = prog.createShader(GL_VERTEX_SHADER);

  // Check status
  if (0 == vertShader) {
    fprintf(stderr, "Error creating vertex shader.\n");
    exit(1);
  }

  // load the shader from the file
  prog.load_shader(vertShader, "shader/basic.vert");

  // compile the shader
  prog.compileShader(vertShader);

  // Check compilation status
  prog.checkCompileStatus(vertShader);

  //////////////////////////////////////////////////////
  /////////// Fragment shader //////////////////////////
  //////////////////////////////////////////////////////

  // Create the fragment shader object
  GLuint fragShader = prog.createShader(GL_FRAGMENT_SHADER);

  // Check status
  if (0 == fragShader) {
    fprintf(stderr, "Error creating fragment shader.\n");
    exit(1);
  }

  // load the shader from the file
  prog.load_shader(fragShader, "shader/basic.frag");

  // compile the shader
  prog.compileShader(fragShader);

  // Check compilation status
  prog.checkCompileStatus(fragShader);

  // Create the program object
  if (!prog.createObject())
    printf("error\n");

  // attach shaders to program object
  prog.attachShader(vertShader);
  prog.attachShader(fragShader);

  // Bind index 0 to the shader input variable "VertexPosition"
  prog.bindAttribLocation(0, "VertexPosition");
  // Bind index 1 to the shader input variable "VertexColor"
  prog.bindAttribLocation(1, "VertexColor");

  // link the program
  prog.link();

  // create and populate the vertex buffer opbject
  CreateVBO();

  prog.printActiveUniforms();
  glClearColor(0.0, 0.0, 0.0, 1.0);
}

void SceneBasic::update(float) {}

void SceneBasic::setMatrices() {
  prog.setUniform("ModelMatrix", model);
  prog.setUniform("ViewMatrix", view);
  prog.setUniform("ProjectionMatrix", projection);
}

void SceneBasic::render() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  prog.use();
  setMatrices();

  glBindVertexArray(vaoHandle);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboHandles[2]);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

  glBindVertexArray(0);
}

void SceneBasic::resize(int w, int h) {
  glViewport(0, 0, w, h);
  width = w;
  height = h;

  float fov = glm::radians(60.0f);
  float aspect = (float)w / (float)h;

  projection = glm::perspective(fov, aspect, 0.1f, 100.0f);
}

void SceneBasic::printActiveUniforms(GLuint programHandle) {

  GLint nUniforms, size, location, maxLen;
  GLchar *name;
  GLsizei written;
  GLenum type;

  glGetProgramiv(programHandle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLen);
  glGetProgramiv(programHandle, GL_ACTIVE_UNIFORMS, &nUniforms);

  name = (GLchar *)malloc(maxLen);

  printf(" Location | Name\n");
  printf("------------------------------------------------\n");
  for (int i = 0; i < nUniforms; ++i) {
    glGetActiveUniform(programHandle, i, maxLen, &written, &size, &type, name);
    location = glGetUniformLocation(programHandle, name);
    printf(" %-8d | %s\n", location, name);
  }

  free(name);
}

void SceneBasic::printActiveAttribs(GLuint programHandle) {

  GLint written, size, location, maxLength, nAttribs;
  GLenum type;
  GLchar *name;

  glGetProgramiv(programHandle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength);
  glGetProgramiv(programHandle, GL_ACTIVE_ATTRIBUTES, &nAttribs);

  name = (GLchar *)malloc(maxLength);

  printf(" Index | Name\n");
  printf("------------------------------------------------\n");
  for (int i = 0; i < nAttribs; i++) {
    glGetActiveAttrib(programHandle, i, maxLength, &written, &size, &type,
                      name);
    location = glGetAttribLocation(programHandle, name);
    printf(" %-5d | %s\n", location, name);
  }

  free(name);
}
