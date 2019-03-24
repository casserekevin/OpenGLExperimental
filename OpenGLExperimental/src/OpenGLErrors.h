#pragma once

//MACRO DE DEBUG
#define ASSERT(x) if(!(x)) __debugbreak();
#define DEBUG_ERROR(x) clearErrors();\
	x;\
	ASSERT(logErrors(#x, __FILE__, __LINE__))

void clearErrors();

bool logErrors(const char* function, const char* file, int line);



