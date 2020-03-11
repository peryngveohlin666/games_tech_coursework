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
public:
	AlienSpaceship(void) : GameObject("AlienSpaceship") {
		mAngle = 180 + rand() % 10;
		mRotation = 0;
		mPosition.x = rand() / 2;
		mPosition.y = rand() / 2;
		mPosition.z = 0.0;
		mVelocity.x = 10.0 * cos(DEG2RAD*mAngle);
		mVelocity.y = 10.0 * sin(DEG2RAD*mAngle);
		mVelocity.z = 0.0;
	}
	~AlienSpaceship(void) {

	}

	bool CollisionTest(shared_ptr<GameObject> o)
	{
		if (o->GetType() != GameObjectType("Bullet")) return false;
		if (mBoundingShape.get() == NULL) return false;
		if (o->GetBoundingShape().get() == NULL) return false;
		return mBoundingShape->CollisionTest(o->GetBoundingShape());
	}

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
		shared_ptr<GameObject> bullet
		(new Bullet(bullet_position, bullet_velocity, mAcceleration, 0, 0, 2000));
		bullet->SetBoundingShape(make_shared<BoundingSphere>(bullet->GetThisPtr(), 2.0f));
		bullet->SetShape(mBulletShape);
		// Add the new bullet to the game world
		mWorld->AddObject(bullet);

	}

	void Update(int t)
	{
		// Call parent update function
		GameObject::Update(t);
	}

	void SetBulletShape(shared_ptr<Shape> bullet_shape) { mBulletShape = bullet_shape; }

	shared_ptr<Shape> mBulletShape;
};