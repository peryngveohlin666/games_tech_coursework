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
	//traverses through all the objects
	for (auto object : objects) {
		//if it collides with a powerup doesn't remove the asteroid
		if (object->GetType() != GameObjectType("PowerUp"))
		{
			// removes the asteroid on collision
			mWorld->FlagForRemoval(GetThisPtr());
			// check the integer value count and if it is bigger than 0 creates 2 asteroids this makes sure that an asteroid only splits twice into smaller asteroids and stops splitting because we wouldn't want that
	if (mCount > 0) {
		mCount--;
		AsteroidDestroyed(mScale, mShape, mCount);
		AsteroidDestroyed(mScale, mShape, mCount);
		//if we are lucky we get some power ups here
		mRandom = 0 + rand() % 11;
		if (mRandom <= 2) {
			CreatePowerUp(1);
		}
		if (mRandom >= 9) {
			CreatePowerUp(2);
		}
		if (mRandom == 7) {
			CreatePowerUp(3);
		}
		//we set the values for scale and shape one third of the previous one so the asteroids will be smaller each time
		mScale /= 3;
		mShape /= 3;
	}
		}

	}
}

/** creates an asteroid gets a value scale and shape so we can decide how big our asteroid will be it also gets a value count so it can
 keep track of how many times the asteroid got destroyed and sets the asteroids position, rotation and acceleration values to the asteroid that it is derived from (aka the asteroid that got destroyed and sets them to the new one) */
void Asteroid::AsteroidDestroyed(float scale, float shape, int count) {
	//creating the sprite
	Animation *anim_ptr = AnimationManager::GetInstance().GetAnimationByName("asteroid1");
	shared_ptr<Sprite> asteroid_sprite
		= make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
	asteroid_sprite->SetLoopAnimation(true);
	shared_ptr<Asteroid> asteroid = make_shared<Asteroid>();
	//creating a hitbox
	asteroid->SetBoundingShape(make_shared<BoundingSphere>(asteroid->GetThisPtr(), shape));
	//setting the sprite
	asteroid->SetSprite(asteroid_sprite);
	asteroid->SetScale(scale);
	asteroid->mScale = scale/3;
	asteroid->mShape = shape/3;
	asteroid->mCount = count;
	//setting the position of the new asteroids to the old one and the same with rotation and acceleration, left the angle and velocity different as I think it is more fun to make them move around in a different direction they were prior to
	asteroid->SetPosition(GetThisPtr()->GetPosition());
	asteroid->SetRotation(GetThisPtr()->GetRotation());
	asteroid->SetAcceleration(GetThisPtr()->GetAcceleration());
	// adds the asteroid to the world
	mWorld->AddObject(asteroid);
}

/*a function to create power ups, it takes an integer as the parameter which determines which type of the power up it is and creates a sprite regarding the type
also sets a shape regarding the one, the functionality for the powerups are implemented in several classes so these are quite extensible and barebones where randomization can be differed and new sprites can be added

*/
void Asteroid::CreatePowerUp(int type) {
	if (type == 1) {
		//setting the sprite
		Animation *anim_ptr = AnimationManager::GetInstance().GetAnimationByName("powerup");
		shared_ptr<Sprite> powerup_sprite
			= make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
		powerup_sprite->SetLoopAnimation(true);
		shared_ptr<PowerUp> powerup = make_shared<PowerUp>();
		powerup->SetSprite(powerup_sprite);
		powerup->SetScale(0.025f);
		//setting the hitbox
		powerup->SetBoundingShape(make_shared<BoundingSphere>(powerup->GetThisPtr(), 0.1f));
		//setting the position to where the asteroid was
		powerup->SetPosition(GetThisPtr()->GetPosition());
		// adds the powerup to the world
		mWorld->AddObject(powerup);
	}
	if (type == 2) {
		//setting the sprite
		Animation *anim_ptr = AnimationManager::GetInstance().GetAnimationByName("powerup2");
		shared_ptr<Sprite> powerup_sprite
			= make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
		powerup_sprite->SetLoopAnimation(true);
		shared_ptr<PowerUp> powerup = make_shared<PowerUp>();
		powerup->SetSprite(powerup_sprite);
		powerup->SetScale(0.030f);
		//setting the hitbox
		powerup->SetBoundingShape(make_shared<BoundingSphere>(powerup->GetThisPtr(), 0.2f));
		powerup->SetPosition(GetThisPtr()->GetPosition());
		// adds the powerup to the world
		mWorld->AddObject(powerup);
	}
	if (type == 3) {
		//setting the sprite
		Animation *anim_ptr = AnimationManager::GetInstance().GetAnimationByName("powerup3");
		shared_ptr<Sprite> powerup_sprite
			= make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
		powerup_sprite->SetLoopAnimation(true);
		shared_ptr<PowerUp> powerup = make_shared<PowerUp>();
		powerup->SetSprite(powerup_sprite);
		powerup->SetScale(0.027f);
		//setting the hitbox
		powerup->SetBoundingShape(make_shared<BoundingSphere>(powerup->GetThisPtr(), 0.2f));
		powerup->SetPosition(GetThisPtr()->GetPosition());
		// adds the powerup to the world
		mWorld->AddObject(powerup);
	}
}
