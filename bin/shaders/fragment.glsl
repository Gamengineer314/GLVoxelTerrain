#version 460 core

in struct V2F {
    vec4 blockData; // x,y,z : block pos, w : light level
    vec4 color; // x,y,z: color, w: random variation ammount
} v2f;

out vec4 color;


#define discretization 8


// Random value between 0 and 1
uniform float seed;
float random(uvec3 block) {
    vec3 vec = fract(vec3(block) * 0.1031 + seed);
    vec += dot(vec, vec.yzx + 33.33);
    return fract((vec.x + vec.y) * vec.z);
}


void main() {
    uvec3 blockPos = uvec3(v2f.blockData.xyz);
    float lightLevel = v2f.blockData.w;
    color = v2f.color;
    color *= lightLevel / 15; // Light (depending on face directions, better lighting could be added)
    color *= 1 + color.w * ((round(random(blockPos) * discretization) / discretization) - 0.5); // Random slight color variation
    color.w = 1;
}