#ifndef ACTORMOVEMENT_H
#define ACTORMOVEMENT_H

#include "mgengine\Behaviours\AbstractActorBehaviour.h"
#include "SFML\System\Clock.hpp"
#include <glm.hpp>

class Mesh;
class AbstractMaterial;
class PlayerMaterial;

class PlayerBehaviour : public AbstractActorBehaviour {
public:
	PlayerBehaviour(Mesh* pMesh, AbstractMaterial* pMatrial, float pSpeed = 4.0f);
	virtual ~PlayerBehaviour();	
	
	virtual void update(float pStep);
	virtual void OnCollision(Actor* pOther);

	void addScore(float pScore);

	int getMultiplier();

	float getScore();

	void setup();
	
private:
	void SpawnBullet(float pBulletPower);
	void SpawnBullet(float pBulletPower, glm::vec3 pDirection);
	void SpawnBullet(float pBulletPower, glm::vec3 pDirection, float pAngle);
	void SpawnNova();
	
	void FireWeapon(float pTime);
	void FireWeapon2(float pTime);
	void FireWeapon3(float pTime);
	void Move();
	void IsInvulnerable(float pTime);

	glm::vec3 _spawnOffset;

	bool	_invulnerable;
	float	_invulnerabilityTime;
	float	_invulnerabilityTimer;
	int		_defaultFlags;

	float	_weaponTimer;
	float	_fireRate;
	bool	_fired;
	bool	_overheat;

	float	_coolDownTime;
	float	_heat;
	float	_timeToOverheat;
	float	_coolDownRate;

	float	_charge;
	float	_chargeThreshold;
	
	float	_score;

	float				_tiltAngle;
	float				_moveSpeed;	
	Mesh*				_mesh;
	AbstractMaterial*	_material;
	PlayerMaterial*		_playerMaterial;

	int _comboMultiplier = 1;

	sf::Clock ScoreClock;
	sf::Time timeSinceLastDeadEnemy = sf::Time::Zero;

};

#endif // !ACTORMOVEMENT_H

