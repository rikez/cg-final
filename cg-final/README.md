# Computer Graphics Final

The Computer Graphics class presented plenty of challenges over the semester, requiring knowledge about *Transformtions*, *Cameras*, *Lighting*, *Modelling*, *Animating*, etc...

At the end, the students had to show their knowledge and coding performance by applying those concepts mentioned above in a project of free theme choice.

## Solution

Spent a couple of nights trying to build something cool, I have built a lot of dynamic cubes with a wood box texture, all over the plan. To complement, a decided to explore the lighting effects on that boxes, using the *direcional* , *point of lights* and *spotlight* at the same scene.

### Point of lights
 - Created light bulbs in the scene, so that the shadowing is clearly visible.

### Directional
 - Just applying the environment light, such as the sunlight.

### Spotlight
 - As I decided to add interactivity to the scene, I built a camera that can be controlled by the keyboard keys. Because of that, I added a spotlight in front of the Camera, meaning that when I approach a box, it gets illuminated according to the theta angle variable presented in the shader files.

 As explained above, in order to add some interactivity to the end user, I added a Camera that can be controlled by the keyboard keys w,a,s,d or the arrows.

 ## Programming Languages & Tools
  - C++
  - OpenGL
  - GLEW
  - GLM
  - SOIL2