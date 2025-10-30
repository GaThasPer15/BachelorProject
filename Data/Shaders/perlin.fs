#version 330

#define M_PI 3.1415926535897932384626433832795

struct VS_OUT{
    vec2 fragTexCoord;
    vec2 _resolution;
};

in VS_OUT vs_out;
uniform sampler2D texture0;
uniform vec4 colDiffuse;

uniform float frequency = 10.0f;
uniform float amplitude = 1.0f;
uniform vec2 seedCoord = vec2(0,0);

out vec4 finalColor;

vec2 randomGradientGen(vec2 edge){
        edge = vec2(dot(edge*M_PI, vec2(sin(edge.x*123.4), cos(edge.y*69.47))),
                    dot(edge/M_PI, vec2(cos(edge.y*213.7), sin(edge.x*11.25))));
        return 2.0*fract(sin(edge*M_PI)*1770.13) - 1.0;
}

float dotGradient(vec2 point, vec2 fractal){
    vec2 gradient = randomGradientGen(point);
    return dot(fractal, gradient);
}

float noise(vec2 coord){
    vec2 borderPoint = floor(coord);
    vec2 f = fract(coord);
    vec2 weigth = smoothstep(0.0, 1.0, f);

    float n0, n1, x0, x1;
    n0 = dotGradient(borderPoint + vec2(0.0, 0.0), f - vec2(0.0, 0.0));
    n1 = dotGradient(borderPoint + vec2(1.0, 0.0), f - vec2(1.0, 0.0));
    x0 = mix(n0, n1, weigth.x);
    n0 = dotGradient(borderPoint + vec2(0.0, 1.0), f -  vec2(0.0, 1.0));
    n1 = dotGradient(borderPoint + vec2(1.0, 1.0), f - vec2(1.0, 1.0));
    x1 = mix(n0, n1, weigth.x);

    return mix(x0, x1, weigth.y);
}

void main(){
    vec2 st = (vs_out.fragTexCoord + seedCoord)/vs_out._resolution;
    vec2 uv = st * vec2(vs_out._resolution.x/vs_out._resolution.y, 1);
    float scale = 480;
    float value = noise(uv * scale * frequency) * amplitude;

    value *= smoothstep(0.0, 1.0, abs(uv.x-0.6));
    value = 0.5f - value;

    float value2 = noise(uv * scale * frequency * 1.5f) * amplitude * 1.5f;
    value2 *= smoothstep(0.0, 1.0, abs(uv.x-0.6));
    value = mix(value, 0.5f - value2, 0.25f);

    // value2 = noise(uv * scale * frequency * 2.5f) * amplitude * 2.0f;
    // value2 *- smoothstep(0.0, 1.0, abs(uv.x-0.6));
    // value = mix(value, 0.5f - value2, 0.2f);

    // value2 = noise(uv * scale * frequency * 3.5f) * amplitude * 3.0f;
    // value2 *- smoothstep(0.0, 1.0, abs(uv.x-0.6));
    // value = mix(value, 0.5f - value2, 0.1f);

    finalColor = vec4(value, value, value, 1.0);
}