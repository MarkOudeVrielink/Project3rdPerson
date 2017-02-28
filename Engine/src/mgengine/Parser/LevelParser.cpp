#include "LevelParser.h"
#include "mge\config.hpp"
#include"mygame\EnemyWave.h"
#include <vector>
std::string const XML_FILE_PATH = "C://MyTemp/PugiXml/MyDemo.xml";
void LevelParser::SaveLevel(Level * pLevel)
{
	pugi::xml_document doc;
	pugi::xml_node declarationNode = doc.append_child(pugi::node_declaration);
	declarationNode.append_attribute("version") = "1.0";
	declarationNode.append_attribute("encoding") = "ISO-8859-1";
	declarationNode.append_attribute("standalone") = "yes";

	pugi::xml_node root = doc.append_child("Level");
	//std::vector<EnemyWave*> * wavesReference = pLevel->getWaves();
	for (auto const wave : *pLevel->getWaves()) {
		pugi::xml_node LevelChild = root.append_child("Wave");
		LevelChild.append_attribute("startTimeWave") = *wave->getStartTime();
		LevelChild.append_attribute("sizeWave") = *wave->getSizeWave();
		LevelChild.append_attribute("delayBetweenEnemies") = *wave->getDelayBetweenEnemies();
		pugi::xml_node WaveChild = LevelChild.append_child("Waypoints");

		int currentWavePointIndex = 0;
		for (auto const wavePoint : *wave->getWaypoints()) {
			//uncomment To add the number of the waypoint, also modify the load function to search the word wave point and ignore the number, use xPath
			pugi::xml_node position = WaveChild.append_child("WavePoint");// +std::to_string(currentWavePointIndex)).c_str());
			int xPos = wavePoint->getPosition().x;
			int yPos = wavePoint->getPosition().y;
			position.append_attribute("x") = xPos;
			position.append_attribute("y")=  yPos;
			currentWavePointIndex++;
		}
	}

	doc.save_file((config::MGE_FONT_PATH +"/demo.xml").c_str(), PUGIXML_TEXT("  "));
	cout << "saved" << endl;
}

Level* LevelParser::LoadLevel(string pName, sf::RenderWindow* pWindow)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file((config::MGE_FONT_PATH + "/demo.xml").c_str());
	if (!result)
	{
		std::cout << "Parse error: " << result.description()
			<< ", character pos= " << result.offset<<endl;
	}
	// A valid XML document must have a single root node
	pugi::xml_node root = doc.document_element();
	Level* level = new Level();
	level->setRenderWindow(pWindow);
	//Go through all the waves in the level
		for (pugi::xml_node wave = root.child("Wave"); wave; wave = wave.next_sibling("Wave"))
		{
			level->NewWave();
			float startTime = wave.attribute("startTimeWave").as_float();
			int sizeWave = wave.attribute("sizeWave").as_int();
			float delayTime = wave.attribute("delayBetweenEnemies").as_float();

			
			//Waypoint list Element
			pugi::xml_node WaveChild = wave.child("Waypoints");
			cout << startTime << " << delay time of wave loaded" << endl;
			//vector<Waypoint*> waypointList;
			//Go throught all the waypoints in the list and store them in a vector<Waypoint>
			
			for (pugi::xml_node waypoint = WaveChild.child("WavePoint"); waypoint; waypoint = waypoint.next_sibling("WavePoint"))
			{
				//cout<<WaveChild.
				std::cout << "Parse error: " << result.description()
					<< ", character pos= " << result.offset << endl;
				cout <<"Hey Im a wavepoint" << endl;
				sf::Vector2f pos;
				pos.x = waypoint.attribute("x").as_float();
				pos.y = waypoint.attribute("y").as_float();
			
				level->CreateWaypoint(pos, startTime);
				cout << pos.x << " << x Of waypoint loaded" << endl;
			}
			cout << "Hey Wave done" << endl;
		}
		cout << "Hey Im DONE" << endl;
		
	return level;
}
