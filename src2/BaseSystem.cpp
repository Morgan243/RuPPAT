#include "BaseSystem.h"

#include <iostream>
using namespace std;
BaseSystem::BaseSystem(Renderer * renderer)
{
    cout<<"System created:"<<endl;
    this->renderer = renderer;
}
