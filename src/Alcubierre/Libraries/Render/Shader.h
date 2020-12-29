#pragma once

#include <glad/glad.h>
#include <Alcubierre/Libraries/Utilities/file.h>

class Shader_Loader
{
public:

	static void CheckShader(GLuint shader,bool link_check)
	{
		GLint state;
		char* error = "";
		if (link_check)
		{
			glGetProgramiv(shader, GL_LINK_STATUS,&state);
			if(!state)
			{
				glGetShaderInfoLog(shader, 1024, NULL, (char*)error);
				fprintf(stdout, "SHADER %i FAILED TO LINK: %s", (int)shader, error);
			}
		}
		else {
			glGetProgramiv(shader, GL_COMPILE_STATUS, &state);
			if (!state)
			{
				glGetShaderInfoLog(shader, 1024, NULL, (char*)error);
				fprintf(stdout, "SHADER %i FAILED TO COMPILE: %s", (int)shader, error);
			}
		}
	}

	static GLuint CompileShader(const char* vert_shad_path,const char* frag_shad_path)
	{
		const char* vert_shad_code = util_file::LoadFile_ifstream(vert_shad_path);
		const char* frag_shad_code = util_file::LoadFile_ifstream(frag_shad_path);

		GLuint vert_shad_id, frag_shad_id, shad_id;

		vert_shad_id = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vert_shad_id, 1, &vert_shad_code, NULL);
		glCompileShader(vert_shad_id);
		CheckShader(vert_shad_id, false);

		frag_shad_id = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(frag_shad_id, 1, &frag_shad_code, NULL);
		glCompileShader(frag_shad_id);
		CheckShader(frag_shad_id, false);

		shad_id = glCreateProgram();
		glAttachShader(shad_id, vert_shad_id);
		glAttachShader(shad_id, frag_shad_id);
		glLinkProgram(shad_id);
		CheckShader(shad_id, true);
		glDeleteShader(frag_shad_id);
		glDeleteShader(vert_shad_id);

		return shad_id;
	}
};