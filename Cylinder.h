//
//  Cylinder.h
//  Lab5
//
//  Created by Wu Zhenyu on 12/15/15.
//  Copyright (c) 2015 Wu Zhenyu. All rights reserved.
//

#ifndef Lab5_Cylinder_h
#define Lab5_Cylinder_h

#include <GL/glew.h>
#include <glm/glm.hpp>

struct Vertex;

class Cylinder{
private:
    GLuint vboHandle;   // a VBO that contains interleaved positions and colors
    GLuint indexVBO;
    GLuint *indices;
    Vertex *verts;
    int nindices;
    int nvertices;
public:
    Cylinder(float baseRadius, float topRadius, float height, int nslices, int nstacks, float red, float green, float blue);

    void initVBO();
    
    void draw(float* local2clip, float* local2eye, float* world2eye, GLuint posHandler, GLuint colorHandler, GLuint normalHandler, GLuint local2clipHandler, GLuint local2eyeHandler, GLuint world2eyeHandler, GLuint normalMatHandler);
};

#endif
