#version 400
in vec2 TexCoord;
in float Layer;

out vec4 oColor;

uniform sampler2D uAtlas;
uniform sampler2D uFont;
uniform sampler2DArray uArray;

// which texture to use:
// 0 - UI Texture Atlas (uAtlas)
// 1 - Main Texture Array (uArray)
// 2 - Font Bitamp Texture (uFont)
uniform int uMode;

void main() {
    switch (uMode) {
        case 1:
            oColor = vec4(texture(uArray, vec3(TexCoord, Layer)));
            break;
        case 2:
            oColor = vec4(texture(uFont, TexCoord).r);
            break;
        default:
            oColor = texture(uAtlas, TexCoord);
            break;
    }
}