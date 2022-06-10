#include "game.h"
#include <iostream>

 
std::vector <Eigen::Vector2f> bezierControlPoints; 
const float g_pointRadius    = 3.0f; 
const float g_distanceClickControlThreshold = 10.0f;
int  g_chosenControlPoint = 0;
 

static void printMat(const Eigen::Matrix4d& mat)
{
	std::cout<<" matrix:"<<std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			std::cout<< mat(j,i)<<" ";
		std::cout<<std::endl;
	}
}

Game::Game()
{
	time = 0;
}

//Game::Game(float angle ,float relationWH, float near, float far) : Scene(angle,relationWH,near,far)
//{ 	
//}

unsigned int Game::CreateTex(int width,int height)
{
	unsigned char* data = new unsigned char[width * height * 4];
	for (size_t i = 0; i < width; i++)
	{
		for (size_t j = 0; j < height; j++)
		{
			data[(i * height + j) * 4] = (i + j) % 256;
			data[(i * height + j) * 4 + 1] = (i + j * 2) % 256;
			data[(i * height + j) * 4 + 2] = (i * 2 + j) % 256;
			data[(i * height + j) * 4 + 3] = (i * 3 + j) % 256;
		}
	}
	unsigned int texIndx = AddTexture(width, height, data, COLOR);
	delete [] data;
	return texIndx;
}
 

void Game::Init()
{		
	unsigned int texIDs[4] = { 0 , 1, 0};
	unsigned int slots[4] = { 0 , 1, 0};

	AddShader("./shaders/pickingShader");
	AddShader("./shaders/basicShader");
	AddShader("./shaders/basicShader2");
	AddShader("./shaders/cubemapShader");
	AddShader("./shaders/pickingShader");
	AddShader("./shaders/bezierShader");

	AddTexture("./textures/box0.bmp", 2);
	AddTexture("./textures/cubemaps/Daylight Box_", 3);
	CreateTex(800, 800); 

	AddMaterial(texIDs, slots, 1);
	AddMaterial(texIDs + 1, slots + 1, 1);
	//cube map
	AddShape(Cube, -1, TRIANGLES);
	SetShapeShader(0, 3);
	SetShapeMaterial(0, 1);   
	pickedShape = 0;
	ShapeTransformation(scaleAll, 100, 0);
	SetShapeStatic(pickedShape);
	//
	AddShape(Plane, -2, TRIANGLES, 2);
	SetShapeShader(1, 4);
	pickedShape = 1;
	SetShapeMaterial(pickedShape, 0);
	ShapeTransformation(zTranslate, -1.1, 1);
	SetShapeStatic(pickedShape);
	pickedShape = 0;
	
	AddShape(Cube, -1, TRIANGLES);
	SetShapeMaterial(2, 0);


	AddShape(Axis, -1, TRIANGLES, 1);
	AddShape(Plane, -1, TRIANGLES, 1);
	SetShapeShader(4, 5);
	SetShapeStatic(4);
	SetShapeStatic(3);
  
	bezierControlPoints.push_back(Eigen::Vector2f(880.0, 200.0));
	bezierControlPoints.push_back(Eigen::Vector2f(1100.0, 350.0));
	bezierControlPoints.push_back(Eigen::Vector2f(1500.0, 450.0));
	bezierControlPoints.push_back(Eigen::Vector2f(1550.0, 650.0));

}

void Game::setPressControlPoint(float x, float y) {
	Eigen::Vector2f press(x,  y); 
	for (int i = 0;i < bezierControlPoints.size(); i++) { 
		if ((press - Eigen::Vector2f(bezierControlPoints[i].x(), 800 - bezierControlPoints[i].y())).norm() < g_distanceClickControlThreshold) {
			std::cout << " Chosen Control Point :  " << i << std::endl;
			g_chosenControlPoint = i;
		}
	}
}

void Game::updateCurve(float x,  float y) {
	bezierControlPoints[g_chosenControlPoint] = Eigen::Vector2f(x  , 800 - y);
}


Eigen::Vector3f Game::Convert2DClickTo3DSpace(float xpos, float ypos,const Eigen::Matrix4f& Proj, const Eigen::Matrix4f& View, const Eigen::Matrix4f& Model, int SCR_WIDTH, int SCR_HIEGHT) {
	// converts a position from the 2d xpos, ypos to a normalized 3d direction
	float x = (2.0f * xpos) / SCR_WIDTH - 1.0f;
	float y = 1.0f - (2.0f * ypos) / SCR_HIEGHT;
	// or (2.0f * ypos) / SCR_HEIGHT - 1.0f; depending on how you calculate ypos/lastY
	float z = 1.0f;
	Eigen::Vector3f  ray_nds = Eigen::Vector3f(x, y, z);
	Eigen::Vector4f  ray_clip = Eigen::Vector4f(ray_nds.x(), ray_nds.y(), -1.0f, 1.0f);

	// eye space to clip we would multiply by projection so
	// clip space to eye space is the inverse projection
	Eigen::Matrix4f projCopy = Proj;

	Eigen::Vector4f ray_eye = projCopy.inverse()  * ray_clip;
	// convert point to forwards
	ray_eye = Eigen::Vector4f(ray_eye.x(), ray_eye.y(), -1.0f, 0.0f);

	Eigen::Matrix4f viewCopy = View;
	// world space to eye space is usually multiply by view so
	// eye space to world space is inverse view
	Eigen::Vector4f inv_ray_wor = (viewCopy.inverse() * ray_eye);
	Eigen::Vector3f ray_wor = Eigen::Vector3f(inv_ray_wor.x(), inv_ray_wor.y(), inv_ray_wor.z());
	ray_wor = ray_wor.normalized();
	return ray_wor;
}

void Game::MoveObjectsAccordingToBezier(const Eigen::Matrix4f& Proj, const Eigen::Matrix4f& View, const Eigen::Matrix4f& Model) {

	Eigen::Matrix4f               M;
	Eigen::Matrix <float, 4, 3 >  MG_Result;
	Eigen::Matrix <float, 4, 3 >  curvePoints;

	Eigen::RowVector4f            T;
	Eigen::Vector3f               currentPosition;
	float                         t = 0;


	curvePoints.row(0) = Convert2DClickTo3DSpace(bezierControlPoints[0].x(), bezierControlPoints[0].y(), Proj, View, Model, 800, 800);
	curvePoints.row(1) = Convert2DClickTo3DSpace(bezierControlPoints[1].x(), bezierControlPoints[1].y(), Proj, View, Model, 800, 800);
	curvePoints.row(2) = Convert2DClickTo3DSpace(bezierControlPoints[2].x(), bezierControlPoints[2].y(), Proj, View, Model, 800, 800);
	curvePoints.row(3) = Convert2DClickTo3DSpace(bezierControlPoints[3].x(), bezierControlPoints[3].y(), Proj, View, Model, 800, 800);


	M << -1, 3, -3, 1,
		3, -6, 3, 0,
		-3, 3, 0, 0,
		1, 0, 0, 0;

	MG_Result = M * curvePoints;

	float stepSize = 0.1;
	while (t <= 1) {

		T[0] = powf(t, 3);
		T[1] = powf(t, 2);
		T[2] = t;
		T[3] = 1; 
		currentPosition = (T * MG_Result);
		data_list[2]->SetTranslation(currentPosition.cast<double>() * 2);
		t += stepSize;
	}
	shouldMoveAccordingToBeizer = false;
}

void Game::Update(const Eigen::Matrix4f& Proj, const Eigen::Matrix4f& View, const Eigen::Matrix4f& Model, unsigned int  shaderIndx, unsigned int shapeIndx)
{
	Shader *s = shaders[shaderIndx];
	int r = ((shapeIndx+1) & 0x000000FF) >>  0;
	int g = ((shapeIndx+1) & 0x0000FF00) >>  8;
	int b = ((shapeIndx+1) & 0x00FF0000) >> 16;


	s->Bind();
	s->SetUniformMat4f("Proj", Proj);
	s->SetUniformMat4f("View", View);
	s->SetUniformMat4f("Model", Model);
	s->SetUniform1i("time", time);
	if (data_list[shapeIndx]->GetMaterial() >= 0 && !materials.empty())
	{ 
		BindMaterial(s, data_list[shapeIndx]->GetMaterial());
	} 
	if (shaderIndx == 0)
		s->SetUniform4f("lightColor", r / 255.0f, g / 255.0f, b / 255.0f, 0.0f);
	else
		s->SetUniform4f("lightColor", 4/100.0f, 6 / 100.0f, 99 / 100.0f, 0.5f);
	 
	if (shaderIndx == 5) {
		s->SetUniform1f("p1_x", bezierControlPoints[0].x());
		s->SetUniform1f("p1_y", bezierControlPoints[0].y());
		s->SetUniform1f("p2_x", bezierControlPoints[1].x());
		s->SetUniform1f("p2_y", bezierControlPoints[1].y());
		s->SetUniform1f("p3_x", bezierControlPoints[2].x());
		s->SetUniform1f("p3_y", bezierControlPoints[2].y());
		s->SetUniform1f("p4_x", bezierControlPoints[3].x());
		s->SetUniform1f("p4_y", bezierControlPoints[3].y());
		s->SetUniform1f("POINT_RADIUS", g_pointRadius); 
	}

	if (shouldMoveAccordingToBeizer && shapeIndx == 2) {
		MoveObjectsAccordingToBezier(Proj, View, Model);
	}
	s->Unbind();
}


void Game::WhenRotate()
{
}

void Game::WhenTranslate()
{
}

void Game::Animate() {
	if (isActive) {

	 }
}

void Game::ScaleAllShapes(float amt,int viewportIndx)
{
	for (int i = 1; i < data_list.size(); i++)
	{
		if (data_list[i]->Is2Render(viewportIndx))
		{
            data_list[i]->MyScale(Eigen::Vector3d(amt, amt, amt));
		}
	}
}

Game::~Game(void)
{
}
