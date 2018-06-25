#ifndef PACKET_H
#define PACKET_H

#define _ROBOT_COUNT 5

struct Vec2D {
    double x,y;
};

struct MovingObject {
    Vec2D pos;
    Vec2D dir;
    Vec2D vel;
    Vec2D acc;
    double angularVel;
};

struct RawMovingObject {
    Vec2D pos;
    Vec2D dir;
};

class Packet
{
public:
    Packet();
    bool parse(char* buffer, int len);
    char* serial();

    int gameState;
    MovingObject ball;
    MovingObject yellow[_ROBOT_COUNT];
    MovingObject blue[_ROBOT_COUNT];

    RawMovingObject R_ball;
    RawMovingObject R_blue[_ROBOT_COUNT];
    RawMovingObject R_yellow[_ROBOT_COUNT];

    char roles[5];
    char mode;

};

#endif // PACKET_H
