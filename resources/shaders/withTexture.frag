#version 400
in vec2 vTexCoords;
in float vVisibility;

out vec4 oColor;

uniform sampler2D uTexture;
uniform vec4 uFogColor;

uniform int uTexWidth;
uniform int uTexHeight;

uniform int uIsHighlight = 0;

void main() {
        if (uIsHighlight != 0) {
                oColor = vec4(1.0, 1.0, 0.0, 1.0);
                return;
        }

        vec2 calcTexCoords = vec2(vTexCoords.x / uTexWidth, vTexCoords.y / uTexHeight);
        vec4 texColor = texture(uTexture, calcTexCoords);
        if (texColor.a < 0.1) {
                discard;
        }
        oColor = texColor;

        // Mix in fog
//        oColor = mix(vec4(pow(uFogColor.xyz, vec3(1.75)), uFogColor.w), oColor, vVisibility);
}