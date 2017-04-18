#include "HUD.h"
#include "mge/config.hpp"

#include <string>

HUD::HUD(sf::RenderWindow * pWindow) : 	_window(pWindow)	
{

#pragma region ViewBorders

	_rightBorderTexture.loadFromFile(config::MGE_TEXTURE_PATH + "Hud_Menu_Screens/Hud_Right.jpg");
	_leftBorderTexture.loadFromFile(config::MGE_TEXTURE_PATH + "Hud_Menu_Screens/Hud_Left.jpg");

	_rightBorder.setTexture(_rightBorderTexture);
	_leftBorder.setTexture(_leftBorderTexture);

	_rightBorder.setPosition(1520, 0);
	_leftBorder.setPosition(0, 0);

	sf::Vector2f rightBorder = _rightBorder.getPosition();
	sf::Vector2f leftBorder = _leftBorder.getPosition();

#pragma endregion

#pragma region Health/Charge
		
	_healthTexture.loadFromFile		(config::MGE_TEXTURE_PATH + "Hud_Menu_Screens/Health.png");
	_chargeTexture.loadFromFile		(config::MGE_TEXTURE_PATH + "Hud_Menu_Screens/Charge.png");
	_overLoadTexture.loadFromFile	(config::MGE_TEXTURE_PATH + "Hud_Menu_screens/overload.png");
		
	_healthBar.setTexture(_healthTexture);
	_chargeBar.setTexture(_chargeTexture);
	_overLoadIcon.setTexture(_overLoadTexture);

	_healthBar.setOrigin(0, 953);
	_chargeBar.setOrigin(0, 936);
		
	_healthBarSize = _healthBar.getScale();
	_chargeBarSize = _chargeBar.getScale();
	
	_overLoadIcon.setScale(sf::Vector2f(0.4f, 0.4f));
	_overLoadIcon.setColor(sf::Color::Transparent);

	_healthBar.setPosition(rightBorder.x + 240, rightBorder.y + 1000);
	_chargeBar.setPosition(rightBorder.x + 150, rightBorder.y + 990);
	_overLoadIcon.setPosition(rightBorder.x + 30, rightBorder.y + 70);

#pragma endregion

#pragma region ScoreDisplay
		
	_progressFillTexture.loadFromFile		(config::MGE_TEXTURE_PATH + "Hud_Menu_Screens/ProgressFill.png");	
	_progressFill.setTexture				(_progressFillTexture);	
	_progressFill.setOrigin					(sf::Vector2f(0, 451));
	_progressFill.setPosition				(leftBorder.x + 154, leftBorder.y + 1026);
	_progressFillSize = _progressFill.getScale();	
	
#pragma endregion

#pragma region Font/Text
	
	_font.loadFromFile(config::MGE_FONT_PATH + "Dream_MMA.ttf");

	_scoreText.setFont(_font);
	_multiplierText.setFont(_font);

	_scoreText.setString("0");
	_multiplierText.setString("x 1");

	_scoreText.setCharacterSize(30);
	_multiplierText.setCharacterSize(30);

	_scoreText.setFillColor(sf::Color::Yellow);
	_multiplierText.setFillColor(sf::Color::Yellow);

	_scoreText.setPosition(leftBorder.x + 170, leftBorder.y + 130);
	_multiplierText.setPosition(leftBorder.x + 170, leftBorder.y + 280);
	
#pragma endregion

}

HUD::~HUD()
{
}

void HUD::draw()
{
	if (_active) {
		_window->pushGLStates();

		_window->draw(_rightBorder);
		_window->draw(_leftBorder);

		_window->draw(_chargeBar);
		_window->draw(_healthBar);
		_window->draw(_overLoadIcon);

		_window->draw(_progressFill);

		_window->draw(_scoreText);
		_window->draw(_multiplierText);

		_window->popGLStates();
	}
}

void HUD::updateHealth(int pHealth)
{
	_healthBar.setScale(_healthBarSize.x, _healthBarSize.y * pHealth/ 3);
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
	_chargeBar.setScale(_chargeBarSize.x, _chargeBarSize.y * pCharge);
	
}

void HUD::updateOverLoad(bool pToggle)
{
	if (pToggle) {
		_overLoadIcon.setColor(sf::Color(255,255,255,255));
	}
	else {
		_overLoadIcon.setColor(sf::Color::Transparent);
	}
}

void HUD::updateWeaponProgress(float pScore)
{
	float percentage;

	if (pScore < 500) {
		percentage = (pScore / 5) / 100;
	} else if (pScore < 1000) {
		percentage = ((pScore - 500) / 5) / 100;
	} else {
		percentage = 1.0f;
	}

	_progressFill.setScale(_progressFillSize.x, _progressFillSize.y * percentage);
}

void HUD::updateScore(int pScore)
{
	sf::String text = std::to_string(pScore);
	_scoreText.setString(text);
}

void HUD::updateMultiplier(int pMultiplier)
{
	sf::String text = std::to_string(pMultiplier);
	_multiplierText.setString("x " + text);
}

void HUD::setActive(bool pActive)
{
	_active = pActive;
	
}

