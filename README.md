![GitHub language count](https://img.shields.io/github/languages/count/dkhor2003/3D_Modeler)
![GitHub top language](https://img.shields.io/github/languages/top/dkhor2003/3D_Modeler?color=yellow)

## 3D Modeler
This program serves as an interface for users to design a 3D model. Users can choose to create a hollow 3D object or a terrain object, and edit its color, texture, resolution, and shape to their liking. Once the user is done editing their model, they can inspect their model by viewing the model in different directions using the built-in camera. If the user is satisfied with their created model, they can export their model into an obj file, and load it into a different graphics application (e.g. Blender, SolidWorks) to be used. 

## Program Demo
https://github.com/dkhor2003/3D_Modeler/assets/120704027/c5b1d627-fe4f-4688-9df3-a9740dc861a6 

The full demo and details are available [here](https://www.youtube.com/watch?v=WUpp72sRBaQ).

## Compiling the program

Compile the shader programs:

    make -f .\Makefile-win shader

Compile the `.exe` program:

    make -f .\Makefile-win

Start the interface:

    .\3D_Modeler.exe

## Program Usage

### __Modeling :vs: Viewing Mode__
There are __2__ modes of interacting with the program: modeling and viewing mode, in which you can switch between the two modes using the `SPACE` key. 

1. **Modeling mode** (Mode for creating and editing the 3D model)
            
    - There are two types of objects that can be created in this interface: hollow or terrain objects. You can choose the model type to be created by hitting `TAB` key. 
            
        - **Hollow objects** &rarr; When modeling hollow objects, users can click on the top half of the window screen to render a point, which serves as a control point for constructing the bezier curve. The points on the bezier curve can then be used to generate a hollow object by revolving them along the center line. Each succesive control points generated will be connected by a line. When three or more points are generated, the bezier curve will be presented to give a guideline for how the surface of the hollow object will look like.
            
        - **Terrain objects** &rarr; When modeling terrain objects, users need to render a certain number of points in order to construct the bezier surface. This number is determined by the u- and v-degree, which users can choose. The number of points needed is $(u + 1) \times (v + 1)$. 

    - In either modeling mode, use mouse to click on the window screen to render a point, which serves as a control point for constructing the bezier curve/surface. Note that adding another control point when modeling a terrain object will hide the surface of the object (if the surface is shown) because the number of control points needed must satisfy $(u + 1) \times (v + 1)$ in order to generate a surface. 
        
    - There are two modes of editing model shape, in which users can switch between them by hitting the `ENTER` key:
            
        - **Moving control points** &rarr; Click and drag on existing control points to move their position. When a control point is moved, the surface of the model will change in real time for users to let users edit their models to their liking. Note that you cannot move a control point below the center line when modeling hollow objects. 
            
        - **Removing control points** &rarr; Click on an existing control point to remove it. This is implemented for users to remove control points that are unnecessary. When a control point is removed, the surface of the model will change accordingly as well. Note that removing a control point when modeling a terrain object will hide the surface of the object (if the surface is shown) because the number of control points needed must satisfy $(u + 1) \times (v + 1)$ in order to generate a surface. 

2. **<u>Viewing mode</u>** (Mode for inspecting the 3D model by controlling the camera)
    
    - When entering viewing mode, the model will first be centered on the screen. There are 5 main controls for manipulating the cemera:
        
        - **Fit All** &rarr; Hit `F` key to move to center the scene on the window so that the whole scene can be viewed clearly while maintaining its rotation components. 
        - **Rotate** &rarr; Hold `R` key while dragging the mouse to rotate the scene around the axis perpendicular to the drag direction.
        - **Pan** &rarr; Hold `P` key while dragging the mouse to move the entire scene towards the drag direction.
        - **Zoom** &rarr; Hold `Z` key while dragging the mouse up and down the window to zoom in or out the scene respectively.
        - **Twist** &rarr; Hold `T` key while dragging the mouse in the direction you want to rotate the scene around the z-axis. 

    - When entering viewing mode while modeling terrain objects, users can manipulate the depth of control points by using the &uarr; or &darr; key to increase or decrease the depth attribute of the selected control point. The selected control point will be colored &#128994;, and users can switch to the next or previous control point by using the &rarr; or &larr; key, respectively.  

### :black_square_button: Utility Buttons
There are also a set of utility buttons in which users can use to modify the color, resolution, or texture of their models. In addition, while modeling terrain objects, there are additional buttons for users to increase or decrease the u- and v-degree. Users can only use these buttons, when they are shown on the screen. To show/unshow the buttons, hit the `U` key. These buttons are only shown in modeling mode, any attempt to show these buttons in viewing mode will force you to go into modeling mode. 
    
1. **Color Buttons** &rarr; There are a set of colored buttons on the bottom of the screen, clicking on any color buttons will apply the color on your created model. 

2. **Arrow Buttons** &rarr; There is an :arrow_up: and :arrow_down: button on the bottom right of the screen. Hitting the :arrow_up: button will increase the resolution of your model, and hitting the :arrow_down: button will decrease the resolution of your model. The maximum resolution that can be achieved is 100 and the minimum resolution that can be achieved is 3. 

3. **Texture Buttons** &rarr; There are 4 texture buttons (buttons with fancy image mapped to them), one of which is a button (the button with "No Texture" on it) for removing the texture on the model. Otherwise, clicking on other texture buttons will apply the image on the button onto the model. 

4. **Range Buttons** (only available when modeling terrain objects) &rarr; There are two range buttons, one for the u-degree and one for the v-degree. Each range has a :heavy_plus_sign: and :heavy_minus_sign: button associated with it to increase or decrease the corresponding degree. U-degree range has an `X` label for it and v-degree has a `Y` label for it. The number of degree will be determined by the number of white strips. 

### :exclamation: Surface Operations
- Hit `B` key to hide/show the surface generated. Note that, while modeling terrain objects, the surface will not be shown if the number of control points does not satisfy $(u + 1) \times (v + 1)$. 
- Hit `N` key to hide/show the normal vectors of the generated surface. 

### :bulb: Light Manipulation
Hit the `1` or `2` key to rotate the light source clockwise or counterclockwise around your created model for better lighting on your desired view direction. 

### :radio_button: Saving and Loading Models
- Once you are satisfied with your created model, use `CTRL + S` keys to save your model into the `models` directory as an obj file. A prompt will appear on the console for you to name your model, which then saves your model as `"modelName".obj` under the `models` directory. 
- Hit `C` to check a model that you saved. This will prompt you to specify a model's name to load from the `models` directory. Once loaded, the model will be displayed on your screen in viewing mode for you to check. Going back to modeling mode will disable this model checking mode. 
- Here is a demo of how to load a saved `obj` file into another 3D graphics software: **Blender**:

https://github.com/dkhor2003/3D_Modeler/assets/120704027/63621aab-5716-4a71-8e73-c9c3792812a9 

### :information_source: Other Features
- Hit `M` key to clear everything (control points, lines, bezier curve, surface, normal vectors) on the interface. 
- Hit `A` key to switch between perspective :left_right_arrow: orthographics perspectives. 
- Hit `ESC` key to quit the program.

## :warning: Current Limitations of the Program 
- Utility buttons do not look aesthetic and the placement of the utility buttons are not the most optimal. 
- Resizing the original window makes control point selection inaccurate. 
- Saved models' `obj` files do not contain information about its color and loaded models do not contain the texture they are saved from. 

