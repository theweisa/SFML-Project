#ifndef PONG_H
#define PONG_H

#include "../game/game.h"

class Pong : public Game {
public:
    void Update() override;
};

#endif