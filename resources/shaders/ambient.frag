#version 330

in vec3 fragPosition;

uniform vec4 colDiffuse;
uniform vec3 viewPos;

out vec4 fragColor;

void main() {
    vec3 lightColor = vec3(1.0, 1.0, 1.0);

    float dist = length(viewPos - fragPosition);
    float fogFactor = 1.0/exp(dist*dist*0.00001);
    vec4 fog = vec4(fogFactor * lightColor, 1.0);

    fragColor = vec4(fog * colDiffuse);
}
