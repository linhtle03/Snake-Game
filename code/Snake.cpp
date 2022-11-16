#include "Snake.h"
Snake::Snake(float startX, float startY)
{

}

FloatRect Snake::getPosition()
{
    return m_Shape.getGlobalBounds();
}
RectangleShape Snake::getShape()
{
    return m_Shape;
}

void Snake::moveLeft()
{
    m_MovingLeft = true;
}

void Snake::moveRight()
{
    m_MovingRight = true;
}

void Snake::moveUp()
{
    m_MovingUp = true;
}

void Snake::moveDown()
{
    m_MovingDown = true;
}

void Snake::update(Time dt)
{
    
}