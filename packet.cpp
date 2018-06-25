#include "packet.h"

Packet::Packet() {

}

bool Packet::parse(char *buffer, int len) {
    if (len < 2)  return false;
    ball.pos.x = static_cast<double>(buffer[0] - '0');
    ball.pos.y = static_cast<double>(buffer[1] - '0');
    return true;
}

char *Packet::serial() {
    char* buf = new char[100];
    return buf;
}
