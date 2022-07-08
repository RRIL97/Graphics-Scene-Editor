#include "game.h" 
#include <iostream>
#include "igl/unproject.h"
#include "igl/project.h"
#include <GLFW/glfw3.h>
#include "BezierMove.h"
 
std::vector <Eigen::Vector2f> bezierControlPoints; 

const float g_pointRadius    = 3.0f; 
const float g_distanceClickControlThreshold = 10.0f;
int         g_chosenControlPoint = 0;

std::vector<BezierMove*> g_bezierObjects;

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
	unsigned int texIDs[6] = {  0, 1, 2, 3, 4 , 5};
	unsigned int slots[6] = {  0, 1, 2, 3, 4 , 5};

	AddShader("./shaders/pickingShader");
	AddShader("./shaders/basicShader");
	AddShader("./shaders/basicShader2");
	AddShader("./shaders/cubemapShader");
	AddShader("./shaders/pickingShader");
	AddShader("./shaders/bezierShader");
	AddShader("./shaders/blurShader");
	AddShader("./shaders/pickingShader");


	AddTexture("./textures/cubemaps/Daylight Box_", 3);
	AddTexture("./textures/cubemaps/ocean/ocean_", 3);
	AddTexture("./textures/cubemaps/land/land_", 3);
	AddTexture("./textures/cubemaps/mountain/mountain_", 3);
	AddTexture("./textures/box0.bmp", 2);
	CreateTex(800, 800); 


	AddMaterial(texIDs, slots, 1);
	AddMaterial(texIDs + 1, slots + 1, 1);
	AddMaterial(texIDs + 2, slots + 2, 1);
	AddMaterial(texIDs + 3, slots + 3, 1);
	AddMaterial(texIDs + 4, slots + 4, 1, " box");
	AddMaterial(texIDs + 5, slots + 5, 1, "some");

	//cube map
	AddShape(Cube, -1, TRIANGLES);
	SetShapeShader(0, 3);
	SetShapeMaterial(0, 0);   
	pickedShape = 0;
	ShapeTransformation(scaleAll, 100, 0);
	SetShapeStatic(pickedShape);
	//scissors
	AddShape(Plane, -2, TRIANGLES, 2);
	SetShapeShader(1, 4);
	pickedShape = 1;
	SetShapeMaterial(pickedShape, 4);
	ShapeTransformation(zTranslate, -1.1, 1);
	SetShapeStatic(pickedShape);
	pickedShape = 2;

	AddShape(Cube, -1, TRIANGLES);
	SetShapeMaterial(2, 4);

	AddShape(Axis, -1, TRIANGLES, 1);
	AddShape(Plane, -1, TRIANGLES, 1);
	SetShapeShader(4, 5);
	SetShapeStatic(4);
	SetShapeStatic(3);
	 
	AddShape(Cube, -1, TRIANGLES);
	SetShapeShader(5, 6);
	SetShapeMaterial(5, 4);

	bezierControlPoints.push_back(Eigen::Vector2f(880.0, 200.0));
	bezierControlPoints.push_back(Eigen::Vector2f(1100.0, 350.0));
	bezierControlPoints.push_back(Eigen::Vector2f(1500.0, 450.0));
	bezierControlPoints.push_back(Eigen::Vector2f(1550.0, 650.0));
	pickedShape = 0;

	/*AddShape(Cube, -1, TRIANGLES, 4);
	SetShapeShader(6, 4);
	pickedShape = 6;
	SetShapeMaterial(pickedShape, 4);*/
	//ShapeTransformation(zTranslate, -1.1, 1);
	pickedShape = 0;

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
	if (shaderIndx == 0 )
		s->SetUniform4f("lightColor", r / 255.0f, g / 255.0f, b / 255.0f, 0.0f);
	else if(shaderIndx == 7)
		s->SetUniform4f("lightColor", 0.5f, 0.6f, 0.7f, 0.5f);
	else
		s->SetUniform4f("lightColor", 4/100.0f, 6 / 100.0f, 99 / 100.0f, 0.5f);
	if (shaderIndx == 3) {
		s->SetUniform1f("fogDensity",fogDensity);
		s->SetUniform1i("showFog", showFog ? 1 : 0);

	}
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
	if (shaderIndx == 6) {
		Eigen::Vector3d pos = data_list[shapeIndx]->MakeTransd().col(3).head(3);
		sigmaBlur = abs(GetCameraPosition().cast<float>()[2] - pos.cast<float>()[2]);
		sigmaBlur = sigmaBlur > 4.0f ?  1.0f + sigmaBlur / 5.0f : 1.0f;
		s->SetUniform1f("sigma", sigmaBlur);
	
	}

	//Move only selected objects according to the bezier curve.  
	if (_bezierObjectCount > 0 && std::find(pShapes.begin(), pShapes.end(), shapeIndx) != pShapes.end()) {
		std::cout << shapeIndx << std::endl;
		BezierMove* bezier = new BezierMove(this, shapeIndx, bezierControlPoints, Proj, View, Model);
		g_bezierObjects.push_back(bezier);

		_bezierObjectCount--;
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
		for (auto currBezierObj : g_bezierObjects) {
			if (!currBezierObj->getHasDoneMoving()) {
				data_list[currBezierObj->GetObjectId()]->SetTranslation(currBezierObj->GetNextMove().cast<double>()); 
			}
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
