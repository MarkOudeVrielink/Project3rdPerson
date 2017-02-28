#pragma once
#include "mgengine\Parser\pugixml.hpp"
#include "mygame\Level.h"
static class LevelParser
{
public:
	static void SaveLevel(Level* pLevel);
	static Level *LoadLevel(string pName, sf::RenderWindow* pWindow);
};