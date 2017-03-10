#include "HUD.h"
#include "mge/config.hpp"

#include <string>

HUD::HUD(sf::RenderWindow * pWindow) : 	_window(pWindow)	
{

#pragma region Health/Charge

	_containerTexture.loadFromFile	(config::MGE_TEXTURE_PATH + "Hud_Menu_Screens/Hud.png");
	_healthTexture.loadFromFile		(config::MGE_TEXTURE_PATH + "Hud_Menu_Screens/Health.png");
	_chargeTexture.loadFromFile		(config::MGE_TEXTURE_PATH + "Hud_Menu_Screens/Charge.png");

	_container.setTexture(_containerTexture);
	_healthBar.setTexture(_healthTexture);
	_chargeBar.setTexture(_chargeTexture);

	_container.setScale(sf::Vector2f(0.28f, 0.4f));
	_healthBar.setScale(sf::Vector2f(0.28f, 0.34f));
	_chargeBar.setScale(sf::Vector2f(0.28f, 0.42f));

	_healthBarSize = _healthBar.getScale();
	_chargeBarSize = _chargeBar.getScale();

	_container.setPosition(1545, 900);
	sf::Vector2f parent = _container.getPosition();

	_chargeBar.setPosition(parent.x + 65, parent.y + 12);
	_healthBar.setPosition(parent.x + 15, parent.y + 56);	

#pragma endregion

#pragma region ViewBorders

	_rightBorderTexture.loadFromFile(config::MGE_TEXTURE_PATH + "Hud_Menu_Screens/Right_Border.jpg");
	_leftBorderTexture.loadFromFile(config::MGE_TEXTURE_PATH + "Hud_Menu_Screens/Left_Border.jpg");

	_rightBorder.setTexture(_rightBorderTexture);
	_leftBorder.setTexture(_leftBorderTexture);

	_rightBorder.setPosition(1520, 0);
	_leftBorder.setPosition(0, 0);

#pragma endregion

#pragma region ScoreDisplay

	_progressContainerTexture.loadFromFile	(config::MGE_TEXTURE_PATH + "Hud_Menu_Screens/ProgressBar.png");
	_progressFillTexture.loadFromFile		(config::MGE_TEXTURE_PATH + "Hud_Menu_Screens/ProgressFill.png");
	_scoreContainerTexture.loadFromFile		(config::MGE_TEXTURE_PATH + "Hud_Menu_Screens/ScoreContainer.png");
	_multiplierContainerTexture.loadFromFile(config::MGE_TEXTURE_PATH + "Hud_Menu_Screens/MultiplierContainer.png");

	_progressContainer.setTexture	(_progressContainerTexture);
	_progressFill.setTexture		(_progressFillTexture);
	_scoreContainer.setTexture		(_scoreContainerTexture);
	_multiplierContainer.setTexture	(_multiplierContainerTexture);

	_progressContainer.setScale		(sf::Vector2f(0.28f,	0.4f));
	_progressFill.setScale			(sf::Vector2f(0.28f,	0.36f));
	_scoreContainer.setScale		(sf::Vector2f(0.34f,	0.4f));
	_multiplierContainer.setScale	(sf::Vector2f(0.38f,	0.34f));

	_progressFillSize = _progressFill.getScale();

	_progressContainer.setPosition(25, 850);
	sf::Vector2f parent2 = _progressContainer.getPosition();
	
	_progressFill.setPosition(parent2.x + 10, parent2.y + 32);
	_scoreContainer.setPosition(parent2.x, parent2.y + 85);
	_multiplierContainer.setPosition(parent2.x + 210, parent2.y + 85);

#pragma endregion

#pragma region Font/Text
	
	_font.loadFromFile(config::MGE_FONT_PATH + "Dream_MMA.ttf");

	_scoreText.setFont(_font);
	_multiplierText.setFont(_font);

	_scoreText.setString("0");
	_multiplierText.setString("x 1");

	_scoreText.setCharacterSize(20);
	_multiplierText.setCharacterSize(20);

	_scoreText.setFillColor(sf::Color::Yellow);
	_multiplierText.setFillColor(sf::Color::Yellow);

	_scoreText.setPosition(parent2.x + 40, parent2.y + 120);
	_multiplierText.setPosition(parent2.x + 250, parent2.y + 112);
	
#pragma endregion

}

HUD::~HUD()
{
}

void HUD::draw()
{
	_window->pushGLStates();

	_window->draw(_rightBorder);
	_window->draw(_leftBorder);

	_window->draw(_container);
	_window->draw(_chargeBar);
	_window->draw(_healthBar);

	_window->draw(_progressContainer);
	_window->draw(_progressFill);
	_window->draw(_scoreContainer);
	_window->draw(_multiplierContainer);

	_window->draw(_scoreText);
	_window->draw(_multiplierText);

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

	_progressFill.setScale(_progressFillSize.x * percentage, _progressFillSize.y);
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

