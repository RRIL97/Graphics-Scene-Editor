#include "game.h" 
#include <time.h>
#include <iostream>
#include "igl/unproject.h"
#include "igl/project.h"
#include <GLFW/glfw3.h>
#include "ObjectMover.h"
#include "CameraMover.h"; 

 
std::vector <Eigen::Vector2f> bezierControlPoints; 

const float g_pointRadius    = 3.0f; 
const float g_distanceClickControlThreshold = 10.0f;
int         g_chosenControlPoint = 0;

std::vector<ObjectMover*> g_bezierObjects;
std::vector<CameraMover*> g_cameraMovers;

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
	m_time = 0;
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
	unsigned int texIDs[9] = {  0, 1, 2, 3, 4 , 5, 6, 7, 8};
	unsigned int slots[9] = {  0, 1, 2, 3, 4 , 5, 6, 7, 8 };

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
	CreateTex(100, 100);
	CreateTex(150, 150);
	CreateTex(300, 300);



	AddMaterial(texIDs, slots, 1);
	AddMaterial(texIDs + 1, slots + 1, 1);
	AddMaterial(texIDs + 2, slots + 2, 1);
	AddMaterial(texIDs + 3, slots + 3, 1);
	AddMaterial(texIDs + 4, slots + 4, 1, " box");
	AddMaterial(texIDs + 5, slots + 5, 1, "some");
	AddMaterial(texIDs + 6, slots + 6, 1, "cameraBeizer1",false);
	AddMaterial(texIDs + 7, slots + 7, 1, "cameraBeizer2", false);
	AddMaterial(texIDs + 8, slots + 8, 1, "cameraBeizer3", false);
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
	bezierControlPoints.push_back(Eigen::Vector2f(1250.0, 550.0));
	bezierControlPoints.push_back(Eigen::Vector2f(1450.0, 750.0));
	pickedShape = 0;
	//split x
	AddShape(Plane, -1, TRIANGLES, 5);
	splitXPlaneIndx = 6;
	SetShapeShader(splitXPlaneIndx, 2);
	ShapeTransformation(xRotate, 3.14159265, 0);
	ShapeTransformation(yRotate, 3.14159265, 0);
	SetShapeStatic(splitXPlaneIndx);
	//split y
	 AddShape(Plane, -1, TRIANGLES, 6);
	 splitYPlaneIndx = 7;
	 SetShapeShader(splitYPlaneIndx, 2);
	 ShapeTransformation(xRotate, 3.14159265, 0);
	 ShapeTransformation(yRotate, 3.14159265, 0);
	 SetShapeStatic(splitYPlaneIndx);
    
	 //camera bezier path 
	 cameraPathStartIndx = 8;
	 for (int i = 0; i < 3; i++) {
		 AddShape(Sphere, -1, TRIANGLES);
		 SetShapeMaterial(cameraPathStartIndx + i, 6 +i);
		 data()->layer = 0;
	 }


	 //Will hold the actual bezier curve drawing
	 AddShape(Axis, -1, TRIANGLES, 0); 
 	 data_list[11]->clear();	 

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
    bezierControlPoints[g_chosenControlPoint] = Eigen::Vector2f(x, 800 - y);

}

Eigen::Matrix4d Game::getTranslateRes(Eigen::Vector3d amt, bool translation)
{
	if (translation)
		ToutCopy.pretranslate(amt);
	else {
		Eigen::Vector3d oldPositionOfObject = (ToutCopy * TinCopy).matrix().block(0, 3, 3, 1);
		ToutCopy.pretranslate(amt - oldPositionOfObject);
	}

	Eigen::Matrix4d mat = Eigen::Matrix4d::Identity();
	mat.col(3) << TinCopy.translation(), 1;

	return (ToutCopy.matrix() * mat);
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
	s->SetUniform1i("time", m_time);
	if (data_list[shapeIndx]->GetMaterial() >= 0 && !materials.empty())
	{ 
		BindMaterial(s, data_list[shapeIndx]->GetMaterial());
	} 
	if (shaderIndx == 0 )
		s->SetUniform4f("lightColor", r / 255.0f, g / 255.0f, b / 255.0f, 0.0f);
	else if(shaderIndx == 7 )
		s->SetUniform4f("lightColor", 0.5f, 0.6f, 0.7f, 0.5f);
	else
		s->SetUniform4f("lightColor", 4/100.0f, 6 / 100.0f, 99 / 100.0f, 0.5f);
	if (shaderIndx == 3) {
		s->SetUniform1f("fogDensity",fogDensity);
		s->SetUniform1i("showFog", showFog ? 1 : 0);

		s->SetUniform1f("performBlurMotion", blurMotion);
		s->SetUniform1f("sigma", blurSigma);

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
		ObjectMover* bezier = new ObjectMover(this, shapeIndx, bezierControlPoints);
		g_bezierObjects.push_back(bezier); 
		_bezierObjectCount--;
	}  
	if (shapeIndx == 11) {
		if (startDrawBezierCurve) {
			for (auto currBezierObj : g_bezierObjects) {

				auto allMoves = currBezierObj->GetAllMoves();
				Eigen::RowVector3d  posBefore = data_list[currBezierObj->GetObjectId()]->MakeTransd().col(3).head(3);
				ToutCopy = data_list[currBezierObj->GetObjectId()]->getTout();
				TinCopy = data_list[currBezierObj->GetObjectId()]->getTin();
				for (int i = 0; i < allMoves.size(); i++) {
					Eigen::RowVector3d posAfter = getTranslateRes(allMoves[i].cast<double>(), true).col(3).head(3);
					data_list[11]->add_edges(posBefore, posAfter, posAfter - posAfter);
					posBefore = posAfter;
				}
			}
			startDrawBezierCurve = false;

		}
		if (startDrawBezierCurveCamera) {
			for (int i = 0; i < g_cameraMovers.size(); i++) {
				auto currentMover = g_cameraMovers[i];
				auto allMoves = currentMover->GetAllMoves();
				Eigen::RowVector3d  posBefore = currCamera->MakeTransd().col(3).head(3);
				ToutCopy = currCamera->getTout();
				TinCopy = currCamera->getTin();
				for (int i = 0; i < allMoves.size(); i++) {
					Eigen::RowVector3d posAfter = getTranslateRes(allMoves[i].cast<double>(), false).col(3).head(3);
					data_list[11]->add_edges(posBefore, posAfter, posAfter - posAfter);
					posBefore = posAfter;
				}
			}
			startDrawBezierCurveCamera = false;
		}
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
	startDrawBezierCurveCamera = true;
	for (int i = 0;i < g_bezierObjects.size(); i ++) {
		startDrawBezierCurveCamera = false;
			auto currBezierObj = g_bezierObjects[i];
			if (time(NULL) - playAnimationMiliTime >= animationDelay) {
				if (!currBezierObj->getHasDoneMoving() && !stopAnimation) {  
					auto nextMove = currBezierObj->GetNextMove().cast<double>(); 
					data_list[currBezierObj->GetObjectId()]->MyTranslate(nextMove,true); 
				}
				else {
					if (!stopAnimation) {
						currBezierObj->CalculateBezierMoves(); 
					    data_list[11]->clear();
					}
					else { 
						data_list[11]->clear(); 
						g_bezierObjects.clear();
					}
				}
			}
			else {
		 
			}
		}

		if (moveCameraBezier) {
			startDrawBezierCurve = true;
			std::vector<Eigen::Vector3d> cameraPath = camerasPaths.find(currCamera->name)->second;
			std::vector<Eigen::Vector3f> cameraPathFloat;
			for (int i = 0; i < cameraPath.size(); i++)
				cameraPathFloat.push_back(cameraPath[i].cast<float>());

			CameraMover* cameraMover = new CameraMover(this, cameraPathFloat);

			g_cameraMovers.push_back(cameraMover);
			moveCameraBezier = false;
		}
		if (!blurMotion) {
			blurMotion = true; 
		}
		for (int i = 0; i < g_cameraMovers.size(); i++) {
			auto currentMover = g_cameraMovers[i];
			 
			if (!currentMover->getHasDoneMoving()) { 
				currCamera->SetTranslation(currentMover->GetNextMove().cast<double>()); 
				blurSigma += 0.01;
			}
			else {
				//we are done remove it
				auto entry = g_cameraMovers[i];
				g_cameraMovers.erase(g_cameraMovers.begin() + i);
				startDrawBezierCurveCamera = false;
				blurMotion = false;
				blurSigma = 0.5;
				delete entry;
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
