#pragma once 

#include "vec.h"
#include "quat.h"
#include "array.h"

#include <assert.h>
#include <stdio.h>

//--------------------------------------

enum Bones 
{
    Bone_Entity             = 0, // no idea what this is           
    Bone_Bip002             = 1,
    Bone_Bip002_Pelvis      = 2,
    Bone_Bip002_Spine       = 3,
    Bone_Bip002_Spine1      = 4,
    Bone_Bip002_Spine2      = 5,
    Bone_Bip002_Neck        = 6,
    Bone_Bip002_Head        = 7,
    Bone_Bip002_L_Clavicle  = 8,
    Bone_Bip002_L_UpperArm  = 9,
    Bone_Bip002_L_Forearm   = 10,
    Bone_Bip002_L_Hand      = 11,
    Bone_Bip002_L_Finger0   = 12,
    Bone_Bip002_L_Finger01  = 13,
    Bone_Bip002_L_Finger02  = 14,
    Bone_Bip002_L_Finger1   = 15,
    Bone_Bip002_L_Finger11  = 16,
    Bone_Bip002_L_Finger12  = 17,
    Bone_Bip002_L_Finger2   = 18,
    Bone_Bip002_L_Finger21  = 19,
    Bone_Bip002_L_Finger22  = 20,
    Bone_Bip002_L_Finger3   = 21,
    Bone_Bip002_L_Finger31  = 22,
    Bone_Bip002_L_Finger32  = 23,
    Bone_Bip002_L_Finger4   = 24,
    Bone_Bip002_L_Finger41  = 25,
    Bone_Bip002_L_Finger42  = 26,
    Bone_L_ForeArm_Twist    = 27,
    Bone_L_UpperArm_Twist   = 28,
    Bone_R_Clavicle         = 29,
    Bone_R_UpperArm         = 30,
    Bone_R_Forearm          = 31,
    Bone_R_Hand             = 32,
    Bone_R_Finger0          = 33,
    Bone_R_Finger01         = 34,
    Bone_R_Finger02         = 35,
    Bone_R_Finger1          = 36,
    Bone_R_Finger11         = 37,
    Bone_R_Finger12         = 38,
    Bone_R_Finger2          = 39,
    Bone_R_Finger21         = 40,
    Bone_R_Finger22         = 41,
    Bone_R_Finger3          = 42,
    Bone_R_Finger31         = 43,
    Bone_R_Finger32         = 44,
    Bone_R_Finger4          = 45,
    Bone_R_Finger41         = 46,
    Bone_R_Finger42         = 47,
    Bone_R_ForeArm_Twist    = 48,
    Bone_R_UpperArm_Twist   = 49,
    Bone_Bone004            = 50,
    Bone_Bone007            = 51,
    Bone_Bone014            = 52,
    Bone_Bip002_L_Thigh     = 53,
    Bone_Bip002_L_Calf      = 54,
    Bone_Bip002_L_Foot      = 55,
    Bone_Bip002_L_Toe0      = 56,
    Bone_Bone009            = 57,
    Bone_Bone010            = 58,
    Bone_Bone002_R_Thigh    = 59,
    Bone_Bip002_R_Calf      = 60,
    Bone_Bip002_R_Foot      = 61,
    Bone_Bip002_R_Toe0      = 62,
    Bone_Bone008            = 63,
    Bone_Bone006            = 64,
    Bone_Bone013            = 65,
};

//--------------------------------------

struct character
{
    array1d<vec3> positions;
    array1d<vec3> normals;
    array1d<vec2> texcoords;
    array1d<unsigned short> triangles;
    
    array2d<float> bone_weights;
    array2d<unsigned short> bone_indices;
    
    array1d<vec3> bone_rest_positions;
    array1d<quat> bone_rest_rotations;
};

void character_load(character& c, const char* filename)
{
    FILE* f = fopen(filename, "rb");
    assert(f != NULL);
    
    array1d_read(c.positions, f);
    array1d_read(c.normals, f);
    array1d_read(c.texcoords, f);
    array1d_read(c.triangles, f);
    
    array2d_read(c.bone_weights, f);
    array2d_read(c.bone_indices, f);
    
    array1d_read(c.bone_rest_positions, f);
    array1d_read(c.bone_rest_rotations, f);
    fclose(f);
}

//--------------------------------------

void linear_blend_skinning_positions(
    slice1d<vec3> anim_positions,
    const slice1d<vec3> rest_positions,
    const slice2d<float> bone_weights,
    const slice2d<unsigned short> bone_indices,
    const slice1d<vec3> bone_rest_positions,
    const slice1d<quat> bone_rest_rotations,
    const slice1d<vec3> bone_anim_positions,
    const slice1d<quat> bone_anim_rotations)
{
    anim_positions.zero();
    
    for (int i = 0; i < anim_positions.size; i++)
    {
        for (int j = 0; j < bone_indices.cols; j++)
        {
            if (bone_weights(i, j) > 0.0f)
            {
                int b = bone_indices(i, j);
                
                vec3 position = rest_positions(i);            
                position = quat_mul_vec3(quat_inv(bone_rest_rotations(b)), position - bone_rest_positions(b));
                position = quat_mul_vec3(bone_anim_rotations(b), position) + bone_anim_positions(b);
                
                anim_positions(i) = anim_positions(i) + bone_weights(i, j) * position;
            }
        } 
    }
}

void linear_blend_skinning_normals(
    slice1d<vec3> anim_normals,
    const slice1d<vec3> rest_normals,
    const slice2d<float> bone_weights,
    const slice2d<unsigned short> bone_indices,
    const slice1d<quat> bone_rest_rotations,
    const slice1d<quat> bone_anim_rotations)
{
    anim_normals.zero();
    
    for (int i = 0; i < anim_normals.size; i++)
    { 
        for (int j = 0; j < bone_indices.cols; j++)
        {
            if (bone_weights(i, j) > 0.0f)
            {
                int b = bone_indices(i, j);
                
                vec3 normal = rest_normals(i);
                normal = quat_mul_vec3(quat_inv(bone_rest_rotations(b)), normal);
                normal = quat_mul_vec3(bone_anim_rotations(b), normal);
                
                anim_normals(i) = anim_normals(i) + bone_weights(i, j) * normal;
            }
        }
    }
    
    for (int i = 0; i < anim_normals.size; i++)
    { 
        anim_normals(i) = normalize(anim_normals(i));
    }
}

