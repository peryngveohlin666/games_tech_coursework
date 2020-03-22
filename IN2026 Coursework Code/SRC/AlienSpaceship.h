#pragma once

#include "GameObject.h"
#include "PowerUp.h"
#include <stdlib.h>
#include "GameUtil.h"
#include "BoundingShape.h"
#include "AnimationManager.h"
#include "Animation.h"
#include "BoundingSphere.h"
#include "GameWorld.h"
#include "Bullet.h"


class AlienSpaceship : public GameObject
{

	//default constructor with default values
public:
	AlienSpaceship(void) : GameObject("AlienSpaceship") {
		//the angle is 180 + random mod 10 because i don't want it to be upside down
		mAngle = 180 + rand() % 10;
		mRotation = 0;
		mPosition.x = rand() / 2;
		mPosition.y = rand() / 2;
		mPosition.z = 0.0;
		mVelocity.x = 10.0 * cos(DEG2RAD*mAngle);
		mVelocity.y = 10.0 * sin(DEG2RAD*mAngle);
		mVelocity.z = 0.0;
	}

	//default destructor
	~AlienSpaceship(void) {

	}

	bool CollisionTest(shared_ptr<GameObject> o)
	{
		//if the object is not a bullet doesn't return collision data so the alien only gets damaged by the bullets of us
		if (o->GetType() != GameObjectType("Bullet")) return false;
		if (mBoundingShape.get() == NULL) return false;
		if (o->GetBoundingShape().get() == NULL) return false;
		return mBoundingShape->CollisionTest(o->GetBoundingShape());
	}

	//checks if the alien spaceship is shot and destroys it
	void OnCollision(const GameObjectList& objects) {
		for (auto object : objects) {
			if (object->GetType() == GameObjectType("Bullet"))
			{
				mWorld->FlagForRemoval(GetThisPtr());

			}
		}
	}

	/** Shoot a bullet. */
	void Shoot(shared_ptr<GameObject> o)
	{
		//to give a randomised feeling to the shooting I only make 8 out of 10 bullets to actually shoot
		mRandom = rand() % 11;
		if (mRandom <= 8) {
			// Check the world exists
			if (!mWorld) return;

			auto SpaceShipPos = o->GetPosition();

			GLVector3f diff = SpaceShipPos - mPosition;


			// Construct a unit length vector in the direction the spaceship is headed
			GLVector3f spaceship_shooting = diff;
			spaceship_shooting.normalize();
			// Calculate the point at the node of the spaceship from position and heading
			GLVector3f bullet_position = mPosition + (spaceship_shooting * 10);
			// Calculate how fast the bullet should travel
			float bullet_speed = 30;
			// Construct a vector for the bullet's velocity
			GLVector3f bullet_velocity = mVelocity + spaceship_shooting * bullet_speed;
			// Construct a new bullet
			// Set the angle here to a randomised value because otherwise it is impossible to play against
			shared_ptr<GameObject> bullet
			(new Bullet(bullet_position, bullet_velocity, mAcceleration, rand() % 120, 0, 2000));
			bullet->SetBoundingShape(make_shared<BoundingSphere>(bullet->GetThisPtr(), 2.0f));
			bullet->SetShape(mBulletShape);
			// Add the new bullet to the game world
			mWorld->AddObject(bullet);

			//also follows the enemy (I mean it is a part of shooting)
			// it is diff/2 so it slows down as it gets closer rather than a constant slow value which gives it a more realistic way of
			// coming its way to hunt us down
			mVelocity = diff/2;
		}

	}

	void Update(int t)
	{
		// Call parent update function
		GameObject::Update(t);

	}

	//to set a bullet shape from a .shape file as i am using the normal bullet class
	void SetBulletShape(shared_ptr<Shape> bullet_shape) { mBulletShape = bullet_shape; }

	//a smart pointer to bulletshape
	shared_ptr<Shape> mBulletShape;
	//a random integer value, just keeping this for computational efficiency on occassions
	int mRandom;
};