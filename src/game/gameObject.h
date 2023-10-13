#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SFML/Graphics.hpp>
#include <string>
#include "animation.h"


/*
game object:
	- the ability to have a sprite & animations
	- ability to be scaled/rotated/have its position set/gotten
	- 

*/
class GameObject {
	protected:
	public:
		// vars
		std::string id;
		sf::Texture texture;
		sf::Sprite sprite;
		sf::Vector2f dimensions;
		std::unordered_map<std::string, Animation*> animations;
		sf::Vector2f localPosition;

		// methods
		GameObject();
		GameObject(std::string, sf::Texture*, sf::Vector2f pxScale=sf::Vector2f(0,0));
		GameObject(const GameObject&);
		~GameObject();

		virtual void Render(sf::RenderTarget&);
		virtual void Update(float);

		// mutators
		void AddAnimation(std::string, Animation*);
		void AddAnimation(std::string key, sf::Texture & spriteSheet, float fps, int startFrameX, int endFramesX, int startFrameY, int endFramesY, int newWidth, int newHeight);

		void SetPosition(sf::Vector2f newPos);
		void SetLocalPosition(sf::Vector2f newLocal);
		void SetSpriteDimensions(sf::Vector2f);
		void SetPosition(float newX, float newY);
		void SetRotation(float newAngle);
		void SetTexture(const sf::Texture*);
		void SetTexture(sf::Texture*, sf::Vector2f);
		void SetTexture(sf::Texture*);

		// accessors
		const sf::Vector2f GetPosition() const { return sprite.getPosition(); }
		const bool GetEndOfAnimation(const std::string key);
		const bool InsideBounds(sf::Vector2f);
};

#endif