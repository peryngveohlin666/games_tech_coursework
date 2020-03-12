#include "Asteroid.h"
#include "Asteroids.h"
#include "Animation.h"
#include "AnimationManager.h"
#include "GameUtil.h"
#include "GameWindow.h"
#include "GameWorld.h"
#include "GameDisplay.h"
#include "Spaceship.h"
#include "BoundingShape.h"
#include "BoundingSphere.h"
#include "GUILabel.h"
#include "AlienSpaceship.h"
#include "Explosion.h"

// PUBLIC INSTANCE CONSTRUCTORS ///////////////////////////////////////////////

/** Constructor. Takes arguments from command line, just in case. */
Asteroids::Asteroids(int argc, char *argv[])
	: GameSession(argc, argv)
{
	mLevel = 0;
	mAsteroidCount = 7;
}

/** Destructor. */
Asteroids::~Asteroids(void)
{
}

// PUBLIC INSTANCE METHODS ////////////////////////////////////////////////////

/** Start an asteroids game. */
void Asteroids::Start()
{
	// Create a shared pointer for the Asteroids game object - DO NOT REMOVE
	shared_ptr<Asteroids> thisPtr = shared_ptr<Asteroids>(this);

	// Add this class as a listener of the game world
	mGameWorld->AddListener(thisPtr.get());

	// Add this as a listener to the world and the keyboard
	mGameWindow->AddKeyboardListener(thisPtr);

	// Add a score keeper to the game world
	mGameWorld->AddListener(&mScoreKeeper);

	// Add this class as a listener of the score keeper
	mScoreKeeper.AddListener(thisPtr);

	// Create an ambient light to show sprite textures
	GLfloat ambient_light[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat diffuse_light[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
	glEnable(GL_LIGHT0);

	//creating animations or getting sprites and creating objects from them to be used
	Animation *explosion_anim = AnimationManager::GetInstance().CreateAnimationFromFile("explosion", 64, 1024, 64, 64, "explosion_fs.png");
	Animation *asteroid1_anim = AnimationManager::GetInstance().CreateAnimationFromFile("asteroid1", 128, 8192, 128, 128, "asteroid1_fs.png");
	Animation *spaceship_anim = AnimationManager::GetInstance().CreateAnimationFromFile("spaceship", 128, 128, 128, 128, "spaceship_fs.png");
	Animation *powerup_anim = AnimationManager::GetInstance().CreateAnimationFromFile("powerup", 256, 256, 256, 256, "heart.png");
	Animation *powerup2_anim = AnimationManager::GetInstance().CreateAnimationFromFile("powerup2", 256, 256, 256, 256, "shield.png");
	Animation *alien_spaceship_anim = AnimationManager::GetInstance().CreateAnimationFromFile("alienspaceship", 128, 8192, 128, 128, "enemy_fs.png");

	// Create a spaceship and add it to the world
	mGameWorld->AddObject(CreateSpaceship());
	// Create some asteroids and add them to the world
	CreateAsteroids(1);

	//creates an alien and intiializes it to the member
	mAlien = CreateAlien();
	//starts the recursive shooting timer but with the value 1000 to give a startup time (also the bullets sometimes don't render right away and you get shot by invisible beams) basically starts making our alien shoot!
	SetTimer(1000, SHOOT);

	//Create the GUI
	CreateGUI();

	// Add a player (watcher) to the game world
	mGameWorld->AddListener(&mPlayer);

	// Add this class as a listener of the player
	mPlayer.AddListener(thisPtr);

	// Start the game
	GameSession::Start();
}

/** Stop the current game. */
void Asteroids::Stop()
{
	// Stop the game
	GameSession::Stop();
}

// PUBLIC INSTANCE METHODS IMPLEMENTING IKeyboardListener /////////////////////

void Asteroids::OnKeyPressed(uchar key, int x, int y)
{
	switch (key)
	{
	case ' ':
		mSpaceship->Shoot();
		break;
	default:
		break;
	}
}

void Asteroids::OnKeyReleased(uchar key, int x, int y) {}

void Asteroids::OnSpecialKeyPressed(int key, int x, int y)
{
	switch (key)
	{
	// If up arrow key is pressed start applying forward thrust
	case GLUT_KEY_UP: mSpaceship->Thrust(10); break;
	// If left arrow key is pressed start rotating anti-clockwise
	case GLUT_KEY_LEFT: mSpaceship->Rotate(90); break;
	// If right arrow key is pressed start rotating clockwise
	case GLUT_KEY_RIGHT: mSpaceship->Rotate(-90); break;
	// Default case - do nothing
	default: break;
	}
}

void Asteroids::OnSpecialKeyReleased(int key, int x, int y)
{
	switch (key)
	{
	// If up arrow key is released stop applying forward thrust
	case GLUT_KEY_UP: mSpaceship->Thrust(0); break;
	// If left arrow key is released stop rotating
	case GLUT_KEY_LEFT: mSpaceship->Rotate(0); break;
	// If right arrow key is released stop rotating
	case GLUT_KEY_RIGHT: mSpaceship->Rotate(0); break;
	// Default case - do nothing
	default: break;
	} 
}


// PUBLIC INSTANCE METHODS IMPLEMENTING IGameWorldListener ////////////////////

void Asteroids::OnObjectRemoved(GameWorld* world, shared_ptr<GameObject> object)
{
	if (object->GetType() == GameObjectType("Asteroid"))
	{
		shared_ptr<GameObject> explosion = CreateExplosion();
		explosion->SetPosition(object->GetPosition());
		explosion->SetRotation(object->GetRotation());
		mGameWorld->AddObject(explosion);
		mAsteroidCount--;
		if (!mSpaceship->mShield) {
			mLivesLeft = mPlayer.mLives;
			// Format the lives left message using an string-based stream
			std::ostringstream msg_stream;
			msg_stream << "Lives: " << mLivesLeft;
			// Get the lives left message as a string
			std::string lives_msg = msg_stream.str();
			mLivesLabel->SetText(lives_msg);
		}
		//if no asteroids left starts the next level
		if (mAsteroidCount <= 0) 
		{ 
			SetTimer(500, START_NEXT_LEVEL); 
		}
	}
	if (object->GetType() == GameObjectType("PowerUp")) {

		//whenever the object is a health power up
		if (object->GetScale() == 0.025f) {

			//give the player another live
				mPlayer.OneUp();

				//set the shields for false (I think the other way it was too easy and i would need both a health text and a shields one which takes too much screen space)
				mSpaceship->mShield = false;
				mPlayer.mShield = false;

				// Format the lives left message using an string-based stream
				std::ostringstream msg_stream;
				msg_stream << "Lives: " << mPlayer.mLives;
				// Get the lives left message as a string
				std::string lives_msg = msg_stream.str();
				mLivesLabel->SetText(lives_msg);
		}
		//whenever the object is a shield power up
		if (object->GetScale() == 0.030f) {
			mPlayer.mShield = true;
			mSpaceship->mShield = true;
			// Format the lives left message using an string-based stream (turns it into Shields ON as I think another text would take too much screen space)
			std::ostringstream msg_stream;
			msg_stream << "Shields: ON";
			// Get the lives left message as a string
			std::string lives_msg = msg_stream.str();
			mLivesLabel->SetText(lives_msg);
		}
	}
	//creates an explosion when the alien spaceship gets destroyed
	if (object->GetType() == GameObjectType("AlienSpaceship")) {
		shared_ptr<GameObject> explosion = CreateExplosion();
		explosion->SetPosition(object->GetPosition());
		explosion->SetRotation(object->GetRotation());
		mGameWorld->AddObject(explosion);
	}
	
}

// PUBLIC INSTANCE METHODS IMPLEMENTING ITimerListener ////////////////////////

void Asteroids::OnTimer(int value)
{
	if (value == CREATE_NEW_PLAYER)
	{
		mSpaceship->Reset();
		mGameWorld->AddObject(mSpaceship);
	}

	if (value == START_NEXT_LEVEL)
	{
	mLevel++;
	// made the game a bit easier since the splitting asteroids make stuff really hard
	int num_asteroids = 2*mLevel;
	CreateAsteroids(num_asteroids);
	//readds the alien to the world on the next level
	mGameWorld->AddObject(mAlien);
	}


	if (value == SHOW_GAME_OVER)
	{
		mGameOverLabel->SetVisible(true);
	}

	//shoots and recursively sets a timer to shoot again for the alien
	if  (value == SHOOT)
	{
		mAlien->Shoot(mSpaceship);
		SetTimer(1000, SHOOT);
	}

}

// PROTECTED INSTANCE METHODS /////////////////////////////////////////////////
shared_ptr<GameObject> Asteroids::CreateSpaceship()
{
	{
		// Create a raw pointer to a spaceship that can be converted to
		// shared_ptrs of different types because GameWorld implements IRefCount
		mSpaceship = make_shared<Spaceship>();
		mSpaceship->SetBoundingShape(make_shared<BoundingSphere>(mSpaceship->GetThisPtr(), 4.0f));
		shared_ptr<Shape> bullet_shape = make_shared<Shape>("bullet.shape");
		mSpaceship->SetBulletShape(bullet_shape);
		Animation *anim_ptr = AnimationManager::GetInstance().GetAnimationByName("spaceship");
		shared_ptr<Sprite> spaceship_sprite =
			make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
		mSpaceship->SetSprite(spaceship_sprite);
		mSpaceship->SetScale(0.1f);
		// Reset spaceship back to centre of the world
		mSpaceship->Reset();
		// Return the spaceship so it can be added to the world
		return mSpaceship;
	}
}

void Asteroids::CreateAsteroids(const uint num_asteroids)
{
	//since each asteroid now splits into 7 asteroids in total we have to multiply this number by seven to check the world correctly for the next level and we have to kill a spaceship
	mAsteroidCount = num_asteroids * 7;
	for (uint i = 0; i < num_asteroids; i++)
	{
		Animation *anim_ptr = AnimationManager::GetInstance().GetAnimationByName("asteroid1");
		shared_ptr<Sprite> asteroid_sprite
			= make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
		asteroid_sprite->SetLoopAnimation(true);
		shared_ptr<GameObject> asteroid = make_shared<Asteroid>();
		asteroid->SetBoundingShape(make_shared<BoundingSphere>(asteroid->GetThisPtr(), 10.0f));
		asteroid->SetSprite(asteroid_sprite);
		asteroid->SetScale(0.2f);
		mGameWorld->AddObject(asteroid);
	}
}

/*
A function to create an alien spaceship object
*/
shared_ptr<AlienSpaceship> Asteroids::CreateAlien() {
	//initializes the sprite and the animation
	Animation *anim_ptr = AnimationManager::GetInstance().GetAnimationByName("alienspaceship");
	shared_ptr<Sprite> alien_spaceship_sprite
		= make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
	alien_spaceship_sprite->SetLoopAnimation(true);
	//creates a smart pointer that is pointing to a new instance of the class AlienSpaceShip
	shared_ptr<AlienSpaceship> alien_spaceship = make_shared<AlienSpaceship>();
	//sets the hitbox
	alien_spaceship->SetBoundingShape(make_shared<BoundingSphere>(alien_spaceship->GetThisPtr(), 4.0f));
	//sets the sprite for the bullets
	shared_ptr<Shape> bullet_shape = make_shared<Shape>("bullet.shape");
	//sets the shape for the bulet
	alien_spaceship->SetBulletShape(bullet_shape);
	//sets the sprite
	alien_spaceship->SetSprite(alien_spaceship_sprite);
	alien_spaceship->SetScale(0.1f);
	//ads the spaceship to the world
	mGameWorld->AddObject(alien_spaceship);
	//returns a pointer to it
	return alien_spaceship;
}

void Asteroids::CreateGUI()
{
	// Add a (transparent) border around the edge of the game display
	mGameDisplay->GetContainer()->SetBorder(GLVector2i(10, 10));
	// Create a new GUILabel and wrap it up in a shared_ptr
	mScoreLabel = make_shared<GUILabel>("Score: 0");
	// Set the vertical alignment of the label to GUI_VALIGN_TOP
	mScoreLabel->SetVerticalAlignment(GUIComponent::GUI_VALIGN_TOP);
	// Add the GUILabel to the GUIComponent  
	shared_ptr<GUIComponent> score_component
		= static_pointer_cast<GUIComponent>(mScoreLabel);
	mGameDisplay->GetContainer()->AddComponent(score_component, GLVector2f(0.0f, 1.0f));

	// Create a new GUILabel and wrap it up in a shared_ptr
	mLivesLabel = make_shared<GUILabel>("Lives: 3");
	// Set the vertical alignment of the label to GUI_VALIGN_BOTTOM
	mLivesLabel->SetVerticalAlignment(GUIComponent::GUI_VALIGN_BOTTOM);
	// Add the GUILabel to the GUIComponent  
	shared_ptr<GUIComponent> lives_component = static_pointer_cast<GUIComponent>(mLivesLabel);
	mGameDisplay->GetContainer()->AddComponent(lives_component, GLVector2f(0.0f, 0.0f));

	// Create a new GUILabel and wrap it up in a shared_ptr
	mGameOverLabel = shared_ptr<GUILabel>(new GUILabel("GAME OVER"));
	// Set the horizontal alignment of the label to GUI_HALIGN_CENTER
	mGameOverLabel->SetHorizontalAlignment(GUIComponent::GUI_HALIGN_CENTER);
	// Set the vertical alignment of the label to GUI_VALIGN_MIDDLE
	mGameOverLabel->SetVerticalAlignment(GUIComponent::GUI_VALIGN_MIDDLE);
	// Set the visibility of the label to false (hidden)
	mGameOverLabel->SetVisible(false);
	// Add the GUILabel to the GUIContainer  
	shared_ptr<GUIComponent> game_over_component
		= static_pointer_cast<GUIComponent>(mGameOverLabel);
	mGameDisplay->GetContainer()->AddComponent(game_over_component, GLVector2f(0.5f, 0.5f));

}

void Asteroids::OnScoreChanged(int score)
{
	// Format the score message using an string-based stream
	std::ostringstream msg_stream;
	msg_stream << "Score: " << score;
	// Get the score message as a string
	std::string score_msg = msg_stream.str();
	mScoreLabel->SetText(score_msg);
}

void Asteroids::OnPlayerKilled(int lives_left)
{
	shared_ptr<GameObject> explosion = CreateExplosion();
	explosion->SetPosition(mSpaceship->GetPosition());
	explosion->SetRotation(mSpaceship->GetRotation());
	mGameWorld->AddObject(explosion);

	// Format the lives left message using an string-based stream
	std::ostringstream msg_stream;
	msg_stream << "Lives: " << lives_left;
	// Get the lives left message as a string
	std::string lives_msg = msg_stream.str();
	mLivesLabel->SetText(lives_msg);

	if (lives_left > 0) 
	{ 
		SetTimer(1000, CREATE_NEW_PLAYER); 
	}
	else
	{
		SetTimer(500, SHOW_GAME_OVER);
	}
}

shared_ptr<GameObject> Asteroids::CreateExplosion()
{
	Animation *anim_ptr = AnimationManager::GetInstance().GetAnimationByName("explosion");
	shared_ptr<Sprite> explosion_sprite =
		make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
	explosion_sprite->SetLoopAnimation(false);
	shared_ptr<GameObject> explosion = make_shared<Explosion>();
	explosion->SetSprite(explosion_sprite);
	explosion->Reset();
	return explosion;
}




