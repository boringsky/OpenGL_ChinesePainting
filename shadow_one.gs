#version 330 core
layout (triangles_adjacency) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec2 texCoords;
    vec3 normal;
    vec3 FragPos;
    vec4 fragPosLightSpace;
} gs_in[];

out VS_OUT {
    vec2 texCoords;
    vec3 normal;
    vec3 FragPos;
    vec4 fragPosLightSpace;
} vs_out;

out vec2 texCoords;
in vec2 TexCoords;

uniform float time;
uniform sampler2D texture_diffuse1;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;






vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 2.0f;
    vec3 direction = normal * ((sin(time) + 1.0f) / 2.0f) * magnitude / 5;
    //    return position + vec4(direction, 0.0f);
    return position + vec4(0.0f, 0.0f ,0.0f ,0.0f);
}

vec4 explode(vec4 position)
{
    return position + vec4(0.0f, 0.0f ,0.0f ,0.0f);
}



vec3 GetNormal()
{
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(a, b));
}

void main() {
    
    vs_out.texCoords = (gs_in[0].texCoords + gs_in[2].texCoords + gs_in[4].texCoords)/3;
    vs_out.normal = (gs_in[0].normal + gs_in[2].normal + gs_in[4].normal)/3;
    vs_out.FragPos = (gs_in[0].FragPos + gs_in[2].FragPos + gs_in[4].FragPos)/3;
    vs_out.fragPosLightSpace = (gs_in[0].fragPosLightSpace + gs_in[2].fragPosLightSpace + gs_in[4].fragPosLightSpace)/3;
    
    vec3 normal = GetNormal();
    texCoords = TexCoords;
  
    gl_Position = explode(gl_in[0].gl_Position);
//    TexCoords = gs_in[0].texCoords;
    EmitVertex();
    gl_Position = explode(gl_in[2].gl_Position);
//    TexCoords = gs_in[2].texCoords;
    EmitVertex();
    gl_Position = explode(gl_in[4].gl_Position);
//    TexCoords = gs_in[4].texCoords;
    EmitVertex();
    
    
    EndPrimitive();

}