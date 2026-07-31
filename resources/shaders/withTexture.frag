#version 400
in vec2 vTexCoords;
in float vLayer;
in float vVisibility;

out vec4 oColor;

uniform sampler2DArray uTexture;
uniform vec4 uFogColor;

uniform int uIsHighlight = 0;

void main() {
    if (uIsHighlight != 0) {
        oColor = vec4(1.0, 1.0, 0.0, 1.0);
        return;
    }

    vec4 texColor = texture(uTexture, vec3(vTexCoords, vLayer));
    if (texColor.a < 0.1) {
        discard;
    }
    oColor = texColor;

    // Mix in fog
    //        oColor = mix(vec4(pow(uFogColor.xyz, vec3(1.75)), uFogColor.w), oColor, vVisibility);
}