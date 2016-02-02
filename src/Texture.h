//
//  Texture.h
//  Lab5
//
//  Created by Wu Zhenyu on 12/15/15.
//  Copyright (c) 2015 Wu Zhenyu. All rights reserved.
//

#ifndef Lab5_Texture_h
#define Lab5_Texture_h
#include <GL/glew.h>
#include <glm/glm.hpp>

class Texture{
private:
    GLubyte *texImage;
    const char* fname;
    int width;
    int height;
    static unsigned int textureCount;
public:
    ////////////////////////////////////////////////////////////
    //
    // create a checkerboad texture
    //
    Texture(const char* name);
    
    
    void read_Image();
    
    void initTexture();
};
#endif
