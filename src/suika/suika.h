#ifndef SUIKA_H
#define SUIKA_H

#include "../game/game.h"
#include "suikaBall.hpp"

class Suika : public Game {
public:
    struct BallData {
        int type;
        std::string assetName;
        float scale;
        int points;
        BallData() {};
        BallData(int _type, std::string asset_name, float _scale, int _points) :
        type(_type), assetName(asset_name), scale(_scale), points(_points) {

        }
    };

    sf::FloatRect boundingBox;
    sf::Vector2f dropPos;
    std::unordered_map<int, BallData> ballDataTable;
    std::vector<int> ballDropList;
    std::vector<SuikaBall*> createdBalls;
    int points = 0;
    int updateSteps = 2;

    int ballToDrop = 0;
    int nextBallToDrop = 0;
    float dropInterval = 0.5f;
    float dropTimer = 0;
    float yLimitPos;

    bool gameOver = false;

    sf::VertexArray loseLine;

    Suika();
    void InitGame() override;
    void InitText() override;
    void InitConstraints();
    void ApplyConstraints(GameObject*);
    void InitAssets() override;
    void UpdateGameObjects() override;
    void UpdateMousePos() override;
    void UpdateObjectPhysics(GameObject*) override;
    void UpdatePollEvents() override;
    void Update() override;
    void SetDisplaySprites();
    void Render() override;
    SuikaBall* InstantiateBall(int type, sf::Vector2f pos);
    SuikaBall* MergeBalls(SuikaBall* ballOne, SuikaBall* ballTwo);
    //GameObject* InstantiateBall(const std::string parent, const std::string textureName, sf::Vector2f pos, PhysicsBody::BodyType bodyType=PhysicsBody::Static);
};

#endif