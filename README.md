# wedding_rover
![](https://github.com/aoraoraor/wedding_rover/blob/master/images/img0.png)

This personal project is for practicing some concepts and frameworks including QML, Qt3D, earth map rendering, model loading, ... etc.

The main idea of this application is to render a 3D earth (height) map, and do some simple animation logic for moving models. The animation idea here is to move a rover to a groom (bachelor) and deliver him to his wedding ceremony.

This project focus will be on the visualizer part (the pathplanner project will be used as just a helper library).

## Prerequisites

This application depends on Qt5 (5.14.0) libraries.

To compile and run this application, make sure you follow the following steps:
1- Install Qt 5.14.0 on your system
2- Refer to the Qt libraries in your Path environment variable, basically the following paths are needed by the application:
${QT_DIR}
${QT_DIR}/bin
${QT_DIR}/qml
${QT_DIR}/plugins

You can then use the qt windows or linux deployment tool "i.e. for windows: windeployqt" for grabbing all qt libraries needed by this application to your release package.

## User Guide

On application startup, it initially loads the following:
1- 3D height map, rendered based on the data specified in assets\elevation.data
2- 3D models for the rover car, bachelor and the target wedding house

Initial loading of the application will look like the following screenshot, having the loaded models on the right side of the screen:
![](https://github.com/aoraoraor/wedding_rover/blob/master/images/img1.png)

On the left side, you’ll find initial positions for all the animation targets (rover, bachelor and wedding), also initial steps are 0 since nothing has moved so far.

Playback buttons will be initially disabled until you perform the first step mentioned below.

You can use your mouse left and right buttons to move around the scene, and the mouse wheel for zooming in/out.

#### Calculating solution

First step is to click on the calculate solution button

![](https://github.com/aoraoraor/wedding_rover/blob/master/images/img2.png)

The application will then run the provided algorithm and render the resultant solution path in RED between the rover, groom and the wedding destination.

It will look like the following:

![](https://github.com/aoraoraor/wedding_rover/blob/master/images/img3.png)

So now you’ll find the playback buttons got enabled and from here you can proceed.

#### Playback controls
![](https://github.com/aoraoraor/wedding_rover/blob/master/images/img4.png)
![](https://github.com/aoraoraor/wedding_rover/blob/master/images/img5.png)

The play button is responsible for toggling (play, pause) the moving action that should move the rover and the bachelor along their paths.

The stop button stops the movement actions and resets the models to get back to their initial positions.

The fast forward button skips 100 forward steps.

#### Animation speed
![](https://github.com/aoraoraor/wedding_rover/blob/master/images/img6.png)

You can move this slider right and left, to reflect fast and slow movements (respectively).

#### Info window
![](https://github.com/aoraoraor/wedding_rover/blob/master/images/img7.png)

This window will get updated on each step move with the new coordinates and the number of steps consumed so far.

## Application Design
Alhought the problem has simple components, I tried to elaborate an idea of extendibility assuming it can be used in adding more features later and drawing more components. 

Accordingly I tried to respect some design principles and patterns.

The following is a basic simple diagram that can show and idea about the application architecture:

![](https://github.com/aoraoraor/wedding_rover/blob/master/images/img8.png)

#### Scene components

Each drawable component is implemented in its own class to handle its relevant drawing logic, all those components share the same interface in which is called here “Renderer”.

All renderers use Qt3D C++ libraries for drawing. They just draw their own shapes and attach their entity to the main scene root entity.

###### HeightMapRenderer

Responsible for drawing the height map based on the provided color map and elevation data. It creates triangles for each point on the map to visualize the 3D map in a smooth manner.

###### ModelRenderer

Gets instantiated once for each 3D model that should be loaded, it loads models using QSceneLoader and places the model at its provided initial location.

In this solution, we have only 3 instances of this ModelRenderer for: rover, bachelor and wedding.

###### PathRenderer

Gets a list of coordinates that represent a complete path, then draws a point for each coordinate with an elevation higher than the height map to visualize the path clearly.

Here we have 2 instances of the PathRenderer, one of the rover path and one for the bachelor path.

#### Animation components
###### Moveable

The idea here is to separate the drawing logic from the component animation logic, accordingly, a new simple interface is introduced called “Moveable”, this should be inherited by any renderer that is interested in being moved.

In our case, we have some static components in our scene like the path and the 3D height map, and some other components that are not static and should be moveable like the rover and the bachelor models. So ModelRenderer class implements this interface.

###### ModelMover

The model mover is responsible for all movement logic on the Moveable components, so it has implementations for functionalities like: moveForward(), fastForward(), reset()

###### Animator

This has our update loop, and handles qml UI requests in terms of play, pause, stop, .. etc.
Also it is responsible for updating UI with the required data for models positions and steps performed.

###### Main Window

The main window here is very simple, it just creates and initializes the qml view and qt3d view.

###### Scene Creator

Responsible for constructing all parts needed for the scene, it should be the only component that understands how the scene should be constructed and what are the required steps of creation.

For example, it only creates the path after the solution gets calculated.

## Further Challenges

Unfortunately, I have faced some challenges in implementing this idea in the most optimized way, and there is no enough good online documentation and resources for Qt3D that can support in this problem described below, so I had to think of a workaround in which of course is not a good option, I will keep this workaround here until I find a solution later and fix it.

#### Path rendering problem

At first, I though I should render the path only after I calculate the solution, so the steps should be as follows:
1- Draw the whole scene that should include the height map, the models and display their initial positions
2- User clicks on Calculate
3- Draw the resultant path in terms of simple point primitives that just has a coordinate and a color

The problem here is that after performing steps 1 and 2 (drawing on the qt3d window and displaying its view), the qt3d will always crash on performing step 3, it does not allow me to draw any primitives after displaying the qt3d view.

The only way to succeed in drawing point primitives was to draw them before displaying the view, because after that it will crash.

I tried drawing other models after displaying the view like any 3D model or Qt3DMeshes and it works fine.

So I had the following workarounds that I had to choose from:
1- Calculate the solution, then draw the whole scene including the resultant path all at once before displaying the view.
But this will violate the initial idea, and there has to be a solution that makes more sense than just chaning the idea.
2- Draw the resultant path using Qt3DSphereMesh after displaying the view.
As mentioned above, meshes can appear without issues after displaying the main qt3d view, although this solution is not the best at all since it loads a lot of a very heavy model while all what I needed was just a simple point with color, but I had to do it like that because I did not find another solution at the time I was working on it.

As another workaround for the heavy model problem, I did not draw spheres over all the provided path coordinates, instead, I draw only 1 sphere every 3 coordinates, this can help make the final solution a little more lighter.

## References

I have learned from and made use of the Qt5.14.0 qt3d qml examples to construct my qml view.
3D models are downloaded from the free downloads of this website:
https://free3d.com/
