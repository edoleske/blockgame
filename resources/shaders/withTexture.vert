#version 400
layout (location = 0) in ivec3 aPos;
layout (location = 1) in ivec2 aTexCoord;

out vec2 vTexCoords;
out float vVisibility;

uniform mat4 view;
uniform mat4 projection;
uniform int renderDistance;

uniform int CHUNK_SIZE_X;
uniform int CHUNK_SIZE_Z;
uniform int chunkX;
uniform int chunkZ;

uniform vec3 uHighlightOffset;

const float fogDensity = 0.01f;
const float fogGradient = 4.0f;

void main() {
    ivec3 pos = aPos + ivec3(uHighlightOffset);
    vec3 worldPosition = vec3(pos.x + (chunkX * CHUNK_SIZE_X), pos.y, pos.z + (chunkZ * CHUNK_SIZE_Z));
    gl_Position = projection * view * vec4(worldPosition, 1.0);

    // Fog Calculation
    float cameraDistance = length(view * vec4(worldPosition, 1.0f));
    float fogDistance = 5.6f * (cameraDistance / renderDistance);
    vVisibility = exp(-pow((fogDistance * fogDensity), fogGradient));
    vVisibility = clamp(vVisibility, 0.0f, 1.0f);

    vTexCoords = aTexCoord;
}