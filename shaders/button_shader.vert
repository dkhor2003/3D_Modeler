#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 uv;

layout(location = 0) out vec3 fragColor;

// Note 128 bytes can only contain 2 4x4 matrices, so we run into the limitation
layout(push_constant) uniform Pushdata
{
    mat4 transform; // projection * view * model
    mat4 modelMatrix;
} pushdata;

void main() 
{
    gl_Position = pushdata.transform * vec4(position, 1.0);

    fragColor = color;

    gl_PointSize = 10;
}

