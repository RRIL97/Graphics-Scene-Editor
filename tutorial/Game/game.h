#pragma once
#include "igl/opengl/glfw/Viewer.h"
enum class editorState { DESIGN, ANIMATION };

class Game : public igl::opengl::glfw::Viewer
{
	int time; 
public:
	int  pickedShape;
	bool shouldMoveAccordingToBeizer;

	Game();
//	Game(float angle,float relationWH,float near, float far);
	void Init();
	void Update(const Eigen::Matrix4f& Proj, const Eigen::Matrix4f& View, const Eigen::Matrix4f& Model, unsigned int  shaderIndx, unsigned int shapeIndx);
	void WhenRotate();
	void WhenTranslate();
	void Animate() override;
	void ScaleAllShapes(float amt, int viewportIndx);
	unsigned int Game::CreateTex(int width, int height);
	void setPressControlPoint(float x, float y);
	void updateCurve(float x, float y);
	void MoveObjectsAccordingToBezier(const Eigen::Matrix4f& Proj, const Eigen::Matrix4f& View, const Eigen::Matrix4f& Model);
	Eigen::Vector3f Convert2DClickTo3DSpace(float xpos, float ypos, const Eigen::Matrix4f& Proj, const Eigen::Matrix4f& View, const Eigen::Matrix4f& Model, int SCR_WIDTH, int SCR_HIEGHT);
	~Game(void);
	void setState(editorState newState) {
		 state = newState;
	}
	editorState getState() {
		return state;
	}

private:
	editorState state = editorState::DESIGN;
};

