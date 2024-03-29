# Comp3015 CW-2

For CW1 info head here - https://github.com/morgangorle/Comp3015-Lab-CW/tree/CW1
## Links

Github Page: https://github.com/morgangorle/Comp3015-Lab-CW

Demonstration Video: https://www.youtube.com/watch?v=2UIXZhPLTwg

## Development Environment

* Developed on Windows 10

* Developed using a mixture of Visual Studio 2019 Platform toolset 142 and Visual Studio 2020 Platform toolset 143.

## Current Functionality

Currently this program renders a chest with a 3D texture rendered onto it alongside a skybox, a plane with a texture alongside some fog.

The program also has an edge detection function which switches it to black and white, to change it back to colour again, comment out the edge detection function in the render loop and also comment out the binding to the frame buffer in the renderScene function and the first GLCLear function in the Render scene, failing to do that will cause the Skybox not to function. Commenting out the binding to the frame buffer is important as only doing the commenting out the edge detection function will cause mass screen flickering.

To accomplish this I ended up making various different versions of a Blinnphong shading to suit my needs in the moment and using a uniform variable to assist me in ensuring the right one is called.

## Known Problems

* The fog currently appears more like a spotlight on the edges of the view of the camera as opposed to looking like actual fog. There's two reasons for this.

- Firstly the lack of an object being placed within the fog ends up making it hard to demonstrate its effects, I talk more about my difficulties in rendering multiple objects in the CW1 video but it was definitely unfortunate.

- Secondly the Fog doesn't effect the Skybox in its current implementation which is unfortunate.

* Minor style clash between how I've styled the rest of the scene and my sky-box.

- This mostly came about due to me rushing to find a sky-box and all the ones I found which were free ended up being hyper realistic which led to this clash.

* The Edge detection function currently has a box around the edge

* When commenting out the edge detection function to switch back to regular mode there's a line in the scene rendering which also has to be commented out to prevent mass flickering.

## Usage

Shaders are located within the shader folder, with the file types of them being indicative of being the fragment or vertex shader.

The vertex shading is relatively simple within this project with the main notable thing being a slight change in functionality when the passNum is 2 to accomodate for the needs of the Skybox

The Fragment shader has a lot more technicalities on this side with multiple variations depending on what the passNum variable is set to, this is to accomodate for the differing shading that different objects need be it the skyBox, Edge detection function, the plane or the chest all of them use slightly different shading which is accounted for.

Meanwhile in the scenebasic_uniform.cpp file I handle rendering the objects and setting all of the uniforms and plenty of other items.




