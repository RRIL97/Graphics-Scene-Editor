#pragma once
#include "tutorial/Game/game.h"

#include "igl/project.h" 
#include "igl/unproject.h" 
#include <iostream>  
#include <thread>
#include <vector>  
#include <GLFW/glfw3.h> 


class CameraMover {
public:
	CameraMover(Game* game, std::vector<Eigen::Vector3f> bezierPoints);

	void CalculateBezierMoves();
	  
	bool getHasDoneMoving() {
		return _objectCalculatedMoves.size() <= 0;
	}
	 
	Eigen::Vector3f GetNextMove() {
		auto nextMove = _objectCalculatedMoves.at(0);
		_objectCalculatedMoves.erase(_objectCalculatedMoves.begin());

		return nextMove;
	}

	int GetCameraID() {
		return _cameraID;
	}
private:  

	Game* _game;

	std::vector<Eigen::Vector3f> _bezierControlPoints;
	std::vector<Eigen::Vector3f> _objectCalculatedMoves;
	int _cameraID;


};
