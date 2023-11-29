#ifndef PONG_H
#define PONG_H

#include "../game/game.h"
#include "paddle.h"

class Pong : public Game {
protected:
    enum GameState { Start, Play, Score, GameOver };
    GameState gameState=Play;
    GameObject* ball;
    Paddle* playerOne;
    Paddle* playerTwo;
    int winningPoints=11;
    float baseBallSpeed=100.f;
    float routineTimer=0;
    int routineStep=0;
    sf::VertexArray line;
public:
    Pong();
    void InitGame() override;
    void Render() override;
    void InitAssets() override;
    void Update() override;
    void OnScreenExit(GameObject&) override;
    void UpdateInputs() override;
    void PlayerScore(Paddle*);
    void CreateBall();
    void ScoreRoutine();
    void WinRoutine();
    void FinishRoutine();
};

#endif