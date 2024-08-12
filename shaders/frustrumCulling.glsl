#version 460 core

layout(local_size_x = 64, local_size_y = 1, local_size_z = 1) in;


struct MeshData {
	vec3 center;
	uint data1; // normal (3b), squaresCount (29b)
	vec3 size;
	uint data2; // startSquare (32b)
};

#define mask3Bits 7u // 0b111


// Inputs
uniform vec4 farPlane; // x,y,z: normal, w: distance
uniform vec4 leftPlane;
uniform vec4 rightPlane;
uniform vec4 upPlane;
uniform vec4 downPlane;
uniform vec3 position;
layout(binding = 0, std430) readonly restrict buffer meshDataBuffer { MeshData meshData[]; }; // All meshes information (position, size, squares indices)

// Outputs
layout(binding = 0, offset = 4) uniform atomic_uint instanceCount; // Number of squares to render
layout(binding = 1, std430) buffer squaresIndicesBuffer { uint squaresIndices[]; }; // Indices of the squares to render


bool outsidePlane(vec3 center, vec3 size, vec4 plane) {
	vec3 normalSign = sign(plane.xyz);
	vec3 closestPoint = center + size * normalSign;
	return dot(closestPoint, plane.xyz) + plane.w < 0;
}


// true if must render mesh
bool cameraCulling(vec3 center, vec3 size, vec3 normal) {
	// Ignore meshes that are invisible because of their normal
	if (dot(center - normal * size - position, normal) > 0) return false;

	// Ignore meshes outside of camera view
	if (outsidePlane(center, size, farPlane)) return false;
	if (outsidePlane(center, size, leftPlane)) return false;
	if (outsidePlane(center, size, rightPlane)) return false;
	if (outsidePlane(center, size, upPlane)) return false;
	if (outsidePlane(center, size, downPlane)) return false;

	return true;
}


void main() {
	MeshData mesh = meshData[gl_GlobalInvocationID.x];
	uint normalID = mesh.data1 & mask3Bits;
	uint squaresCount = mesh.data1 >> 3;
	uint startSquare = mesh.data2;
    vec3 normal = vec3(0, 0, 0);
    normal[normalID >> 1] = -2 * float(normalID & 1u) + 1;

	if (cameraCulling(mesh.center, mesh.size, normal)) {
		uint startIndex = atomicCounterAdd(instanceCount, squaresCount);
		for (uint i = 0; i < squaresCount; i++) {
			squaresIndices[startIndex + i] = startSquare + i;
		}
	}
}