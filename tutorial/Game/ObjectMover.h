#pragma once
#include "tutorial/Game/game.h"

#include "igl/project.h" 
#include "igl/unproject.h" 
#include <iostream>  
#include <thread>
#include <vector>  
#include <GLFW/glfw3.h> 


class ObjectMover {
public:
	ObjectMover(Game* game, int objectId, std::vector<Eigen::Vector2f> bezierControlPoints, const Eigen::Matrix4f& Proj, const Eigen::Matrix4f& View, const Eigen::Matrix4f& Model);

	void CalculateBezierMoves();

	bool getHasDoneMoving() {
		return _objectCalculatedMoves.size() <= 0;
	}

	/// <summary>
	/// Returns the next Bezier move (Vector)
	/// </summary>
	/// <returns></returns>

	Eigen::Vector3f GetNextMove() {
		if (firstMove) {
			_game->data_list[_objectId]->ZeroTrans();
			firstMove = false;
		}
		auto nextMove = _objectCalculatedMoves.at(0);
		_objectCalculatedMoves.erase(_objectCalculatedMoves.begin());

		return nextMove;
	}

	std::vector<Eigen::Vector3f> GetAllMoves() {
		return _objectCalculatedMoves;
	}

	int GetObjectId() {
		return _objectId;
	}
private:
	Eigen::Matrix4f _proj;
	Eigen::Matrix4f _view;
	Eigen::Matrix4f _model;
	float HEIGHT = 800;
	float WIDTH = 800;

	Game* _game;

	std::vector<Eigen::Vector2f> _bezierControlPoints;
	std::vector<Eigen::Vector3f> _objectCalculatedMoves;
	int _objectId;

	bool firstMove = true;

};
