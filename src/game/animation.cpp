#include "animation.h"

//initialize the animation; use brace initialization since there are a lot of variables to initialize with values that are static
Animation::Animation(sf::Sprite& newSprite, sf::Texture& newSpriteSheet, float newSpeed, int startFrameX, int startFrameY, int endFramesX, int endFramesY, int newWidth, int newHeight)
	: sprite(newSprite), spriteSheet(newSpriteSheet), animationTimer(newSpeed), width(newWidth), height(newHeight)
{
	this->timer = 0.f;	//timer set to 0
	this->endOfAnimation = false;

	//the first frame of the animation is a certain rect of the sprite sheet
	this->startRect = sf::IntRect(startFrameX * width, startFrameY * height, width, height);

	//current frame starts at the first frame
	this->currentRect = startRect;

	//the last rect is the one at endFramesX * width and height
	this->endRect = sf::IntRect(endFramesX * width, endFramesY * height, width, height);

	//set the animations sprite to the sprite sheet
	this->sprite.setTexture(spriteSheet, true);

	//set the initial frame of the sprite sheet
	this->sprite.setTextureRect(startRect);
}

//play an animation
void Animation::play(const float & deltaTime)
{
	//update the timer
	this->timer += 100.f * deltaTime;

	//reset the timer if it goes over the speed of a new frame
	if (timer >= animationTimer)
	{
		//reset the timer
		this->timer = 0.f;

		//update the animation if the current animation is not the end
		if (currentRect != endRect)
		{
			this->currentRect.left += width;
		}
		//if the animation reaches the end, set the current frame to the first frame
		else
		{
			this->currentRect.left = startRect.left;
		}

		//just for the spawn animation to get a more accurate end of animation
		if (currentRect == endRect)
		{
			this->endOfAnimation = true;
		}
		else
		{
			this->endOfAnimation = false;
		}

		//set the sprites texture to the current rect
		this->sprite.setTextureRect(currentRect);
	}
}

//reset the animation
void Animation::reset()
{
	this->timer = 0.f;
	this->currentRect = this->startRect;
}

//get if the animation is on the last frame
const bool Animation::getEndOfAnimation() const
{
	return endOfAnimation;
}