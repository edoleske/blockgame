#version 400
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in float aLayer;

out vec2 TexCoord;
out float Layer;

uniform mat4 projection;

void main() {
    TexCoord = aTexCoord;
    Layer = aLayer;
    gl_Position = projection * vec4(aPos.xy, 0.0, 1.0);
}