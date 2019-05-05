# OpenGL cel shading + outline + feature edge rendering
Demonstration of an OpenGL implementation of cel shading with feature edge rendering.

This uses Phong shading discretized into 3 shades of color. 

Object outlining is done using stencil buffering and a duplicate wireframe rendering of the object.

Feature edges are detected using the dot product of the view vector and the normal vectors of the triangle faces.

Permanent (view angle independent) contour lines are added using the derivatives of the normal vectors in screen space (in the fragment shader).

![pig](https://github.com/connor-penrod/cel_shading_opengl/blob/master/celshading.gif)

