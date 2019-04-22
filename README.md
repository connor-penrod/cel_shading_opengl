# OpenGL cel shading + outline + feature edge rendering
Demonstration of an OpenGL implementation of cel shading with feature edge rendering.

Uses Phong shading discretized into 3 shades of color. 
Object outline is done using stencil buffering and a duplicate wireframe rendering of the object.
Feature edges are detected using the dot product of the view vector and the normal vectors of the triangle faces.

![pig](https://github.com/connor-penrod/cel_shading_opengl/edit/master/pig.gif)

