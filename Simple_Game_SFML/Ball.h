#ifndef BALL_H
#define BALL_H

#include <SFML/Graphics.hpp>

class Ball : public sf::Drawable
{
public:
	Ball(float t_X, float t_Y);
	Ball() = delete;
	~Ball() = default;

	void update();

	float left();
	float right();
	float top();
	float bottom();

	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();

	sf::Vector2f getPosition();
	void setPosition(float x, float y);

	sf::CircleShape shape;
private:
	const float ballRadius{ 10.0f };
	const float ballVelocity{ 5.0f };
	sf::Vector2f velocity{ ballVelocity,ballVelocity };
	void draw(sf::RenderTarget& target, sf::RenderStates state)const override;
};

#endif // BALL_H
