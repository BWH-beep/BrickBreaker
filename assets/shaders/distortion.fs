#version 330
uniform sampler2D texture0;
uniform float time;
in vec2 fragTexCoord;
out vec4 finalColor;

void main() {
    vec2 uv = fragTexCoord;
    // 扭曲效果
    uv.x += sin(uv.y * 10.0 + time * 2.0) * 0.01;
    uv.y += cos(uv.x * 10.0 + time * 1.5) * 0.01;
    
    vec4 color = texture(texture0, uv);
    // 颜色增强
    color.rgb *= 1.2;
    // 添加光晕
    color.rgb += vec3(0.1, 0.05, 0.2) * sin(time * 2.0);
    
    finalColor = color;
}
