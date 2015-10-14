#pragma once

#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GL/glut.h>
#include <string>

class GPGPU
{
public:
	GPGPU(int w, int h);

	void restart();
	void update(float time,float waveLength1,float velocity1,float amplitude1,float xdir1,float ydir1,
			float waveLength2,float velocity2,float amplitude2,float xdir2,float ydir2,
			float waveLength3,float velocity3,float amplitude3,float xdir3,float ydir3,
			float waveLength4,float velocity4,float amplitude4,float xdir4,float ydir4,int obj);
	void display();
	int loadShader(char* filename, std::string& text);

private:
    int _width;				// width of the screen
	int _height;			// height of the screen
	int _initialized;		// if the cells are initialized (=1) or not (=0)

	GLuint _initializedLoc;
    GLuint _texUnitLoc;
	GLuint _timeLoc;

	GLuint _waveLength1Loc;
	GLuint _velocity1Loc;
	GLuint _amplitude1Loc;
	GLuint _direction1Loc;

	GLuint _waveLength2Loc;
	GLuint _velocity2Loc;
	GLuint _amplitude2Loc;
	GLuint _direction2Loc;

	GLuint _waveLength3Loc;
	GLuint _velocity3Loc;
	GLuint _amplitude3Loc;
	GLuint _direction3Loc;

	GLuint _waveLength4Loc;
	GLuint _velocity4Loc;
	GLuint _amplitude4Loc;
	GLuint _direction4Loc;

	GLuint _objLoc;
    
    GLuint _textureId;		// The texture ID used to store data array
    GLuint _programId;		// the program ID

    GLuint _fragmentShader;
};

