#version 400
in vec2 texCoords;

out vec4 oColor;

uniform sampler2D uTexture;

void main() {
        oColor = texture(uTexture, texCoords);
}