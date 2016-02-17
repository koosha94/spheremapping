//
//  CLGL.cpp
//  NGRenderer
//
//  Created by koosha mir on 2/10/16.
//
//

#include "CLGL.hpp"

#if defined(__APPLE__) && defined(__MACH__)
CGLContextObj CLGLUtils::gclContex;
#endif
cl_context CLGLUtils::context;

void CLGLUtils::init(){
    
    
    //--------------------------------------start of CGL code----------------------------------------------
#if defined(__APPLE__) && defined(__MACH__)
    gclContex = CGLGetCurrentContext();
    CGLShareGroupObj sharegroup = CGLGetShareGroup(gclContex);
    cl_context_properties properties[] =
    {
        CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE,
        (cl_context_properties) sharegroup,
        0
    };
    context = clCreateContext(properties, 0, 0, NULL, 0, 0);
    gcl_gl_set_sharegroup(sharegroup);
    //----------------------------------------end of CGL code----------------------------------------------
    //--------------------------------------start of GLX code----------------------------------------------
#elif defined(__linux__) || defined(__unix__) || defined(__gnu_linux__)
    cl_context_properties properties[] = { CL_GL_CONTEXT_KHR,
        ( cl_context_properties ) glXGetCurrentContext(), // GLX Context
        CL_GLX_DISPLAY_KHR, ( cl_context_properties ) glXGetCurrentDisplay(), // GLX Display
        CL_CONTEXT_PLATFORM, ( cl_context_properties ) platform_id, // OpenCL platform
        0
    };
    // Find CL capable devices in the current GL context
    devices[32];
    size_t size;
    clGetGLContextInfoKHR_fn _clGetGLContextInfoKHR = ( clGetGLContextInfoKHR_fn ) clGetExtensionFunctionAddress ( "clGetGLContextInfoKHR" );
    //  clGetGLContextInfoKHR = (clGetGLContextInfoKHR_fn)clGetExtensionFunctionAddress("clGetGLContextInfoKHR");
    _clGetGLContextInfoKHR ( properties, CL_DEVICES_FOR_GL_CONTEXT_KHR,
                            32 * sizeof ( cl_device_id ), devices, &size );
    // Create a context using the supported devices
    count = size / sizeof ( cl_device_id );
    context = clCreateContext ( properties, count, devices, NULL, 0, 0 );
    //----------------------------------------end of GLX code----------------------------------------------
    //--------------------------------------start of WGL code----------------------------------------------
    
#elif defined(_WIN32__) || defined(_WIN32) || defined(_WIN64) || defined(__WINDOWS__) || defined(__TOS_WIN__)
    // Create CL context properties, add WGL context & handle to DC
    cl_context_properties properties[] = {
        CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(), // WGL Context
        CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(), // WGL HDC
        CL_CONTEXT_PLATFORM, (cl_context_properties)platform_id, // OpenCL platform
        0
    };
    // Find CL capable devices in the current GL context
    devices[32];
    size_t size;
    clGetGLContextInfoKHR_fn _clGetGLContextInfoKHR = ( clGetGLContextInfoKHR_fn ) clGetExtensionFunctionAddress ( "clGetGLContextInfoKHR" );
    _clGetGLContextInfoKHR(properties, CL_DEVICES_FOR_GL_CONTEXT_KHR, 32 * sizeof(cl_device_id), devices, &size);
    // Create a context using the supported devices
    count = size / sizeof(cl_device_id);
    context = clCreateContext(properties, count, devices, NULL, 0, 0);
#endif
    //----------------------------------------end of WGL code----------------------------------------------
    //------------------------------------creating an openGL texture-----------------------------------

}