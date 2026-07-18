#version 400
in vec2 TexCoord;

out vec4 oColor;

uniform sampler2D uTexture;
uniform bool isText;

void main() {
    if (isText) {
        oColor = vec4(texture(uTexture, TexCoord).r);
        return;
    }

    oColor = texture(uTexture, TexCoord);
}