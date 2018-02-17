#include "Graphics.h"
#include "debug.h"
#include <iostream>

void g_ondisplay()
{
  Graphics::Instance()->onDisplay();
}
void g_onidle()
{
  Graphics::Instance()->idle();
}
void g_onreshape(int width, int height)
{
  Graphics::Instance()->onReshape(width, height);
}

Graphics::Graphics()
  : screen_width(800)
  , screen_height(600) // todo: make into data (load from file?)
{
  // int foo; glGetIntegerv(GL_MAJOR_VERSION, &foo); std::cout << foo; // 3

  // Glut-related initialising functions
  int argc = 0;
  char * argv = 0;
  glutInit(&argc,  &argv);
  glutInitDisplayMode(GLUT_RGBA|GLUT_ALPHA|GLUT_DOUBLE|GLUT_DEPTH);
  glutInitWindowSize(screen_width, screen_height);
  glutCreateWindow("My First Open GL Triangle");

  // Extension wrangler initialising
  GLenum glew_status = glewInit();
  ErrorIf(glew_status != GLEW_OK, 
    "Error: %s\n", glewGetErrorString(glew_status));
  ErrorIf(!GLEW_VERSION_2_0, "Your graphics card doesn't support OpenGl 2.0");

  ErrorIf(init_resources() != 1, "failed to initialize graphics");
  glutDisplayFunc(g_ondisplay);
  glutReshapeFunc(g_onreshape);
  glutIdleFunc(g_onidle);
  glEnable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
Graphics * Graphics::Instance()
{
  static Graphics * instance;
  return instance ? instance : instance = new Graphics();
}
Graphics::~Graphics()
{
  free_resources();
}
void Graphics::Update(float dt)
{
  for (auto it = m_components.begin(); it != m_components.end(); ++it)
  {
    Model * component = *it;
  }

  //idle();
  //onDisplay();
  glutMainLoop();
}

Model::Model()
{
  Graphics::Instance()->m_components.insert(this);
}
Model::~Model()
{
  Graphics::Instance()->m_components.erase(this);
}


// Returns 1 when all is ok, 0 with a displayed error
int Graphics::init_resources(void)
{
  GLfloat cube_vertices[] = 
  {
    // front
    -1.0, -1.0,  1.0,
    1.0, -1.0,  1.0,
    1.0,  1.0,  1.0,
    -1.0,  1.0,  1.0,

    // top
    -1.0,  1.0,  1.0,
    1.0,  1.0,  1.0,
    1.0,  1.0, -1.0,
    -1.0,  1.0, -1.0,

    // back
    1.0, -1.0, -1.0,
    -1.0, -1.0, -1.0,
    -1.0,  1.0, -1.0,
    1.0,  1.0, -1.0,

    // bottom
    -1.0, -1.0, -1.0,
    1.0, -1.0, -1.0,
    1.0, -1.0,  1.0,
    -1.0, -1.0,  1.0,

    // left
    -1.0, -1.0, -1.0,
    -1.0, -1.0,  1.0,
    -1.0,  1.0,  1.0,
    -1.0,  1.0, -1.0,

    // right
    1.0, -1.0,  1.0,
    1.0, -1.0, -1.0,
    1.0,  1.0, -1.0,
    1.0,  1.0,  1.0,
  };

  glGenBuffers(1, &vbo_cube_verts);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_cube_verts);
  glBufferData(
    GL_ELEMENT_ARRAY_BUFFER, 
    sizeof(cube_vertices), 
    cube_vertices,
    GL_STATIC_DRAW);


  GLfloat cube_texcoords[2 * 4 * 6] = {
    // front
    0.0, 0.0,
    1.0, 0.0,
    1.0, 1.0,
    0.0, 1.0,
  };
  for (int i = 1; i < 6; ++i)
  {
    memcpy(&cube_texcoords[i*4*2], &cube_texcoords[0], 2*4*sizeof(GLfloat));
  }
  glGenBuffers(1, &vbo_cube_texcoords);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_texcoords);
  glBufferData(
    GL_ARRAY_BUFFER, sizeof(cube_texcoords), cube_texcoords, GL_STATIC_DRAW);


  GLushort cube_elements[] = {
    // front
    0,  1,  2,
    2,  3,  0,
    // top
    4,  5,  6,
    6,  7,  4,
    // back
    8,  9, 10,
    10, 11,  8,
    // bottom
    12, 13, 14,
    14, 15, 12,
    // left
    16, 17, 18,
    18, 19, 16,
    // right
    20, 21, 22,
    22, 23, 20,
  };

  glGenBuffers(1, &ibo_cube_elements);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
  glBufferData(
    GL_ELEMENT_ARRAY_BUFFER, 
    sizeof(cube_elements), 
    cube_elements, 
    GL_STATIC_DRAW);

  int w, h;
  //unsigned char* img = SOIL_load_image("resources/images/4x4window.jpg",&w,&h,0,0);
  //unsigned char* img = SOIL_load_image("resources/images/1x1red.jpg",&w,&h,0,0);
  //unsigned char* img = SOIL_load_image("resources/images/whismur.png",&w,&h,0,0);
  //unsigned char* img = SOIL_load_image("resources/images/whismurPOT.png",&w,&h,0,0);
  //unsigned char* img = SOIL_load_image("resources/images/eye256.png",&w,&h,0,0);

  unsigned char* img = SOIL_load_image("resources/images/box.png",&w,&h, 0, SOIL_LOAD_RGBA);
  ErrorIf(!img, "%s", SOIL_last_result());
  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,w,h,0,GL_RGBA,GL_UNSIGNED_BYTE,img);

  // program
  GLuint vs = create_shader("resources/shaders/tri.v.txt", GL_VERTEX_SHADER);
  GLuint fs = create_shader("resources/shaders/tri.f.txt", GL_FRAGMENT_SHADER);
  ErrorIf(!vs || !fs, "Failed to create shaders");
  program = glCreateProgram();
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  GLint link_ok = GL_FALSE;
  glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
  ErrorIfGL(!link_ok, program, "Error linking program");

  // attribute
  attribute_coord3d = glGetAttribLocation(program, "coord3d");
  ErrorIf(attribute_coord3d == -1, "Couldnt bind attribute %s", "coord3d");

  // attribute
  attribute_texcoord = glGetAttribLocation(program, "texcoord");
  ErrorIf(attribute_texcoord == -1, "Couldnt bind attribute %s", "texcoord");

  // uniform
  uniform_mvp = glGetUniformLocation(program, "mvp");
  ErrorIf(uniform_mvp == -1, "Couldnt bind uniform %s", "mvp");

  // uniform
  uniform_myTexture = glGetUniformLocation(program, "mytexture");
  ErrorIf(uniform_myTexture == -1, "Couldnt bind uniform %s", "mytexture");


  return 1;
}

void Graphics::idle()
{
  std::cout << "idle" << std::endl;
  // [-1,1] every 5 seconds
  float angle = glutGet(GLUT_ELAPSED_TIME) / 1000.0f * 45; // 45 deg/s

  glm::vec3 axis_y(0,1,0);
  glm::mat4 anim = glm::rotate(glm::mat4(1.0f), angle, axis_y);

  glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0,0.0,-4.0));
  glm::mat4 view = 
    glm::lookAt(glm::vec3(0.0,2.0,0.0),
    glm::vec3(0.0,0.0,-4.0),
    glm::vec3(0.0,1.0,0.0));
  glm::mat4 projection = 
    glm::perspective(45.0f, 1.0f * screen_width / screen_height,0.1f, 10.0f);

  glm::mat4 mvp = projection * view * model * anim;
  glUseProgram(program);
  glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));
  glutPostRedisplay();
}

void Graphics::onDisplay()
{
  std::cout << "ondisplay" << std::endl;
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(program);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glUniform1i(uniform_myTexture, /*GL_TEXTURE*/0);

  glEnableVertexAttribArray(attribute_coord3d); // open shader var
  glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_verts); // put stuff in buffer
  glVertexAttribPointer(attribute_coord3d, 3, GL_FLOAT, GL_FALSE, 0, 0); //send

  glEnableVertexAttribArray(attribute_texcoord);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_texcoords);
  glVertexAttribPointer(attribute_texcoord, 2, GL_FLOAT, GL_FALSE, 0, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
  int size; 
  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
  glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

  glDisableVertexAttribArray(attribute_coord3d);
  glDisableVertexAttribArray(attribute_texcoord);

  glutSwapBuffers();
}

void Graphics::onReshape(int width, int height)
{
  screen_width = width;
  screen_height = height;
  glViewport(0,0,screen_width, screen_height);
}

void Graphics::free_resources()
{
  glDeleteProgram(program);

  glDeleteBuffers(1, &vbo_cube_verts);
  glDeleteBuffers(1, &vbo_cube_texcoords);
  glDeleteBuffers(1, &ibo_cube_elements);
  glDeleteTextures(1, &texture_id);
}