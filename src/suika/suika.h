#ifndef SUIKA_H
#define SUIKA_H

#include "../game/game.h"

class Suika : public Game {
public:
    Suika();
    void InitGame() override;
};

#endif