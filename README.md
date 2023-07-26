## Description
---
This takes in any gltf file and then extracts the model data including the following in the respective order
 - position
 - normal
 - texcoords
 - triangle indicies
 - bone weights
 - bone indicies
 - bone rest position
 - bone rest rotation

## to be done:
---
 - [x] put in bone positions and rotations
 - [x] extract features
 - [x] Check how the bones are structured and organized, and how the bone indicies are ordered, if the bones need to be organized in a specific order, this includes the bones enum in character.h
 - [x] also try to take out the equipment mesh, since all meshes are independent from each other, that shouldn't interfere with the overall model 
 - [ ] transform all joints from local space into world space, also figure out why tf the root bone has such a weird translation vec value.
 - [ ] 