#ifndef HUD_D
#define HUD_D

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>


class HUD {
public:	
	HUD(sf::RenderWindow* pWindow, std::string pContainer, std::string pHealBar, std::string pChargeBar);
	~HUD();

	void draw();

	void updateHealth(int pHealth);
	void updateCharge(float pCharge);

private:

	sf::Sprite _container;
	sf::Sprite _healthBar;
	sf::Sprite _chargeBar;

	sf::Texture _containerTexture;
	sf::Texture _healthTexture;
	sf::Texture _chargeTexture;

	sf::Vector2f _uniformScaling;
	sf::Vector2f _healthBarSize;
	sf::Vector2f _chargeBarSize;
	
	sf::RenderWindow* _window;

};
#endif // !HUD_D

