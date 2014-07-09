/*
Bananaman
An open source game project

This project uses the free GFX engine developed by Romain Marucchi-Foino
http://gfx.sio2interactive.com/

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of

1. The origin of this software must not be misrepresented; you must not claim that
you wrote the original software. If you use this software in a product, an acknowledgment
in the product would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not be misrepresented
as being the original software.

3. This notice may not be removed or altered from any source distribution.
*/

#include "bananaman.h"

#define OBJ_FILE (char*)"model.obj"
#define VERTEX_SHADER ( char * )"vertex.glsl"
#define FRAGMENT_SHADER ( char * )"fragment.glsl"
#define DEBUG_SHADERS 1

// Used for loading hte obj file
OBJ *obj = NULL;

// pointer to the mesh data inside the obj file
OBJMESH *objmesh = NULL;

TEXTURE *texture = NULL;

// THe shader programs
PROGRAM *program = NULL;


MEMORY *m = NULL;

BANANAMAN bananaman = { bananamanInit,
                        bananamanDraw,
						bananamanTouchBegan,
						bananamanTouchMoved};

unsigned char auto_rotate = 0;
vec2 touch = { 0.0f, 0.0f };
vec3 rot_angle = { 0.0f, 0.0f, 0.0f };


void program_draw_callback(void* ptr) {
	PROGRAM *currentProgram = (PROGRAM*)ptr;
	unsigned int i=0;

	while(i < currentProgram->uniform_count) {
		if(strcmp(currentProgram->uniform_array[i].name, "MODELVIEWPROJECTIONMATRIX") == 0) {
			glUniformMatrix4fv(currentProgram->uniform_array[i].location, 1, GL_FALSE, (float*)GFX_get_modelview_projection_matrix());
		} else if(strcmp(currentProgram->uniform_array[i].name, "MODELVIEWMATRIX") == 0) {
			glUniformMatrix4fv(currentProgram->uniform_array[i].location, 1, GL_FALSE, (float*)GFX_get_modelview_matrix());
		} else if(strcmp(currentProgram->uniform_array[i].name, "PROJECTIONMATRIX") == 0) {
			glUniformMatrix4fv(currentProgram->uniform_array[i].location, 1, GL_FALSE, (float*)GFX_get_projection_matrix());
		} else if(strcmp(currentProgram->uniform_array[i].name, "NORMALMATRIX") == 0) {
			glUniformMatrix3fv(currentProgram->uniform_array[i].location, 1, GL_FALSE, (float*)GFX_get_normal_matrix());
		} else if(strcmp(currentProgram->uniform_array[i].name, "LIGHTPOSITION") == 0) {
			vec3 l = { 0.0f, 0.0f, 0.0f };
			glUniform3fv(currentProgram->uniform_array[i].location, 1, (float*)&l);
		} else if(strcmp(currentProgram->uniform_array[i].name, "DIFFUSE") == 0 && !currentProgram->uniform_array[i].constant) {
			// GL_TEXTURE0
			currentProgram->uniform_array[i].constant = 1;
			glUniform1i(currentProgram->uniform_array[i].location, 0);
		}
		i++;
	}
}

void bananamanInit( int width, int height ) {
	atexit( bananamanExit );

	GFX_start();
	glViewport( 0.0f, 0.0f, width, height );

	GFX_set_matrix_mode(PROJECTION_MATRIX);
	GFX_load_identity();
	GFX_set_perspective(45.0f, (float)width/(float)height, 0.1f, 100.0f, 0.0f);

	program = PROGRAM_create("default", VERTEX_SHADER, FRAGMENT_SHADER, 1, DEBUG_SHADERS, NULL, program_draw_callback);
	obj = OBJ_load(OBJ_FILE, 1);

	texture = TEXTURE_create(obj->objmaterial[0].map_diffuse,
							 obj->objmaterial[0].map_diffuse,
							 1,
							 TEXTURE_MIPMAP,
							 TEXTURE_FILTER_2X,
							 0.0f);


	objmesh = &obj->objmesh[0];
	unsigned char* vertex_array = NULL,
			       *vertex_start = NULL;
	unsigned int i=0,
			index = 0,
			index_uv = 0,
			stride = 0,
			size = 0;

	size = objmesh->n_objvertexdata * (sizeof(vec3) + sizeof(vec3) + sizeof(vec2));
	vertex_array = (unsigned char*)malloc(size);
	vertex_start = vertex_array;

	while(i < objmesh->n_objvertexdata) {
		index = objmesh->objvertexdata[i].vertex_index;
		index_uv = objmesh->objvertexdata[i].uv_index;

		memcpy(vertex_array, &obj->indexed_vertex[index], sizeof(vec3));
		vertex_array += sizeof(vec3);

		memcpy(vertex_array, &obj->indexed_normal[index], sizeof(vec3));
		vertex_array += sizeof(vec3);

		memcpy(vertex_array, &obj->indexed_uv[index_uv], sizeof(vec2));
		vertex_array += sizeof(vec2);

		i++;
	}

	char *info = (char*)malloc(sizeof(char)*1024);
	sprintf(info, "GFX: %d items in the vertex data", i);
	__android_log_write(ANDROID_LOG_INFO, "UTK", info);
	free(info);

	// Create a new vertex buffer object and store it in objmesh's vbo
	glGenBuffers(1, &objmesh->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, objmesh->vbo);
	glBufferData(GL_ARRAY_BUFFER, size, vertex_start, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	free(vertex_start);


	// Now, we describe the triangles with these vertices
	glGenBuffers(1, &objmesh->objtrianglelist[0].vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objmesh->objtrianglelist[0].vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, objmesh->objtrianglelist[0].n_indice_array * sizeof(unsigned short), objmesh->objtrianglelist[0].indice_array, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	// Now to create a VAO
	unsigned char attribute;
	stride = sizeof(vec3) + sizeof(vec3) + sizeof(vec2);

	glGenVertexArraysOES(1, &objmesh->vao);
	glBindVertexArrayOES(objmesh->vao);

	glBindBuffer(GL_ARRAY_BUFFER, objmesh->vbo);
	attribute = PROGRAM_get_vertex_attrib_location(program, "POSITION");
	glEnableVertexAttribArray(attribute);

	// TODO How does glVertexAttribPointer know to look at the vbo?
	glVertexAttribPointer(attribute, 3, GL_FLOAT, GL_FALSE, stride, NULL);

	attribute = PROGRAM_get_vertex_attrib_location(program, "NORMAL");
	glEnableVertexAttribArray(attribute);
	glVertexAttribPointer(attribute, 3, GL_FLOAT, GL_FALSE, stride, BUFFER_OFFSET(sizeof(vec3)));

	attribute = PROGRAM_get_vertex_attrib_location(program, "TEXCOORD0");
	glEnableVertexAttribArray(attribute);
	glVertexAttribPointer(attribute, 2, GL_FLOAT, GL_FALSE, stride, BUFFER_OFFSET(sizeof(vec3) + sizeof(vec3)));

	// Now bind the triangle list vbo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objmesh->objtrianglelist[0].vbo);

	// End the VAO
	glBindVertexArrayOES(0);
}


void bananamanDraw( void ) {
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	GFX_set_matrix_mode(MODELVIEW_MATRIX);
	GFX_load_identity();

	vec3 e = {0.0f, -4.0f, 0.0f},
	     c = {0.0f, 0.0f, 0.0f},
	     u = {0.0f, 0.0f, 1.0f};

	GFX_look_at(&e, &c, &u);

	if(auto_rotate) rot_angle.z += 2.0f;
	GFX_rotate(rot_angle.x, 1, 0, 0);
	GFX_rotate(rot_angle.z, 0, 0, 1);

	glBindVertexArrayOES(objmesh->vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->tid);
	PROGRAM_draw(program);
	glDrawElements(GL_TRIANGLES, objmesh->objtrianglelist[0].n_indice_array, GL_UNSIGNED_SHORT, NULL);
}


void bananamanExit( void ) {
    SHADER_free( program->vertex_shader );
    SHADER_free( program->fragment_shader );
    PROGRAM_free( program );
    TEXTURE_free(texture);
    OBJ_free(obj);
}

void bananamanTouchBegan(float x, float y, unsigned int tap_count) {
	if(tap_count==2) auto_rotate = !auto_rotate;

	touch.x = x;
	touch.y = y;
}

void bananamanTouchMoved(float x, float y, unsigned int tap_count) {
	auto_rotate = 0;
	rot_angle.z += -(touch.x - x);
	rot_angle.x += -(touch.y - y);

	touch.x = x;
	touch.y = y;
}
