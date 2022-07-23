#include "ObjectMover.h"   

ObjectMover::ObjectMover(Game * game, int objectId , std::vector<Eigen::Vector2f> bezierControlPoints,
	const Eigen::Matrix4f& Proj, const Eigen::Matrix4f& View, const Eigen::Matrix4f& Model ) : _objectId(objectId) ,  _bezierControlPoints(bezierControlPoints) ,
     _proj(Proj), _view(View), _model(Model), _game(game) {
	 
	CalculateBezierMoves();
}


/// <summary>
/// Calculates Bezier Moves
/// </summary>



void ObjectMover::CalculateBezierMoves() {
	_objectCalculatedMoves.clear();
	Eigen::Matrix4f               M;
	Eigen::Matrix <float, 4, 3 >  MG_Result;
	Eigen::Matrix <float, 4, 3 >  curvePoints;

	Eigen::RowVector4f            T;
	Eigen::Vector3f               calculatedNewPos;
	float                         t = 0;

	std::cout << _bezierControlPoints[0].x() << WIDTH << std::endl;

	for (int i = 0; i < 4;i++)
		curvePoints.row(i) = Eigen::Vector3f(_bezierControlPoints[i].x()  , _bezierControlPoints[i].y(), 0);

	M << -1, 3, -3, 1,
		3, -6, 3, 0,
		-3, 3, 0, 0,
		1, 0, 0, 0;

	MG_Result = M * curvePoints;

	std::vector<Eigen::Vector3f> moves;

	float stepSize = 0.01;
	while (t <= 1) {

		T[0] = powf(t, 3);
		T[1] = powf(t, 2);
		T[2] = t;
		T[3] = 1;

		calculatedNewPos = (T * MG_Result);
		moves.push_back(calculatedNewPos);
		t += stepSize;
	} 
	for (int i = 1;i < moves.size(); i++)
		_objectCalculatedMoves.push_back((moves[i] - moves[i - 1]) * 0.6  + _game->data_list[_objectId]->GetTranslation().cast<float>());
}
