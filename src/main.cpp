#include <stdio.h>
#include <stdlib.h>
#include <stddef.h> // offsetof
#include <math.h> // sinf
#include <iostream>

#include "Core.h"
#include "Physics.h"
#include "Graphics.h"

#include <Windows.h>
#include <stdio.h>
int ( WINAPIV * __vsnprintf )( char *, size_t, const char*, va_list ) = _vsnprintf;
int main(int argc, char* argv[])
{
  Core core;
  core.AddSystem(new Physics());
  core.AddSystem(Graphics::Instance());
  core.Run();

  return 0;
}
