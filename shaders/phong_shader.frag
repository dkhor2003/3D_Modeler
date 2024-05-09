#version 450

/* only location and type (vec3) matter, the name doesn't need to match */

layout (location = 0) in vec3 fragColor;
layout (location = 1) in vec3 fragPosWorld;
layout (location = 2) in vec3 fragNormalWorld;

layout (location = 0) out vec4 outColor;

struct PointLight
{
    vec4 position; // ignore w
    vec4 color;    // w is intensity
};

layout(set = 0, binding = 0) uniform GlobalUBO 
{
    mat4 projection;
    mat4 view;
    mat4 inverseView;
    vec4 ambientLightColor; // w is intensity
    PointLight pointLight;
} ubo;

layout(push_constant) uniform Pushdata
{
    mat4 modelMatrix;
    mat4 normalMatrix;
} pushdata;


void main()
{
    // Phong Shading
    //vec3 directionToLight = ubo.pointLight.position.xyz - fragPosWorld;
    //float attenuation = 1.0 / dot(directionToLight, directionToLight); // distance squared

    //vec3 lightColor = ubo.pointLight.color.xyz * ubo.pointLight.color.w * attenuation;
    //vec3 ambientLight = ubo.ambientLightColor.xyz * ubo.ambientLightColor.w;
    //vec3 diffuseLight = lightColor * max(dot(normalize(fragNormalWorld), normalize(directionToLight)), 0); // make sure to normalize again for fragNormalWorld
    //outColor = vec4((diffuseLight + ambientLight) * fragColor, 1.0);

    // Blinn-Phong Shading
    vec3 diffuseLight = ubo.ambientLightColor.xyz * ubo.ambientLightColor.w;
    vec3 specularLight = vec3(0.0);
    vec3 surfaceNormal = normalize(fragNormalWorld);
    vec3 cameraPosWorld = ubo.inverseView[3].xyz;
    vec3 viewDirection = normalize(cameraPosWorld - fragPosWorld);

    PointLight light = ubo.pointLight;
    vec3 directionToLight = light.position.xyz - fragPosWorld;
    float attenuation = 1.0 / dot(directionToLight, directionToLight); // distance squared
    
    directionToLight = normalize(directionToLight);

    float cosAngIncidence = max(dot(surfaceNormal, directionToLight), 0);
    vec3 intensity = light.color.xyz * light.color.w * attenuation;

    diffuseLight += intensity * cosAngIncidence;

    // specular lighting
    vec3 halfAngle = normalize(directionToLight + viewDirection);
    float blinnTerm = dot(surfaceNormal, halfAngle);
    blinnTerm = clamp(blinnTerm, 0, 1);
    blinnTerm = pow(blinnTerm, 20.0); // higher values -> sharper highlight
    specularLight += intensity * blinnTerm;

    outColor = vec4(diffuseLight * fragColor + specularLight * fragColor, 1.0);
}

