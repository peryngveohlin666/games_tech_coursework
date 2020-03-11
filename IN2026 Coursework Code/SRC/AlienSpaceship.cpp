#include "GameUtil.h"
#include "GameWorld.h"
#include "Bullet.h"
#include "AlienSpaceship.h"
#include "BoundingSphere.h"

/**  Default constructor. */
AlienSpaceship::AlienSpaceship()
	: GameObject("AlienSpaceship"), mThrust(0)
{
}

/** Construct an alien spaceship with given position, velocity, acceleration, angle, and rotation. */
AlienSpaceship::AlienSpaceship(GLVector3f p, GLVector3f v, GLVector3f a, GLfloat h, GLfloat r)
	: GameObject("Spaceship", p, v, a, h, r), mThrust(0)
{
}

/** Destructor. */
Spaceship::~AlienSpaceship(void)
{
}

// PUBLIC INSTANCE METHODS ////////////////////////////////////////////////////

/** Update this alien spaceship. */
void AlienSpaceship::Update(int t)
{
	// Call parent update function
	GameObject::Update(t);
}