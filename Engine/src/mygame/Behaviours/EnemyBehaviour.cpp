#include "EnemyBehaviour.h"
#include <list>
#include "mygame/Waypoint.h"
#include "mygame/Enemy.h"
#include <glm.hpp>
EnemyBehaviour::EnemyBehaviour(std::vector<Waypoint*> *pWayPoints)
{
	_wayPoints = pWayPoints;
	_moveSpeed = 20.0f;
}

EnemyBehaviour::EnemyBehaviour(std::vector<Waypoint*>* pWayPoints, float * ptime)
{
	_wayPoints = pWayPoints;
	_moveSpeed = 20.0f;
	_levelEditorMode = true;//MoveAccording to the time, so no constant movement
	_referenceToArtificialLevelTime = ptime;
	_spawnedTime = *ptime;

}

EnemyBehaviour::~EnemyBehaviour()
{
}

void EnemyBehaviour::SaveOriginalTransform()
{
	_originalTransform = _owner->getTransform();
}

void EnemyBehaviour::update(float pStep)
{
	
	if (!_levelEditorMode) {
		AiBasic(pStep);
	}
	else
		UpdateEditorMode(pStep);

	/*	///TODO: Apply rotation with Slerp to smoothly look at the next waypoint while moving
		///May help http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/
		//_owner->setTransform(glm::inverse(glm::lookAt(tarjet, tarjet + _owner->getWorldPosition(), glm::vec3(0, 1, 0))));
		glm::vec3 forward = glm::normalize( _owner->getLocalPosition() - tarjet);
		glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0, 1, 0), forward));
		glm::vec3 up = glm::cross(forward, right);

		_owner->setTransform(
			glm::mat4(glm::vec4(right, 0), glm::vec4(up, 0), glm::vec4(forward, 0), glm::vec4(_owner->getLocalPosition(), 1))
		);
		cout << _owner->getWorldPosition() << endl;
		*/

}
//FRAMERATE ISSUE COULD BE FIX IF WE SAVED LAST TIME WE CHANGE WAYPOINT AND ONLY ROTATE WHEN WE CHANGE NODE
void EnemyBehaviour::UpdateEditorMode(float pStep)
{
	if (*_referenceToArtificialLevelTime >= 0) {
		//If we havent move to the second we should be then calculate that position
		while (_secondsAlreadyRendered < *_referenceToArtificialLevelTime - _spawnedTime)//FORWARD
		{
			AiBasic(pStep);
			_secondsAlreadyRendered += 1.0f / 60.0f;//Harcoded guess that we have 60 frames per second

		}

		//If we havent move to the second we should be then calculate that position
		while (_secondsAlreadyRendered - *_referenceToArtificialLevelTime + _spawnedTime > 0.017f)//BACKWARDS when the difference is 1 sec
		{

			AiBasicBackWards(pStep);
			_secondsAlreadyRendered -= 1.0f / 60.0f;//Harcoded guess that we have 60 frames per second
		}
	}

}
//Pass the frame length and move the enemy the distance it should move in that frame
// Approx -> 0.01666666 length of frame * 60 = is equal to one sec
void EnemyBehaviour::AiBasic(float pStep)
{
	if (_tarjet == nullptr || _movingBackwards == true) {
		
		if ((float)_index <= 0)
			_index = 1;
		else _index++;
		
		if ((float)_index >= _wayPoints->size())
			_index = _wayPoints->size() - 1;
		cout << _wayPoints->size() << endl;
		//ERROR, somehow size of waypoints is equal 0
		_tarjet = _wayPoints->at(_index);//index++ maybe
		_movingBackwards = false;
	}


	glm::vec3 tarjet(160.0f / 1920 * _tarjet->getPosition().x - 80, 0, 80.0f / 1080 * _tarjet->getPosition().y - 40);
	glm::vec3 pos = _owner->getWorldPosition();
	glm::vec3 forward = glm::normalize(_owner->getWorldPosition() - tarjet);
	glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0, 1, 0), forward));
	glm::vec3 up = glm::cross(forward, right);
	if (_wayPoints->size() >= (float)_index) {
		//THIS OVERRIDE SCALING(?), the 1.5f change the scale, if higher number -> smaller model,  if smaller number -> bigger model
		_owner->setTransform(
			glm::mat4(glm::vec4(right, 0), glm::vec4(up, 0), glm::vec4(forward, 0), glm::vec4(_owner->getWorldPosition(), 1.0f))
		);
	}

	//_owner->setTransform(glm::transpose(glm::lookAt(_owner->getWorldPosition(), glm::vec3(tarjet) , glm::vec3(0, 1, 0))));
	glm::vec2 delta = glm::vec2(tarjet.x, tarjet.z) - glm::vec2(pos.x, pos.z);
	float length = glm::length(delta);
	if (length< _moveSpeed*pStep && _wayPoints->size() > (float)_index) {
		cout << _owner->getWorldPosition().x << "<x" << _owner->getWorldPosition().z << "<z" << endl;
		_tarjet = _wayPoints->at(_index++);

	}
	else {
		if (_wayPoints->size() == _index) //Increment index so we dont keep rotating in order to keep moving and leave the screen
			_index++;
		float xPos = delta.x / length;//Not using since we rotate to the direction we want to move and just need to move forward now
		float yPos = delta.y / length;//

		_owner->translate(glm::vec3(0 * _moveSpeed, 0.0f, -1 * pStep*_moveSpeed));

	}
}
void EnemyBehaviour::AiBasicBackWards(float pStep)
{
	if (_tarjet == nullptr)
	{
		cout << "haven�t started yet... returning" << endl;
		return;
		//_tarjet = _wayPoints->at(_index++);//index++ maybe
	}
	if (_movingBackwards == false)//Check if is first frame that we move backwards to invert waypoint
	{
		
		if ((float)_index > _wayPoints->size())
			_index = _wayPoints->size();
		if ((float)_index > 0 && _wayPoints->size() >= (float)_index) //check if there is a waypoint behind
		{				
			//cout << _wayPoints->at(_index--)->getPosition().x << endl;
			 _index--;
			_tarjet = _wayPoints->at(_index);
		}
		_movingBackwards = true;
	}
	//ROTATION
	glm::vec3  tarjet = glm::vec3(160.0f / 1920 * _tarjet->getPosition().x - 80, 0, 80.0f / 1080 * _tarjet->getPosition().y - 40);
	glm::vec3 forward = glm::normalize(-_owner->getWorldPosition() + tarjet);
	glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0, 1, 0), forward));
	glm::vec3 up = glm::cross(forward, right);//could be constant since top view... anyways it can be improved
	if (_index>=0) //check if there is a waypoint behind
	{//If we haven�t reached the last waypoint keep rotating
		_owner->setTransform(glm::mat4(glm::vec4(right, 0), glm::vec4(up, 0), glm::vec4(forward, 0), glm::vec4(_owner->getWorldPosition(), 1.0f))
		);
		
	}
	

	glm::vec3 pos = _owner->getWorldPosition();
	glm::vec2 delta = glm::vec2(tarjet.x, tarjet.z) - glm::vec2(pos.x, pos.z);
	float length = glm::length(delta);

	if (length< _moveSpeed*pStep && _index >0)
	{
		_index--;
		//cout << _owner->getWorldPosition().x << "<x" << _owner->getWorldPosition().z << "<z" << endl;
		_tarjet = _wayPoints->at(_index);
	}
	else
	{
		/*if (0 == _index) //Decrement index so we dont keep rotating in order to keep moving and leave the screen
			_index--;*/
		//cout << _index << endl;
		_owner->translate(glm::vec3(0 * _moveSpeed, 0.0f, 1 * pStep*_moveSpeed));
	}
	
}
///OLD AI but still was working with some comments about improvemnts, in other words, saved in case I mess up
/*
//TODO: Interpolate rotation with next waypoint
if (_tarjet == nullptr) {
	cout << "MOVE" << endl;
	_tarjet = _wayPoints->at(_index++);//index++ maybe
}


glm::vec3 tarjet(160.0f / 1920 * _tarjet->getPosition().x - 80, 0, 80.0f / 1080 * _tarjet->getPosition().y - 40);
glm::vec3 pos = _owner->getWorldPosition();
glm::vec3 forward = glm::normalize(_owner->getLocalPosition() - tarjet);
glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0, 1, 0), forward));
glm::vec3 up = glm::cross(forward, right);
if (_wayPoints->size() >= _index) {
	//THIS OVERRIDE SCALING(?), the 1.5f change the scale, if higher number -> smaller model,  if smaller number -> bigger model
	_owner->setTransform(
		glm::mat4(glm::vec4(right, 0), glm::vec4(up, 0), glm::vec4(forward, 0), glm::vec4(_owner->getLocalPosition(), 1.0f))
	);
}

//_owner->setTransform(glm::transpose(glm::lookAt(_owner->getWorldPosition(), glm::vec3(tarjet) , glm::vec3(0, 1, 0))));
glm::vec2 delta = glm::vec2(tarjet.x, tarjet.z) - glm::vec2(pos.x, pos.z);
float length = glm::length(delta);
if (length< _moveSpeed*pStep && _wayPoints->size() > _index) {
	cout << _owner->getWorldPosition().x << "<x" << _owner->getWorldPosition().z << "<z" << endl;
	_tarjet = _wayPoints->at(_index++);

}
else {
	if (_wayPoints->size() == _index) //Increment index so we dont keep rotating in order to keep moving and leave the screen
		_index++;
	float xPos = delta.x / length;//Not using since we rotate to the direction we want to move and just need to move forward now
	float yPos = delta.y / length;//

	_owner->translate(glm::vec3(0 * _moveSpeed, 0.0f, -1 * pStep*_moveSpeed));
}*/