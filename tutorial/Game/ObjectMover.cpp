#include "ObjectMover.h"   

ObjectMover::ObjectMover(Game * game, int objectId , std::vector<Eigen::Vector2f> bezierControlPoints) : _objectId(objectId) ,  _bezierControlPoints(bezierControlPoints) ,
     _game(game) { 
	CalculateBezierMoves();
}


Eigen::Vector2f getBezier2DPoint (float t, Eigen::Vector2f p1, Eigen::Vector2f p2, Eigen::Vector2f p3, Eigen::Vector2f p4) {
	Eigen::Vector2f builtPoint;
	builtPoint = (pow((1 - t), 3) * p1) + (3 * pow((1 - t), 2) * t * p2) + (3 * pow(t, 2) * (1 - t) * p3) + pow(t, 3) * p4;

	return builtPoint;
}

Eigen::Vector3f  getVelocity (float t, Eigen::Vector2f p1, Eigen::Vector2f p2, Eigen::Vector2f p3, Eigen::Vector2f p4) {
	Eigen::Vector2f currPoint = getBezier2DPoint(t, p1, p2, p3, p4);
	Eigen::Vector2f nextPoint = getBezier2DPoint(2*t, p1, p2, p3, p4);

	return Eigen::Vector3f(nextPoint.x() - currPoint.x(), nextPoint.y() - currPoint.y(), 0);
}

void ObjectMover::CalculateBezierMoves() {
	_objectCalculatedMoves.clear();

	float                         t = 0; 
	while (t <= 1) {
		auto moveDirection = getVelocity(t, _bezierControlPoints[0], _bezierControlPoints[1], _bezierControlPoints[2], _bezierControlPoints[3]) / 30000;
		_objectCalculatedMoves.push_back(moveDirection);
		t += 0.005;
	} 
}
