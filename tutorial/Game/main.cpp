#include "igl/opengl/glfw/renderer.h"
#include "game.h"

#include "InputManager.h"

int main(int argc,char *argv[])
{
	const int DISPLAY_WIDTH = 1600;
	const int DISPLAY_HEIGHT = 800;
	const float CAMERA_ANGLE = 45.0f;
	const float NEAR = 1.0f;
	const float FAR = 120.0f;
	const int infoIndx = 2; 
	std::list<int> x, y;
	x.push_back(DISPLAY_WIDTH/2);
	x.push_back(DISPLAY_WIDTH);
	y.push_back(DISPLAY_HEIGHT);
    Display disp = Display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "OPENGL");
    igl::opengl::glfw::imgui::ImGuiMenu* menu = new igl::opengl::glfw::imgui::ImGuiMenu();
    Renderer* rndr = new Renderer(CAMERA_ANGLE, (float)DISPLAY_WIDTH/(float)DISPLAY_HEIGHT/2, NEAR, FAR);
	Game *scn = new Game();  //initializing scene
	
    Init(disp,menu); //adding callback functions
	scn->Init();    //adding shaders, textures, shapes to scene
    rndr->Init(scn,x,y,1,menu); // adding scene and viewports to the renderer
	rndr->AddCamera(Eigen::Vector3d(0, 0, 1), 0, 1, 1, 10, 2);//adding camera
	rndr->AddViewport(0, 0, DISPLAY_WIDTH / 2, DISPLAY_HEIGHT); //add viewport for plane
	//blending
	rndr->AddDraw(2, 0, 0, 0, rndr->inAction2 | rndr->scissorTest | rndr->blend );
	//line 
	rndr->AddViewport(0, 0, DISPLAY_WIDTH / 2, DISPLAY_HEIGHT); //add viewport for picking shape

	//rndr->AddDraw(3, 0, 4, 0, rndr->stencilTest| rndr->depthTest | rndr->stencil2 | rndr->scaleAbit | rndr->inAction2 |rndr->onPicking);
	//rndr->AddDraw(2, 0, 4, 0, rndr->stencilTest | rndr->inAction2 | rndr->depthTest);
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);   glClearColor(0.0, 0.0, 0.0, 0.0);
	disp.SetRenderer(rndr);
    disp.launch_rendering(rndr);
	 

	delete scn;
	delete rndr;
	delete menu;
	
	return 0;
}
