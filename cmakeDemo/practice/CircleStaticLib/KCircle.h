#pragma once

#ifndef KCIRCLE_H
#define KCIRCLE_H

class KCircle
{
private:
	double rad; 

public:

	KCircle(){}
	KCircle(double r);
	KCircle(const KCircle& other);

	double getRad();
	void setRad(double r);

	double getArea();
	double getLeng();

};

#endif

