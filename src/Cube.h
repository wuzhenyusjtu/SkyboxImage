//
//  Cube.h
//  Lab5
//
//  Created by Wu Zhenyu on 12/15/15.
//  Copyright (c) 2015 Wu Zhenyu. All rights reserved.
//

#ifndef Lab5_Cube_h
#define Lab5_Cube_h

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "global.h"

class Cube{
private:
    Vertex verts[8];       // cube vertices
    GLubyte indices[6*6];   // cube face indices: 6 faces, 6 indices each face
    GLuint vboHandle;   // a VBO that contains interleaved positions and colors
    GLuint indexVBO;
public:
    Cube(float size, float r, float g, float b);
    void initVBO();
    void draw(float* local2clip, float* local2eye, float* world2eye, GLuint posHandler, GLuint colorHandler, GLuint normalHandler, GLuint local2clipHandler, GLuint local2eyeHandler, GLuint world2eyeHandler, GLuint normalMatHandler, GLuint eye2worMatHandler);
};

#endif
