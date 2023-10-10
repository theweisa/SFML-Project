#ifndef LEVEL_EDITOR_H
#define LEVEL_EDITOR_H

#include "game/game.h"
#include <Array>

class LevelEditor : public Game {
    protected:
        sf::VertexArray grid;
        sf::Vector2i gridDimensions;
        sf::Vector2f gridMargins;
    public:
        LevelEditor();
        ~LevelEditor();
        void InitGrid(int, int, sf::Vector2f, sf::Vector2f);
        void Render() override;
};

#endif