#version 330

in vec3 vertexPosition;
in vec2 VertexTexCoord;
uniform mat4 mvp;
uniform vec2 resolution;

struct VS_OUT{
    vec2 fragTexCoord;
    vec2 _resolution;
};

out VS_OUT vs_out;

void main(){
    vs_out._resolution = resolution;
    vs_out.fragTexCoord = VertexTexCoord;
    gl_Position = mvp * vec4(vertexPosition, 1.0);
}