#include "particleEmitter.h"

int main()
{
    srand(static_cast<unsigned>(time(0)));
    ParticleEmitter game;
    game.Run();
    return 0;
}