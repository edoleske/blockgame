#version 400
in vec2 vTexCoords;
in float vVisibility;

out vec4 oColor;

uniform sampler2D uTexture;
uniform vec4 uFogColor;

void main() {
        oColor = texture(uTexture, vTexCoords);

        // Mix in fog
//        oColor = mix(vec4(pow(uFogColor.xyz, vec3(1.75)), uFogColor.w), oColor, vVisibility);
}