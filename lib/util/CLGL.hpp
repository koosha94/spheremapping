//
//  CLGL.hpp
//  NGRenderer
//
//  Created by koosha mir on 2/10/16.
//
//

#ifndef CLGL_hpp
#define CLGL_hpp

#if defined(__APPLE__) && defined(__MACH__)
#include <OpenGL/OpenGL.h>
#elif defined(__linux__) || defined(__unix__) || defined(__gnu_linux__)
#include <GL/glx.h>
#elseif defined(_WIN32__) || defined(_WIN32) || defined(_WIN64) || defined(__WINDOWS__) || defined(__TOS_WIN__)
#include <Windows.h>
#endif
#ifdef __APPLE__
#include <OpenCL/opencl.h>
static const char* CL_GL_SHARING_EXT = "cl_APPLE_gl_sharing";
#else
#include <CL/cl_ext.h>
#include <CL/cl_platform.h>
#include <CL/cl.h>
#include <CL/cl_gl.h>
static const char* CL_GL_SHARING_EXT = "cl_khr_gl_sharing";
#endif



class CLGLUtils{
public:
#if defined(__APPLE__) && defined(__MACH__)
    static CGLContextObj gclContex;
#endif
    static cl_context context;
    static void init();
};
#endif /* CLGL_hpp */
