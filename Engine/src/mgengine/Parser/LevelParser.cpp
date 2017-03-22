#include "LevelParser.h"
#include "mge\config.hpp"
#include"mygame\EnemyWave.h"
#include <vector>
#include "glm.hpp"
std::string const XML_FILE_PATH = "C://MyTemp/PugiXml/MyDemo.xml";
void LevelParser::SaveLevel(Level * pLevel,string pfileName = "demo")
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
		LevelChild.append_attribute("health") = *wave->getHealth();
		LevelChild.append_attribute("speed") = *wave->getSpeed();
		LevelChild.append_attribute("shootRatio") = *wave->getShootRatio();
		LevelChild.append_attribute("enemyType") = *wave->getEnemyType();
		LevelChild.append_attribute("enemyBehaviour") = *wave->getEnemyBehaviour();
		LevelChild.append_attribute("sizeWave") = *wave->getSizeWave();
		LevelChild.append_attribute("delayBetweenEnemies") = *wave->getDelayBetweenEnemies();
		//LevelChild.append_attribute("waypointDirection_x") = wave->GetMainWaypointDirection()->getWorldPos().x;
		//LevelChild.append_attribute("waypointDirection_y") = wave->GetMainWaypointDirection()->getWorldPos().y;
		//LevelChild.append_attribute("waypointDirection_z") = wave->GetMainWaypointDirection()->getWorldPos().z;

		if (wave->GetMainWaypointDirection() != NULL) {
			int xPos = wave->GetMainWaypointDirection()->getScreenPosition().x;
			int yPos = wave->GetMainWaypointDirection()->getScreenPosition().y;

			LevelChild.append_attribute("Screen_x") = xPos;
			LevelChild.append_attribute("Screen_y") = yPos;
		}
		pugi::xml_node WaveChild = LevelChild.append_child("Waypoints");
	

		int currentWavePointIndex = 0;
		for (auto const wavePoint : *wave->getWaypoints()) {
			//uncomment To add the number of the waypoint, also modify the load function to search the word wave point and ignore the number, use xPath
			pugi::xml_node position = WaveChild.append_child("WavePoint");// +std::to_string(currentWavePointIndex)).c_str());
			float xWorldPos = wavePoint->getWorldPos().x;
			float yWorldPos = wavePoint->getWorldPos().y;
			float zWorldPos = wavePoint->getWorldPos().z;
			//cout << wavePoint->getWorldPos().x<<"X IN PARSER"<<endl;
			position.append_attribute("World_x") = xWorldPos;
			position.append_attribute("World_y") = yWorldPos;
			position.append_attribute("World_z") = zWorldPos;
			int xPos = wavePoint->getScreenPosition().x;
			int yPos = wavePoint->getScreenPosition().y;
			position.append_attribute("Screen_x") = xPos;
			position.append_attribute("Screen_y")=  yPos;
			
			currentWavePointIndex++;
		}
	}

	doc.save_file((config::MGE_FONT_PATH +"/"+ pfileName+".xml").c_str(), PUGIXML_TEXT("  "));
	//cout << "saved... "<< pfileName << endl;
}

Level* LevelParser::LoadLevel(string pName ="demo", sf::RenderWindow* pWindow= NULL)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file((config::MGE_FONT_PATH + "/"+ pName+".xml").c_str());
	if (!result)
	{
		//std::cout << "Parse error: " << result.description()
			//<< ", character pos= " << result.offset<<endl;
		return NULL;
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
			float speed = wave.attribute("speed").as_float();
			float shootRatio = wave.attribute("shootRatio").as_float();
			int enemyType = wave.attribute("enemyType").as_int();
			int enemyBehaviour = wave.attribute("enemyBehaviour").as_int();
			float health = wave.attribute("health").as_float();

			//Can be improved
			
			//glm::vec3 moveMainWaypoint = glm::vec3(wave.attribute("World_x").as_float(), wave.attribute("World_y").as_float(), wave.attribute("World_z").as_float());
			//Waypoint* waypointp = new Waypoint(moveMainWaypoint, sf::Vector2f(0, 0), 0, level->getIndexWave(), pWindow);
			sf::Vector2f posMain;
			//posMain.x = wave.attribute("Screen_x").as_float();
			//posMain.y = wave.attribute("Screen_y").as_float();
			//level->CreateMainWaypointMoveDirection(moveMainWaypoint, posMain,0);
			level->getCurrentWave()->setSizeWave(sizeWave);
			level->getCurrentWave()->setDelayBetweenEnemies(delayTime);
			level->getCurrentWave()->setSpeed(speed);
			level->getCurrentWave()->setShootRatio(shootRatio);
			level->getCurrentWave()->setEnemyType(Materials::ID(enemyType));
			level->getCurrentWave()->setEnemyBehaviour(enemyBehaviour);
			level->getCurrentWave()->setHealth(health);


			//Waypoint list Element
			pugi::xml_node WaveChild = wave.child("Waypoints");
			//cout << startTime << " << delay time of wave loaded" << endl;
			//vector<Waypoint*> waypointList;
			//Go throught all the waypoints in the list and store them in a vector<Waypoint>
			
			for (pugi::xml_node waypoint = WaveChild.child("WavePoint"); waypoint; waypoint = waypoint.next_sibling("WavePoint"))
			{
				//cout<<WaveChild.
			//	std::cout << "Parse error: " << result.description()

				//	<< ", character pos= " << result.offset << endl;
				//cout <<"Hey Im a wavepoint" << endl;

			//		<< ", character pos= " << result.offset << endl;
			//	cout <<"Hey Im a wavepoint" << endl;

				sf::Vector2f pos;
				pos.x = waypoint.attribute("Screen_x").as_float();
				pos.y = waypoint.attribute("Screen_y").as_float();
			

				//level->CreateWaypoint(pos, startTime);

				glm::vec3 worldPos;
				worldPos.x = waypoint.attribute("World_x").as_float();
				worldPos.y = waypoint.attribute("World_y").as_float();
				worldPos.z = waypoint.attribute("World_z").as_float();
				level->CreateWaypoint(worldPos,pos, startTime);

				//cout << pos.x << " << x Of waypoint loaded" << endl;
			}
			//cout << "Hey Wave done" << endl;
		}
		//cout << "Hey Im DONE" << endl;
		
	return level;
}
