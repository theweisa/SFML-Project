#ifndef SUIKA_H
#define SUIKA_H

#include "../game/game.h"

class Suika : public Game {
public:
    sf::FloatRect boundingBox;

    Suika();
    void InitGame() override;
    void InitConstraints();
    void ApplyConstraints(GameObject*);
    void InitAssets() override;
    void UpdateObjectPhysics(GameObject*) override;
    void UpdatePollEvents() override;
};

#endif