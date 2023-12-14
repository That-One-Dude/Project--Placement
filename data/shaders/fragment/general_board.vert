#version 420 core
layout (location = 0) in vec3 pos;
layout (location = 1) in int cellIndex;

int run() {
    gl_Position = vec4(pos.xyz, 1.0);
    return cellIndex;
}