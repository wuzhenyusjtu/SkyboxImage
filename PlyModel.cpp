//
//  PlyModel.cpp
//  Lab5
//
//  Created by Wu Zhenyu on 12/15/15.
//  Copyright (c) 2015 Wu Zhenyu. All rights reserved.
//

#include <stdio.h>
#include <fstream>
#include <iostream>
#include "PlyModel.h"
#include "ply.h"
#include "global.h"

PlyModel::PlyModel(FILE* fp){
    if (fp == NULL) {
        std::cout << "ERROR: NULL input file pointer!" << std::endl;
        exit(0);
    }
    _input = read_ply(fp);
    _plyVertices = NULL;
    _plyFaces = NULL;
    store_ply(_input, &_plyVertices, &_plyFaces, &_vertexCount, &_faceCount, &_vertexNormCount, &_faceNormCount);
    close_ply(_input);
    if (_plyVertices==NULL||_plyFaces==NULL) {
        std::cout << "ERROR: NULL vertices or faces pointer!" << std::endl;
        exit(0);
    }
}

void PlyModel::store_ply(PlyFile* input, PlyVertex ***vertices, PlyFace ***faces,
               unsigned int* vertexcount, unsigned int* facecount,
               int* vertexnormals, int* facenormals) {
    int i, j;
    
    // go through the element types
    for(i = 0; i < input->num_elem_types; i = i + 1) {
        printf("%s%d\n", "Number of Element Types:    ",input->num_elem_types);
        int count;
        
        // setup the element for reading and get the element count
        char* element = setup_element_read_ply(input, i, &count);
        
        // get vertices
        if(strcmp("vertex", element) == 0) {
            *vertices = (PlyVertex**)malloc(sizeof(PlyVertex) * count);
            *vertexcount = count;
            
            // run through the properties and store them
            for(j = 0; j < input->elems[i]->nprops; j = j + 1) {
                printf("%s%d\n", "Number of Properties:    ",input->elems[i]->nprops);
                PlyProperty* property = input->elems[i]->props[j];
                PlyProperty setup;
                
                if(strcmp("x", property->name) == 0 &&
                   property->is_list == PLY_SCALAR) {
                    
                    setup.name = string_list[0];
                    setup.internal_type = Float32;
                    setup.offset = offsetof(PlyVertex, x);
                    setup.count_internal = 0;
                    setup.count_offset = 0;
                    
                    setup_property_ply(input, &setup);
                }
                else if(strcmp("y", property->name) == 0 &&
                        property->is_list == PLY_SCALAR) {
                    
                    setup.name = string_list[1];
                    setup.internal_type = Float32;
                    setup.offset = offsetof(PlyVertex, y);
                    setup.count_internal = 0;
                    setup.count_offset = 0;
                    
                    setup_property_ply(input, &setup);
                }
                else if(strcmp("z", property->name) == 0 &&
                        property->is_list == PLY_SCALAR) {
                    
                    setup.name = string_list[2];
                    setup.internal_type = Float32;
                    setup.offset = offsetof(PlyVertex, z);
                    setup.count_internal = 0;
                    setup.count_offset = 0;
                    
                    setup_property_ply(input, &setup);
                }
                else if(strcmp("nx", property->name) == 0 &&
                        property->is_list == PLY_SCALAR) {
                    
                    setup.name = string_list[3];
                    setup.internal_type = Float32;
                    setup.offset = offsetof(PlyVertex, nx);
                    setup.count_internal = 0;
                    setup.count_offset = 0;
                    
                    setup_property_ply(input, &setup);
                    *vertexnormals = 1;
                }
                else if(strcmp("ny", property->name) == 0 &&
                        property->is_list == PLY_SCALAR) {
                    
                    setup.name = string_list[4];
                    setup.internal_type = Float32;
                    setup.offset = offsetof(PlyVertex, ny);
                    setup.count_internal = 0;
                    setup.count_offset = 0;
                    
                    setup_property_ply(input, &setup);
                    *vertexnormals = 1;
                }
                else if(strcmp("nz", property->name) == 0 &&
                        property->is_list == PLY_SCALAR) {
                    
                    setup.name = string_list[5];
                    setup.internal_type = Float32;
                    setup.offset = offsetof(PlyVertex, nz);
                    setup.count_internal = 0;
                    setup.count_offset = 0;
                    
                    setup_property_ply(input, &setup);
                    *vertexnormals = 1;
                }
                // dunno what it is
                else {
                    fprintf(stderr, "unknown property type found in %s: %s\n",
                            element, property->name);
                }
            }
            
            // do this if you want to grab the other data
            // list_pointer = get_other_properties_ply
            //                (input, offsetof(Vertex, struct_pointer));
            printf("%s%d\n", "Number of Vertices:    ", count);
            // copy the data
            for(j = 0; j < count; j = j + 1) {
                (*vertices)[j] = (PlyVertex*)malloc(sizeof(PlyVertex));
                
                get_element_ply(input, (void*)((*vertices)[j]));
            }
        }
        // get faces
        else if(strcmp("face", element) == 0) {
            *faces = (PlyFace**)malloc(sizeof(PlyFace) * count);
            *facecount = count;
            
            // run through the properties and store them
            for(j = 0; j < input->elems[i]->nprops; j = j + 1) {
                printf("%s%d\n", "Number of Properties:    ",input->elems[i]->nprops);
                PlyProperty* property = input->elems[i]->props[j];
                PlyProperty setup;
                
                if(strcmp("vertex_indices", property->name) == 0 &&
                   property->is_list == PLY_LIST) {
                    
                    setup.name = string_list[6];
                    setup.internal_type = Uint32;
                    setup.offset = offsetof(PlyFace, vertices);
                    setup.count_internal = Uint32;
                    setup.count_offset = offsetof(PlyFace, count);
                    
                    setup_property_ply(input, &setup);
                }
                else if(strcmp("nx", property->name) == 0 &&
                        property->is_list == PLY_SCALAR) {
                    
                    setup.name = string_list[3];
                    setup.internal_type = Float32;
                    setup.offset = offsetof(PlyFace, nx);
                    setup.count_internal = 0;
                    setup.count_offset = 0;
                    
                    setup_property_ply(input, &setup);
                    *facenormals = 1;
                }
                else if(strcmp("ny", property->name) == 0 &&
                        property->is_list == PLY_SCALAR) {
                    
                    setup.name = string_list[4];
                    setup.internal_type = Float32;
                    setup.offset = offsetof(PlyFace, ny);
                    setup.count_internal = 0;
                    setup.count_offset = 0;
                    
                    setup_property_ply(input, &setup);
                    *facenormals = 1;
                }
                else if(strcmp("nz", property->name) == 0 &&
                        property->is_list == PLY_SCALAR) {
                    
                    setup.name = string_list[5];
                    setup.internal_type = Float32;
                    setup.offset = offsetof(PlyFace, nz);
                    setup.count_internal = 0;
                    setup.count_offset = 0;
                    
                    setup_property_ply(input, &setup);
                    *facenormals = 1;
                }
                // dunno what it is
                else {
                    fprintf(stderr, "unknown property type found in %s: %s\n",
                            element, property->name);
                }
            }
            
            // do this if you want to grab the other data
            // list_pointer = get_other_properties_ply
            //                (input, offsetof(Face, struct_pointer));
            
            printf("%s%d\n", "Number of Faces:    ", count);
            // copy the data
            for(j = 0; j < count; j = j + 1) {
                (*faces)[j] = (PlyFace*)malloc(sizeof(PlyFace));
                
                get_element_ply(input, (void*)((*faces)[j]));
            }
        }
        // who knows?
        else {
            fprintf(stderr, "unknown element type found: %s\n", element);
        }
    }
    
    // if you want to grab the other data do this
    // get_other_element_ply(input);
}

void PlyModel::initVBO(){
    _verts = new Vertex[_vertexCount];
    // Assume the model are meshed by triangles
    _indices = new GLuint[_faceCount*3];
    float min[3], max[3];
    min[0] = min[1] = min[2] = 99999999;
    max[0] = max[1] = max[2] = -99999999;
    for (int i=0; i<_vertexCount; i++) {
        _verts[i].location[0] = _plyVertices[i]->x;
        _verts[i].location[1] = _plyVertices[i]->y;
        _verts[i].location[2] = _plyVertices[i]->z;
        _verts[i].location[3] = 1.0;
        if (_verts[i].location[0] < min[0]) min[0] = _verts[i].location[0];
        if (_verts[i].location[1] < min[1]) min[1] = _verts[i].location[1];
        if (_verts[i].location[2] < min[2]) min[2] = _verts[i].location[2];
        if (_verts[i].location[0] > max[0]) max[0] = _verts[i].location[0];
        if (_verts[i].location[1] > max[1]) max[1] = _verts[i].location[1];
        if (_verts[i].location[2] > max[2]) max[2] = _verts[i].location[2];
        _verts[i].normal[0] = _plyVertices[i]->nx;
        _verts[i].normal[1] = _plyVertices[i]->ny;
        _verts[i].normal[2] = _plyVertices[i]->nz;
        _verts[i].normal[3] = 0.0;
        
        _verts[i].color[0] = 1.0;
        _verts[i].color[1] = 1.0;
        _verts[i].color[2] = 1.0;
        _verts[i].color[3] = 1.0;
    }
    for (int i=0; i<_faceCount; i++) {
        if (_plyFaces[i]->count != 3) {
            std::cout << "ERROR: Cannot work on non-triangluar face!" << std::endl;
            exit(0);
        }
        _indices[i*3] = _plyFaces[i]->vertices[0];
        _indices[i*3+1] = _plyFaces[i]->vertices[1];
        _indices[i*3+2] = _plyFaces[i]->vertices[2];
    }
    glGenBuffers(1, &_vboHandle);
    glBindBuffer(GL_ARRAY_BUFFER, _vboHandle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*_vertexCount, _verts, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glGenBuffers(1, &_indexVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*_faceCount*3, _indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);  // clean up
    delete _verts;
    delete _indices;
}

void PlyModel::draw(float* local2clip, float* local2eye, float* world2eye, GLuint posHandler, GLuint colorHandler, GLuint normalHandler, GLuint local2clipHandler, GLuint local2eyeHandler, GLuint world2eyeHandler, GLuint normalMatHandler)
{
    glBindBuffer(GL_ARRAY_BUFFER, _vboHandle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexVBO);
    
    glVertexAttribPointer(posHandler,4,GL_FLOAT, GL_FALSE, 48,(char*) NULL+0);  // position
    glVertexAttribPointer(colorHandler,4,GL_FLOAT, GL_FALSE, 48,(char*) NULL+32); // color
    glVertexAttribPointer(normalHandler,4,GL_FLOAT, GL_FALSE, 48,(char*) NULL+16); // normal
    
    glm::mat4 normal_matrix = glm::inverse((glm::mat4)*local2eye);
    normal_matrix = glm::transpose(normal_matrix);
    
    glUniformMatrix4fv(local2clipHandler, 1, GL_FALSE, (float*) local2clip);   // pass the local2clip matrix
    glUniformMatrix4fv(local2eyeHandler, 1, GL_FALSE, (float*) local2eye);   // pass the local2eye matrix
    glUniformMatrix4fv(world2eyeHandler, 1, GL_FALSE, (float*) &normal_matrix[0][0]);   // pass the local2eye matrix
    glUniformMatrix4fv(normalMatHandler, 1, GL_FALSE, (float*) world2eye);   // pass the local2eye matrix
    
    glDrawElements(GL_TRIANGLES, _faceCount*3, GL_UNSIGNED_INT, (char*) NULL+0);
}