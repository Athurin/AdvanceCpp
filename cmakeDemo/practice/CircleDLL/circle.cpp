#include "circle.h"

static constexpr double PI = 3.14;


ExportCircle *getClassObj()
{
    return new Circle();
}

void releaseClassObj(ExportCircle *pCircle)
{
    delete pCircle;
}

double Circle::getRadius() const
{
    return m_radius;
}

void Circle::setRadius(double r)
{
    this->m_radius = r;
}

void Circle::print() const
{
    std::cout << "circle areaa: " << this->getArea() << std::endl;
    std::cout << "circle perimeter: " << this->getPerimeter() << std::endl;
}

constexpr double Circle::getArea() const
{
    return PI * m_radius * m_radius;
}

constexpr double Circle::getPerimeter() const
{
    return 2 * PI * m_radius;
}