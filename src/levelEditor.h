#ifndef LEVEL_EDITOR_H
#define LEVEL_EDITOR_H

#include "game/game.h"
#include <Array>

class LevelEditor : public Game {
    protected:
        sf::VertexArray grid;
        sf::Vector2i gridDimensions;
        sf::Vector2f unitDimensions;
        sf::Vector2f xMargins;
        sf::Vector2f yMargins;
    public:
        LevelEditor();
        ~LevelEditor();
        void InitGrid(int, int, sf::Vector2f, sf::Vector2f);
        sf::Vector2f GetRelativeGridPosition(sf::Vector2f pos);
        void Render() override;
        void Update() override;
};

#endif