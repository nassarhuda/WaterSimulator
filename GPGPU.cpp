#include "GPGPU.h"
#include <iostream>
#include <fstream>

GPGPU::GPGPU(int w, int h) : _initialized(0), _width(w), _height(h)
{
    // Create a texture to store the framebuffer
    glGenTextures(1, &_textureId);
    glBindTexture(GL_TEXTURE_2D, _textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGB, GL_FLOAT, 0);

    _programId = glCreateProgram();

	// Load fragment shader which will be used as computational kernel
	std::string edgeFragSource2;
	loadShader("water.glsl", edgeFragSource2);

    // Create the fragment program
    _fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* source = edgeFragSource2.c_str();
    glShaderSource(_fragmentShader, 1, &source, NULL);
    glCompileShader(_fragmentShader);
    glAttachShader(_programId, _fragmentShader);

    // Link the shader into a complete GLSL program.
    glLinkProgram(_programId);

	// Check program
	{
		int infologLength = 0;
		glGetProgramiv(_programId, GL_INFO_LOG_LENGTH, &infologLength);
		if (infologLength > 0) {
			char *infoLog = (char *)malloc(infologLength);
			glGetProgramInfoLog(_programId, infologLength, NULL, infoLog);
			printf("%s\n",infoLog);
			free(infoLog);
		}
	}

    // Get location of the uniform variables
	_timeLoc = glGetUniformLocation(_programId,"time");
    _texUnitLoc = glGetUniformLocation(_programId, "texUnit");
	_initializedLoc = glGetUniformLocation(_programId, "initialized");

	_waveLength1Loc = glGetUniformLocation(_programId,"waveLength1");
	_velocity1Loc = glGetUniformLocation(_programId,"velocity1");
	_amplitude1Loc = glGetUniformLocation(_programId,"amplitude1");
	_direction1Loc = glGetUniformLocation(_programId,"direction1");

	_waveLength2Loc = glGetUniformLocation(_programId,"waveLength2");
	_velocity2Loc = glGetUniformLocation(_programId,"velocity2");
	_amplitude2Loc = glGetUniformLocation(_programId,"amplitude2");
	_direction2Loc = glGetUniformLocation(_programId,"direction2");

	_waveLength3Loc = glGetUniformLocation(_programId,"waveLength3");
	_velocity3Loc = glGetUniformLocation(_programId,"velocity3");
	_amplitude3Loc = glGetUniformLocation(_programId,"amplitude3");
	_direction3Loc = glGetUniformLocation(_programId,"direction3");

	_waveLength4Loc = glGetUniformLocation(_programId,"waveLength4");
	_velocity4Loc = glGetUniformLocation(_programId,"velocity4");
	_amplitude4Loc = glGetUniformLocation(_programId,"amplitude4");
	_direction4Loc = glGetUniformLocation(_programId,"direction4");
	_objLoc = glGetUniformLocation(_programId,"obj");


}

void GPGPU::restart()
{
	_initialized = 0;
}

void GPGPU::update(float time,float waveLength1,float velocity1,float amplitude1,float xdir1,float ydir1,
			float waveLength2,float velocity2,float amplitude2,float xdir2,float ydir2,
			float waveLength3,float velocity3,float amplitude3,float xdir3,float ydir3,
			float waveLength4,float velocity4,float amplitude4,float xdir4,float ydir4,int obj)
{

	//time++;
	//printf("time is %f\n",time);
    // Backup the viewport dimensions
    int vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);

    glViewport(0, 0, _width, _height);
        
    // Copy the results to the texture
	glBindTexture(GL_TEXTURE_2D, _textureId);

	glUseProgram(_programId);
	
	
	glUniform1f(_timeLoc,time);
	glUniform1f(_waveLength1Loc,waveLength1);
	glUniform1f(_velocity1Loc,velocity1);
	glUniform1f(_amplitude1Loc,amplitude1);
	float directions1[2] = {xdir1,ydir1};
	glUniform2fv(_direction1Loc,1,directions1);

	glUniform1f(_waveLength2Loc,waveLength2);
	glUniform1f(_velocity2Loc,velocity2);
	glUniform1f(_amplitude2Loc,amplitude2);
	float directions2[2] = {xdir2,ydir2};
	glUniform2fv(_direction2Loc,1,directions2);

	glUniform1f(_waveLength3Loc,waveLength3);
	glUniform1f(_velocity3Loc,velocity3);
	glUniform1f(_amplitude3Loc,amplitude3);
	float directions3[2] = {xdir3,ydir3};
	glUniform2fv(_direction3Loc,1,directions3);

	glUniform1f(_waveLength4Loc,waveLength4);
	glUniform1f(_velocity4Loc,velocity4);
	glUniform1f(_amplitude4Loc,amplitude4);
	float directions4[2] = {xdir4,ydir4};
	glUniform2fv(_direction4Loc,1,directions4);
	glUniform1i(_objLoc,obj);
	

    glUniform1i(_texUnitLoc, 0);
	glUniform1i(_initializedLoc, _initialized);
	_initialized = 1;

	// By drawing a quad, the fragment shader will be called for each pixel.
    glBegin(GL_QUADS);
    {            
        glTexCoord2f(0, 0); glVertex3f(-1, -1, -0.5f);
        glTexCoord2f(1, 0); glVertex3f( 1, -1, -0.5f);
        glTexCoord2f(1, 1); glVertex3f( 1,  1, -0.5f);
        glTexCoord2f(0, 1); glVertex3f(-1,  1, -0.5f);
    }
    glEnd();

    glUseProgram(0);
        
    // Copy the rendered image to the texture so that we can start from the current state in the next iteration
    glBindTexture(GL_TEXTURE_2D, _textureId);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, _width, _height);
        
    // Restore the stored viewport dimensions
	glViewport(vp[0], vp[1], vp[2], vp[3]);
}

void GPGPU::display()
{
    // Bind the texture
    glBindTexture(GL_TEXTURE_2D, _textureId);
    glEnable(GL_TEXTURE_2D);

    // Render the result image.
    glBegin(GL_QUADS);
    {
        glTexCoord2f(0, 0); glVertex3f(-1, -1, -0.5f);
        glTexCoord2f(1, 0); glVertex3f( 1, -1, -0.5f);
        glTexCoord2f(1, 1); glVertex3f( 1,  1, -0.5f);
        glTexCoord2f(0, 1); glVertex3f(-1,  1, -0.5f);
    }
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

int GPGPU::loadShader(char* filename, std::string& text)
{
	std::ifstream ifs;
	ifs.open(filename, std::ios::in);

	std::string line;
	while (ifs.good()) {
        getline(ifs, line);

		text += line + "\n";
    }

	return 0;
}