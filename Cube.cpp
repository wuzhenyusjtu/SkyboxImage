//
//  Cube.cpp
//  Lab5
//
//  Created by Wu Zhenyu on 12/15/15.
//  Copyright (c) 2015 Wu Zhenyu. All rights reserved.
//

#include <stdio.h>
#include "Cube.h"

Cube::Cube(float size, float r, float g, float b)
{
    // the radius of the stack
    // we draw two stacks for a cube
    float radius = size / sqrt(2.0);
    // Delta angle
    float Dangle = 2*M_PI/4.0;
    for (int i=0; i<2; i++) {
        for (int j=0; j<4; j++) {
            verts[i*4+j].location[0]=radius*sin(M_PI/4.0+Dangle*j);
            verts[i*4+j].location[1]=size/2-size*i;
            verts[i*4+j].location[2]=radius*cos(M_PI/4.0+Dangle*j);
            verts[i*4+j].location[3]=1;
            float module = sqrt(pow(verts[i*4+j].location[1],2)+pow(radius, 2));
            verts[i*4+j].normal[1] = verts[i*4+j].location[1]/module;
            verts[i*4+j].normal[2] = verts[i*4+j].location[2]/module;
            verts[i*4+j].normal[0] = verts[i*4+j].location[0]/module;
            verts[i*4+j].normal[3] = 0.0;
            verts[i*4+j].color[0]=r;
            verts[i*4+j].color[1]=g;
            verts[i*4+j].color[2]=b;
            verts[i*4+j].color[3]=1;
        }
    }
    
    // negative z face
    indices[0] = 0;   indices[1] = 1;   indices[2] = 2;
    indices[3] = 2;   indices[4] = 3;   indices[5] = 0;
    // positive z face
    indices[6] = 4;   indices[7] = 5;   indices[8] = 6;
    indices[9] = 6;   indices[10] = 7;   indices[11] =4;
    // negative y face
    indices[12] = 4;   indices[13] = 0;   indices[14] = 3;
    indices[15] = 3;   indices[16] = 7;   indices[17] = 4;
    // positive y face
    indices[18] = 5;   indices[19] = 1;   indices[20] = 2;
    indices[21] = 2;   indices[22] = 6;   indices[23] = 5;
    // negative x face
    indices[24] = 4;   indices[25] = 5;   indices[26] = 1;
    indices[27] = 1;   indices[28] = 0;   indices[29] = 4;
    // positive x face
    indices[30] = 7;   indices[31] = 6;   indices[32] = 2;
    indices[33] = 2;   indices[34] = 3;   indices[35] = 7;
}

void Cube::initVBO()
{
    glGenBuffers(1, &vboHandle);   // create an interleaved VBO object
    glBindBuffer(GL_ARRAY_BUFFER, vboHandle);   // bind the first handle
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*8, verts, GL_STATIC_DRAW); // allocate space and copy the position data over
    glBindBuffer(GL_ARRAY_BUFFER, 0);   // clean up
    
    glGenBuffers(1, &indexVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte)*36, indices, GL_STATIC_DRAW);  // load the index data
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);  // clean up
}

void Cube::draw(float* local2clip, float* local2eye, float* world2eye, GLuint posHandler, GLuint colorHandler, GLuint normalHandler, GLuint local2clipHandler, GLuint local2eyeHandler, GLuint world2eyeHandler, GLuint normalMatHandler, GLuint eye2worMatHandler)
{
    glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
    
    glVertexAttribPointer(posHandler,4,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+0);  // position
    glVertexAttribPointer(colorHandler,4,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+32); // color
    glVertexAttribPointer(normalHandler,4,GL_FLOAT, GL_FALSE, sizeof(Vertex),(char*) NULL+16); // normal
    
    glm::mat4 normal_matrix = glm::inverse((glm::mat4)*local2eye);
    normal_matrix = glm::transpose(normal_matrix);
    
    glm::mat4 eye2world = glm::inverse((glm::mat4)*world2eye);
    
    glUniformMatrix4fv(local2clipHandler, 1, GL_FALSE, (float*) local2clip);   // pass the local2clip matrix
    glUniformMatrix4fv(local2eyeHandler, 1, GL_FALSE, (float*) local2eye);   // pass the local2eye matrix
    glUniformMatrix4fv(normalMatHandler, 1, GL_FALSE, (float*) &normal_matrix[0][0]);   // pass the local2eye matrix
    glUniformMatrix4fv(world2eyeHandler, 1, GL_FALSE, (float*) world2eye);   // pass the local2eye matrix
    glUniformMatrix4fv(eye2worMatHandler, 1, GL_FALSE, (float*) &eye2world[0][0]);   // pass the local2eye matrix
    
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (char*) NULL+0);
}