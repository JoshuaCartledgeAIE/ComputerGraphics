// Phong fragment shader with diffuse texture support
#version 410

in vec3 vNormal;
in vec4 vPosition;
in vec2 vTexCoords;

uniform sampler2D DiffuseTexture;

uniform vec3 LightDirection;
uniform vec3 LightColour;
uniform vec3 SpecularColour;
uniform vec3 AmbientColour;

uniform vec3 Ka; // ambient colour of the surface
uniform vec3 Kd; // diffuse colour of the surface
uniform vec3 Ks; // specular colour of the surface
uniform float SpecularPower; // tightness of specular highlights

uniform vec3 CameraPosition;


out vec4 FragColour;

void main() {
    // Diffuse lighting
    // make sure the normal and light direction are normalised
    vec3 N = normalize(vNormal);
    vec3 L = normalize(LightDirection);

    // calculate lambert term (negating light direction)
    float lambertTerm = clamp(dot(vNormal, -LightDirection), 0, 1);

    // Specular lighting
    vec3 R = reflect(L, N);
    vec3 V = normalize(CameraPosition - vPosition.xyz);
    float specularTerm = pow(clamp(dot(R, V), 0, 1), SpecularPower);

    // Texture colour
    vec3 textureColour = texture(DiffuseTexture, vTexCoords).rgb;

    // final lighting calculations
    vec3 diffuse = LightColour * Kd * lambertTerm * textureColour;
    vec3 specular = SpecularColour * Ks * specularTerm * textureColour;
    vec3 ambient = AmbientColour * Ka;

    FragColour = vec4(ambient + diffuse + specular, 1) ;
}