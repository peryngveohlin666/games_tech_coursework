#pragma once

#include "GameObject.h"
#include "PowerUp.h"
#include <stdlib.h>
#include "GameUtil.h"
#include "BoundingShape.h"
#include "AnimationManager.h"
#include "Animation.h"
#include "BoundingSphere.h"


class PowerUp : public GameObject
{
public:
	PowerUp(void) : GameObject("PowerUp"){
		mAngle = rand() % 360;
		mRotation = 0;
		mPosition.x = rand() / 2;
		mPosition.y = rand() / 2;
		mPosition.z = 0.0;
		mVelocity.x = 0.0;
		mVelocity.y = 0.0;
		mVelocity.z = 0.0;
	}
	~PowerUp(void) {
		
	}

	bool CollisionTest(shared_ptr<GameObject> o) {
		if (GetType() == o->GetType()) return false;
		if (mBoundingShape.get() == NULL) return false;
		if (o->GetBoundingShape().get() == NULL) return false;
		return mBoundingShape->CollisionTest(o->GetBoundingShape());
	}
	void OnCollision(const GameObjectList& objects) {
		for (auto object : objects) {
			if (object->GetType() == GameObjectType("Spaceship"))
			{
				mWorld->FlagForRemoval(GetThisPtr());
			}
		}
	}
};