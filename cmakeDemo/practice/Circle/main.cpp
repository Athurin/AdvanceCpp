#include<iostream>
#include"KCircle.h"

int main()
{
	KCircle c(1.9);

	std::cout << "面积为：" << c.getArea() << std::endl;
	std::cout << "周长为：" << c.getLeng() << std::endl;

	return 0;
}