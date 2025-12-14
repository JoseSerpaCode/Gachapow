#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

out vec4 finalColor;

uniform sampler2D texture0;
uniform vec2 resolution;
uniform float time;

// =======================================
//   CURVATURA CRT
// =======================================
vec2 curve(vec2 uv)
{
    uv = uv * 2.0 - 1.0;

    float r = dot(uv, uv);
    uv *= 1.0 + r * 0.07;   // tu curvatura suave

    return uv * 0.5 + 0.5;
}

// =======================================
//   SCANLINES + EFECTO CRT
// =======================================
vec3 apply_scanlines(vec3 col, vec2 uv)
{
    float scan = sin(uv.y * resolution.y * 1.5);
    col -= col * (0.08 * scan);

    float mask = sin(uv.y * resolution.y * 0.5 + time * 20.0) * 0.03;
    col += mask;

    return col;
}

void main()
{
    vec2 uv = curve(fragTexCoord);

    // --- borde negro limpio ---
    if (uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0) {
        finalColor = vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }

    // --- clamp para evitar artefactos ---  <===== FIX IMPORTANTE
    uv = clamp(uv, 0.001, 0.999);

    vec3 col = texture(texture0, uv).rgb;

    col = apply_scanlines(col, uv);

    finalColor = vec4(col, 1.0) * fragColor;
}
