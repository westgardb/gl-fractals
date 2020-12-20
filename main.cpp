/*-----------------------

OpenGL Fractal Generator
@file main.cpp
@author Ben Westgard
@date Dec 16, 2020

-----------------------*/

#include <cstdio>
#include <cmath>
#include <iostream>
#include <fstream>

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "debug.hpp"

const int WIDTH = 1000;
const int HEIGHT = 1000;

struct vec2
{
	float x, y;
};

char* LoadShaderFile(const std::string& filepath)
{
	std::ifstream ifs(filepath, std::ifstream::ate);
	if (!ifs)
	{
		printf("Failed to open file!\n");
		return NULL;
	}
	int length = ifs.tellg();
	ifs.seekg(0, ifs.beg);
	char* buffer = new char[length+1];
	ifs.read(buffer, length);
	ifs.close();
	if(buffer[length] != '\0') 
	{
		buffer[length] = '\0';
	}
	return buffer;
}

int main()
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow(
		"OpenGL Fractal Generator",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WIDTH,
		HEIGHT,
		SDL_WINDOW_OPENGL 
	);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
	SDL_GLContext glcontext = SDL_GL_CreateContext(window);
	SDL_GL_SetSwapInterval(1);	// limits the frame swap interval to the refresh rate

	if (glewInit() != GLEW_OK)
	{
		std::cerr << "ERROR: GLEW failed to initialize!\n";
		return 1;
	}

	glEnable(GL_DEBUG_OUTPUT | GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(MessageCallback, NULL);

	int vertices[] = {
		-1, -1,
		 1, -1,
		 1,  1,
		-1, -1,
		 1,  1,
		-1,  1,

	};

	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(int), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_INT, GL_FALSE, 0, NULL);

	const char* vssource = LoadShaderFile("shaders/basic.vert");
	unsigned int VertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShader, 1, &vssource, NULL);
	glCompileShader(VertexShader);
	
	const char* fssource = LoadShaderFile("shaders/basic.frag");
	unsigned int FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShader, 1, &fssource, NULL);
	glCompileShader(FragmentShader);

	unsigned int ShaderProgram = glCreateProgram();
	glAttachShader(ShaderProgram, VertexShader);
	glAttachShader(ShaderProgram, FragmentShader);
	glLinkProgram(ShaderProgram);

	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader);

	glUseProgram(ShaderProgram);

	float scale = 1.6f;

	vec2 c = {-0.749f, 0.198f};
	vec2 center = {0.0f, 0.0f};
	vec2 winSize = {WIDTH, HEIGHT};

	unsigned int scale_location = glGetUniformLocation(ShaderProgram, "scale");
	unsigned int c_location = glGetUniformLocation(ShaderProgram, "c");
	unsigned int center_location = glGetUniformLocation(ShaderProgram, "center");
	unsigned int winSize_location = glGetUniformLocation(ShaderProgram, "winSize");

	//---------DRAW------------
	auto Draw = [&] () {
		glUniform1f(scale_location, scale);
		glUniform2f(c_location, c.x, c.y);
		glUniform2f(center_location, center.x, center.y);
		glUniform2f(winSize_location, winSize.x, winSize.y);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		SDL_GL_SwapWindow(window);
	};

	Draw();

	float iterator = 0.0f;
	bool window_alive = true;
	while(window_alive)
	{
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym)
					{
						case 'q':
							window_alive = false;
							break;
						case 'i': c.y += 0.005f; Draw(); break;
						case 'j': c.x -= 0.005f; Draw(); break;
						case 'k': c.y -= 0.005f; Draw(); break;
						case 'l': c.x += 0.005f; Draw(); break;

						case 'z': scale *= 0.95f; Draw(); break;
						case 'x': scale *= 1.05f; Draw(); break;

						case SDLK_UP:    center.y += 0.03f * scale; Draw(); break;
						case SDLK_DOWN:  center.y -= 0.03f * scale; Draw(); break;
						case SDLK_LEFT:  center.x -= 0.03f * scale; Draw(); break;
						case SDLK_RIGHT: center.x += 0.03f * scale;	Draw(); break;
					}
					break;

				case SDL_QUIT:
					window_alive = false;
					break;
			}
		}

		//cy = 0.365 + 0.02f * sin(iterator);
		//Draw();

		iterator += 0.003f;
	}

	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
