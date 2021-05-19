#ifndef PADDLE_H
#define PADDLE_H

#include <SFML/Graphics.hpp>
using namespace std;

class Paddle : public sf::Drawable
{
public:
	Paddle(float t_X, float t_Y);
	Paddle() = delete;
	~Paddle() = default;

	void update();

	float left();
	float right();
	float top();
	float bottom();

	sf::Vector2f getPosition();
	void setPosition(float x, float y);

private:
	sf::RectangleShape paddle;
	const float paddleWidth{ 80.0f };
	const float paddleHeight{ 20.0f };
	const float paddleVelocity{ 6.0f };
	void draw(sf::RenderTarget& target, sf::RenderStates state)const override;
	sf::Vector2f velocity{ paddleVelocity,0.f };
};

#endif // PADDLE_H
