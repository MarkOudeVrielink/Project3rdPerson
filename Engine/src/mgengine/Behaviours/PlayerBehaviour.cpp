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
#include "glm.hpp"
#include "mgengine\Collision\CollisionFilters.h"

PlayerBehaviour::PlayerBehaviour(Mesh* pMesh,AbstractMaterial* pMaterial, float pSpeed) : AbstractActorBehaviour(), _moveSpeed(pSpeed), _mesh(pMesh), _material(pMaterial) 
{
	_spawnOffset			= glm::vec3(0, 0, -3.0f);

	_invulnerable			= false;
	_invulnerabilityTime	= 2;
	_invulnerabilityTimer	= 0;	

	_weaponTimer	= 0;
	_fireRate		= 0.3f;
	_fired			= false;

	_coolDownTime	= 3;
	_overheat		= false;
	_heat			= 0;
	_timeToOverheat	= 6;
	_coolDownRate	= 3;
	
	_charge				= 100;
	_chargeThreshold	= 50;	

	_tiltAngle		= 0.3f;

	_score			= 0;	
}

PlayerBehaviour::~PlayerBehaviour() {

}
 
void PlayerBehaviour::update(float pStep) {	
	Move();
	//_score = 1000;
	if( _score < 500)
		FireWeapon(pStep);		
	else if( _score < 1000)
		FireWeapon2(pStep);		
	else if( _score >= 1000)		
		FireWeapon3(pStep);

	
	
	if (_invulnerable)
		IsInvulnerable(pStep);	

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && _charge >= _chargeThreshold) {
		_owner->getWorld()->GetResourceManager()->PlaySound(SoundEffect::Player_Nova);
		SpawnNova();
		_charge = 0;
	}
	//score
	float delay = 1.3;
	if (ScoreClock.getElapsedTime().asSeconds() - timeSinceLastDeadEnemy.asSeconds() > delay)
	{
		_comboMultiplier = 1;
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
			_comboMultiplier = 1;
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
void PlayerBehaviour::addScore(float pScore)
{
		timeSinceLastDeadEnemy = ScoreClock.getElapsedTime();
		_comboMultiplier++;

	if (_comboMultiplier > 5)_comboMultiplier = 5;
	_score += pScore*_comboMultiplier;
	
}
int PlayerBehaviour::getMultiplier()
{
	return _comboMultiplier;
}
float PlayerBehaviour::getScore()
{
	return _score;
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
void PlayerBehaviour::FireWeapon2(float pTime)
{
	if (!_fired && !_overheat) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			SpawnBullet(1.0f, glm::vec3(0.5f,0, -0.866f),-30);
			SpawnBullet(1.0f, glm::vec3(-0.5f, 0, -0.866f),30);
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
		_weaponTimer += pTime;
	}

	if (_heat > 0) {
		_heat -= pTime * _coolDownRate;
		_heat < 0 ? 0 : _heat;
	}
	float advancedFireRate = _fireRate - .5f;
	if (_weaponTimer >= _fireRate) {
		_fired = false;
		if (_weaponTimer >= _fireRate + _coolDownTime) {
			_overheat = false;
			_heat = 0;
		}
	}
}
void PlayerBehaviour::FireWeapon3(float pTime)
{
	if (!_fired && !_overheat) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			SpawnBullet(1.0f, glm::vec3(0.5f, 0, -0.866f),-30);
			SpawnBullet(1.0f, glm::vec3(-0, 0, -1));
			SpawnBullet(1.0f, glm::vec3(-0.5f, 0, -0.866f),30);
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
		_weaponTimer += pTime;
	}

	if (_heat > 0) {
		_heat -= pTime * _coolDownRate;
		_heat < 0 ? 0 : _heat;
	}
	float advancedFireRate = _fireRate - .9f;
	if (_weaponTimer >= _fireRate) {
		_fired = false;
		if (_weaponTimer >= _fireRate + _coolDownTime) {
			_overheat = false;
			_heat = 0;
		}
	}
}
void PlayerBehaviour::Move()
{
	float moveSpeed = 0.0f;
	btVector3 force = btVector3(0, 0, 0);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		moveSpeed = -_moveSpeed;
		force += btVector3(0, 0, moveSpeed);	
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		moveSpeed = _moveSpeed;
		force += btVector3(0, 0, moveSpeed);			
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		moveSpeed = _moveSpeed;
		force += btVector3(moveSpeed, 0, 0);
		
		_owner->SetRotation(glm::vec3(0, 0, 1), -_tiltAngle);	
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		moveSpeed = -_moveSpeed;
		force += btVector3(moveSpeed, 0, 0);
		
		_owner->SetRotation(glm::vec3(0, 0, 1), _tiltAngle);		
	}	
	_ownerBody->translate(force / 25);
}

void PlayerBehaviour::IsInvulnerable(float pTime)
{	
	_invulnerabilityTimer += pTime;
	if (_invulnerabilityTimer < _invulnerabilityTime) {
		_ownerBody->setCollisionFlags(_ownerBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
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
	bullet->setActorBehaviour(new BulletBehaviour(0.8f, pBulletPower, 1));
	_owner->getWorld()->add(bullet);	
}
void PlayerBehaviour::SpawnBullet(float pBulletPower, glm::vec3 pDirection)
{
	glm::vec3 spawnPoint = _owner->getWorldPosition() + _spawnOffset;

	ObjectActor* bullet = new ObjectActor(_owner->getWorld(), "bullet", spawnPoint, new btSphereShape(0.4f), ActorType::Type_Bullet, CF::COL_PLAYERBULLET, CF::playerBulletCollidesWith, 1);
	bullet->scale(glm::vec3(10, 10, 10));
	bullet->setMesh(_owner->getWorld()->GetResourceManager()->getMesh(Meshes::Bullet));
	bullet->setMaterial(_owner->getWorld()->GetResourceManager()->getMaterial(Materials::Bullet));
	BulletBehaviour * bulletBehave = new BulletBehaviour(0.8f, pBulletPower, 1, Custom);
	bulletBehave->setBulletDirection(pDirection);
	bullet->setActorBehaviour(bulletBehave);
	_owner->getWorld()->add(bullet);
}
//Bullet rotated in y axis by PAngle
void PlayerBehaviour::SpawnBullet(float pBulletPower, glm::vec3 pDirection, float pAngle)
{
	glm::vec3 spawnPoint = _owner->getWorldPosition() + _spawnOffset;

	ObjectActor* bullet = new ObjectActor(_owner->getWorld(), "bullet", spawnPoint, new btSphereShape(0.4f), ActorType::Type_Bullet, CF::COL_PLAYERBULLET, CF::playerBulletCollidesWith, 1);
	bullet->scale(glm::vec3(10, 10, 10));
	bullet->setMesh(_owner->getWorld()->GetResourceManager()->getMesh(Meshes::Bullet));
	bullet->setMaterial(_owner->getWorld()->GetResourceManager()->getMaterial(Materials::Bullet));
	BulletBehaviour * bulletBehave = new BulletBehaviour(0.8f, pBulletPower, 1, Custom);
	bulletBehave->setBulletDirection(pDirection);
	bullet->setActorBehaviour(bulletBehave);
	bulletBehave->setBulletRotation(glm::vec3(0, 1, 0), glm::radians(pAngle));
	_owner->getWorld()->add(bullet);
}