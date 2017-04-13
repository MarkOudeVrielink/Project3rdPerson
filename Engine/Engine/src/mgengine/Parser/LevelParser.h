#pragma once
#include "mgengine\Parser\pugixml.hpp"
#include "mygame\Level.h"
class LevelParser
{
public:
	//static void SaveLevel(Level* pLevel);
	static void SaveLevel(Level * pLevel, string pfileName);
	static Level *LoadLevel(string pName, sf::RenderWindow* pWindow);
};