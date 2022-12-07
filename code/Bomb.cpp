#include "Bomb.h"

Bomb::Bomb(float startX, float startY)
{
	m_Position.x = startX;
	m_Position.y = startY;

	m_Shape.setSize(sf::Vector2f(100, 100));
	m_Shape.setPosition(m_Position);
}

FloatRect Bomb::getPosition()
{
	return m_Shape.getGlobalBounds();
}

RectangleShape Bomb::getShape()
{
	return m_Shape;
}

void Bomb::update(Time dt)
{
	// Update the ball position variables
	m_Position.y += m_DirectionY * m_Speed * dt.asSeconds();
	m_Position.x += m_DirectionX * m_Speed * dt.asSeconds();

	// Move the ball and the bat
	m_Shape.setPosition(m_Position);
}