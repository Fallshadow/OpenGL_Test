#pragma once

#include <GL/glew.h>

#ifdef _DEBUG
#define ASSERT(x) if(!x) __debugbreak();
#define GLCall(x) ClearAllError();\
		x;\
		ASSERT(LogError(#x, __FILE__, __LINE__))
#else
#define ASSERT(x) x
#define GLCall(x) x
#endif


void ClearAllError();

bool LogError(const char* function, const char* file, int line);
