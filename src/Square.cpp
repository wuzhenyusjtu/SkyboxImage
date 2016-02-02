//
//  Square.cpp
//  Lab5
//
//  Created by Wu Zhenyu on 12/15/15.
//  Copyright (c) 2015 Wu Zhenyu. All rights reserved.
//

#include <stdio.h>
#include "Square.h"

Square::Square(){
    sqverts[0].location[0] = -0.5; sqverts[0].location[1] = 0;  sqverts[0].location[2] = -0.5;  sqverts[0].location[3] = 1;
    sqverts[0].normal[0] = 0; sqverts[0].normal[1] = 1;  sqverts[0].normal[2] = 0;  sqverts[0].normal[3] = 0;
    sqverts[0].texCoord[0] = 0; sqverts[0].texCoord[1] = 0;
    sqverts[0].color[0] = 0; sqverts[0].color[1] = 0;  sqverts[0].color[2] = 1;  sqverts[0].color[3] = 1;
    
    sqverts[1].location[0] = 0.5; sqverts[1].location[1] = 0;  sqverts[1].location[2] = -0.5;  sqverts[1].location[3] = 1;
    sqverts[1].normal[0] = 0; sqverts[1].normal[1] = 1;  sqverts[1].normal[2] = 0;  sqverts[1].normal[3] = 0;
    sqverts[1].texCoord[0] = 1; sqverts[1].texCoord[1] = 0;
    sqverts[1].color[0] = 1; sqverts[1].color[1] = 0;  sqverts[1].color[2] = 1;  sqverts[1].color[3] = 1;
    
    sqverts[2].location[0] = 0.5; sqverts[2].location[1] = 0;  sqverts[2].location[2] = 0.5;  sqverts[2].location[3] = 1;
    sqverts[2].normal[0] = 0; sqverts[2].normal[1] = 1;  sqverts[2].normal[2] = 0;  sqverts[2].normal[3] = 0;
    sqverts[2].texCoord[0] = 1; sqverts[2].texCoord[1] = 1;
    sqverts[2].color[0] = 1; sqverts[2].color[1] = 1;  sqverts[2].color[2] = 1;  sqverts[2].color[3] = 1;
    
    sqverts[3].location[0] = -0.5; sqverts[3].location[1] = 0;  sqverts[3].location[2] = 0.5;  sqverts[3].location[3] = 1;
    sqverts[3].normal[0] = 0; sqverts[3].normal[1] = 1;  sqverts[3].normal[2] = 0;  sqverts[3].normal[3] = 0;
    sqverts[3].texCoord[0] = 0; sqverts[3].texCoord[1] = 1;
    sqverts[3].color[0] =0; sqverts[3].color[1] = 1;  sqverts[3].color[2] = 1;  sqverts[3].color[3] = 1;
    
    
    sqindices[0] = 0;   sqindices[1] = 1;   sqindices[2] = 2;
    sqindices[3] = 2;   sqindices[4] = 3;   sqindices[5] = 0;
    
}

void Square::initVBO()
{
    glGenBuffers(1, &vboHandle);   // create an interleaved VBO object
    
    glBindBuffer(GL_ARRAY_BUFFER, vboHandle);   // bind the second handle
    glBufferData(GL_ARRAY_BUFFER, sizeof(myVertex)*4, sqverts, GL_STATIC_DRAW); // allocate space and copy the position data over
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);   // clean up
    
    glGenBuffers(1, &indexVBO);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*6, sqindices, GL_STATIC_DRAW);  // load the index data
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);  // clean up
    
    // by now, we moved the position and color data over to the graphics card. There will be no redundant data copy at drawing time
}

void Square::draw(float* local2clip, float* local2eye, float* world2eye, GLuint posHandler, GLuint colorHandler, GLuint normalHandler, GLuint coordHandler, GLuint local2clipHandler, GLuint local2eyeHandler, GLuint world2eyeHandler, GLuint normalMatHandler)
{
    glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
    
    glEnableVertexAttribArray(coordHandler);
    
    glVertexAttribPointer(posHandler,4,GL_FLOAT, GL_FALSE, sizeof(myVertex),(char*) NULL+0);  // position
    glVertexAttribPointer(normalHandler,4,GL_FLOAT, GL_FALSE, sizeof(myVertex),(char*) NULL+16); // normal
    glVertexAttribPointer(colorHandler,4,GL_FLOAT, GL_FALSE, sizeof(myVertex),(char*) NULL+32); // color
    glVertexAttribPointer(coordHandler,2,GL_FLOAT, GL_FALSE, sizeof(myVertex),(char*) NULL+48); // texCoord
    
    glm::mat4 normal_matrix = glm::inverse((glm::mat4)*local2eye);
    normal_matrix = glm::transpose(normal_matrix);
    
    
    glUniformMatrix4fv(local2clipHandler, 1, GL_FALSE, (float*) local2clip);   // pass the local2clip matrix
    glUniformMatrix4fv(local2eyeHandler, 1, GL_FALSE, (float*) local2eye);   // pass the local2eye matrix
    glUniformMatrix4fv(normalMatHandler, 1, GL_FALSE, (float*) &normal_matrix[0][0]);   // pass the local2eye matrix
    glUniformMatrix4fv(world2eyeHandler, 1, GL_FALSE, (float*) world2eye);   // pass the local2eye matrix
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (char*) NULL+0);
}