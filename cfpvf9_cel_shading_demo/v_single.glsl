#version 410

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;

uniform mat4 ModelView;
uniform mat4 Projection;
uniform float offset;


void main()
{
    gl_Position = Projection * ModelView * vec4(vPosition[0] + offset, vPosition[1], vPosition[2], 1.0);
}
