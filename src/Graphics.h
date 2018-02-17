#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <set>
#include <map>
#include <string>

#include "Core.h"

#include <GL/glew.h>

#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SOIL.h>

#include "shader_utils.h"


class Graphics;
class Model;

class Graphics : public System
{
public:
  static Graphics * Instance();
  virtual void Update(float dt);
  
//private:
  Graphics();
  ~Graphics();
  int init_resources();
  void idle();
  void onDisplay();
  void onReshape(int width, int height);
  void free_resources();

  friend class Model;
  std::set<Model*> m_components;

  // todo: stuff
  int screen_width;
  int screen_height;

  GLuint vbo_cube_verts , vbo_cube_texcoords;
  GLuint ibo_cube_elements;
  GLuint program;
  GLuint texture_id;
  GLint attribute_coord3d, attribute_texcoord;
  GLint uniform_mvp, uniform_myTexture;
};

class Model : public Component
{
public:
  Model();
  ~Model();
private:
};
#endif
