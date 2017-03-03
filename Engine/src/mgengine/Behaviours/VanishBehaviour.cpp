#include "mgengine\Behaviours\VanishBehaviour.h"
#include "mgengine\Core\Actor.h"



VanishBehaviour::VanishBehaviour(float pLifeTime) : _lifeTime(pLifeTime), _timeLived(0)
{
}

VanishBehaviour::~VanishBehaviour()
{
}

void VanishBehaviour::update(float pStep)
{
	_timeLived += pStep;

	if (_timeLived >= _lifeTime)
		_owner->Destroy();

	if (_timeLived < _lifeTime / 3)
		_owner->scale(_owner->getScale() + _timeLived/30);

	if(_timeLived > _lifeTime /3)
		if(_owner->getScale().x > 0.1f)
		_owner->scale(_owner->getScale() - _timeLived/40);

}

void VanishBehaviour::OnCollision()
{
}
