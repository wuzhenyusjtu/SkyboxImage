//
//  Texture.cpp
//  Lab5
//
//  Created by Wu Zhenyu on 12/15/15.
//  Copyright (c) 2015 Wu Zhenyu. All rights reserved.
//

#include <stdio.h>
#include "Texture.h"

Texture::Texture(const char* name){
    fname = name;
}


void Texture::read_Image()
{
    FILE* in = fopen(fname, "r");
    
    int ccv;
    char header[100];
    fscanf(in, "%s %d %d %d", header, &width, &height, &ccv);
    printf("%s %d %d %d\n", header, width, height, ccv);
    int r, g, b;
    
    texImage = new GLubyte[height*width*4];
    for (int i=height-1; i>=0; i--)
        for (int j=0; j<width; j++)
        {
            fscanf(in, "%d %d %d", &r, &g, &b);
            texImage[i*width*4+j*4+0] = (GLubyte)r; texImage[i*width*4+j*4+1] = (GLubyte)g;
            texImage[i*width*4+j*4+2] = (GLubyte)b;	texImage[i*width*4+j*4+3] = 255;
        }
    fclose(in);
}

void Texture::initTexture() {
    textureCount++;
    
    GLuint renderTex;
    
    read_Image();
    
    glGenTextures(1, &renderTex);
    //std::cout << "TEXTURE COUNT" << std::endl;
    //std::cout << textureCount << std::endl;
    glActiveTexture(GL_TEXTURE0+textureCount-1);
    glBindTexture(GL_TEXTURE_2D, renderTex);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width,
                 height,0, GL_RGBA, GL_UNSIGNED_BYTE,
                 texImage);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    delete[] texImage;
    
    //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

unsigned int Texture::textureCount=0;

