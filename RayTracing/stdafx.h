// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"
typedef unsigned char byte;

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: reference additional headers your program requires here
#include <stdio.h>
#include <tchar.h>
#include <math.h>
#include <array>
#include <sstream>
#include <string>
#include <ctime>

#include <iostream>
#include <fstream>
#include <vector>

#include "Utility.h"
#include "Vector.h"
#include "Point.h"
#include "Ray.h"
#include "Color.h"
#include "Finish.h"
#include "Shape.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Intersection.h"
#include "Light.h"
#include "Window.h"

#include "RayCaster.h"
#include "Parsing.h"