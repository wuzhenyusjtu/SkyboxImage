//
//  Cone.cpp
//  Lab5
//
//  Created by Wu Zhenyu on 12/15/15.
//  Copyright (c) 2015 Wu Zhenyu. All rights reserved.
//

#include <stdio.h>
#include "Cone.h"
#include "global.h"

Cone::Cone(float baseRadius, float height, int nslices, int nstacks, float red, float green, float blue)
{
    // tan(angle)
    float tantheta = baseRadius/height;
    
    // the radius of stack, varies at different position of the axis
    float radius;
    
    // Number of vertices
    nvertices = nslices * nstacks;
    verts = new Vertex[nvertices];
    
    // delta angle
    float Dangle = 2*M_PI/(float)(nslices-1);
    
    for (int j =0; j<nstacks; j++){
        //std::cout << j << std::endl;
        radius = baseRadius-(height-height*j/(float)(nstacks-1))*tantheta;
        for (int i=0; i<nslices; i++) {
            int idx = j*nslices + i;
            // mesh[j][i]
            float angle = Dangle * i;
            verts[idx].location[2] = radius * cos(angle);
            verts[idx].location[0] = radius * sin(angle);
            verts[idx].location[1] = height/2-height*j/(float)(nstacks-1);
            verts[idx].location[3] = 1.0;
            verts[idx].normal[2]= cos(angle);
            verts[idx].normal[0] = sin(angle);
            verts[idx].normal[1] = 0.0;
            verts[idx].normal[3] = 0.0;
            verts[idx].color[0] = red; verts[idx].color[1] = green; verts[idx].color[2] = blue;
            verts[idx].color[3] = 1.0;
        }
        // now create the index array
    }
    nindices = (nstacks-1)*2*(nslices+1);
    indices = new GLuint[nindices];
    int n = 0;
    for (int j =0; j<nstacks-1; j++)
        for (int i=0; i<=nslices; i++) {
            int idx = j*nslices + i; // mesh[j][mi]
            int idx2 = (j+1) * nslices + i;
            indices[n++] = idx;
            indices[n++] = idx2;
        }
    
}

void Cone::initVBO()
{
    glGenBuffers(1, &vboHandle);
    glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*nvertices, verts, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenBuffers(1, &indexVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*nindices, indices, GL_STATIC_DRAW);  // load the index data
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);  // clean up
}

void Cone::draw(float* local2clip, float* local2eye, float* world2eye, GLuint posHandler, GLuint colorHandler, GLuint normalHandler, GLuint local2clipHandler, GLuint local2eyeHandler, GLuint world2eyeHandler, GLuint normalMatHandler)
{
    glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
    
    glVertexAttribPointer(posHandler,4,GL_FLOAT, GL_FALSE, 48,(char*) NULL+0);  // position
    glVertexAttribPointer(colorHandler,4,GL_FLOAT, GL_FALSE, 48,(char*) NULL+32); // color
    glVertexAttribPointer(normalHandler,4,GL_FLOAT, GL_FALSE, 48,(char*) NULL+16); // normal
    
    glm::mat4 normal_matrix = glm::inverse((glm::mat4)*local2eye);
    normal_matrix = glm::transpose(normal_matrix);
    
    glUniformMatrix4fv(local2clipHandler, 1, GL_FALSE, (float*) local2clip);   // pass the local2clip matrix
    glUniformMatrix4fv(local2eyeHandler, 1, GL_FALSE, (float*) local2eye);   // pass the local2eye matrix
    glUniformMatrix4fv(normalMatHandler, 1, GL_FALSE, (float*) &normal_matrix[0][0]);   // pass the local2eye matrix
    glUniformMatrix4fv(world2eyeHandler, 1, GL_FALSE, (float*) world2eye);   // pass the local2eye matrix
    
    glDrawElements(GL_TRIANGLE_STRIP, nindices, GL_UNSIGNED_INT, (char*) NULL+0);
}