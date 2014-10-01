/*
 *  openglincludes.h
 *  modelViewer
 *
 *  Created by Colin Hunt on 14-10-01.
 *  Copyright 2014 Hunt Enterprises. All rights reserved.
 *
 */

#ifndef OPENGLINCLUDES_H
#define OPENGLINCLUDES_H

#ifdef __APPLE__

#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <OpenGL/glext.h>

#else
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/glext.h>
#pragma comment(lib, "glew32.lib") 
#endif

#endif // OPENGLINCLUDES_H