// Phong fragment shader with full three texture support
#version 410

in vec3 vNormal;
in vec4 vPosition;
in vec2 vTexCoords;
in vec3 vTangent;
in vec3 vBiTangent;

uniform sampler2D DiffuseTexture;
uniform sampler2D SpecularTexture;
uniform sampler2D NormalTexture;

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
    vec3 T = normalize(vTangent);
    vec3 B = normalize(vBiTangent);
    vec3 L = normalize(LightDirection);

    // Texture colours
    vec3 texDiffuseColour = texture(DiffuseTexture, vTexCoords).rgb;
    vec3 texSpecularColour = texture(SpecularTexture, vTexCoords).rgb;
    vec3 texNormalValue = texture(NormalTexture, vTexCoords).rgb;

    mat3 TBN = mat3(T,B,N);

    N = TBN * (texNormalValue * 2 - 1);

    // calculate lambert term (negating light direction)
    float lambertTerm = clamp(dot(N, -LightDirection), 0, 1);

    // Specular lighting
    vec3 R = reflect(L, N);
    vec3 V = normalize(CameraPosition - vPosition.xyz);
    float specularTerm = pow(clamp(dot(R, V), 0, 1), SpecularPower);
    

    // final lighting calculations
    vec3 diffuse = LightColour * Kd * lambertTerm * texDiffuseColour;
    vec3 specular = SpecularColour * Ks * specularTerm * texSpecularColour;
    vec3 ambient = AmbientColour * Ka;

    FragColour = vec4(ambient + diffuse + specular, 1) ;
}