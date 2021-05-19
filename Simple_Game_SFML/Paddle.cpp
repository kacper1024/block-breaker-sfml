#include "Paddle.h"

Paddle::Paddle(float t_X, float t_Y)
{
	paddle.setPosition(t_X, t_Y);
	paddle.setSize({ this->paddleWidth,this->paddleHeight });
	paddle.setFillColor(sf::Color::Blue);
	paddle.setOrigin(paddleWidth / 2.f, paddleHeight / 2.f);
}

void Paddle::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
	target.draw(this->paddle, state);
}

void Paddle::update()
{
	this->paddle.move(this->velocity);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && this->left() > 0)
		velocity.x = -paddleVelocity;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && this->right() < 800)
		velocity.x = paddleVelocity;
	else
		velocity.x = 0;

}

float Paddle::left()
{
	return this->paddle.getPosition().x - paddle.getSize().x / 2.f;
}

float Paddle::right()
{
	return this->paddle.getPosition().x + paddle.getSize().x / 2.f;
}

float Paddle::top()
{
	return this->paddle.getPosition().y - paddle.getSize().y / 2.f;
}

float Paddle::bottom()
{
	return this->paddle.getPosition().y + paddle.getSize().y / 2.f;
}

sf::Vector2f Paddle::getPosition()
{
	return paddle.getPosition();
}

void Paddle::setPosition(float x, float y)
{
	paddle.setPosition(x, y);
}
