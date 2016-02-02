//
//  PlyModel.h
//  Lab5
//
//  Created by Wu Zhenyu on 12/15/15.
//  Copyright (c) 2015 Wu Zhenyu. All rights reserved.
//

#ifndef Lab5_PlyModel_h
#define Lab5_PlyModel_h
#include <GL/glew.h>
#include <glm/glm.hpp>

struct PlyFile;
struct PlyVertex;
struct PlyFace;
struct Vertex;

class PlyModel{
private:
    char* string_list[7] = {"x", "y", "z", "nx", "ny", "nz", "vertex_indices"};
    typedef struct PlyVertex {
        float x, y, z;
        float nx, ny, nz;
    } PlyVertex;
    typedef struct PlyFace {
        unsigned int count;
        unsigned int *vertices;
        float nx, ny, nz;
    } PlyFace;
    PlyFile* _input;
    PlyVertex** _plyVertices;
    PlyFace** _plyFaces;
    unsigned int _vertexCount;
    unsigned int _faceCount;
    int _vertexNormCount;
    int _faceNormCount;
    GLuint _vboHandle;   // a VBO that contains interleaved positions and colors
    GLuint _indexVBO;
    Vertex* _verts;
    GLuint* _indices;
    
public:
    PlyModel(FILE* fp);
    void store_ply(PlyFile* input, PlyVertex ***vertices, PlyFace ***faces,
                   unsigned int* vertexcount, unsigned int* facecount,
                   int* vertexnormals, int* facenormals);
    void initVBO();
    
    void draw(float* local2clip, float* local2eye, float* world2eye, GLuint posHandler, GLuint colorHandler, GLuint normalHandler, GLuint local2clipHandler, GLuint local2eyeHandler, GLuint world2eyeHandler, GLuint normalMatHandler);
};


#endif

