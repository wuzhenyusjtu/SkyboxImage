
attribute vec4 position;
attribute vec4 color;
attribute vec4 normal;
attribute vec2 texCoord;

varying vec3 N;
varying vec3 L;
varying vec3 R;
varying vec3 V;
varying vec2 VtexCoord;
varying vec4 pos_in_eye;

uniform vec4 light_pos;

uniform mat4 local2clip;
uniform mat4 local2eye;
uniform mat4 normal_matrix;



void main(){
    
    
    gl_Position = local2clip * position;
    
    N = normalize(vec3(normal_matrix * normal));
    vec4 Lpos =  local2eye * light_pos;
    vec4 Vpos =  local2eye * position;
	
    pos_in_eye = Vpos;
    
	L = normalize(vec3(Lpos - Vpos)); // pass as a varying
    
	R = normalize(reflect(-L,N)); // pass as a varying
    
    V = normalize(vec3(-Vpos)); // pass as a varying
    
    VtexCoord = texCoord;
}