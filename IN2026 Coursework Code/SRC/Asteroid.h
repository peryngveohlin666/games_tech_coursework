#ifndef __ASTEROID_H__
#define __ASTEROID_H__

#include "GameObject.h"

class Asteroid : public GameObject
{
public:
	int mCount = 2;
	float mScale = 0.1f;
	float mShape = 5.0f;
	Asteroid(void);
	~Asteroid(void);

	bool CollisionTest(shared_ptr<GameObject> o);
	void OnCollision(const GameObjectList& objects);

	void AsteroidDestroyed(float scale, float shape, int count);
	void CreatePowerUp();
};

#endif
