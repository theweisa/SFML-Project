#ifndef PONG_H
#define PONG_H

#include "../game/game.h"
#include "paddle.h"

class Pong : public Game {
protected:
    enum GameState { PreStart, Start, Pause, Play, Score, GameOver };
    GameState gameState;
    GameState previousState;
    GameObject* ball;
    Paddle* playerOne;
    Paddle* playerTwo;
    int winningPoints=11;
    bool sendLeft;
    float baseBallSpeed=100.f;
    float routineTimer=0;
    int routineStep=0;
    sf::VertexArray line;
public:
    Pong();
    ~Pong() {};
    void InitGame() override;
    void StartGame(bool playerOne, bool playerTwo);
    void ShowMenu();
    void StartMenu();
    void Render() override;
    void InitAssets() override;
    void Update() override;
    void UpdatePollEvents() override;
    void UpdateGameObjects() override;
    void OnScreenExit(GameObject&) override;
    void UpdateInputs() override;
    void PlayerScore(Paddle*);
    void CreateBall();
    void ScoreRoutine();
    void WinRoutine();
    void FinishRoutine();
    void UpdateDeltaTime() override;
    void TogglePause(bool);
    void RandomizeBallDirection();
};

#endif