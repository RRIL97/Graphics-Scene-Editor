# 3D scene editor
#### This is our final project that was done for the computer graphics class BGU. 
The projects files are under the game directory.

# Features

## Menu Features
#### Animation  Options

- choose bezier curve for any object in the scene - every object can have it's own uniquely generated curve in 3d space. The curve itself is choosen by the user via a 2d drawing in the right pane & is fully customizable. 
In total there are 4 control points that the user can control via his mouse.

- remove a bezier curve from an object - 
You can clear a bezier curve that was uniquely choosen before for a specific object.

- choose a bezier curve for the camera -
The user can choose a unique bezier path for the animation camera in the same way as for objects.

- remove the bezier curve from the camera - 
You can clear a bezier curve that was uniquely choosen before for the camera.

- **blur motion option (Bonus feature)**
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
##### Camera movment (**Bonus feature**)
- choose path - another way to choose a bezier path for the camera (in addition to the animation option above), this is done by adding "3" spheres to the 3d world that  the user can move with his mouse . This spheres are the "control" points of the bezier curve in 3d space. Each camera can have it's own fully unique path & is not  dependent on any of the other cameras.
- set path - set the choosen path according to the position of the 3 balls .
- move bezier - move the camera according to the choosen path - the path curve is drawn on screen. This movement has the blured motion (Bonus) feature.
#### Screen Options
- zoom area - zoom in to a choosen area and zoom out to a default state - this feature was done using a scissor test and unprojecting screen coordinated to world coordinates.
##### split x 
- when choosen render the same scene in the right screen, the screen can have it's own camera.
- **Bloom effect (Bonus feature)**- Bloom effect on the right screen that is achieved via the standard method of Bloom that is described in opengl.
- **Blur (Bonus feature)**- blur the scene in the right screen, the blur intensity is determinated by the objects distance from the camera (the closer the camera gets they become less blur and vice versa)
##### split y 
-  render the same scene in the up screen, the screen can have it's own camera.
-  **Bloom effect (Bonus feature)**- Bloom effect on the up screen that is achieved via the standard method of Bloom that is described in opengl.
-  **Blur (Bonus feature)**- blur the scene in the up screen, the blur intensity is determinated by the objects distance from the camera (the closer the camera gets they become less blur and vice versa)
#### Objects Options
##### Material options
- add material - add a new material from a file.
- set material - set the material for all choosen objects.
##### Blur options (**Bonus Feature**)
-  make blur -make all choosen objects blurred - the blur intensity is determinated by the distance of the object from the camera (the closer the camera gets it is less blurred).
- remove blur - unblur all choosen objects.
##### Transparent options (**Bonus Feature**)
- make transparent - make all choosen objects transparent
- remove transparency- remove transparency from all choosen objects
##### Bloom options (**Bonus Feature**)
- make Bloom - render bloom effect on all choosen objects. 
- remove Bloom- remove the bloom effect from all choosen objects.
- Intensity - choose the bloom effect intensity.  
#### Layers
- add a layer - add a new layer ,each layer has a name.
- remove a layer - remove a layer from the list, all objects in that layer will be moved to the defualt layer which can't be removed. 
- set layer - change the layer for all choosen objects.

## Keyboard Features
#### Mouse controls
##### Zoom area is off
- click an object with the left mouse button to rotate an object.
- click an object with the right mouse button to move an object.
- click and drag with the left mouse button to select multi objects to rotate.
- click and drag with the right mouse button to select multi objects to move.
- on mouse scroll if there are picked objects -change their size, otherwise zoom in or out.
All choosen objects are highlighted using stencil test.
##### Zoom area is on
- click and drag with the any mouse button to select the area to zoom into.
#### Keyboard keys
- U,D,R,L - to move the camera. 
- arrows UP,DOWN,RIGHT,LEFT - to rotate the camera.
- S - to switch cameras in main scene - also works during animation .
- C - to move camera control to another screen, used to move control from main screen to the split screens (split x and split y options) and back.
 when we move control - all keyboard operations on a camera will be done on the camera of the screen in control.

# Shaders
we have used the following shaders:
- pickingShader - for the scissors test
- basicShader - defualt shader for every object in the scene.
- basicShader2 - used to render the scene to  the split screens, also contains the bloom effect for the scene (split screens).
- cubemapShader - for the theme
- bezierShader - for rendring the 2D bezier curve.
- blurShader - for blur objects option.
- bloomShader - for bloom effect on objects.
- blurShaderScene - for blur option on scene (split screens).











