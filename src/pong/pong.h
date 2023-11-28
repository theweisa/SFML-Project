#ifndef PONG_H
#define PONG_H

#include "../game/game.h"
#include "paddle.h"

class Pong : public Game {
protected:
    Paddle* playerOne;
    Paddle* playerTwo;
    sf::VertexArray line;
public:
    Pong();
    void InitGame() override;
    void Render() override;
    void InitAssets() override;
    void Update() override;
    void OnScreenExit(GameObject&) override;
    void UpdateInputs() override;
};

#endif