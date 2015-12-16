uniform int use_texture;

varying vec3 N;
varying vec3 L;
varying vec3 R;
varying vec3 V;
varying vec2 VtexCoord;
varying vec4 pos_in_eye;

uniform vec4 light_ambient;
uniform vec4 light_diffuse;
uniform vec4 light_specular;
uniform vec4 light_pos;

uniform mat4 eye2world;

uniform vec4 mat_ambient;
uniform vec4 mat_diffuse;
uniform vec4 mat_specular;
uniform float mat_shine;

uniform bool isLightSource;

uniform sampler2D tex;

uniform samplerCube cubeMap;


void main(){
    vec4 ambient = light_ambient * mat_ambient;
    float NdotL;
    if (dot(N,L)<0.0) NdotL = 0.0;
    else NdotL = dot(N, L);
    
    vec4 diffuse = light_diffuse * mat_diffuse * NdotL;
    
    float RdotV;
    if (dot(R,V)<0.0) RdotV = 0.0;
    else RdotV = dot(R, V);
    
    vec4 specular = light_specular*mat_specular*pow(RdotV,mat_shine);
    if (isLightSource) {
        gl_FragColor = mat_diffuse;
    }
    else{
        if (use_texture == 0) {
            gl_FragColor = ambient + diffuse + specular;
        }
        
        else if(use_texture == 1){
            vec4 texcolor;
            //texcolor = texture2D(tex, vec2(0.5,0.5));
            texcolor = texture2D(tex, VtexCoord);
            //gl_FragColor = texcolor;
            gl_FragColor = texcolor;
            //gl_FragColor = vec4(VtexCoord[0],VtexCoord[1],0,0);
            
        }
        else {
            vec4 env_color;
            vec3 ref, view_vector;
            view_vector = normalize(vec3(pos_in_eye-vec4(0,0,0,1)));
            ref = normalize(reflect(view_vector, N));
            ref = vec3(eye2world*vec4(ref,0));
            ref[1] = -ref[1];

            //ref = vec3(1,0,0);
            env_color = textureCube(cubeMap, ref);
            //gl_FragColor = vec4(1,1,1,1);
            //gl_FragColor = vec4(ref[0],ref[1],0,0);
            gl_FragColor = env_color;
        }
    }
}