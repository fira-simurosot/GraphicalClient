#ifndef DRAWPACKET_H
#define DRAWPACKET_H

#include <vector>

struct col {
    double r,g,b,a;
};

struct vec2 {
    double x,y;
    col c;
};

struct cir2{
    vec2 center;
    col c;
    double a1, a2;
    double rad;
    bool fill;
};

struct seg2 {
    vec2 p1,p2;
    col c;
};

struct rec2 {
    vec2 p1,p2,p3,p4;
    col c;
    bool fill;
};

struct tex2 {
    vec2 pos;
    double size;
    col c;
    char* tex;
};

struct pol2 {
    std::vector<vec2> vecs;
    bool fill;
    col c;
};

class DrawPacket
{
public:
    DrawPacket();
    std::vector<vec2> vectors;
    std::vector<cir2> circles;
    std::vector<seg2> segments;
    std::vector<rec2> rects;
    std::vector<pol2> polygons;
    std::vector<tex2> texts;


};

#endif // DRAWPACKET_H
