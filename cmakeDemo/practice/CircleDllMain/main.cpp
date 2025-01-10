#include "circle.h"

int main()
{

    auto userCircle = getClassObj();
  
    if(userCircle)
    {
        userCircle->setRadius(5.0);
        userCircle->print();
    }

    releaseClassObj(userCircle);
    return 0;
}