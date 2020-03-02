#include <stdlib.h>
#include "GameUtil.h"
#include "Asteroid.h"
#include "BoundingShape.h"
#include "AnimationManager.h"
#include "Animation.h"
#include "BoundingSphere.h"
#include "PowerUp.h"


Asteroid::Asteroid(void) : GameObject("Asteroid")
{
	mAngle = rand() % 360;
	mRotation = 0; // rand() % 90;
	mPosition.x = rand() / 2;
	mPosition.y = rand() / 2;
	mPosition.z = 0.0;
	mVelocity.x = 10.0 * cos(DEG2RAD*mAngle);
	mVelocity.y = 10.0 * sin(DEG2RAD*mAngle);
	mVelocity.z = 0.0;
}



Asteroid::~Asteroid(void)
{
}

bool Asteroid::CollisionTest(shared_ptr<GameObject> o)
{
	if (GetType() == o->GetType()) return false;
	if (mBoundingShape.get() == NULL) return false;
	if (o->GetBoundingShape().get() == NULL) return false;
	return mBoundingShape->CollisionTest(o->GetBoundingShape());
}

void Asteroid::OnCollision(const GameObjectList& objects)
{
	for (auto object : objects) {
		if (object->GetType() != GameObjectType("PowerUp"))
		{
			// removes the asteroid on collision
			mWorld->FlagForRemoval(GetThisPtr());
			// check the integer value count and if it is bigger than 0 creates 2 asteroids
	if (mCount > 0) {
		mCount--;
		AsteroidDestroyed(mScale, mShape, mCount);
		AsteroidDestroyed(mScale, mShape, mCount);
		CreatePowerUp();
		mScale /= 3;
		mShape /= 3;
	}
		}

	}
}

// creates an asteroid gets a value scale and shape so we can decide how big our asteroid will be it also gets a value count so it can
// keep track of how many times the asteroid got destroyed
// sets the asteroids position, rotation and acceleration values to the asteroid that it is derived from (aka the asteroid that got destroyed and sets them to the new one)
void Asteroid::AsteroidDestroyed(float scale, float shape, int count) {
	Animation *anim_ptr = AnimationManager::GetInstance().GetAnimationByName("asteroid1");
	shared_ptr<Sprite> asteroid_sprite
		= make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
	asteroid_sprite->SetLoopAnimation(true);
	shared_ptr<Asteroid> asteroid = make_shared<Asteroid>();
	asteroid->SetBoundingShape(make_shared<BoundingSphere>(asteroid->GetThisPtr(), shape));
	asteroid->SetSprite(asteroid_sprite);
	asteroid->SetScale(scale);
	asteroid->mScale = scale/3;
	asteroid->mShape = shape/3;
	asteroid->mCount = count;
	asteroid->SetPosition(GetThisPtr()->GetPosition());
	asteroid->SetRotation(GetThisPtr()->GetRotation());
	asteroid->SetAcceleration(GetThisPtr()->GetAcceleration());
	// adds the asteroid to the world
	mWorld->AddObject(asteroid);
}

void Asteroid::CreatePowerUp() {
	Animation *anim_ptr = AnimationManager::GetInstance().GetAnimationByName("powerup");
	shared_ptr<Sprite> powerup_sprite
		= make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
	powerup_sprite->SetLoopAnimation(true);
	shared_ptr<PowerUp> powerup = make_shared<PowerUp>();
	powerup->SetSprite(powerup_sprite);
	powerup->SetScale(0.025f);
	powerup->SetBoundingShape(make_shared<BoundingSphere>(powerup->GetThisPtr(), 0.1f));
	powerup->SetPosition(GetThisPtr()->GetPosition());
	// adds the pwerup to the world
	mWorld->AddObject(powerup);

}
