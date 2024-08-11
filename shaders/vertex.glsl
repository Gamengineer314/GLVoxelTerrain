#version 460 core

out struct V2F {
    vec4 blockData; // x,y,z : block pos, w : light level
    vec4 color; // x,y,z: color, w: random variation ammount
} v2f;


struct Square {
    uint data1; // x (13b), z (13b)
    uint data2; // y (9b), width (6b), height (6b), normal (3b), color (8b)
};

#define mask2Bits 3u             // 0b11
#define mask3Bits 7u             // 0b111
#define mask4Bits 15u            // 0b1111
#define mask6Bits 63u            // 0b111111
#define mask9Bits 511u           // 0b1111111111
#define mask13Bits 8191u         // 0b1111111111111

const uint faceLightLevels[6] = {
    12, // x+
    12, // x-
    15, // y+
    9,  // y-
    12, // z+
    12, // z-
};

#define COLOR(r, g, b, variation) vec4(r / 255.0, g / 255.0, b / 255.0, variation)
const vec4 colors[5] = {
    // r, g, b,     color variation
    COLOR(255, 0, 0,        0),     // Error color
    COLOR(245, 245, 60,     0.1),   // Sand
    COLOR(15, 220, 0,       0.1),   // Grass
    COLOR(130, 130, 135,    0.06),  // Stone
    COLOR(235, 235, 235,    0.06)   // Snow
};


layout(binding = 0, std430) readonly restrict buffer squaresBuffer { Square squares[]; };
layout(location = 0) in uint squareIndex;

uniform mat4 vpMatrix;
uniform vec3 position;
uniform float quadsInterleaving; // Size increase to remove small (1 pixel) gaps between triangles


void main() {
    // Get square
    Square square = squares[squareIndex];

    // Unpack data
    vec3 cubePos = vec3(square.data1 & mask13Bits, square.data2 & mask9Bits, square.data1 >> 13);
    uint normalID = (square.data2 >> 21) & mask3Bits;
    float width = ((square.data2 >> 9) & mask6Bits) + 1;
    float height = ((square.data2 >> 15) & mask6Bits) + 1;
    uint normalAxis = normalID >> 1;

    // Position
    vec3 pos = cubePos;
    float interleaving = distance(position, cubePos) * quadsInterleaving * 0.001f;
    pos[1u & ~normalAxis] += -interleaving + ((uint(gl_VertexID) & 1u) ^ uint(normalAxis != 0) ^ (normalID & 1u)) * (width + 2 * interleaving);
    pos[2u & ~normalAxis] += -interleaving + (gl_VertexID >> 1) * (height + 2 * interleaving);
    vec3 normal = vec3(0, 0, 0);
    normal[normalAxis] = -2 * float(normalID & 1u) + 1;

    // Output
    gl_Position = vpMatrix * vec4(pos, 1);
    v2f.blockData = vec4(pos - normal * 0.5f, faceLightLevels[normalID]);
    v2f.color = colors[square.data2 >> 24];
}