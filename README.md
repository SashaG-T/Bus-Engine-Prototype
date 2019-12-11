Engine Prototype
---------------------

This project is test the idea of a central queue use for a communication bus between
various engine components. These components include the state stack and the entities;
infact the idea behind the project was to have a minimal starting point for any game.

Starting from a minimal project can allow for modular engine components that can be
reused in other games. Need to draw a sprite? Add the sprite component. Need a component to handled a collision? Plop a message into the queue to have the physics component to process it.

The idea of a queue also allows for acceleration with multithreading. Certain
components may not need to communicate to each other, or mutexes can be used.

We also want all the objects to be instantiable through copies and easily copied with
no dependence on other copies.

Compiling
---------

The engine uses some code taken from the Progeny Engine, the engine for the game Generic Jumper. The engine implements a small portion of the code currently. SDL2 is require to compile and run the prototype.