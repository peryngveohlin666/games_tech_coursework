#include <stdlib.h>
#include "GameUtil.h"
#include "Asteroid.h"
#include "BoundingShape.h"
#include "AnimationManager.h"
#include "Animation.h"
#include "BoundingSphere.h"

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
	mCount--;
	mWorld->FlagForRemoval(GetThisPtr());
	CreateAsteroid(mScale, mShape);
	CreateAsteroid(mScale, mShape);
	mScale /= 2;
	mShape /= 2;
}

void Asteroid::CreateAsteroid(float scale, float shape) {
	Animation *anim_ptr = AnimationManager::GetInstance().GetAnimationByName("asteroid1");
	shared_ptr<Sprite> asteroid_sprite
		= make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
	asteroid_sprite->SetLoopAnimation(true);
	shared_ptr<Asteroid> asteroid = make_shared<Asteroid>();
	asteroid->SetBoundingShape(make_shared<BoundingSphere>(asteroid->GetThisPtr(), shape));
	asteroid->SetSprite(asteroid_sprite);
	asteroid->SetScale(scale);
	asteroid->mScale = scale/2;
	asteroid->mShape = shape/2;
	mWorld->AddObject(asteroid);
}
