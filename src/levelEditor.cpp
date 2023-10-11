#include "levelEditor.h"

LevelEditor::LevelEditor() {
    InitGrid(8, 8, sf::Vector2f(25.f, 25.f), sf::Vector2f(25.f, 25.f));
    InitAssets();
    currentTiles.push_back(new GameObject("1", assets["test tile"], unitDimensions));
    //currentTiles.push_back(sf::RectangleShape(unitDimensions));
}

LevelEditor::~LevelEditor() {
    
}
void LevelEditor::InitGrid(int xDim, int yDim, sf::Vector2f newXMargins, sf::Vector2f newYMargins) {
    gridDimensions = sf::Vector2i(xDim, yDim);
    xMargins = newXMargins;
    yMargins = newYMargins;
    grid = sf::VertexArray(sf::Lines, gridDimensions.x*gridDimensions.y*2);
    unitDimensions = sf::Vector2f((windowWidth-(xMargins.x+xMargins.y))/(gridDimensions.x-1), (windowHeight-(yMargins.x+yMargins.y))/(gridDimensions.y-1));
    int verticePos = 0;

    for (int i = 0; i < gridDimensions.y; i++) {
        grid[verticePos].position = sf::Vector2f(xMargins.x, i*unitDimensions.y+yMargins.x);
        grid[verticePos+1].position = sf::Vector2f(windowWidth-xMargins.y, i*unitDimensions.y+yMargins.x);
        verticePos += 2;
    }
    for (int i = 0; i < gridDimensions.x; i++) {
        grid[verticePos].position = sf::Vector2f(i*unitDimensions.x+xMargins.x, yMargins.x);
        grid[verticePos+1].position = sf::Vector2f(i*unitDimensions.x+xMargins.x, windowHeight-yMargins.y);
        verticePos += 2;
    }
}

void LevelEditor::InitAssets() {
    // cwd: "C:\\Users\\Andrew Wei\\Documents\\SFML-Project\\build\\bin\\Debug" lmao
    const std::string assetPath = "../../../assets/textures/";
    AddAsset("test tile", assetPath+"testTile.png");
    AddAsset("test tile 2", assetPath+"testTile2.png");
    AddAsset("eraser tile", assetPath+"eraser.png");
}
sf::Vector2f LevelEditor::GetRelativeGridPosition(sf::Vector2f pos) {
    sf::Vector2i unitPos = sf::Vector2i((pos.x-xMargins.x) / unitDimensions.x, (pos.y-yMargins.x) / unitDimensions.y);
    sf::Vector2f gridPos = sf::Vector2f(
        std::clamp(xMargins.x+unitPos.x * unitDimensions.x, xMargins.x, windowWidth-xMargins.x-unitDimensions.x),
        std::clamp(yMargins.x+unitPos.y * unitDimensions.y, yMargins.x, windowHeight-yMargins.y-unitDimensions.y)
    );
    //std::cout << "Unit pos: (" << unitPos.x << ", " << unitPos.y << ")" << std::endl;
    //std::cout << "Grid pos: (" << gridPos.x << ", " << gridPos.y << ")" << std::endl;
    return gridPos; 
    
}

void LevelEditor::UpdateInputs() {
    Game::UpdateInputs();
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        switch(toolState) {
            case Brush:
                PlaceTiles();
                break;
            case Erase:
                EraseTiles();
                break;
            case Eyedrop:
                EyedropTile();
                break;
            default:
                break;
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
        toolState = Brush;
        SetCurrentTileTextures(assets["test tile"]);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
        toolState = Brush;
        SetCurrentTileTextures(assets["test tile 2"]);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
        toolState = Erase;
        SetCurrentTileTextures(assets["eraser tile"]);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::B)) {
        toolState = Brush;
        SetCurrentTileTextures(assets["test tile"]);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::I)) {
        toolState = Eyedrop;
        SetCurrentTileTextures(assets["eraser tile"]);
    }
}

void LevelEditor::PlaceTiles() {
    EraseTiles();
    for (auto tile : currentTiles) {
        gameObjects["tiles"].push_back(new GameObject(*tile));
    }
}

void LevelEditor::EraseTiles() {
    for (auto tile : currentTiles) {
        int objIndex = 0;
        for (auto *obj : gameObjects["tiles"]) {
            if (obj->InsideBounds(tile->GetPosition())) {
                DeleteGameObject("tiles", objIndex);
                objIndex--;
            }
            else {
                objIndex++;
            }
        }
    }
}

void LevelEditor::EyedropTile() {
    for (auto *obj : gameObjects["tiles"]) {
        if (obj->InsideBounds(mousePosView)) {
            std::cout << "eyedrop" << std::endl;
            for (auto tile : currentTiles) {
                tile->SetTexture(obj->sprite.getTexture());
            }
            toolState = Brush;
            return;
        }
    }
}

void LevelEditor::SetCurrentTileTextures(sf::Texture* newTexture) {
    for (auto tile : currentTiles) {
        tile->SetTexture(newTexture, unitDimensions);
    }
}

void LevelEditor::Update() {
    Game::Update();
    if (currentTiles.size() > 0)
        currentTiles[0]->SetPosition(GetRelativeGridPosition(mousePosView));
}

void LevelEditor::Render() {
    Game::Render();
    window->draw(grid);
    for (auto objKey : gameObjects) {
        for (auto& obj : objKey.second) {
            obj->Render(*window);
        }
    }
    for (auto& tile : currentTiles) {
        tile->Render(*window);
    }
}