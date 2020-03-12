#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "GameUtil.h"

#include "GameObject.h"
#include "GameObjectType.h"
#include "IPlayerListener.h"
#include "IGameWorldListener.h"

class Player : public IGameWorldListener
{
public:
	Player() { 
		mLives = 3; 
		//to check if the shields are up
		mShield = false;
	}
	virtual ~Player() {}

	void OnWorldUpdated(GameWorld* world) {}

	void OnObjectAdded(GameWorld* world, shared_ptr<GameObject> object) {}

	void OnObjectRemoved(GameWorld* world, shared_ptr<GameObject> object)
	{
		//checks if the collision is with a spaceship
		if (object->GetType() == GameObjectType("Spaceship")) {
			//checks if the shields are not up
			if (!mShield) {
				//if shields are not up you lose a life and die
				mLives -= 1;
				FirePlayerKilled();
			}
			if (mShield) {
				//if shields are up you just live we fire the player killed still but we have tweaked it so that the character does not die
				mShield = false;
				FirePlayerKilled();
			}
		}
	}

	void AddListener(shared_ptr<IPlayerListener> listener)
	{
		mListeners.push_back(listener);
	}

	void FirePlayerKilled()
	{
		// Send message to all listeners
		for (PlayerListenerList::iterator lit = mListeners.begin();
			lit != mListeners.end(); ++lit) {
			(*lit)->OnPlayerKilled(mLives);
		}
	}

	void OneUp() {
		mLives++;
	}

	int mLives;
	//to determine if the shields are up for the player class
	bool mShield;

private:

	typedef std::list< shared_ptr<IPlayerListener> > PlayerListenerList;

	PlayerListenerList mListeners;
};

#endif
