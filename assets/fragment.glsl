varying lowp vec3 lightcolor;

void main( void ) {
	gl_FragColor = vec4(lightcolor, 1.0);
}
