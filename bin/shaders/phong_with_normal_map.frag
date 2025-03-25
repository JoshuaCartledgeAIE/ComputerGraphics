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
uniform vec3 AmbientColour;

const int MAX_LIGHTS = 4;
uniform int NumLights;
uniform vec3 PointLightColour[MAX_LIGHTS];
uniform vec3 PointLightPosition[MAX_LIGHTS];

uniform vec3 Ka; // ambient colour of the surface
uniform vec3 Kd; // diffuse colour of the surface
uniform vec3 Ks; // specular colour of the surface
uniform float SpecularPower; // tightness of specular highlights

uniform vec3 CameraPosition;


out vec4 FragColour;

vec3 getDiffuseColour(vec3 direction, vec3 colour, vec3 normal)
{
    // calculate lambert term (negating light direction) and multiply it by the light colour
    return colour * max (0, dot(normal, -direction));
}

vec3 getSpecularColour(vec3 direction, vec3 colour, vec3 normal, vec3 view)
{
    // calculate specular term (negating light direction) and multiply it by the light colour
    vec3 reflectedNormal = reflect(direction, normal);
    return colour * pow(clamp(dot(reflectedNormal, view), 0, 1), SpecularPower);
}

void main() {
    
    // make sure all the direction variables are normalised
    vec3 normal = normalize(vNormal);
    vec3 tangent = normalize(vTangent);
    vec3 biTangent = normalize(vBiTangent);
    vec3 sunLightDirection = normalize(LightDirection);

    // Texture colours
    vec3 texDiffuseColour = texture(DiffuseTexture, vTexCoords).rgb;
    vec3 texSpecularColour = texture(SpecularTexture, vTexCoords).rgb;
    vec3 texNormalValue = texture(NormalTexture, vTexCoords).rgb;

    // Apply normal map to normal vector
    mat3 TBN = mat3(tangent,biTangent,normal);
    normal = TBN * (texNormalValue * 2 - 1);
    
    // Diffuse sun lighting
    vec3 diffuseTotal = getDiffuseColour(sunLightDirection, LightColour, normal);

    // Specular sun lighting
    // Calculate View vector
    vec3 view = normalize(CameraPosition - vPosition.xyz);
    vec3 specularTotal = getSpecularColour(sunLightDirection, LightColour, normal, view);

    // Point light calculations
    for (int i=0; i<NumLights; i++)
    {
        // find normalised direction vector and distance from light to this point on the mesh
        vec3 direction = vPosition.xyz - PointLightPosition[i];
        float distance = length(direction);
        direction = direction/distance;

        // attenuate light intensity according to inverse square law
        vec3 colour = PointLightColour[i] / (distance * distance);

        // add to both colour totals
        diffuseTotal += getDiffuseColour(direction, colour, normal);
        specularTotal += getSpecularColour(direction, colour, normal, view);
    }
    

    // final lighting calculations
    vec3 diffuse = diffuseTotal * Kd * texDiffuseColour;
    vec3 specular = specularTotal * Ks * texSpecularColour;
    vec3 ambient = AmbientColour * Ka;

    FragColour = vec4(ambient + diffuse + specular, 1) ;
}