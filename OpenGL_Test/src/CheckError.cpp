#include <iostream>
#include "CheckError.h"

void ClearAllError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool LogError(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[GL Error] : " << error << "\n" << function << "\n" << file << " : " << line << std::endl;
		return false;
	}
	return true;
}