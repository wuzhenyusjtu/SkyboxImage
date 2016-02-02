//
//  CubeMap.cpp
//  Lab5
//
//  Created by Wu Zhenyu on 12/15/15.
//  Copyright (c) 2015 Wu Zhenyu. All rights reserved.
//

#include <stdio.h>
#include "CubeMap.h"


CubeMap::CubeMap(const char *front, const char *back, const char *left, const char* right, const char* top, const char*bottom){
    read_Image(front, &_frontImage, frontWidth, frontHeight);
    read_Image(back, &_backImage, backWidth, backHeight);
    read_Image(left, &_leftImage, leftWidth, leftHeight);
    read_Image(right, &_rightImage, rightWidth, rightHeight);
    read_Image(top, &_topImage, topWidth, topHeight);
    read_Image(bottom, &_bottomImage, bottomWidth, bottomHeight);
}

void CubeMap::read_Image(const char *fname, GLubyte **texImage, int &width, int &height)
{
    FILE* in = fopen(fname, "r");
    int ccv;
    char header[100];
    fscanf(in, "%s %d %d %d", header, &width, &height, &ccv);
    
    printf("%s %d %d %d\n", header, width, height, ccv);
    int r, g, b;
    
    *texImage = new GLubyte[height*width*4];
    for (int i=height-1; i>=0; i--)
        for (int j=0; j<width; j++)
        {
            fscanf(in, "%d %d %d", &r, &g, &b);
            (*texImage)[i*width*4+j*4+0] = (GLubyte)r; (*texImage)[i*width*4+j*4+1] = (GLubyte)g;
            (*texImage)[i*width*4+j*4+2] = (GLubyte)b; (*texImage)[i*width*4+j*4+3] = 255;
        }
    fclose(in);
}

void CubeMap::init_cubemap(){
    glEnable(GL_TEXTURE_CUBE_MAP);
    glGenTextures(1, &cubemap_texture);
    glActiveTexture(GL_TEXTURE8);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_texture);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    
    //glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, leftWidth, leftHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, _leftImage);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, rightWidth, rightHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, _rightImage);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, leftWidth, leftHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, _leftImage);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, topWidth, topHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, _bottomImage);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, bottomWidth, bottomHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, _topImage);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, frontWidth, frontHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, _frontImage);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, backWidth, backHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, _backImage);
    
    /*
     GLint isTexResident = GL_FALSE;
     glGetTexParameteriv( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_RESIDENT, &isTexResident );
     
     if ( isTexResident == GL_TRUE )
     {
     std::cout << "****************************************" << std::endl;
     std::cout << "TRUE" << std::endl;
     }
     else
     {
     std::cout << "****************************************" << std::endl;
     std::cout << "FALSE" << std::endl;
     }
     */
}
