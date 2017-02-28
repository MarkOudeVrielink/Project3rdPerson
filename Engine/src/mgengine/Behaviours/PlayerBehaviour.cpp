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
#include "mge\core\Mesh.hpp"

#include "mgengine\Collision\CollisionFilters.h"

PlayerBehaviour::PlayerBehaviour(Mesh* pMesh,AbstractMaterial* pMaterial, float pSpeed) : AbstractActorBehaviour(), _moveSpeed(pSpeed), _mesh(pMesh), _material(pMaterial) 
{
	_invulnerable			= false;
	_invulnerabilityTime	= 50;
	_invulnerabilityTimer	= 0;	

	_weaponTimer	= 0;
	_fireRate		= 12;
	_fired			= false;

	_coolDownTime	= 50;
	_overheat		= false;
	_heat			= 0;
	
	_charge			= 100;
	_maxCharge		= 100;
	_beginNova		= 0;
	_endNova		= 30;

	_score			= 0;
}

PlayerBehaviour::~PlayerBehaviour() {

}
 
void PlayerBehaviour::update(float pStep) {	
	Move();
	FireWeapon();	
	
	if (_invulnerable)
		IsInvulnerable();	

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && _charge >= _maxCharge) {
		SpawnNova();
		_charge = 0;
	}
}

void PlayerBehaviour::OnCollision(Actor * pOther)
{
	ActorType type = pOther->GetType();

	if(type == ActorType::Type_Enemy){		
		if (!_invulnerable) {
			//_ownerBody->translate(btVector3(0, 0, 10));

			ControlledActor* player = (ControlledActor*)_owner;
			_defaultFlags = _ownerBody->getCollisionFlags();
			_invulnerable = true; //TODO: Add some kind of visual effect.

			_owner->GetWorld()->GetResourceManager()->PlaySound(SoundEffect::Player_Hit);
			player->TakeDamage(4);
		}
	}
	else if (type ==ActorType::Type_PickUp) {
		PickUpBehaviour* pickup = (PickUpBehaviour*)pOther->getActorBehaviour();
		_charge += pickup->GetCharge();
		_score	+= pickup->GetPoints();
		std::cout << "score: " << _score << " | charge " << _charge << std::endl;

		_owner->GetWorld()->GetResourceManager()->PlaySound(SoundEffect::Drop_Pick);
		_ownerBody->setLinearVelocity(btVector3(0, 0, 0));
	}
}

/*works in a crummy way... but it serves it's purpose.*/
void PlayerBehaviour::SpawnNova()
{	
	/*glm::vec3 spawnPoint = glm::vec3(0, 0, 20);

	ObjectActor* nova = new ObjectActor(_owner->GetWorld(), "Nova", spawnPoint, new btBoxShape(btVector3(75,1,1)), ActorType::Type_Nova, CF::COL_PLAYERNOVA, CF::playerNovaCollidesWith, 1);
	nova->setActorBehaviour(new BulletBehaviour(5.5f, 10, 10));
	nova->setMesh(_mesh);
	nova->setMaterial(_material);
	_owner->getParent()->add(nova);

	std::cout << "Nova" << std::endl;*/
}

void PlayerBehaviour::FireWeapon()
{
	if (!_fired && !_overheat) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			SpawnBullet(1.0f);
			std::cout << _heat << std::endl;
			if (_heat > 100) {
				_overheat = true;
			}
		}
	}
	else {
		_weaponTimer++;
		if (_heat >= 1)
			_heat -= 0.5;
	}

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
		
		_owner->SetRotation(glm::vec3(0, 0, 1), -0.05f);	
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		moveSpeed = -_moveSpeed;
		force += btVector3(moveSpeed, 0, 0);
		
		_owner->SetRotation(glm::vec3(0, 0, 1), 0.05f);		
	}	
	_ownerBody->translate(force / 25);
}

void PlayerBehaviour::IsInvulnerable()
{
	_invulnerabilityTimer++;
	if (_invulnerabilityTimer < _invulnerabilityTime) {
		_ownerBody->setCollisionFlags(_ownerBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
	} else {
		_ownerBody->setCollisionFlags(_defaultFlags);		
		_invulnerable = false;
		_invulnerabilityTimer = 0;
	}
}

void PlayerBehaviour::SpawnBullet(float pBulletPower)
{
	glm::vec3 spawnPoint = _owner->getWorldPosition() + glm::vec3(0, 0, -3.0f);

	ObjectActor* bullet = new ObjectActor(_owner->GetWorld(), "bullet", spawnPoint, new btSphereShape(0.4f), ActorType::Type_Bullet, CF::COL_PLAYERBULLET, CF::playerBulletCollidesWith, 1);
	bullet->scale(glm::vec3(0.5f, 0.5f, 0.5f));
	bullet->setMesh(_mesh);
	bullet->setMaterial(_material);
	bullet->setActorBehaviour(new BulletBehaviour(0.6f, pBulletPower));
	_owner->GetWorld()->add(bullet);

	_weaponTimer = 0;
	_fired = true;
	_heat += 20;

	_owner->GetWorld()->GetResourceManager()->PlaySound(SoundEffect::Player_Shoot, 20.0f);
}

