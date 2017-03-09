#include "HUD.h"

HUD::HUD(sf::RenderWindow * pWindow, std::string pContainer, std::string pHealBar, std::string pChargeBar) : _window(pWindow), _uniformScaling(sf::Vector2f(0.3f, 0.3f))
{
	_containerTexture.loadFromFile(pContainer);
	_container.setTexture(_containerTexture);
	_container.setScale(_uniformScaling);
	_container.setPosition(0, 1000);

	_healthTexture.loadFromFile(pHealBar);
	_healthBar.setTexture(_healthTexture);
	_healthBar.setScale(_uniformScaling);
	_healthBarSize = _healthBar.getScale();

	_chargeTexture.loadFromFile(pChargeBar);
	_chargeBar.setTexture(_chargeTexture);
	_chargeBar.setScale(_uniformScaling);
	_chargeBarSize = _chargeBar.getScale();

	sf::Vector2f parent = _container.getPosition();
	_chargeBar.setPosition(parent.x + 65, parent.y + 10);
	_healthBar.setPosition(parent.x + 10, parent.y + 40);	
}

HUD::~HUD()
{
}

void HUD::draw()
{
	_window->pushGLStates();
	_window->draw(_container);
	_window->draw(_chargeBar);
	_window->draw(_healthBar);
	_window->popGLStates();
}

void HUD::updateHealth(int pHealth)
{
	_healthBar.setScale(_healthBarSize.x * pHealth /10 * 3, _healthBarSize.y);
}

void HUD::updateCharge(float pCharge)
{
	pCharge /= 100;

	if (pCharge > 1.0f) {
		pCharge = 1.0f;
	}

	if (pCharge < 0.0f) {
		pCharge = 0.0f;
	}
	_chargeBar.setScale(_chargeBarSize.x * pCharge, _chargeBarSize.y);
	
}

