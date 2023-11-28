#ifndef PONG_H
#define PONG_H

#include "../game/game.h"

class Pong : public Game {
protected:
    GameObject* playerOne;
    GameObject* playerTwo;
    sf::Vertex line[2];
public:
    Pong();
    void InitGame() override;
    void Render() override;
    void InitAssets() override;
    void Update() override;
    void OnScreenExit(GameObject&) override;
};

#endif