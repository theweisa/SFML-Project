#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SFML/Graphics.hpp>
#include "animation.h"

/*
game object:
	- the ability to have a sprite & animations
	- ability to be scaled/rotated/have its position set/gotten
	- 

*/
class GameObject {
	protected:
		sf::Texture texture;
		sf::Sprite sprite;
		std::unordered_map<std::string, Animation*> animations;
	public:
		GameObject();
		GameObject(sf::Texture);
		GameObject(Animation*);
		~GameObject();

		// mutators
		void AddAnimation(std::string, Animation*);
		void AddAnimation(std::string key, sf::Texture & spriteSheet, float fps, int startFrameX, int endFramesX, int startFrameY, int endFramesY, int newWidth, int newHeight);

		void SetPosition(sf::Vector2f newPos);
		void SetPosition(float newX, float newY);
		void SetRotation(float newAngle);

		// accessors
		const sf::Vector2f GetPosition() const;
		const float GetRotation() const;
		const bool GetEndOfAnimation(const std::string key);
};

#endif