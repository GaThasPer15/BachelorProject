#version 330

#define M_PI 3.1415926535897932384626433832795

struct VS_OUT{
    vec2 fragTexCoord;
    vec2 _resolution;
};

in VS_OUT vs_out;
uniform sampler2D texture0;
uniform vec4 colDiffuse;

uniform float frequency = 5000.0f;
uniform float amplitude = 1.0f;
uniform vec2 seedCoord = vec2(0,0);

out vec4 finalColor;

vec2 randomGradientGen(vec2 edge){
        edge = vec2(dot(edge, vec2(127.1, 311.7)),
                    dot(edge, vec2(269.5, 183.3))   );
        return -1.0 + 2.0*fract(sin(edge)*43758.5453123);
}

float dotGradient(vec2 point, vec2 fractal){
    vec2 gradient = randomGradientGen(point);
    return dot(fractal, gradient);
}

float noise(vec2 coord){
    vec2 borderPoint = floor(coord);
    vec2 f = fract(coord);

    // vec2 weigth = f*f*(3.0-2.0*f);
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
    
    vec2 st = vs_out.fragTexCoord/vs_out._resolution;
    vec2 uv = st * vec2(vs_out._resolution.x/vs_out._resolution.y, 1) + seedCoord;
    st.x *= (vs_out._resolution.x/vs_out._resolution.y);
    float value = noise(st * frequency);
    value *= smoothstep(0.0, 1.0, abs(st.x-0.6));
    // value = value*value*(3.0 - 2.0*value);
    ///////////////////////////
    value = 0.5f - value;
    ///////////////////////////
    float value2 = noise(st * frequency * 1.5f);
    value2 *= smoothstep(0.0, 1.0, abs(st.x-0.6));

    value = mix(value, 0.5f - value2, 0.5f);

    finalColor = vec4(value, value, value, 1.0);
}