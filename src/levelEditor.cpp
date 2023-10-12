#include "levelEditor.h"
#include <filesystem>
#include <cmath>

LevelEditor::LevelEditor() {
    InitGrid(32, 32, sf::Vector2f(25.f, 25.f), sf::Vector2f(25.f, 25.f));
    InitAssets();
    currentTiles.push_back(new GameObject("1", assets["test tile"], unitDimensions));
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
    std::string assetPath = "../../../assets/textures/";
    #if _WIN32
        assetPath = "../../../assets/textures/";
    #elif __APPLE__
        assetPath = "../../assets/textures/";
    #endif
    // cwd: "C:\\Users\\Andrew Wei\\Documents\\SFML-Project\\build\\bin\\Debug" lmao
    // mac: "/Users/andrewwei/Documents/SFML-Project/build/bin" idk
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
        SetCurrentTileTextures(assets["test tile"], "1");
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
        toolState = Brush;
        SetCurrentTileTextures(assets["test tile 2"], "2");
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
        if (tile->GetPosition().x < xMargins.x || tile->GetPosition().x > windowWidth-xMargins.x || tile->GetPosition().y < yMargins.x || tile->GetPosition().y > windowHeight-yMargins.y) {
            continue;
        }
        gameObjects["tiles"].push_back(new GameObject(*tile));
    }
}

// idk ill work on you more later
void LevelEditor::EraseTiles() {
    std::cout << "tiles: " << currentTiles.size() << std::endl;
    for (auto tile : currentTiles) {
        int objIndex = 0;
        for (auto itr = gameObjects["tiles"].begin(); itr != gameObjects["tiles"].end(); itr++) {
            if ((*itr)->GetPosition() == tile->GetPosition()) {
                delete (*itr);
                gameObjects["tiles"].erase(itr);
                break;
            }
        }
        
        /*for (auto &obj : gameObjects["tiles"]) {
            if (obj->InsideBounds(tile->GetPosition()+(unitDimensions/2.f))) {
                DeleteGameObject("tiles", objIndex);
                break;
            }
            if (obj->GetPosition() == tile->GetPosition()) {
                DeleteGameObject("tiles", objIndex);
                break;
            }
            objIndex++;
        }*/
    }
}

void LevelEditor::EyedropTile() {
    for (auto *obj : gameObjects["tiles"]) {
        if (obj->InsideBounds(mousePosView)) {
            std::cout << "eyedrop" << std::endl;
            for (auto tile : currentTiles) {
                tile->SetTexture(obj->sprite.getTexture());
                tile->id = obj->id;
            }
            toolState = Brush;
            return;
        }
    }
}

void LevelEditor::SetCurrentTileTextures(sf::Texture* newTexture, std::string newId) {
    for (auto tile : currentTiles) {
        tile->SetTexture(newTexture, unitDimensions);
        if (newId != "") tile->id = newId;
    }
}

void LevelEditor::ChangeBrushSize(int newSize) {
    if (newSize < 1 || newSize > std::min(gridDimensions.x, gridDimensions.y) || currentTiles.size() <= 0) {
        return;
    }
    brushSize = newSize;
    std::cout << "new size: " << brushSize << std::endl;
    GameObject * tile = new GameObject(*currentTiles[0]);
    for (auto& t : currentTiles) {
        delete t;
    }
    currentTiles.clear();
    // GetRelativeGridPosition(mousePosView)
    // what i have: current mouse grid position; can set the local positions to be based on it
    // should be from -unitDim -> +unitDim; should be size / 2 rounded down; then go fr -result -> +result. has to be an int result; 
    // ie.) 2: -2/2 = -1 -> 1
    for (int i = -brushSize/2; i < std::ceil(float(brushSize)/2.f); i++) {
        for (int j = -brushSize/2; j < std::ceil(float(brushSize)/2.f); j++) {
            GameObject * newTile = new GameObject(*tile);
            newTile->SetLocalPosition(sf::Vector2f(unitDimensions.x * i, unitDimensions.y * j));
            newTile->SetPosition(GetRelativeGridPosition(mousePosView));
            currentTiles.push_back(newTile);
        }
    }
    delete tile;

    // set relative positions based on the size
}

void LevelEditor::Update() {
    Game::Update();
    for (auto tile : currentTiles) {
        tile->SetPosition(GetRelativeGridPosition(mousePosView));
    }
}

void LevelEditor::UpdatePollEvents() {
    while (window->pollEvent(ev)) {
		switch (ev.type) {
            case sf::Event::Closed:
                window->close();
                break;
            case sf::Event::MouseWheelScrolled:
                std::cout << "scrolled: " << ev.mouseWheelScroll.delta << std::endl;
                if (ev.mouseWheelScroll.delta > 0) {
                    ChangeBrushSize(brushSize+1);
                }
                else if (ev.mouseWheelScroll.delta < 0) {
                    ChangeBrushSize(brushSize-1);
                }
                break;
            default:
                break;
        }
	}
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