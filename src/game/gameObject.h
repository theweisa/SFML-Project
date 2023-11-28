#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SFML/Graphics.hpp>
#include <string>

class PhysicsBody;
#include "physicsBody.h"
#include "animation.h"

/*
game object:
	- the ability to have a sprite & animations
	- ability to be scaled/rotated/have its position set/gotten
	- yo mama

*/

class GameObject {
	protected:
	public:
		// vars
		std::string id;
		sf::Texture texture;
		sf::Sprite sprite;
		sf::Vector2f scale;
		sf::Vector2f dimensions;
		std::unordered_map<std::string, Animation*> animations;
		sf::Vector2f localPosition;
		PhysicsBody* body = nullptr;

		// methods
		GameObject();
		GameObject(sf::Texture*, sf::Vector2f pxScale=sf::Vector2f(0,0));
		GameObject(const GameObject&);
		~GameObject();

		virtual void Render(sf::RenderTarget&);
		virtual void Update(float);
		virtual void UpdatePhysicsBody(float);

		// mutators
		void AddPhysicsBody(PhysicsBody*);
		void AddAnimation(std::string, Animation*);
		void AddAnimation(std::string key, sf::Texture & spriteSheet, float fps, int startFrameX, int endFramesX, int startFrameY, int endFramesY, int newWidth, int newHeight);

		virtual void SetPosition(sf::Vector2f newPos);
		virtual void SetScale(sf::Vector2f newScale);
		virtual void SetLocalPosition(sf::Vector2f newLocal);
		void SetSpriteDimensions(sf::Vector2f);
		virtual void SetPosition(float newX, float newY);
		void SetRotation(float newAngle);
		void SetTexture(const sf::Texture*);
		void SetTexture(sf::Texture*, sf::Vector2f);
		void SetTexture(sf::Texture*);

		// accessors
		virtual const sf::Vector2f GetPosition() const { return sprite.getPosition(); }
		const bool GetEndOfAnimation(const std::string key);
		const bool InsideBounds(sf::Vector2f);
};

#endif