#include "Texture.h"
#include "../../SOIL/SOIL.h"

std::map<std::string, TextureData> Texture::_texturePool;
GLuint Texture::programID;
bool Texture::shaderLoaded = false;

Texture::Texture(const char* filename) {
	int channels = 0;

	textureID = loadImage(filename, width, height, channels);

	load(0,0, width, height, width, height);
}

Texture::Texture(const char* filename, float x, float y, float subWidth, float subHeight) {
	int channels = 0;

	textureID = loadImage(filename, width, height, channels);

	load(x,y,subWidth,subHeight,width,height);
}


void Texture::load(float x, float y, float subWidth, float subHeight, float imageWidth, float imageHeight) {
	if(!shaderLoaded)
	{
		programID = LoadShaders();
		shaderLoaded=true;
	}
	scale = 1;

			float topLeftS = x / imageWidth;
			float topLeftT = (y+subHeight) / imageHeight;

			float topRightS = (x + subWidth) / imageWidth;
			float topRightT = (y+subHeight) / imageHeight;

			float bottomRightS = (x + subWidth) / imageWidth;
			float bottomRightT = (y) / imageHeight;

			float bottomLeftS = x / imageWidth;
			float bottomLeftT = (y)/ imageHeight;

	//Center
	/*GLfloat quad_vertices[] = {
		-width/2.f, -height/2.f, //Bottom Left
		 width/2.f,  -height/2.f, //Bottom Right
		 width/2.f,   height/2.f, //Top Right
		-width/2.f,  height/2.f,}; //Top Left*/

	GLfloat quad_vertices[] = {
		 0.0f, subHeight, //Bottom Left
		 subWidth,  subHeight, //Bottom Right
		 subWidth,   0.0f, //Top Right
		 0.0f,  0.0f,}; //Top Left



	glGenBuffers(1, &vbo_quad_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_quad_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW);

	GLfloat quad_texcoords[] = {
		topLeftS, topLeftT,
		topRightS, topRightT,
		bottomRightS, bottomRightT, 
		bottomLeftS, bottomLeftT, 
	};

	glGenBuffers(1, &vbo_quad_texcoords);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_quad_texcoords);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_texcoords), quad_texcoords, GL_STATIC_DRAW);

	GLushort quad_elements[] = {0,  1,  2,  2,  3,  0,};
	glGenBuffers(1, &ibo_quad_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_quad_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quad_elements), quad_elements, GL_STATIC_DRAW);
  
	attribute_coord2d = glGetAttribLocation(programID, "coord2d");
	attribute_texcoord = glGetAttribLocation(programID, "texcoord");
	uniform_mvp = glGetUniformLocation(programID, "mvp");
	uniform_mytexture = glGetUniformLocation(programID, "mytexture");
	if(attribute_coord2d==-1 || attribute_texcoord==-1 || uniform_mvp==-1 || uniform_mytexture==-1)
		printf("Cound not bind attributes in texture");
}

void Texture::render(glm::mat4 mvp) {
	 glUseProgram(programID);
	 glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));

	 glActiveTexture(GL_TEXTURE0);
	 glBindTexture(GL_TEXTURE_2D, textureID);
	 glUniform1i(uniform_mytexture, /*GL_TEXTURE*/0);

	 glEnableVertexAttribArray(attribute_coord2d);
	 glEnableVertexAttribArray(attribute_coord2d);
	 // Describe our vertices array to OpenGL (it can't guess its format automatically)
	 glBindBuffer(GL_ARRAY_BUFFER, vbo_quad_vertices);
	 glVertexAttribPointer(
		attribute_coord2d, // attribute
		2,                 // number of elements per vertex, here (x,y,z)
		GL_FLOAT,          // the type of each element
		GL_FALSE,          // take our values as-is
		0,                 // no extra data between each position
		0                  // offset of first element
	);

	glEnableVertexAttribArray(attribute_texcoord);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_quad_texcoords);
	glVertexAttribPointer(
		attribute_texcoord, // attribute
		2,                  // number of elements per vertex, here (x,y)
		GL_FLOAT,           // the type of each element
		GL_FALSE,           // take our values as-is
		0,                  // no extra data between each position
		0                   // offset of first element
	 );

	/* Push each element in buffer_vertices to the vertex shader */
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_quad_elements);
	int size;  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

	 glDisableVertexAttribArray(attribute_coord2d);
	glDisableVertexAttribArray(attribute_texcoord);
}

GLuint Texture::loadImage(std::string filename, int &width, int &height, int &channels)
{
	if(_texturePool.find(filename) != _texturePool.end())
	{
		TextureData data = _texturePool.find(filename)->second;
		width = data.width;
		height = data.height;
		channels = data.channels;
		return data.texture;
	}

	printf("Creating new texture: %s \n", filename.c_str());

	unsigned char* image = SOIL_load_image
	(
		filename.c_str(),
		&width, &height, &channels,
		SOIL_LOAD_AUTO
	);

	GLuint id  = SOIL_create_OGL_texture(image, width, height, channels, 0, SOIL_FLAG_INVERT_Y);

	if(textureID == 0)
		printf("Cannot load Texture:%s\n",filename);

	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	TextureData data;
	data.width = width;
	data.height = height;
	data.texture = id;
	data.channels = channels;
	_texturePool.insert(std::make_pair(filename, data));

	return id;
}