// classic Phong vertex shader
#version 410

layout( location = 0 ) in vec4 Position;
layout( location = 1 ) in vec4 Normal;

out vec3 vNormal;
out vec4 vPosition;

uniform mat4 ProjectionViewModel;

// we need this matrix to transform the normal
uniform mat4 ModelMatrix;

void main() {
	vNormal = (ModelMatrix * Normal).xyz;
	gl_Position = ProjectionViewModel * Position;
	vPosition = ModelMatrix * Position;
}