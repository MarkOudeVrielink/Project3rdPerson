 #include "PlayerBehaviour.h"
#include "mge\core\World.hpp"
#include "AbstractActorBehaviour.h"
#include "mgengine\Behaviours\BulletBehaviour.h"
#include "mgengine\Behaviours\PickUpBehaviour.h"

#include "mgengine\Core\Actor.h"
#include "mgengine\Core\ObjectActor.h"
#include "mgengine\Core\ControlledActor.h"
#include <SFML\Window\Keyboard.hpp>

#include "mge\materials\ColorMaterial.hpp"
#include "mgengine\Materials\PlayerMaterial.h"
#include "mge\core\Mesh.hpp"

#include "mgengine\Collision\CollisionFilters.h"


#include "mge\config.hpp"



PlayerBehaviour::PlayerBehaviour(float pSpeed) : AbstractActorBehaviour(), _maxSpeed(pSpeed)
{
	_spawnOffset			= glm::vec3(0, 0, -3.0f);
	_force					= glm::vec3(0,0,0);
	_dashPower				= 2.0f;
	_doubleTapTime			= 0.75f;
	_horizontalInput		= 0.0f;
	_verticalInput			= 0.0f;
	_acceleration			= 0.06f;
	_decceleration			= 0.03f;

	_tapCount				= 0;
	_leftPressed			= false;
	_rightPressed			= false;

	_invulnerable			= false;
	_invulnerabilityTime	= 2;
	_invulnerabilityTimer	= 0;	

	_weaponTimer			= 0;
	_fireRate				= 0.3f;
	_fired					= false;

	_coolDownTime			= 3;
	_overheat				= false;
	_heat					= 0;
	_timeToOverheat			= 6;
	_coolDownRate			= 3;
	
	_charge					= 100;
	_chargeThreshold		= 50;	

	_tiltAngle				= 0.3f;

	_score					= 0;
		
}

PlayerBehaviour::~PlayerBehaviour() {	
}
 
void PlayerBehaviour::update(float pStep) {	
	Move(pStep);
	FireWeapon(pStep);		
	
	if (_invulnerable)
		IsInvulnerable(pStep);	

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && _charge >= _chargeThreshold) {
		_owner->getWorld()->GetResourceManager()->PlaySound(SoundEffect::Player_Nova);
		SpawnNova();
		_charge = 0;
	}
}

void PlayerBehaviour::OnCollision(Actor * pOther)
{
	ActorType type = pOther->getType();

	if(type == ActorType::Type_Enemy){		
		if (!_invulnerable) {
			ControlledActor* player = (ControlledActor*)_owner;
			_defaultFlags = _ownerBody->getCollisionFlags();
			_invulnerable = true;

			_owner->getWorld()->GetResourceManager()->PlaySound(SoundEffect::Player_Hit);
			
			player->TakeDamage(1);			
			if (player->GetHealth() <= 0) {
				_owner->Destroy();
			}
		}
	}
	else if (type ==ActorType::Type_PickUp) {
		PickUpBehaviour* pickup = (PickUpBehaviour*)pOther->getActorBehaviour();
		_charge += pickup->GetCharge();
		_score	+= pickup->GetPoints();
		std::cout << "score: " << _score << " | charge " << _charge << std::endl;

		if (_charge >= _chargeThreshold)
			_playerMaterial->setCharged(true);

		_owner->getWorld()->GetResourceManager()->PlaySound(SoundEffect::Drop_Pick);
		_ownerBody->setLinearVelocity(btVector3(0, 0, 0));
	}
}

void PlayerBehaviour::setup()
{
	_playerMaterial = (PlayerMaterial*)_owner->getWorld()->GetResourceManager()->getMaterial(Materials::Player);
		
	//TODO: remove
	if (_charge >= _chargeThreshold)
		_playerMaterial->setCharged(true);
}

void PlayerBehaviour::SpawnNova()
{	
	glm::vec3 spawnPoint = glm::vec3(0, 0, 0);
	
	ObjectActor* nova = new ObjectActor(_owner->getWorld(), "Nova", spawnPoint, new btBoxShape(btVector3(50,1,80)), ActorType::Type_Nova, CF::COL_PLAYERNOVA, CF::playerNovaCollidesWith, 1);
	nova->setActorBehaviour(new BulletBehaviour(0, 10, 0.5f));
	//nova->setMesh(_mesh);//TODO: remove before release, or alternetivly add sort of explosion thingy.
	//nova->setMaterial(_material);
	_owner->getWorld()->add(nova);	

	_playerMaterial->setCharged(false);
}

void PlayerBehaviour::FireWeapon(float pTime)
{		
	if (!_fired && !_overheat) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			SpawnBullet(1.0f);

			_weaponTimer = 0;
			_fired = true;
			_heat += pTime * 85;//TODO: change value to something sensible.

			_owner->getWorld()->GetResourceManager()->PlaySound(SoundEffect::Player_Shoot, 30.0f);
			if (_heat >= _timeToOverheat) {
				_overheat = true;
				_owner->getWorld()->GetResourceManager()->PlaySound(SoundEffect::Player_Overheat, 40.0f);
			}
		}
	}
	else {
		_weaponTimer +=pTime;
	}

	if (_heat > 0){
		_heat -= pTime * _coolDownRate;
		_heat < 0 ? 0 : _heat;
	}

	if (_weaponTimer >= _fireRate) {
		_fired = false;
		if (_weaponTimer >= _fireRate + _coolDownTime) {
			_overheat = false;
			_heat = 0;
		}
	}
}

#pragma region Movement

void PlayerBehaviour::Move(float pDeltaTime)
{
	bool horizontalPressed	= false;
	bool verticalPressed	= false;
	
	_checkInput(horizontalPressed, verticalPressed);
	_checkDoubleTap();
	_deccelerate(horizontalPressed, verticalPressed);
	
	_force = glm::vec3(_horizontalInput, 0, _verticalInput);//TODO: why does it not normalize?
	_force *= _maxSpeed;
	
	_ownerBody->translate(btVector3(_force.x, _force.y, _force.z));
}

void PlayerBehaviour::_checkInput(bool& h, bool& v)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		_verticalInput <= -1 ? -1 : _verticalInput -= _acceleration;

		_owner->SetRotation(glm::vec3(1, 0, 0), -_tiltAngle);
		v = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		_verticalInput >= 1 ? 1 : _verticalInput += _acceleration;

		_owner->SetRotation(glm::vec3(1, 0, 0), _tiltAngle);
		v = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		_horizontalInput >= 1 ? 1 : _horizontalInput += _acceleration;

		_owner->SetRotation(glm::vec3(0, 0, 1), -_tiltAngle);
		_rightPressed = true;
		h = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		_horizontalInput <= -1 ? -1 : _horizontalInput -= _acceleration;

		_owner->SetRotation(glm::vec3(0, 0, 1), _tiltAngle);
		_leftPressed = true;
		h = true;
	}
}

void PlayerBehaviour::_deccelerate(bool h, bool v)
{
	/*If either the horizontal or vertical input is not being pressed we start deccelerating.*/
	if (!h) {
		_horizontalInput > 0 ? _horizontalInput -= _decceleration : 0;
		_horizontalInput < 0 ? _horizontalInput += _decceleration : 0;
	}
	if (!v) {
		_verticalInput > 0 ? _verticalInput -= _decceleration : 0;
		_verticalInput < 0 ? _verticalInput += _decceleration : 0;
	}
}

void PlayerBehaviour::_checkDoubleTap()
{
	int tapDirection = 0;

	/*Check for key releases.*/
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && _rightPressed) {
		_rightPressed = false;
		tapDirection = 1;

		_tapClock = 0;
		_tapCount += 1;
	}

	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && _leftPressed) {
		_leftPressed = false;
		tapDirection = -1;

		_tapClock = 0;
		_tapCount += 1;
	}

	/*Start a counter at one tap.*/
	if (_tapCount >= 1) {
		_tapClock += 0.05f;
		
		/*Check if we tapped a second time within the timeframe.*/
		if (_tapClock <= _doubleTapTime && _tapCount >= 2) {
			_tapClock = 0;
			_tapCount = 0;
			
			/*Check tap direction and set movement according to it.*/
			if (tapDirection == 1) {
				_horizontalInput = _dashPower;
			}
			else if (tapDirection == -1) {
				_horizontalInput = -_dashPower;
			}
			//TODO: barrel roll
		}

		if (_tapClock > _doubleTapTime) {
			_tapClock = 0;
			_tapCount = 0;
		}
	}	
}

#pragma endregion

void PlayerBehaviour::IsInvulnerable(float pTime)
{	
	_invulnerabilityTimer += pTime;
	if (_invulnerabilityTimer < _invulnerabilityTime) {
		_ownerBody->setCollisionFlags(_ownerBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);//TODO:: change
		_playerMaterial->setInvulnerable(true);
	} else {
		_ownerBody->setCollisionFlags(_defaultFlags);		
		_playerMaterial->setInvulnerable(false);
		
		_invulnerable = false;
		_invulnerabilityTimer = 0;
	}
}

void PlayerBehaviour::SpawnBullet(float pBulletPower)
{
	glm::vec3 spawnPoint = _owner->getWorldPosition() + _spawnOffset;

	ObjectActor* bullet = new ObjectActor(_owner->getWorld(), "bullet", spawnPoint, new btSphereShape(0.4f), ActorType::Type_Bullet, CF::COL_PLAYERBULLET, CF::playerBulletCollidesWith, 1);
	bullet->scale(glm::vec3(10, 10, 10));
	bullet->setMesh(_owner->getWorld()->GetResourceManager()->getMesh(Meshes::Bullet));
	bullet->setMaterial(_owner->getWorld()->GetResourceManager()->getMaterial(Materials::Bullet));
	bullet->setActorBehaviour(new BulletBehaviour(1.4f, pBulletPower, 1));
	_owner->getWorld()->add(bullet);	
}

