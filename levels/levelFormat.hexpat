#include <std/mem>

struct Point {
    char header;
    if (header != 'P') break;
    s32 x;
    s32 y;
    s32 typeLength;
    char type[typeLength];
    s32 parameterLength;
    char parameters[parameterLength];
};

struct Rectangle {
    char header;
    if (header != 'R') break;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
    s32 typeLength;
    char type[typeLength];
    s32 parameterLength;
    char parameters[parameterLength];
};

struct Level {
    u8 GroundLayer[512*256];
    u8 CollisionLayer[512*256];
    Point points[while(!std::mem::eof())];
    Rectangle rectangles[while(!std::mem::eof())];
    
};

Level level @ 0x0;