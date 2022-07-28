# 3D scene editor
#### This is our final project that was done for the computer graphics class BGU.

# Features

## Menu Features
#### Animation  Options

- choose bezier curve for any object in the scene. 

Every object can have it's own uniquely generated curve in 3d space. The curve itself is choosen by the user via a 2d drawing in the right pane & is fully customizable. 
In total there are 4 control points that the user can control via his mouse.

- remove a bezier curve from an object - 
You can clear a bezier curve that was uniquely choosen before for a specific object.

- choose a bezier curve for the camera -
The user can choose a unique bezier path for the animation camera in the same way as for objects.

- remove the bezier curve from the camera - 
You can clear a bezier curve that was uniquely choosen before for the camera.

- blur motion option -  (**Bonus feature**)
This is a bonus option. It blures our motion whenever the camera is being moved via the bezier curve to give a unique "movement" effect.

- time slider -
Define how much time after the start button is hit the animation will start running.

- start animation button - switch to animation camera, and after the "time slider" user defined time has passed each object will move according  to it's bezier curve that was previously choosen by the user. The generated curve is shown in the 3d world with the help of layout edges.  (Both for camera & objects)

- pause animation button - pause the animation , the animation can be later continued from the exact same place that it was paused at.The animation camera is still used to view the scene. 

- stop animation button - stop the animation switch back to design mode camera, when pressing start the objects will start their movement from the beginning of their curve.

#### Mesh Options
- load a new oject from file , the object will appear at (0,0,0)
#### Theme Options
- choose between 4 different themes (cubemaps) - Daylight,Ocean,River and Mountains
- **(Bonus)** Fog - 
- Add fog to the scene and choose it's density - the fog density is also determinated by the camera zoom (the closer we get the less fog we will see)

#### Viewing Options
- Fov slider 
- Orthographic view 
#### Cameras Options
##### add/set camera
- add a new camera - each camera has it own name and can be choose by the user.
- set camera - choose the camera for the main scene - you can change your camera to any of the cameras that exist and that were added by the user. 
- set camera for animation- choose the camera for the animation - you can change your camera to any of the cameras that exist and that were added by the user. 
- set camera for right screen - The user can choose the camera that will be used in the right screen  (for split x option explained later).  
- set camera for up screen -  The user can choose the camera that will be used in the up screen  (for split y option explained later).  

This allows the user to view the scene from different angles - even during animation !
##### Camera movment
- choose path - Choose bezier path for each camera, this is done by adding "3" spheres to the 3d world that the user can move with his mouse . This spheres are the "control" points of the bezier curve in 3d space. Each camera can have it's own fully unique path & is not dependent on any of the other cameras.
- set path - set the choosen path according to the position of the 3 balls .
- move bezier - move the camera according to the choosen path - the path curve is drawn on screen. This movement has the blured motion (Bonus) feature
#### Screen Options
- zoom area - zoom in to a choosen area and zoom out to a default state - this feature was done using scissor test and unprojecting screen coordinated to world coordinated.
- split x - render the same scene in the right screen, the screen can have it's own camera. 
- split y -  render the same scene in the up screen, the screen can have it's own camera.
#### Objects Options
##### Material options
- add material - add a new material from file 
- set material - set the material for all choosen objects
##### Blur options
- (**Bonus Feature**) make blur -make all choosen objects blurred - the blur intensity is determinated by the distance of the object from the camera (the closer the camera gets it is less blurred) , You can also blur the right screen upon split X or blur the up screen upon split Y.
- remove blur - unblur all choosen objects / unblur right screen / unblur up screen.
##### Transparent options
- make transparent - make all choosen objects transparent
- remove transparency- remove transparency from all choosen objects
#### Layers
- add layer - add a new layer ,each layer has a name 
- remove layer - remove a layer from the list, all obects in that layer will be moved to the defualt layer 
- set layer - change the layer for all choosen objects

## Keyboard Features
#### Mouse controls
##### Zoom area is off
- click an object with the left mouse button to rotate an objects
- click an object with the right mouse button to move an objects
- click and drag with the left mouse button to select multi object to rotate
- click and drag with the right mouse button to select multi object to move
- on mouse scroll if there are picked objects -change their size, otherwise zoom in/out.
All choosen objects are highlighted using stencil test
##### Zoom area is on
- click and drag with the any mouse button to select the area to zoom into
#### Keyboard keys
- U,D,R,L - to move the camera 
- UP,DOWN,RIGHT,LEFT - to rotate the camera
- S - to switch cameras in main scene
- C - to move camera control to the split screens (split x and split y options)  
** when we move control - all keyboard operations on a camera will be done on the camera of the screen in control 

# Shaders











