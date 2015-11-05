#include "Engine.h"
#include <iostream>
#include <unistd.h>
using namespace std;




Engine::Engine():
    player_control(&renderer),
    physics(&renderer),
    renderer(1024, 768)
{
    usleep(5000000);
}

