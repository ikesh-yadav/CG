#pragma once
unsigned int CuboidIndices[36] = {
    //bottom face
    0, 1, 2,
    2, 3, 0,
    //top face
    4, 5, 6,
    6, 7, 4,
    //left face
    0, 4, 7,
    7, 3, 0,
    //right face
    6, 5, 1,
    1, 2, 6,
    //back face
    4, 5, 1,
    1, 0, 4,
    //font face
    7, 6, 2,
    2, 3, 7
};
