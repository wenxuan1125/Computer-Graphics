#version 430 core
layout(triangles) in;
layout(line_strip, max_vertices = 12) out;

in VS_OUT {
    vec3 normal;
} gs_in[];

uniform mat4 M, V, P;
uniform float r;
uniform float g;
uniform float b;
uniform int triangle;
uniform float show;
out vec4 color;

float rand(vec3 co){ return fract(sin(dot(co.xyz ,vec3(12.9898,78.233, 52.3256))) * 43758.5453); }

void main()
{
    
    if(triangle==1){
        
        if(mod(dot(gl_in[0].gl_Position, gl_in[1].gl_Position), 200)<show){
            color=vec4(1.0,1.0,1.0,1.0);
            gl_Position = P * V * M *gl_in[0].gl_Position;
            EmitVertex();
            gl_Position = P * V * M *gl_in[1].gl_Position;
            EmitVertex();
            EndPrimitive();
        }
        

        if(mod(dot(gl_in[1].gl_Position, gl_in[2].gl_Position), 200)<show){
            color=vec4(1.0,1.0,1.0,1.0);
            gl_Position = P * V * M *gl_in[1].gl_Position;
            EmitVertex();
            gl_Position = P * V * M *gl_in[2].gl_Position;
            EmitVertex();
            EndPrimitive();
        }

        if(mod(dot(gl_in[0].gl_Position, gl_in[2].gl_Position), 200)<show){
            color=vec4(1.0,1.0,1.0,1.0);
            gl_Position = P * V * M *gl_in[0].gl_Position;
            EmitVertex();
            gl_Position = P * V * M *gl_in[2].gl_Position;
            EmitVertex();
            EndPrimitive();
        }
        

    

        
    }
    

    
    color=vec4(r,g,b,1.0);
    gl_Position = P * V * M *gl_in[2].gl_Position+vec4(gs_in[2].normal, 0.0f)*rand(gl_in[2].gl_Position.xyz)*30;
    EmitVertex();
    gl_Position = P * V * M *gl_in[2].gl_Position+vec4(gs_in[2].normal, 0.0f)*rand(gl_in[2].gl_Position.xyz)*30.3;
    EmitVertex();
    EndPrimitive();

    /*
    color=vec4(r,g,b,1.0);
    gl_Position = gl_in[2].gl_Position+vec4(gs_in[2].normal, 0.0f)*rand(gl_in[2].gl_Position.xyz)*30;
    EmitVertex();
    gl_Position = gl_in[2].gl_Position+vec4(gs_in[2].normal, 0.0f)*rand(gl_in[2].gl_Position.xyz)*30.3;
    EmitVertex();
    EndPrimitive();*/

    

    






    
}
