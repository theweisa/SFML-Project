#ifndef SUIKA_BALL_H
#define SUIKA_BALL_H

#include "../game/gameObject.h"

class SuikaBall : public GameObject {
public:
    enum BallType {test1, test2};
    BallType type;
    int points;

    SuikaBall(std::string id, sf::Texture*, sf::Vector2f pos=sf::Vector2f(0,0)) {
        
    }
};

#endif