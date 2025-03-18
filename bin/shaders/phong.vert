// Phong vertex shader with diffuse texture support
#version 410

layout( location = 0 ) in vec4 Position;
layout( location = 1 ) in vec4 Normal;
layout( location = 2 ) in vec2 TexCoords;

out vec3 vNormal;
out vec4 vPosition;
out vec2 vTexCoords;

uniform mat4 ProjectionViewModel;

// we need this matrix to transform the normal
uniform mat4 ModelMatrix;

void main() {
	vTexCoords = TexCoords;
	vNormal = (ModelMatrix * Normal).xyz;
	gl_Position = ProjectionViewModel * Position;
	vPosition = ModelMatrix * Position;
}