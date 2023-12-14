#version 420 core

layout (std140, binding = 1) uniform palletData {
    vec4 colors[256];
};

layout (std140, binding = 2) uniform cellAssociations {
    int palletIndices[1024];
}

out VS_OUT {
    vec4 color;
} vs_out;

int run();

void main() {
    int cellIndex = run();
    vs_out.color = colors[palletIndices[cellIndex]];
}