#include "Collisions.h"

bool Collisions::Circle(SDL_FPoint midpointA, float radiusA, SDL_FPoint midpointB, float radiusB){
    float minimumDistance = radiusA + radiusB;
    float differenceX = midpointA.x - midpointB.x;
    float differenceY = midpointA.y - midpointB.y;
    float actualDistance = sqrtf(powf(differenceX,2)+powf(differenceY,2));
    return actualDistance < minimumDistance;
}

bool Collisions::Box(SDL_Rect boxA, SDL_Rect boxB){
    bool xCollide = true;
	bool yCollide = true;
	if (boxA.x + (boxA.w/2) < boxB.x - (boxB.w/2)) // A is left of B
        xCollide = false;
    else if (boxA.x - (boxA.w/2) > boxB.x + (boxB.w/2)) // A is right of B
        xCollide = false;
    
    if (boxA.y - (boxA.h/2) > boxB.y + (boxB.h/2)) // A is above B
        yCollide = false;
    else if (boxA.y + (boxA.h/2) < boxB.y - (boxB.h/2)) // A is below B
        yCollide = false;

	return (xCollide && yCollide);
}