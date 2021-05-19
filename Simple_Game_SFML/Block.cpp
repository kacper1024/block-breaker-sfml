#include "Block.h"

Block::Block(float t_X, float t_Y, float t_Width, float t_Height)
{
	block.setPosition(t_X, t_Y);
	block.setSize({ t_Width,t_Height });
	block.setFillColor(sf::Color::Green);
	block.setOrigin(t_Width / 2.f, t_Height / 2.f);
}

void Block::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(this->block, states);
}

float Block::left()
{
	return this->block.getPosition().x - block.getSize().x / 2.f;
}

float Block::right()
{
	return this->block.getPosition().x + block.getSize().x / 2.f;
}

float Block::top()
{
	return this->block.getPosition().y - block.getSize().y / 2.f;
}

float Block::bottom()
{
	return this->block.getPosition().y + block.getSize().y / 2.f;
}

sf::Vector2f Block::getPosition()
{
	return block.getPosition();
}

bool Block::isDestroyed()
{
	return this->destroyed;
}
void Block::destroy()
{
	this->destroyed = true;
}
sf::Vector2f Block::getSize()
{
	return block.getSize();
}

void Block::update()
{

}
