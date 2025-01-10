#include"KCircle.h"
#include<iostream>

KCircle::KCircle(double r):
	rad(r)
{
	std::cout << "Build a Kcircle." << std::endl;
}

KCircle::KCircle(const KCircle& other)
{
	rad = other.rad;
	std::cout << "Build a Kcircle." << std::endl;
}

double KCircle::getRad()
{
	return rad;
}

void KCircle::setRad(double r)
{
	rad = r;
}

double KCircle::getArea()
{
	return rad * rad * (3.14) / 2.0;
}

double KCircle::getLeng()
{
	return rad * (3.14) * 2.0;
}