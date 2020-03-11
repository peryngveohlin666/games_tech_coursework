#ifndef __ALIENSPACESHIP_H__
#define __ALIENSPACESHIP_H__

#include "GameUtil.h"
#include "GameObject.h"
#include "Shape.h"

class AlienSpaceship : public GameObject
{
public:
	AlienSpaceship();
	AlienSpaceship(GLVector3f p, GLVector3f v, GLVector3f a, GLfloat h, GLfloat r);
	virtual ~AlienSpaceship(void);

	virtual void Update(int t);
	virtual void Render(void);

	float mThrust;
};


#endif