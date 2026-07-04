#version 400
in vec2 TexCoord;

out vec4 oColor;

uniform sampler2D uTexture;
uniform int uTexWidth;
uniform int uTexHeight;

void main() {
//    vec2 calcTexCoords = vec2(vTexCoords.x / uTexWidth, vTexCoords.y / uTexHeight);
    oColor = texture(uTexture, TexCoord);
}