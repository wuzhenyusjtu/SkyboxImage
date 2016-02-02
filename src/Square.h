//
//  Square.h
//  Lab5
//
//  Created by Wu Zhenyu on 12/15/15.
//  Copyright (c) 2015 Wu Zhenyu. All rights reserved.
//

#ifndef Lab5_Square_h
#define Lab5_Square_h
#include <GL/glew.h>
#include <glm/glm.hpp>

class Square{
    struct myVertex{
        float location[4];
        float normal[4];
        float color[4];
        float texCoord[2];
    };
private:
    myVertex sqverts[4];
    GLuint sqindices[6];
    GLuint vboHandle;   // a VBO that contains interleaved positions and colors
    GLuint indexVBO;
public:
    Square();
    void initVBO();
    void draw(float* local2clip, float* local2eye, float* world2eye, GLuint posHandler, GLuint colorHandler, GLuint normalHandler, GLuint coordHandler, GLuint local2clipHandler, GLuint local2eyeHandler, GLuint world2eyeHandler, GLuint normalMatHandler);
};

#endif
