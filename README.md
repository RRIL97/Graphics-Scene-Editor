# 3D scene editor
#### This is the final project done for Computer graphics class.

# Features

## Menu Features
#### Animation  Options
- choose a bezier curve for every objects, each object can have it own curve
- remove a bezier curve from an objects
- choose a bezier curve for the camera
- remove the bezier curve from the camera
- blur motion option - when choosen ??
- time slider - define how much time after the start button is hit the animation will start running
- start animation button - after the time from the time slider is passed , each object that had a curve choosen will start to move according to that curve, the objects curve is drawn on the screen.
- pause animation button -pause the animation , the animation can be later continued from the exact same place it was paused
- stop animation button -stop the animation , when pressing start the objects will start their movement from the beginning of their curve
#### Mesh Options
- load a new oject from file , the object will appear at (0,0,0)
#### Theme Options
- choose between 4 different themes - Daylight,Ocean,River and Mountains
- add a fog to the scene and choose it's density - the fog density is also determinated by the camera zoom (the closer we get the less fog we will see)

#### Viewing Options
- Fov slider 
- Orthographic view 
#### Cameras Options
##### add/set camera
- add a new camera - each camera has it own name 
- set camera - choose the camera for the main scene  
- set camera for right screen - choose the camera for the right screen (for split x option explained later)  
- set camera for up screen- choose the camera for the up screen (for split y option explained later)  
This allows the user to view the scene for different angles - even during animation !
##### Camera movment
- choose path - when pressed 3 balls will appear on screen.Those balls can be used to set the camera movement path.
- set path - set the choosen path according to the position of the 3 balls .
- move bezier - move the camera according to the choosen path - the path curve is drawn on screen.This movement has the blured motion feature
#### Screen Options
- zoom area - zoom in to a choosen area and zoom out to a default state - this feature was done using scissor test and unprojecting screen coordinated to world coordinated.
- split x - render the same scene in the right screen, the screen can have it's own camera. 
- split y -  render the same scene in the up screen, the screen can have it's own camera.
#### Objects Options
##### Material options
- add material - add a new material from file 
- set material - set the material for all choosen objects
##### Blur options
- make blur -make all choosen objects blurred - the blur intensity is determinated by the distance of the object from the camera (the closer the camera gets it is less blurred)
- remove blur - unblur all choosen objects
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











