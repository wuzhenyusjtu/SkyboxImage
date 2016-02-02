//
//  CubeMap.h
//  Lab5
//
//  Created by Wu Zhenyu on 12/15/15.
//  Copyright (c) 2015 Wu Zhenyu. All rights reserved.
//

#ifndef Lab5_CubeMap_h
#define Lab5_CubeMap_h
#include <GL/glew.h>
#include <glm/glm.hpp>

class CubeMap{
private:
    GLubyte *_frontImage;
    int frontWidth;
    int frontHeight;
    GLubyte *_backImage;
    int backWidth;
    int backHeight;
    GLubyte *_leftImage;
    int leftWidth;
    int leftHeight;
    GLubyte *_rightImage;
    int rightWidth;
    int rightHeight;
    GLubyte *_topImage;
    int topWidth;
    int topHeight;
    GLubyte *_bottomImage;
    int bottomWidth;
    int bottomHeight;
    GLuint cubemap_texture;
public:
    CubeMap(const char *front, const char *back, const char *left, const char* right, const char* top, const char*bottom);
    
    void read_Image(const char *fname, GLubyte **texImage, int &width, int &height);
    
    void init_cubemap();
    
};

#endif
