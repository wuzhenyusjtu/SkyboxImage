//
//  Sphere.cpp
//  Lab5
//
//  Created by Wu Zhenyu on 12/15/15.
//  Copyright (c) 2015 Wu Zhenyu. All rights reserved.
//

#include <stdio.h>
#include "Sphere.h"
#include "global.h"

Sphere::Sphere(float radius,int nslices, int nstacks, float r, float g, float b)
{
    // Number of vertices
    nvertices = nslices * nstacks;
    verts = new Vertex[nvertices];
    
    // Delta angle
    float deltaTheta = 2*M_PI/(float)(nslices-1);
    float deltaPhi = M_PI/(float)(nstacks-1);
    for (int j =0; j<nstacks; j++)
    {
        // Compute the radius of each stack
        float phi = deltaPhi * j;
        //cout << phi << endl;
        for (int i=0; i<nslices; i++) {
            int idx = j*nslices + i; // mesh[j][i]
            float theta = deltaTheta * i;
            verts[idx].location[1] = radius*cos(phi);
            verts[idx].location[2] = radius*sin(phi)*cos(theta);
            verts[idx].location[0] = radius*sin(phi)*sin(theta);
            verts[idx].location[3] = 1.0;
            float module = sqrt(pow(verts[idx].location[1],2)+pow(r, 2));
            verts[idx].normal[1] = verts[idx].location[1]/module;
            verts[idx].normal[2] = verts[idx].location[2]/module;
            verts[idx].normal[0] = verts[idx].location[0]/module;
            verts[idx].normal[3] = 0.0;
            verts[idx].color[0] = r; verts[idx].color[1] = g; verts[idx].color[2] = b;
            verts[idx].color[3] = 1.0;
        }
    }
    
    // now create the index array
    nindices = (nstacks-1)*2*(nslices+1);
    indices = new GLuint[nindices];
    int n = 0;
    for (int j =0; j<nstacks-1; j++)
        for (int i=0; i<=nslices; i++) {
            int mi = i % nslices;
            int idx = j*nslices + mi; // mesh[j][mi]
            int idx2 = (j+1) * nslices + mi;
            indices[n++] = idx;
            indices[n++] = idx2;
        }
}

void Sphere::initVBO()
{
    glGenBuffers(1, &vboHandle);   // create an interleaved VBO object
    glBindBuffer(GL_ARRAY_BUFFER, vboHandle);   // bind the first handle
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*nvertices, verts, GL_STATIC_DRAW); // allocate space and copy the position data over
    glBindBuffer(GL_ARRAY_BUFFER, 0);   // clean up
    
    glGenBuffers(1, &indexVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*nindices, indices, GL_STATIC_DRAW);  // load the index data
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);  // clean up
    
    
}

void Sphere::draw(float ratio, float* local2clip, float* local2eye, float* world2eye, GLuint posHandler, GLuint colorHandler, GLuint normalHandler, GLuint local2clipHandler, GLuint local2eyeHandler, GLuint world2eyeHandler, GLuint normalMatHandler, GLuint eye2worMatHandler)
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
    
    glDrawElements(GL_TRIANGLE_STRIP, nindices*ratio, GL_UNSIGNED_INT, (char*) NULL+0);
}