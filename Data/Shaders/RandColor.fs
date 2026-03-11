#version 330

in vec2 fragTexCoord;
in vec4 fragColor;
uniform sampler2D texture0;
uniform vec4 colDiffuse;

uniform vec2 hexCoord;

out vec4 finalColor;

float randValue(vec2 seed){
    return fract(dot(vec2(34242.3545f, 2246.745f), vec2(cos(seed.x * 994.5f), sin(seed.y * 177.031))));
}

void main(){
    vec4 texelColor = texture(texture0, fragTexCoord) * colDiffuse * fragColor;
    finalColor = vec4(randValue(hexCoord), 
                        randValue(vec2(hexCoord.x, hexCoord.x)), 
                        randValue(vec2(hexCoord.y, hexCoord.y)), 
                        texelColor.a);
}