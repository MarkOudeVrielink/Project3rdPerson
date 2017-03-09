#ifndef ACTORMOVEMENT_H
#define ACTORMOVEMENT_H

#include "mgengine\Behaviours\AbstractActorBehaviour.h"

#include <glm.hpp>

class Mesh;
class AbstractMaterial;
class PlayerMaterial;

class PlayerBehaviour : public AbstractActorBehaviour {
public:
	PlayerBehaviour(float pSpeed = 1.0f);
	virtual ~PlayerBehaviour();	
	
	virtual void update(float pStep);
	virtual void OnCollision(Actor* pOther);

	void setup();
	
private:
	void SpawnBullet(float pBulletPower);
	void SpawnNova();
	
	void FireWeapon(float pTime);
	void Move(float pDeltaTime);
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
	
	bool	_leftPressed;
	bool	_rightPressed;
	float	_tapClock;
	int		_tapCount;
	float	_doubleTapTime;

	float		_tiltAngle;
	float		_maxSpeed;
	float		_dashPower;
	float		_horizontalInput;
	float		_verticalInput;
	float		_acceleration;
	float		_decceleration;
	glm::vec3	_force;	

	void		_checkInput(bool& h, bool& v);
	void		_deccelerate(bool h, bool v);
	void		_checkDoubleTap();
	
	PlayerMaterial*		_playerMaterial;	
};

#endif // !ACTORMOVEMENT_H

