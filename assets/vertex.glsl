uniform mediump mat4 MODELVIEWMATRIX;
uniform mediump mat4 PROJECTIONMATRIX;
uniform mediump mat3 NORMALMATRIX;
uniform mediump vec3 LIGHTPOSITION;

varying lowp vec3 lightcolor;

attribute mediump vec3 POSITION;
attribute lowp vec3 NORMAL;
lowp vec3 normal;

void main( void ) { 	
	mediump vec3 position = vec3(MODELVIEWMATRIX * vec4(POSITION, 1.0));
	normal = normalize(NORMALMATRIX * NORMAL);
	mediump vec3 lightdirection = normalize(LIGHTPOSITION-position);
	lowp float ndot1 = max(dot(normal, lightdirection), 0.0);
	lightcolor = vec3(ndot1, ndot1, ndot1);
	gl_Position = PROJECTIONMATRIX * vec4(position, 1.0);
}