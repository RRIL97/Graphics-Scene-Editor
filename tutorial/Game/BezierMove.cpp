#include "BezierMove.h"   

BezierMove::BezierMove(Game * game, int objectId , std::vector<Eigen::Vector2f> bezierControlPoints,
	const Eigen::Matrix4f& Proj, const Eigen::Matrix4f& View, const Eigen::Matrix4f& Model ) : _objectId(objectId) ,  _bezierControlPoints(bezierControlPoints) ,
     _proj(Proj), _view(View), _model(Model), _game(game) {
	 
	CalculateBezierMoves();
}


/// <summary>
/// Calculates Bezier Moves
/// </summary>


void BezierMove::CalculateBezierMoves() {
	Eigen::Matrix4f               M;
	Eigen::Matrix <float, 4, 3 >  MG_Result;
	Eigen::Matrix <float, 4, 3 >  curvePoints;

	Eigen::RowVector4f            T;
	Eigen::Vector3f               calculatedNewPos;
	float                         t = 0;

	for (int i = 0; i < 4;i++)
		curvePoints.row(i) = GetPositionUnprojected(_bezierControlPoints[i].x() - 800, 800 - _bezierControlPoints[i].y(), _proj, _view, _model);
	M << -1, 3, -3, 1,
		3, -6, 3, 0,
		-3, 3, 0, 0,
		1, 0, 0, 0;

	MG_Result = M * curvePoints;

	std::vector<Eigen::Vector3f> moves;

	float stepSize = 0.05;
	while (t <= 1) {

		T[0] = powf(t, 3);
		T[1] = powf(t, 2);
		T[2] = t;
		T[3] = 1;

		calculatedNewPos = (T * MG_Result);
		moves.push_back(calculatedNewPos);
		t += stepSize;
	}
	float smoothFactor = 0.7;
	for (int i = 1;i < moves.size(); i++)
		_objectCalculatedMoves.push_back((moves[i] - moves[i - 1]) * smoothFactor + _game->data_list[_objectId]->GetTranslation().cast<float>());
}


Eigen::Vector3f BezierMove::GetPositionUnprojected(float posX, float posY, const Eigen::Matrix4f& Proj, const Eigen::Matrix4f& View, const Eigen::Matrix4f& Model) {


	Eigen::Matrix4f modelview = View * Model;
	Eigen::Vector4f viewport = { 0.0, 0.0, 800, 800 };

	float winX = posX;
	float winY = viewport[3] - posY;

	float depth;
	glReadPixels(winX, winY - posY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

	const float epsi = 0.00001f;
	if (depth > 1.0f - epsi)
	{
		Eigen::Vector3f world_origin{ 0.0f, 0.0f, 0.0f };
		Eigen::Vector3f origin_ndc = igl::project(world_origin, View, Proj, viewport);
		depth = origin_ndc[2];
	}
	Eigen::Vector3f screenCoords{ winX, winY, depth };
	Eigen::Vector3f posUnprojected = igl::unproject(screenCoords, modelview, Proj, viewport);

	return posUnprojected;
}