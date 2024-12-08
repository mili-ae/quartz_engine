#version 330

in vec4 vCol;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec4 DirectionalLightSpacePos;

out vec4 color;

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

struct Light
{
    vec3 color;
    float ambientIntensity;
    float diffuseIntensity;
};

struct DirectionalLight
{
    Light base;
    vec3 direction;
};

struct PointLight
{
    Light base;
    vec3 position;
    float constant;
    float linear;
    float exponent;
};

struct SpotLight
{
    PointLight base;
    vec3 direction;
    float edge;
};

struct OmniShadowMap
{
    samplerCube shadowMap;
    float farPlane;
};

struct Material
{
    float specularIntensity;
    float shininess;
};

uniform int pointLightCount;
uniform int spotLightCount;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform OmniShadowMap omniShadowMaps[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS];

uniform sampler2D theTexture;
uniform sampler2D directionalShadowMap;

uniform Material material;

uniform vec3 cameraPosition;

vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

vec4 calculateLightByDirection(Light light, vec3 direction, float shadowFactor)
{
    vec4 ambientColor = vec4(light.color, 1.0f) * light.ambientIntensity;

    // Calculating diffuse lighting
    // Calculating the value between 0 and 1 that gives how much diffuse lighting there is,
    // as in how dark the surface gets. We normalize so that the dot product ONLY takes into
    // account the cos of the angle between the vectors. Normalizing removes the magnitude of
    // each vector from the equation.
    float diffuseFactor = max(dot(normalize(Normal), normalize(direction)), 0.0f);
    // vec4 diffuseColor = vec4(light.color, 1.0f) * light.diffuseIntensity * diffuseFactor;
    vec4 diffuseColor = vec4(light.color * light.diffuseIntensity * diffuseFactor, 1.0f);

    vec4 specularColor = vec4(0, 0, 0, 0);

    if (diffuseFactor > 0.0f)
    {
        vec3 fragToCamera = normalize(cameraPosition - FragPos);

        // Vector of where the light is reflected around the normal. We want light to bounce
        // off on the other side of the normal. Reflect does this. First argument is what we want
        // to reflect, and the second is what we are reflecting around
        vec3 reflectedVertex = normalize(reflect(direction, normalize(Normal)));
        
        // Find the angle between these 2 vectors. This is why we normalized both of those vectors
        // so that we can get the cos between the two
        float specularFactor = dot(fragToCamera, reflectedVertex);
        if (specularFactor > 0.0f)
        {
            specularFactor = pow(specularFactor, material.shininess);
            specularColor = vec4(light.color * material.specularIntensity * specularFactor, 1.0f);
        }
    }

    return (ambientColor + (1.0f - shadowFactor) * (diffuseColor + specularColor));
}

float calculateDirectionalShadowFactor(DirectionalLight light)
{
    // Special operation to get the coord system to make them between -1 & 1
    vec3 projCoords = DirectionalLightSpacePos.xyz / DirectionalLightSpacePos.w;
    projCoords = (projCoords * 0.5) + 0.5; // Convert to a 0 to 1 scale for the shadow map

    float currentDepth = projCoords.z; // How far away the point is from the light, forwards and backwards

    // set up shadow bias to avoid shadow acne
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(light.direction); // Get light direction we are working with

    float bias = max(0.05 * (1 - dot(normal, lightDir)), 0.005);

    float shadow = 0.0; // PCF - smoother shadows

    vec2 texelSize = 1.0 / textureSize(directionalShadowMap, 0);

    // Get the average of all texels around the point, to do PCF
    // Iterating from -1 to 1, with 0 as the middle coordinate
    // Increasing the min value (-1) for x and y will give higher 
    // quality of PCF, but will exponentially be more costly on performance
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            // Goes into the shadow map and takes the texture there at the point we are.
            // But needs to be added to that point our CURRENT x and y coords of the loop
            // Using orthogonal view from light source, so only XY will work on the texture
            float pcfDepth = texture(directionalShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }

    // Average of the pixels that went overin the prev loop
    // 9 because 3 rows (x goes -1, 0, 1) and 3 cols (y goes -1, 0, 1) so 3x3
    shadow /= 9.0;

    // if point is beyond the far plane of the frustum, show no shadows past the far plane
    if (projCoords.z > 1.0)
    {
        shadow = 0.0;
    }

    return shadow;
}

float calculateOmniShadowFactor(PointLight light, int shadowIndex)
{
    vec3 fragToLight = FragPos - light.position;
    float current = length(fragToLight);

    float shadow = 0.0;
    float bias = 0.5;
    float samples = 20;

    float viewDistance = length(cameraPosition - FragPos);
    float diskRadius = (1.0 + (viewDistance / omniShadowMaps[shadowIndex].farPlane)) / 25.0;

    for (int i = 0; i < samples; i++)
    {
        float closest = texture(omniShadowMaps[shadowIndex].shadowMap, fragToLight + gridSamplingDisk[i] * diskRadius).r;
        closest *= omniShadowMaps[shadowIndex].farPlane;
        if (current - bias > closest)
        {
            shadow += 1.0;
        }
    }

    shadow /= float(samples);
    return shadow;

    // vec3 fragToLight = FragPos - light.position;
    // float current = length(fragToLight);

    // float shadow = 0.0;
    // float bias = 0.5;
    // float samples = 4.0;
    // float offset = 0.1;

    // for (float x = -offset; x < offset; x += offset / (samples * 0.5))
    // {
    //     for (float y = -offset; y < offset; y += offset / (samples * 0.5))
    //     {
    //         for (float z = -offset; z < offset; z += offset / (samples * 0.5))
    //         {
    //             float closest = texture(omniShadowMaps[shadowIndex].shadowMap, fragToLight + vec3(x, y, z)).r;
    //             closest *= omniShadowMaps[shadowIndex].farPlane;
    //             if (current - bias > closest)
    //             {
    //                 shadow += 1.0;
    //             }
    //         }
    //     }
    // }

    // shadow /= (samples * samples * samples);
    // return shadow;
}



vec4 calculateDirectionalLight()
{
    float shadowFactor = calculateDirectionalShadowFactor(directionalLight);
    return calculateLightByDirection(directionalLight.base, directionalLight.direction, shadowFactor);
}

vec4 calcPointLight(PointLight pLight, int shadowIndex)
{
    // Get vector from point light to fragment
    vec3 direction = FragPos - pLight.position;
    float distance = length(direction);
    direction = normalize(direction);

    float shadowFactor = calculateOmniShadowFactor(pLight, shadowIndex);

    vec4 color = calculateLightByDirection(pLight.base, direction, shadowFactor);
    // Formula to calculate attenuation
    float attenuation = pLight.exponent * distance * distance + 
                        pLight.linear * distance + 
                        pLight.constant;
    return (color / attenuation);
}

vec4 calcSpotLight(SpotLight sLight, int shadowIndex)
{
    vec3 rayDirection = normalize(FragPos - sLight.base.position);
    float slFactor = dot(rayDirection, sLight.direction);

    if (slFactor > sLight.edge)
    {
        vec4 color = calcPointLight(sLight.base, shadowIndex);

        return color * (1.0f - (1.0f - slFactor) * (1.0f / (1.0f - sLight.edge)));
    } else {
        return vec4(0, 0, 0, 0);
    }
}

vec4 calcPointLights()
{
    vec4 totalColor = vec4(0, 0, 0, 0);
    for (int i = 0; i < pointLightCount; i++)
    {
        totalColor += calcPointLight(pointLights[i], i);
    }

    return totalColor;
}

vec4 calcSpotLights()
{
    vec4 totalColor = vec4(0, 0, 0, 0);
    for (int i = 0; i < spotLightCount; i++)
    {
        totalColor += calcSpotLight(spotLights[i], i + pointLightCount);
    }

    return totalColor;
}

void main()
{
    vec4 finalColor = calculateDirectionalLight();
    finalColor += calcPointLights();
    finalColor += calcSpotLights();

    color = texture(theTexture, TexCoord) * finalColor;
}