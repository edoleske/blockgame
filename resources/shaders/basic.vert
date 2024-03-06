#version 400
layout (location = 0) in ivec3 aPos;

uniform mat4 view;
uniform mat4 projection;

uniform int CHUNK_SIZE_X;
uniform int CHUNK_SIZE_Z;
uniform int chunkX;
uniform int chunkZ;

void main() {
    vec3 worldPosition = vec3(aPos.x + (chunkX * CHUNK_SIZE_X), aPos.y, aPos.z + (chunkZ * CHUNK_SIZE_Z));
    gl_Position = projection * view * vec4(worldPosition, 1.0);
}