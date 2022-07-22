#include "CameraMover.h"

CameraMover::CameraMover(Game* game, int cameraID, std::vector<Eigen::Vector3f> bezierPoints) {
	this->_game = game;
	this->_bezierControlPoints = bezierPoints;
}

void CameraMover::CalculateBezierMoves() {
	_objectCalculatedMoves.clear();
	Eigen::Matrix4f               M;
	Eigen::Matrix <float, 4, 3 >  MG_Result;
	Eigen::Matrix <float, 4, 3 >  curvePoints;

	Eigen::RowVector4f            T;
	Eigen::Vector3f               calculatedNewPos;
	float                         t = 0;

	for (int i = 0; i < 4;i++)
		curvePoints.row(i) = _bezierControlPoints[i];
	M << -1, 3, -3, 1,
		3, -6, 3, 0,
		-3, 3, 0, 0,
		1, 0, 0, 0;

	MG_Result = M * curvePoints;
	 
	float stepSize = 0.02;
	while (t <= 1) {

		T[0] = powf(t, 3);
		T[1] = powf(t, 2);
		T[2] = t;
		T[3] = 1;

		calculatedNewPos = (T * MG_Result);
		_objectCalculatedMoves.push_back(calculatedNewPos);
		t += stepSize;
	} 
}