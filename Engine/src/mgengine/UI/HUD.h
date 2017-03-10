#ifndef HUD_D
#define HUD_D

#include <SFML/Graphics.hpp>
#include <iostream>

class HUD {
public:	
	HUD(sf::RenderWindow* pWindow);
	~HUD();

	void draw();

	void updateHealth(int pHealth);
	void updateCharge(float pCharge);
	void updateWeaponProgress(float pScore);
	void updateScore(int pScore);
	void updateMultiplier(int pMultiplier);

private:

#pragma region Health/Charge

	sf::Sprite _container;
	sf::Sprite _healthBar;
	sf::Sprite _chargeBar;

	sf::Texture _containerTexture;
	sf::Texture _healthTexture;
	sf::Texture _chargeTexture;
	
	sf::Vector2f _healthBarSize;
	sf::Vector2f _chargeBarSize;

#pragma endregion

#pragma region ViewBorders

	sf::Texture _leftBorderTexture;
	sf::Texture _rightBorderTexture;

	sf::Sprite _leftBorder;
	sf::Sprite _rightBorder;	

#pragma endregion

#pragma region ScoreDisplay

	sf::Texture _progressContainerTexture;
	sf::Texture _progressFillTexture;
	sf::Texture _scoreContainerTexture;
	sf::Texture _multiplierContainerTexture;

	sf::Sprite _progressContainer;
	sf::Sprite _progressFill;
	sf::Sprite _scoreContainer;
	sf::Sprite _multiplierContainer;

	sf::Vector2f _progressFillSize;

	sf::Font	_font;

	sf::Text	_scoreText;
	sf::Text	_multiplierText;

#pragma endregion

	sf::RenderWindow* _window;

};
#endif // !HUD_D

