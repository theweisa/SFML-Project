#include "levelEditor.h"

LevelEditor::LevelEditor() {
    InitGrid(64, 64, sf::Vector2f(25.f, 25.f), sf::Vector2f(25.f, 25.f));
}
LevelEditor::~LevelEditor() {
    for (int i = grid.getVertexCount()-1; i >= 0; i--) {
        delete &grid[i];
    }
    grid.clear();
}
void LevelEditor::InitGrid(int xDim, int yDim, sf::Vector2f xMargins, sf::Vector2f yMargins) {
    gridDimensions = sf::Vector2i(xDim, yDim);
    grid = sf::VertexArray(sf::Lines, gridDimensions.x*gridDimensions.y*2);
    int verticePos = 0;

    for (int i = 0; i < gridDimensions.y; i++) {
        grid[verticePos].position = sf::Vector2f(xMargins.x, (i*(windowHeight-(yMargins.x+yMargins.y))/(gridDimensions.y-1))+yMargins.x);
        grid[verticePos+1].position = sf::Vector2f(windowWidth-xMargins.y, (i*(windowHeight-(yMargins.x+yMargins.y))/(gridDimensions.y-1))+yMargins.x);
        verticePos += 2;
    }
    for (int i = 0; i < gridDimensions.x; i++) {
        grid[verticePos].position = sf::Vector2f((i*(windowWidth-(xMargins.x+xMargins.y))/(gridDimensions.x-1))+xMargins.x, yMargins.x);
        grid[verticePos+1].position = sf::Vector2f((i*(windowWidth-(xMargins.x+xMargins.y))/(gridDimensions.x-1))+xMargins.x, windowHeight-yMargins.y);
        verticePos += 2;
    }
}

void LevelEditor::Render() {
    Game::Render();
    window->draw(grid);
}