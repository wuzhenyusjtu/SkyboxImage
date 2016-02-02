//
//  Cone.h
//  Lab5
//
//  Created by Wu Zhenyu on 12/15/15.
//  Copyright (c) 2015 Wu Zhenyu. All rights reserved.
//

#ifndef Lab5_Cone_h
#define Lab5_Cone_h

#include <GL/glew.h>
#include <glm/glm.hpp>

struct Vertex;

class Cone{
private:
    GLuint vboHandle;   // a VBO that contains interleaved positions and colors
    GLuint indexVBO;
    int nindices;
    int nvertices;
    GLuint *indices;
    Vertex *verts;
public:
    Cone(float baseRadius, float height, int nslices, int nstacks, float red, float green, float blue);
    void initVBO();
    void draw(float* local2clip, float* local2eye, float* world2eye, GLuint posHandler, GLuint colorHandler, GLuint normalHandler, GLuint local2clipHandler, GLuint local2eyeHandler, GLuint world2eyeHandler, GLuint normalMatHandler);
};

#endif
