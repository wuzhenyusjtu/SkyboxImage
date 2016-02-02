
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <vector>
#include <stack>
#include <cmath>
#include "PlyModel.h"
#include "global.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "CubeMap.h"
#include "Cube.h"
#include "Cone.h"
#include "Square.h"
#include "Texture.h"

using namespace std;

#ifdef __APPLE__
#include <GL/glew.h>
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#else
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#endif

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "ply.h"

//draw Cylinder
//draw hemisphere
//draw Cube
//build MVP Matrix
GLuint programObject;
GLuint SetupGLSL(char*);

#define SetMaterialColor(d, r, g, b, a)  glUniform4f(d, r, g, b, a);

float angle1=0, angle2=0;

int xform_mode = 0;
#define XFORM_NONE    0
#define XFORM_ROTATE  1
#define XFORM_SCALE 2

int press_x, press_y;
int release_x, release_y;
float z_angle = 0.0;
float x_angle = 0.0;
float scale_size = 1;

bool USE_REFLECTION = false;
bool WIRE_FRAME =false;

// Vertices Initialization

// Display Function
void display();

// Key Event Handler
void mykey(unsigned char, int, int);

// Motion handler
void mymotion(int x, int y);

// Mouse event handler
void mymouse(int button, int state, int x, int y);

/////////////////////////////////
// Define Light Properties -  Ia, Id, Is, and light position
//////////////////////////////////

GLfloat light_ambient[4] = {1,1,1,1};  //Ia
GLfloat light_diffuse[4] = {1,1,1,1};  //Id
GLfloat light_specular[4] = {1,1,1,1};  //Is
GLfloat light_pos [4] = {-0.1, 0.25, 0, 1}; //Light Position

/////////////////////////////////
// Define Default Material Properties -  Ka, Kd, Ks, Shininess
//////////////////////////////////

GLfloat mat_ambient[4] = {0.4,0.4,0.4,1};  //Ka
GLfloat mat_diffuse[4] = {1,1,1,1};  //Kd
GLfloat mat_specular[4] = {1,1,1,1};  //Ks
GLfloat mat_shine[1] = {20}; // Shine

class Draw{
private:
    PlyModel* _plymodel1;
    PlyModel* _plymodel2;
    Cube* _cube;
    Cylinder* _cylinder;
    Sphere* _sphere;
    Cone* _cone;
    Square* _square;
    glm::mat4 _projection;
    glm::mat4 _view;
    glm::mat4 _model;
    glm::mat4 _local2clip;
    glm::mat4 _local2eye;
    glm::mat4 _world2eye;
    glm::mat4 _normalMat;
    float _color[4];
    GLuint _posHandler;
    GLuint _colorHandler;
    GLuint _normalHandler;
    GLuint _coordHandler;
    GLuint _Kd;
    GLuint _Ks;
    GLuint _isLightSource;
    GLuint _use_texture;
    GLuint _loc2clipMatHandler;
    GLuint _loc2eyeMatHandler;
    GLuint _normalMatHandler;
    GLuint _wor2eyeMatHandler;
    GLuint _eye2worMatHandler;
public:
    static glm::mat4 _modelM;
    static stack<glm::mat4> _mat_stack;
    Draw(PlyModel* plymodel1, PlyModel* plymodel2, Cube* cube, Cylinder* cylinder, Sphere* sphere, Cone* cone, Square* square,glm::mat4 model, glm::mat4 view, glm::mat4 projection, GLuint posHandler, GLuint colorHandler, GLuint normalHandler, GLuint coordHandler, GLuint Kd, GLuint Ks,GLuint isLightSource, GLuint use_texture, GLuint loc2clipMatHandler, GLuint loc2eyeMatHandler, GLuint normalMatHandler, GLuint wor2eyeMatHandler, GLuint eye2worMatHandler)
    {
        _plymodel1 = plymodel1;
        _plymodel2 = plymodel2;
        _cube = cube;
        _cylinder = cylinder;
        _sphere = sphere;
        _cone = cone;
        _square = square;
        _projection = projection;
        _view = view;
        _model = model;
        _local2clip = _projection * _view * _model;
        _local2eye = _view * _model;
        _world2eye = _view;
        _posHandler = posHandler;
        _colorHandler = colorHandler;
        _normalHandler = normalHandler;
        _coordHandler = coordHandler;
        _Kd = Kd;
        _Ks = Ks;
        _isLightSource = isLightSource;
        _use_texture = use_texture;
        _loc2clipMatHandler = loc2clipMatHandler;
        _loc2eyeMatHandler = loc2eyeMatHandler;
        _normalMatHandler = normalMatHandler;
        _wor2eyeMatHandler = wor2eyeMatHandler;
        _eye2worMatHandler = eye2worMatHandler;
        _mat_stack.push(_modelM);
        drawPlymodels();
        drawLightSource();
        drawWalls();
        drawRelectModel();
        drawRobot();
        drawClouds();
        _modelM = Draw::_mat_stack.top();
        _mat_stack.pop();
    }
    
    void drawRobot(){
        double scale = 0.25;
        glUniform1i(_isLightSource, false);
        if (USE_REFLECTION) {
            glUniform1i(_use_texture,2);
        }
        else glUniform1i(_use_texture,0);

        glm::mat4 model = glm::scale(_model, glm::vec3(scale, scale, scale));
        model = glm::translate(model, glm::vec3(0.0, -1.0,0.0));
        
        // Draw Body
        _modelM = _mat_stack.top();
        _modelM = glm::scale(_modelM,glm::vec3(6.0,6.0,6.0));
        //_color[0] = 0; _color[1] = 1; _color[2] = 0;
        _modelM = _projection * _view * model * _modelM;
        SetMaterialColor(_Kd, 0, 1, 0, 1);
        _cylinder->draw(&_modelM[0][0], &_local2eye[0][0], &_world2eye[0][0], _posHandler, _colorHandler, _normalHandler, _loc2clipMatHandler, _loc2eyeMatHandler, _wor2eyeMatHandler, _normalMatHandler);
        
        // Draw Left Upper Arm
        _modelM = _mat_stack.top();
        _modelM = glm::translate(_modelM, glm::vec3(-0.2,0.1,0.0));
        _modelM = glm::rotate(_modelM, glm::radians(angle1), glm::vec3(0.0f, 0.0f, 1.0f));
        //_modelM = glm::rotate(_modelM, glm::radians(angle2), glm::vec3(0.0f, 0.0f, 1.0f));
        _modelM = glm::translate(_modelM, glm::vec3(0.0,-0.075,0.0));
        _modelM = glm::scale(_modelM, glm::vec3(1.0,3.0,1.0));
        _modelM = _projection * _view * model * _modelM;
        SetMaterialColor(_Kd, 0, 1, 0, 1);
        _cylinder->draw(&_modelM[0][0], &_local2eye[0][0], &_world2eye[0][0], _posHandler, _colorHandler, _normalHandler, _loc2clipMatHandler, _loc2eyeMatHandler, _wor2eyeMatHandler, _normalMatHandler);
        
        // Draw Left Lower Arm
        _modelM = _mat_stack.top();
        _modelM = glm::translate(_modelM, glm::vec3(-0.2,0.1,0.0));
        _modelM = glm::rotate(_modelM, glm::radians(angle1), glm::vec3(0.0f, 0.0f, 1.0f));
        //_modelM = glm::rotate(_modelM, glm::radians(angle2), glm::vec3(0.0f, 0.0f, 1.0f));
        _modelM = glm::translate(_modelM, glm::vec3(0.0,-0.225,0.0));
        _modelM = glm::translate(_modelM, glm::vec3(0.0,0.075,0.0));
        _modelM = glm::rotate(_modelM, glm::radians(angle2), glm::vec3(0.0f, 0.0f, 1.0f));
        //_modelM = glm::rotate(_modelM, glm::radians(angle4), glm::vec3(0.0f, 0.0f, 1.0f));
        _modelM = glm::translate(_modelM, glm::vec3(0.0,-0.075,0.0));
        _modelM = glm::scale(_modelM, glm::vec3(1.0,3.0,1.0));
        _modelM = _projection * _view * model * _modelM;
        SetMaterialColor(_Kd, 0, 1, 0, 1);
        _cylinder->draw(&_modelM[0][0], &_local2eye[0][0], &_world2eye[0][0], _posHandler, _colorHandler, _normalHandler, _loc2clipMatHandler, _loc2eyeMatHandler, _wor2eyeMatHandler, _normalMatHandler);
        
        // Draw Right Arm
        _modelM = _mat_stack.top();
        _modelM = glm::translate(_modelM, glm::vec3(0.2,0.1,0.0));
        _modelM = glm::rotate(_modelM, glm::radians(angle1), glm::vec3(0.0f, 0.0f, 1.0f));
        //_modelM = glm::rotate(_modelM, glm::radians(angle2), glm::vec3(0.0f, 0.0f, 1.0f));
        _modelM = glm::translate(_modelM, glm::vec3(0.0,-0.075,0.0));
        _modelM = glm::scale(_modelM, glm::vec3(1.0,3.0,1.0));
        _modelM = _projection * _view * model * _modelM;
        SetMaterialColor(_Kd, 0, 1, 0, 1);
        _cylinder->draw(&_modelM[0][0], &_local2eye[0][0], &_world2eye[0][0], _posHandler, _colorHandler, _normalHandler, _loc2clipMatHandler, _loc2eyeMatHandler, _wor2eyeMatHandler, _normalMatHandler);
        
        // Draw Left Lower Arm
        _modelM = _mat_stack.top();
        _modelM = glm::translate(_modelM, glm::vec3(0.2,0.1,0.0));
        _modelM = glm::rotate(_modelM, glm::radians(angle1), glm::vec3(0.0f, 0.0f, 1.0f));
        //_modelM = glm::rotate(_modelM, glm::radians(angle2), glm::vec3(0.0f, 0.0f, 1.0f));
        _modelM = glm::translate(_modelM, glm::vec3(0.0,-0.225,0.0));
        _modelM = glm::translate(_modelM, glm::vec3(0.0,0.075,0.0));
        _modelM = glm::rotate(_modelM, glm::radians(angle2), glm::vec3(0.0f, 0.0f, 1.0f));
        //_modelM = glm::rotate(_modelM, glm::radians(angle4), glm::vec3(0.0f, 0.0f, 1.0f));
        _modelM = glm::translate(_modelM, glm::vec3(0.0,-0.075,0.0));
        _modelM = glm::scale(_modelM, glm::vec3(1.0,3.0,1.0));
        _modelM = _projection * _view * model * _modelM;
        SetMaterialColor(_Kd, 0, 1, 0, 1);
        _cylinder->draw(&_modelM[0][0], &_local2eye[0][0], &_world2eye[0][0], _posHandler, _colorHandler, _normalHandler, _loc2clipMatHandler, _loc2eyeMatHandler, _wor2eyeMatHandler, _normalMatHandler);
        
        // Draw Left Leg
        _modelM = _mat_stack.top();
        _modelM = glm::translate(_modelM, glm::vec3(-0.06,-0.255,0.0));
        _modelM = glm::scale(_modelM, glm::vec3(1.0,4.0,1.0));
        _modelM = _projection * _view * model * _modelM;
        SetMaterialColor(_Kd, 0, 1, 0, 1);
        _cylinder->draw(&_modelM[0][0], &_local2eye[0][0], &_world2eye[0][0], _posHandler, _colorHandler, _normalHandler, _loc2clipMatHandler, _loc2eyeMatHandler, _wor2eyeMatHandler, _normalMatHandler);
        
        // Draw Right Leg
        _modelM = _mat_stack.top();
        _modelM = glm::translate(_modelM, glm::vec3(0.06,-0.255,0.0));
        _modelM = glm::scale(_modelM, glm::vec3(1.0,4.0,1.0));
        _modelM = _projection * _view * model * _modelM;
        SetMaterialColor(_Kd, 0, 1, 0, 1);
        _cylinder->draw(&_modelM[0][0], &_local2eye[0][0], &_world2eye[0][0], _posHandler, _colorHandler, _normalHandler, _loc2clipMatHandler, _loc2eyeMatHandler, _wor2eyeMatHandler, _normalMatHandler);
        
        // Draw Face
        _modelM = _mat_stack.top();
        _modelM = glm::translate(_modelM, glm::vec3(0.0,0.16,0.0));
        _modelM = glm::scale(_modelM, glm::vec3(7.0,7.0,7.0));
        _modelM = _projection * _view * model * _modelM;
        SetMaterialColor(_Kd, 0, 1, 0, 1);
        _sphere->draw(1.0, &_modelM[0][0], &_local2eye[0][0], &_world2eye[0][0], _posHandler, _colorHandler, _normalHandler, _loc2clipMatHandler, _loc2eyeMatHandler, _wor2eyeMatHandler, _normalMatHandler,_eye2worMatHandler);
        
        // Draw Left Eye
        _modelM = _mat_stack.top();
        _modelM = glm::translate(_modelM, glm::vec3(0.05,0.2,-0.12));
        _modelM = _projection * _view * model * _modelM;
        SetMaterialColor(_Kd, 1, 1, 1, 1);
        _sphere->draw(1.0, &_modelM[0][0],  &_local2eye[0][0], &_world2eye[0][0], _posHandler, _colorHandler, _normalHandler, _loc2clipMatHandler, _loc2eyeMatHandler, _wor2eyeMatHandler, _normalMatHandler,_eye2worMatHandler);
        
        // Draw Right Eye
        _modelM = _mat_stack.top();
        _modelM = glm::translate(_modelM, glm::vec3(-0.05,0.2,-0.12));
        _modelM = _projection * _view * model * _modelM;
        SetMaterialColor(_Kd, 1, 1, 1, 1);
        _sphere->draw(1.0, &_modelM[0][0], &_local2eye[0][0], &_world2eye[0][0], _posHandler, _colorHandler, _normalHandler, _loc2clipMatHandler, _loc2eyeMatHandler, _wor2eyeMatHandler, _normalMatHandler,_eye2worMatHandler);
        
        // Draw left Antenna
        _modelM = _mat_stack.top();
        _modelM = glm::translate(_modelM, glm::vec3(-0.05,0.32,0.0));
        _modelM = glm::scale(_modelM, glm::vec3(0.5,2.0,1.0));
        _modelM = glm::rotate(_modelM, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        _modelM = _projection * _view * model * _modelM;
        SetMaterialColor(_Kd, 0, 1, 0, 1);
        _cylinder->draw(&_modelM[0][0], &_local2eye[0][0], &_world2eye[0][0], _posHandler, _colorHandler, _normalHandler, _loc2clipMatHandler, _loc2eyeMatHandler, _wor2eyeMatHandler, _normalMatHandler);
        
        // Draw right Antenna
        _modelM = _mat_stack.top();
        _modelM = glm::translate(_modelM, glm::vec3(0.05,0.32,0.0));
        _modelM = glm::scale(_modelM, glm::vec3(0.5,2.0,1.0));
        _modelM = glm::rotate(_modelM, glm::radians(-5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        _modelM = _projection * _view * model * _modelM;
        SetMaterialColor(_Kd, 0, 1, 0, 1);
        _cylinder->draw(&_modelM[0][0], &_local2eye[0][0], &_world2eye[0][0], _posHandler, _colorHandler, _normalHandler, _loc2clipMatHandler, _loc2eyeMatHandler, _wor2eyeMatHandler, _normalMatHandler);
    }
    
    void drawClouds(){
        double scale = 0.25;
        glUniform1i(_isLightSource, false);
        if (USE_REFLECTION) {
            glUniform1i(_use_texture,2);
        }
        else glUniform1i(_use_texture,0);
        
        glm::mat4 model = glm::scale(_model, glm::vec3(scale, scale, scale));
        model = glm::translate(model, glm::vec3(0.0, 0.3,0.0));
        
        // Draw Clouds
        for (int i=0; i<2; i++) {
            _modelM = glm::mat4(1.0f);
            int sign = pow(-1, i);
            //_modelM = _mat_stack.top();
            _modelM = glm::translate(_modelM, glm::vec3(-0.6*pow(-1, i), 0.6, 0.0));
            _modelM = glm::scale(_modelM, glm::vec3(0.2/0.02, 0.2/0.02, 0.2/0.02));
            SetMaterialColor(_Kd, 1, 1, 1, 1);
            _modelM = _projection * _view * model * _modelM;
            _sphere->draw(0.3, &_modelM[0][0], &_local2eye[0][0], &_world2eye[0][0], _posHandler, _colorHandler, _normalHandler, _loc2clipMatHandler, _loc2eyeMatHandler, _wor2eyeMatHandler, _normalMatHandler,_eye2worMatHandler);
            
            _modelM = glm::mat4(1.0f);
            _modelM = glm::translate(_modelM, glm::vec3(-0.6*pow(-1, i), 0.75, 0.0));
            _modelM = glm::scale(_modelM, glm::vec3(0.08/0.02, 0.08/0.02, 0.08/0.02));
            SetMaterialColor(_Kd, 1, 1, 1, 1);
            _modelM = _projection * _view * model * _modelM;
            _sphere->draw(0.45, &_modelM[0][0], &_local2eye[0][0], &_world2eye[0][0], _posHandler, _colorHandler, _normalHandler, _loc2clipMatHandler, _loc2eyeMatHandler, _wor2eyeMatHandler, _normalMatHandler,_eye2worMatHandler);
            
            _modelM = glm::mat4(1.0f);
            _modelM = glm::translate(_modelM, glm::vec3(-0.7*pow(-1, i), 0.7, 0.0));
            _modelM = glm::scale(_modelM, glm::vec3(0.08/0.02, 0.08/0.02, 0.08/0.02));
            SetMaterialColor(_Kd, 1, 1, 1, 1);
            _modelM = _projection * _view * model * _modelM;
            _sphere->draw(0.45, &_modelM[0][0], &_local2eye[0][0], &_world2eye[0][0], _posHandler, _colorHandler, _normalHandler, _loc2clipMatHandler, _loc2eyeMatHandler, _wor2eyeMatHandler, _normalMatHandler,_eye2worMatHandler);
        }
    }
    void drawPlymodels(){
        glUniform1i(_isLightSource, false);
        if (USE_REFLECTION) {
            glUniform1i(_use_texture,2);
        }
        else glUniform1i(_use_texture,0);


        _modelM = _mat_stack.top();
        _modelM = glm::translate(_modelM,glm::vec3(-0.2,-0.5,0.15));
        _modelM = glm::scale(_modelM,glm::vec3(1.0,1.0,1.0));
        //_color[0] = 0; _color[1] = 1; _color[2] = 0;
        _modelM = _projection * _view * _model * _modelM;
        SetMaterialColor(_Kd, 1, 1, 1, 1);
        _plymodel1->draw(&_modelM[0][0], &_local2eye[0][0], &_world2eye[0][0], _posHandler, _colorHandler, _normalHandler, _loc2clipMatHandler, _loc2eyeMatHandler, _wor2eyeMatHandler, _normalMatHandler);

        _modelM = _mat_stack.top();
        _modelM = glm::translate(_modelM,glm::vec3(0.2,-0.5,0.15));
        _modelM = glm::scale(_modelM,glm::vec3(1.0,1.0,1.0));
        //_color[0] = 0; _color[1] = 1; _color[2] = 0;
        _modelM = _projection * _view * _model * _modelM;
        SetMaterialColor(_Kd, 1, 1, 0, 1);
        _plymodel2->draw(&_modelM[0][0], &_local2eye[0][0], &_world2eye[0][0], _posHandler, _colorHandler, _normalHandler, _loc2clipMatHandler, _loc2eyeMatHandler, _wor2eyeMatHandler, _normalMatHandler);
    }
    
    void drawRelectModel(){
        glUniform1i(_isLightSource, false);
        if (USE_REFLECTION)
            glUniform1i(_use_texture, 0);
        else glUniform1i(_use_texture, 2);
        GLuint tex_loc = glGetUniformLocation(programObject, "cubeMap"); // query location
        printf("%s%d", "TEXTURE LOCATION\n", tex_loc);
        glUniform1i(tex_loc, 8);
        _modelM = glm::mat4(1.0f);
        _modelM = glm::scale(_modelM, glm::vec3(4.5,4.5,4.5));
        _modelM = _projection * _view * _model * _modelM;
        SetMaterialColor(_Kd, 1, 1, 1, 1);
        _sphere->draw(1.0, &_modelM[0][0], &_local2eye[0][0], &_world2eye[0][0], _posHandler, _colorHandler, _normalHandler, _loc2clipMatHandler, _loc2eyeMatHandler, _wor2eyeMatHandler, _normalMatHandler, _eye2worMatHandler);
        
    }
    
    void drawWalls(){
        glUniform1i(_isLightSource, false);

        glUniform1i(_use_texture,1);

        //use environment cubemap
        GLuint tex_loc = glGetUniformLocation(programObject, "tex"); // query location
        printf("%s%d", "TEXTURE LOCATION\n", tex_loc);
        // floor
        const float angle = glm::radians(90.0);
        
        glUniform1i(tex_loc, 0);
        // negative z
        // Back side
        _modelM = glm::mat4(1.0f);
        _modelM = glm::rotate(_modelM, -angle, glm::vec3(1.0, 0.0,0.0));
        _modelM = glm::translate(_modelM, glm::vec3(0.0, -0.5,0.0));
        //_modelM = glm::scale(_modelM, glm::vec3(4.0, 4.0, 1.0));
        _modelM = _projection*_view*_model*_modelM;
        _square->draw(&_modelM[0][0], &_local2eye[0][0], &_world2eye[0][0], _posHandler, _colorHandler, _normalHandler, _coordHandler, _loc2clipMatHandler, _loc2eyeMatHandler, _wor2eyeMatHandler, _normalMatHandler);
        
        glUniform1i(tex_loc, 1);
        // positive z
        // Front side
        _modelM = glm::mat4(1.0f);
        _modelM = glm::rotate(_modelM, -angle, glm::vec3(1.0, 0.0,0.0));
        _modelM = glm::translate(_modelM, glm::vec3(0.0, 0.5,0.0));
        //_modelM = glm::scale(_modelM, glm::vec3(4.0, 4.0, 1.0));
        _modelM = _projection*_view*_model*_modelM;
        _square->draw(&_modelM[0][0], &_local2eye[0][0], &_world2eye[0][0], _posHandler, _colorHandler, _normalHandler, _coordHandler, _loc2clipMatHandler, _loc2eyeMatHandler, _wor2eyeMatHandler, _normalMatHandler);
        
        
        glUniform1i(tex_loc, 2);
        // negative x
        // Left side
        _modelM = glm::mat4(1.0f);
        //_modelM = glm::rotate(_modelM, angle, glm::vec3(1.0, 0.0, 0.0));
        _modelM = glm::rotate(_modelM, -angle, glm::vec3(1.0, 0.0, 0.0));
        _modelM = glm::rotate(_modelM, -angle, glm::vec3(0.0, 0.0, 1.0));
        _modelM = glm::translate(_modelM, glm::vec3(0.0, -0.5,0.0));
        //_modelM = glm::scale(_modelM, glm::vec3(4.0, 2.0, 1.0));
        _modelM = _projection*_view*_model*_modelM;
        _square->draw(&_modelM[0][0], &_local2eye[0][0], &_world2eye[0][0], _posHandler, _colorHandler, _normalHandler, _coordHandler, _loc2clipMatHandler, _loc2eyeMatHandler, _wor2eyeMatHandler, _normalMatHandler);
        
        
        glUniform1i(tex_loc, 3);
        // positive x
        // right side
        _modelM = glm::mat4(1.0f);
        //_modelM = glm::rotate(_modelM, angle, glm::vec3(1.0, 0.0, 0.0));
        _modelM = glm::rotate(_modelM, -angle, glm::vec3(1.0, 0.0, 0.0));
        _modelM = glm::rotate(_modelM, -angle, glm::vec3(0.0, 0.0, 1.0));
        _modelM = glm::translate(_modelM, glm::vec3(0.0, 0.5,0.0));
        //_modelM = glm::scale(_modelM, glm::vec3(4.0, 2.0, 1.0));
        _modelM = _projection*_view*_model*_modelM;
        _square->draw(&_modelM[0][0], &_local2eye[0][0], &_world2eye[0][0], _posHandler, _colorHandler, _normalHandler, _coordHandler, _loc2clipMatHandler, _loc2eyeMatHandler, _wor2eyeMatHandler, _normalMatHandler);
        
        glUniform1i(tex_loc, 4);
        // positive y
        // top side
        _modelM = glm::mat4(1.0f);
        //_modelM = glm::rotate(_modelM, angle, glm::vec3(1.0, 0.0, 0.0));
        _modelM = glm::translate(_modelM, glm::vec3(0.0, 0.5, 0.0));
        //_modelM = glm::scale(_modelM, glm::vec3(4.0, 4.0, 1.0));
        _modelM = _projection*_view*_model*_modelM;
        _square->draw(&_modelM[0][0], &_local2eye[0][0], &_world2eye[0][0], _posHandler, _colorHandler, _normalHandler, _coordHandler, _loc2clipMatHandler, _loc2eyeMatHandler, _wor2eyeMatHandler, _normalMatHandler);
        
        glUniform1i(tex_loc, 5);
        // negative y
        // bottom side
        _modelM = glm::mat4(1.0f);
        //_modelM = glm::rotate(_modelM, angle, glm::vec3(1.0, 0.0, 0.0));
        _modelM = glm::translate(_modelM, glm::vec3(0.0, -0.5, 0.0));
        //_modelM = glm::scale(_modelM, glm::vec3(4.0, 4.0, 1.0));
        _modelM = _projection*_view*_model*_modelM;
        _square->draw(&_modelM[0][0], &_local2eye[0][0], &_world2eye[0][0], _posHandler, _colorHandler, _normalHandler, _coordHandler, _loc2clipMatHandler, _loc2eyeMatHandler, _wor2eyeMatHandler, _normalMatHandler);
}
    
    void drawLightSource(){
        glUniform1i(_use_texture,0);
        // Draw Moon
        _modelM = glm::mat4(1.0f);
        //_modelM = _mat_stack.top();
        _modelM = glm::translate(_modelM, glm::vec3(light_pos[0], light_pos[1], light_pos[2]));
        _modelM = glm::scale(_modelM, glm::vec3(0.13/0.07, 0.13/0.07, 0.13/0.07));
        //_modelM = glm::rotate(_modelM, (float)-M_PI/2, glm::vec3(0.0f, 0.0f, 1.0f));
        SetMaterialColor(_Kd, 1, 0, 0, 1);
        _modelM = _projection * _view * _model * _modelM;
        glUniform1i(_isLightSource, true);
        _sphere->draw(1.0, &_modelM[0][0], &_local2eye[0][0], &_world2eye[0][0], _posHandler, _colorHandler, _normalHandler, _loc2clipMatHandler, _loc2eyeMatHandler, _wor2eyeMatHandler, _normalMatHandler,_eye2worMatHandler);
    }
};

glm::mat4 Draw::_modelM = glm::mat4(1.0f);
stack<glm::mat4> Draw::_mat_stack;

Cube *cube;
Cylinder *cylinder;
Sphere *sphere;
Cone *cone;
Square *square;
PlyModel *bunny;
PlyModel *dragon;
Texture *textureFront;
Texture *textureBack;
Texture *textureLeft;
Texture *textureRight;
Texture *textureTop;
Texture *textureBottom;
CubeMap *cubeMap;

int main(int argc, char** argv)
{
    // Vertices Initialization
    /*char cwd[1024];
     if (getcwd(cwd, sizeof(cwd)) != NULL)
     fprintf(stdout, "Current working dir: %s\n", cwd);
     else
     perror("getcwd() error");
     */
    //FILE* bunny_fp = fopen("newBunny.ply", "r");
    //bunny = new PlyModel(bunny_fp);
    //CubeMap(const char *front, const char *back, const char *left, const char* right, const char* up, const char*down){

    cubeMap = new CubeMap("front.ppm","back.ppm", "left.ppm", "right.ppm", "top.ppm", "bottom.ppm");
    textureFront = new Texture("front.ppm");
    textureBack = new Texture("back.ppm");
    textureLeft = new Texture("left.ppm");
    textureRight = new Texture("right.ppm");
    textureTop = new Texture("top.ppm");
    textureBottom = new Texture("bottom.ppm");
    FILE* ply_bunny = fopen("newBunny.ply", "r");
    FILE* ply_dragon = fopen("newDragon.ply", "r");
    bunny = new PlyModel(ply_bunny);
    dragon = new PlyModel(ply_dragon);
    cube = new Cube(0.05, 0, 1, 0);
    cylinder = new Cylinder(0.025, 0.025, 0.05, 60, 60, 0, 1, 0);
    sphere = new Sphere(0.02, 60, 61, 0, 1, 0);
    cone = new Cone(0.2, 0.2, 60, 60, 0, 0, 0.8);
    square = new Square();
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
    glutInitWindowSize(600,600);
    glutCreateWindow("ShanghaiSkyline");
    
    glutDisplayFunc(display);
    glutMouseFunc(mymouse);
    glutMotionFunc(mymotion);
    glutKeyboardFunc(mykey);
    
    glewInit();
    cubeMap->init_cubemap();
    
    textureFront->initTexture();
    textureBack->initTexture();
    textureLeft->initTexture();
    textureRight->initTexture();
    textureTop->initTexture();
    textureBottom->initTexture();

    square->initVBO();
    cube->initVBO();
    cylinder->initVBO();
    sphere->initVBO();
    cone->initVBO();
    bunny->initVBO();
    dragon->initVBO();
    programObject = SetupGLSL("shader");
    glutMainLoop();
}

void display()
{
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glEnableClientState(GL_VERTEX_ARRAY); // enable the vertex array
    glEnableClientState(GL_COLOR_ARRAY); // enable the color array on the client side
    glEnableClientState(GL_ELEMENT_ARRAY_BUFFER);
    
    if (WIRE_FRAME) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    
    glUseProgram(programObject);
    
    GLuint posHandler = glGetAttribLocation(programObject, "position");
    GLuint colorHandler = glGetAttribLocation(programObject, "color");
    GLuint normalHandler = glGetAttribLocation(programObject, "normal");
    GLuint coordHandler = glGetAttribLocation(programObject, "texCoord");
    GLuint loc2clipMatHandler = glGetUniformLocation(programObject, "local2clip");
    GLuint loc2eyeMatHandler = glGetUniformLocation(programObject, "local2eye");
    GLuint normalMatHandler = glGetUniformLocation(programObject, "normal_matrix");
    GLuint wor2eyeMatHandler = glGetUniformLocation(programObject, "world2eye");
    GLuint eye2worMatHandler = glGetUniformLocation(programObject, "eye2world");

    GLuint Ia = glGetUniformLocation(programObject, "light_ambient");
    GLuint Id = glGetUniformLocation(programObject, "light_diffuse");
    GLuint Is = glGetUniformLocation(programObject, "light_specular");
    GLuint Lpos = glGetUniformLocation(programObject, "light_pos");
    
    GLuint Ka = glGetUniformLocation(programObject, "mat_ambient");
    GLuint Kd = glGetUniformLocation(programObject, "mat_diffuse");
    GLuint Ks = glGetUniformLocation(programObject, "mat_specular");
    GLuint Shine = glGetUniformLocation(programObject, "mat_shine");
    GLuint use_texture = glGetUniformLocation(programObject,"use_texture");
    GLuint isLightSource = glGetUniformLocation(programObject, "isLightSource");
    
    glUniform4f(Ia, light_ambient[0], light_ambient[1], light_ambient[2], light_ambient[3]);
    glUniform4f(Id, light_diffuse[0], light_diffuse[1], light_diffuse[2], light_diffuse[3]);
    glUniform4f(Is, light_specular[0], light_specular[1], light_specular[2], light_specular[3]);
    glUniform4f(Lpos, light_pos[0], light_pos[1], light_pos[2], light_pos[3]);
    
    glUniform4f(Ka, mat_ambient[0], mat_ambient[1], mat_ambient[2], mat_ambient[3]);
    glUniform4f(Kd, mat_diffuse[0], mat_diffuse[1], mat_diffuse[2], mat_diffuse[3]);
    glUniform4f(Ks, mat_specular[0], mat_specular[1], mat_specular[2], mat_specular[3]);
    glUniform1f(Shine, mat_shine[0]);
    glUniform1i(isLightSource, false);
    glEnableVertexAttribArray(posHandler);
    glEnableVertexAttribArray(colorHandler);
    glEnableVertexAttribArray(normalHandler);
    //glEnableVertexAttribArray(coordHandler);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(z_angle), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::rotate(model, glm::radians(x_angle), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(scale_size, scale_size, scale_size));
    
    glm::mat4 projection = glm::perspective(glm::radians(60.0f),1.0f,0.1f,100.0f);
    
    glm::mat4 view = glm::lookAt(glm::vec3(-1.5, 1.5, -1.5),
                                 glm::vec3(0.0, 0.0, 0.0),
                                 glm::vec3(0.0, 1.0, 0.0));
    
    
    
    Draw draw = Draw(bunny, dragon, cube, cylinder, sphere, cone, square, model, view, projection, posHandler, colorHandler, normalHandler, coordHandler, Kd, Ks, isLightSource, use_texture,loc2clipMatHandler, loc2eyeMatHandler, normalMatHandler, wor2eyeMatHandler, eye2worMatHandler);
    //
    // This following is required, otherwise no display
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_ELEMENT_ARRAY_BUFFER);
    
    glutSwapBuffers();
}

// Key Event Handler
void mykey(unsigned char key, int x, int y)
{
    float d_angle = 15;
    if (key == 'q') exit(1);
    if (key == 't')
        Draw::_modelM = glm::rotate(Draw::_modelM, glm::radians(d_angle), glm::vec3(0.0f, 0.0f, 1.0f));
    if (key == 'T')
        Draw::_modelM = glm::rotate(Draw::_modelM, glm::radians(-d_angle), glm::vec3(0.0f, 0.0f, 1.0f));
    if (key == 'r')
        Draw::_modelM = glm::translate(Draw::_modelM, glm::vec3(0.2f, 0.0f, 0.0f));
    if (key == 'l')
        Draw::_modelM = glm::translate(Draw::_modelM, glm::vec3(-0.2f, 0.0f, 0.0f));
    if (key == 'f')
        Draw::_modelM = glm::translate(Draw::_modelM, glm::vec3(0.0f, 0.2f, 0.0f));
    if (key == 'b')
        Draw::_modelM = glm::translate(Draw::_modelM, glm::vec3(0.0f, -0.2f, 0.0f));
    if (key == 'c') {
        Draw::_modelM =  glm::mat4(1.0f);
        angle1 = angle2 = 0;
    }
    if (key == 's')
        WIRE_FRAME = !WIRE_FRAME;
    if (key == 'g')
        USE_REFLECTION = !USE_REFLECTION;
    if (key == '1')
        angle1 += 20;
    if (key == '!')
        angle1 -= 20;
    if (key == '2')
        angle2 += 20;
    if (key == '@')
        angle2 -= 20;
    if (key == '-') {
        light_diffuse[0] -= 0.1; light_diffuse[1] -= 0.1; light_diffuse[2] -= 0.1;
        if (light_diffuse[0] < 0.1) light_diffuse[0] = 0;
        if (light_diffuse[1] < 0.1) light_diffuse[1] = 0;
        if (light_diffuse[2] < 0.1) light_diffuse[2] = 0;
    }
    if (key == '+')
        light_diffuse[0] += 0.1; light_diffuse[1] += 0.1; light_diffuse[2] += 0.1;
    if (key == '9') {
        light_ambient[0] -= 0.1; light_ambient[1] -= 0.1; light_ambient[2] -= 0.1;
        if (light_ambient[0] < 0.1) light_ambient[0] = 0;
        if (light_ambient[1] < 0.1) light_ambient[1] = 0;
        if (light_ambient[2] < 0.1) light_ambient[2] = 0;
    }
    if (key == '0') {
        light_ambient[0] += 0.1; light_ambient[1] += 0.1; light_ambient[2] += 0.1;
    }
    if (key == '3')
        light_pos[0] += 0.2;
    if (key == '#')
        light_pos[0] -= 0.2;
    if (key == '4')
        light_pos[1] += 0.2;
    if (key == '$')
        light_pos[1] -= 0.2;
    if (key == '5')
        light_pos[2] += 0.2;
    if (key == '%')
        light_pos[2] -= 0.2;
    glutPostRedisplay();
}

void mymotion(int x, int y)
{
    if (xform_mode==XFORM_ROTATE) {
        z_angle += (x - press_x)/5.0;
        if (z_angle > 180) z_angle -= 360;
        else if (z_angle <-180) z_angle += 360;
        press_x = x;
        
        x_angle -= (y - press_y)/5.0;
        if (x_angle > 180) x_angle -= 360;
        else if (x_angle <-180) x_angle += 360;
        press_y = y;
    }
    else if (xform_mode == XFORM_SCALE){
        float old_size = scale_size;
        scale_size *= (1+ (y - press_y)/60.0);
        if (scale_size <0) scale_size = old_size;
        press_y = y;
    }
    glutPostRedisplay();
}

void mymouse(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN) {
        press_x = x; press_y = y;
        std::cout << "x: " << x << "y: " << y << std::endl;
        if(button == GLUT_LEFT_BUTTON)
            xform_mode = XFORM_ROTATE;
        else if (button == GLUT_RIGHT_BUTTON)
            xform_mode = XFORM_SCALE;
    }
    else if (state == GLUT_UP) {
        xform_mode = XFORM_NONE;
    }
}