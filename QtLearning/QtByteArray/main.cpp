#include <iostream>

#include "QtByteArray.h"

int main()
{
	testFromRawData();
	std::cout << "------------------------------------------------------------\n\n\n\n\n\n";

	findIndex();
	std::cout << "------------------------------------------------------------\n\n\n\n\n\n";


	QByteVSQString();
	std::cout << "------------------------------------------------------------\n\n\n\n\n\n";

	Base64();

	return 0;
}
