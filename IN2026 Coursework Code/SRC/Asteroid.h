#ifndef __ASTEROID_H__
#define __ASTEROID_H__

#include "GameObject.h"

class Asteroid : public GameObject
{
public:
	//to keep count of previous values for split asteroids
	int mCount = 2;
	//a member random value because more computational efficiency can be achieved using tricks even though we will use a very little more memory
	int mRandom = 0;
	//to keep count of previous values for split asteroids
	float mScale = 0.1f;
	//to keep count of previous values for split asteroids
	float mShape = 5.0f;


	Asteroid(void);
	~Asteroid(void);

	bool CollisionTest(shared_ptr<GameObject> o);
	void OnCollision(const GameObjectList& objects);

	void AsteroidDestroyed(float scale, float shape, int count);
	void CreatePowerUp(int type);
};

#endif
