//
//  main.cpp
//  try
//
//  Created by Saeed Boor Boor on 1/21/16.
//  Copyright © 2016 Saeed Boor Boor. All rights reserved.
//


#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>
#include <stdlib.h>
#include "stb_image.h"
#include <assert.h>
#include <math.h>
#include "math_funcs.h"
#include <string>
#include <iostream>
#include <fstream>

// camera matrices. it's easier if they are global
mat4 view_mat;
mat4 proj_mat;
vec3 cam_pos (0.0f, 0.0f, 5.0f);

void create_cube_map (
                      const char* front,
                      const char* back,
                      const char* top,
                      const char* bottom,
                      const char* left,
                      const char* right,
                      GLuint* tex_cube
                      );

bool load_cube_map_side (GLuint texture, GLenum side_target, const char* file_name);
void _update_fps_counter (GLFWwindow* window);
std::string loadShader(std::string fname);

int g_gl_width = 640;
int g_gl_height = 480;
GLFWwindow* window = NULL;

int main () {
    // start GL context and O/S window using the GLFW helper library
    if (!glfwInit ()) {
        fprintf (stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }
    
    // uncomment these lines if on Apple OS X
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow (640, 480, "Hello Triangle", NULL, NULL);
    if (!window) {
        fprintf (stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent (window);
    
    
    // start GLEW extension handler
    glewExperimental = GL_TRUE;
    glewInit ();
    
    // get version info
    const GLubyte* renderer = glGetString (GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString (GL_VERSION); // version as a string
    printf ("Renderer: %s\n", renderer);
    printf ("OpenGL version supported %s\n", version);
    
    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable (GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"
    
    // OTHER STUFF GOES HERE NEXT
    /*float points[] = {
     -10.0f,  10.0f, -10.0f,
     -10.0f, -10.0f, -10.0f,
     10.0f, -10.0f, -10.0f,
     10.0f, -10.0f, -10.0f,
     10.0f,  10.0f, -10.0f,
     -10.0f,  10.0f, -10.0f,
     
     -10.0f, -10.0f,  10.0f,
     -10.0f, -10.0f, -10.0f,
     -10.0f,  10.0f, -10.0f,
     -10.0f,  10.0f, -10.0f,
     -10.0f,  10.0f,  10.0f,
     -10.0f, -10.0f,  10.0f,
     
     10.0f, -10.0f, -10.0f,
     10.0f, -10.0f,  10.0f,
     10.0f,  10.0f,  10.0f,
     10.0f,  10.0f,  10.0f,
     10.0f,  10.0f, -10.0f,
     10.0f, -10.0f, -10.0f,
     
     -10.0f, -10.0f,  10.0f,
     -10.0f,  10.0f,  10.0f,
     10.0f,  10.0f,  10.0f,
     10.0f,  10.0f,  10.0f,
     10.0f, -10.0f,  10.0f,
     -10.0f, -10.0f,  10.0f,
     
     -10.0f,  10.0f, -10.0f,
     10.0f,  10.0f, -10.0f,
     10.0f,  10.0f,  10.0f,
     10.0f,  10.0f,  10.0f,
     -10.0f,  10.0f,  10.0f,
     -10.0f,  10.0f, -10.0f,
     
     -10.0f, -10.0f, -10.0f,
     -10.0f, -10.0f,  10.0f,
     10.0f, -10.0f, -10.0f,
     10.0f, -10.0f, -10.0f,
     -10.0f, -10.0f,  10.0f,
     10.0f, -10.0f,  10.0f
     };*/
    
    float points[] = {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        
        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
        
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
        
        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
        
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };
    
    //points for camera triangle
    GLfloat cam_points[] = {
        0.0f,	0.5f,	0.0f,
        0.5f, -0.5f,	0.0f,
        -0.5f, -0.5f,	0.0f
    };
    

    
    
    
    GLuint vbo;
    glGenBuffers (1, &vbo);
    glBindBuffer (GL_ARRAY_BUFFER, vbo);
    glBufferData (GL_ARRAY_BUFFER, 3 * 36 * sizeof (float), &points, GL_STATIC_DRAW);
    
    GLuint userCam_vbo;
    glGenBuffers (1, &userCam_vbo);
    glBindBuffer (GL_ARRAY_BUFFER, userCam_vbo);
    glBufferData (GL_ARRAY_BUFFER, 9 * sizeof (GLfloat), cam_points,
                  GL_STATIC_DRAW);

    
    GLuint vao;
    glGenVertexArrays (1, &vao);
    glBindVertexArray (vao);
    glEnableVertexAttribArray (0);
    glBindBuffer (GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    GLuint userCam_vao;
    glGenVertexArrays (1, &userCam_vao);
    glBindVertexArray (userCam_vao);
    glEnableVertexAttribArray (0);
    glBindBuffer (GL_ARRAY_BUFFER, userCam_vbo);
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    

    
    //Create Cube map
    GLuint cube_map_texture;
    create_cube_map ("/Users/saeedboorboor/PhD/Projects/spheremapping/spheremapping/negz.jpg", "/Users/saeedboorboor/PhD/Projects/spheremapping/spheremapping/posz.jpg", "/Users/saeedboorboor/PhD/Projects/spheremapping/spheremapping/posy.jpg", "/Users/saeedboorboor/PhD/Projects/spheremapping/spheremapping/negy.jpg", "/Users/saeedboorboor/PhD/Projects/spheremapping/spheremapping/negx.jpg", "/Users/saeedboorboor/PhD/Projects/spheremapping/spheremapping/posx.jpg", &cube_map_texture);
    
    
    const char* vertex_shader =
    "#version 400\n"
    "in vec3 vp;"
    "uniform mat4 P, V;"
    "out vec3 texcoords;"
    "vec3 newP;"
    "void main () {"
    "    texcoords = vp;"
    "    gl_Position = P * V * vec4 (vp, 1.0);"
    "}";
    
    const char* fragment_shader = loadShader("/Users/saeedboorboor/PhD/Projects/spheremapping/spheremapping/fragmentShader.frag").c_str();
    /*"#version 400\n"
    "in vec3 texcoords;"
    "uniform samplerCube cube_texture;"
    "out vec4 frag_colour;"
    "vec4 cubeToLatLon(samplerCube cubemap, vec3 inUV) {"
    "vec3 cubmapTexCoords;"
    //"cubmapTexCoords.x = inUV.x*sqrt(1 - ( (inUV.y * inUV.y)/2 ) - ( (inUV.z * inUV.z)/2 ) + ( ( (inUV.y * inUV.y) * (inUV.z * inUV.z))/3));"
    "cubmapTexCoords.x = inUV.x;"
    //"cubmapTexCoords.y= inUV.y*sqrt(1 - ( (inUV.z * inUV.z)/2 ) - ( (inUV.x * inUV.x)/2 ) + ( ( (inUV.z * inUV.z) * (inUV.x * inUV.x))/3));"
    "cubmapTexCoords.y = inUV.y;"
    "cubmapTexCoords.z = inUV.z*sqrt(1 - ( (inUV.x * inUV.x)/2 ) - ( (inUV.y * inUV.y)/2 ) + ( ( (inUV.x * inUV.x) * (inUV.y * inUV.y))/3));"
    //"cubmapTexCoords.z = inUV.z;"
    "return texture(cubemap, cubmapTexCoords);"
    "}"
    "void main () {"
    //"  frag_colour = texture (cube_texture, texcoords);"
    "  frag_colour = cubeToLatLon (cube_texture, texcoords);"
    "}";*/
    
    
    const char* userCam_vertex_shader =
    "#version 400\n"
    "attribute vec3 vp;"
    "void main () {"
    "	gl_Position = vec4 (vp, 1.0);"
    "}";
    /* the fragment shader colours each fragment (pixel-sized area of the
     triangle) */
    const char* userCam_fragment_shader =
    "#version 400\n"
    "void main () {"
    "	gl_FragColor = vec4 (0.5, 0.0, 0.5, 0.2);"
    "}";
    /* GL shader objects for vertex and fragment shader [components] */
    GLuint userCam_vs, userCam_fs;
    /* GL shader programme object [combined, to link] */
    GLuint userCam_shader_programme;
    
    
    
    userCam_vs = glCreateShader (GL_VERTEX_SHADER);
    glShaderSource (userCam_vs, 1, &userCam_vertex_shader, NULL);
    glCompileShader (userCam_vs);
    userCam_fs = glCreateShader (GL_FRAGMENT_SHADER);
    glShaderSource (userCam_fs, 1, &userCam_fragment_shader, NULL);
    glCompileShader (userCam_fs);
    userCam_shader_programme = glCreateProgram ();
    glAttachShader (userCam_shader_programme, userCam_fs);
    glAttachShader (userCam_shader_programme, userCam_vs);
    glLinkProgram (userCam_shader_programme);
    
    GLuint vs = glCreateShader (GL_VERTEX_SHADER);
    glShaderSource (vs, 1, &vertex_shader, NULL);
    glCompileShader (vs);
    GLuint fs = glCreateShader (GL_FRAGMENT_SHADER);

    glShaderSource (fs, 1, &fragment_shader, NULL);
    glCompileShader (fs);
    
    
    GLuint cube_sp = glCreateProgram ();
    glAttachShader (cube_sp, fs);
    glAttachShader (cube_sp, vs);
    glLinkProgram (cube_sp);
    
    int cube_V_location = glGetUniformLocation (cube_sp, "V");
    int cube_P_location = glGetUniformLocation (cube_sp, "P");
    
    /*-------------------------------CREATE GLOBAL CAMERA--------------------------------*/
    #define ONE_DEG_IN_RAD (2.0 * M_PI) / 360.0 // 0.017444444
    // input variables
    float near = 0.1f; // clipping plane
    float far = 100.0f; // clipping plane
    float fovy = 67.0f; // 67 degrees
    float aspect = (float)g_gl_width / (float)g_gl_height; // aspect ratio
    proj_mat = perspective (fovy, aspect, near, far);
    
    float cam_speed = 3.0f; // 1 unit per second
    float cam_heading_speed = 50.0f; // 30 degrees per second
    float cam_heading = 0.0f; // y-rotation in degrees
    mat4 T = translate (identity_mat4 (), vec3 (-cam_pos.v[0], -cam_pos.v[1], -cam_pos.v[2]));
    mat4 R = rotate_y_deg (identity_mat4 (), -cam_heading);
    versor q = quat_from_axis_deg (-cam_heading, 0.0f, 1.0f, 0.0f);
    view_mat = R * T;
    // keep track of some useful vectors that can be used for keyboard movement
    vec4 fwd (0.0f, 0.0f, -1.0f, 0.0f);
    vec4 rgt (1.0f, 0.0f, 0.0f, 0.0f);
    vec4 up (0.0f, 1.0f, 0.0f, 0.0f);
    
    /*-------------------------------CREATE USER'S CAMERA--------------------------------*/


    float user_cam_speed = 3.0f; // 1 unit per second
    float user_cam_heading_speed = 50.0f; // 30 degrees per second
    float user_cam_heading = 0.0f; // y-rotation in degrees
    mat4 user_T = translate (identity_mat4 (), vec3 (-cam_pos.v[0], -cam_pos.v[1], -cam_pos.v[2]));
    mat4 user_R = rotate_y_deg (identity_mat4 (), -cam_heading);
    versor user_q = quat_from_axis_deg (-user_cam_heading, 0.0f, 1.0f, 0.0f);
    view_mat = R * T;
    // keep track of some useful vectors that can be used for keyboard movement
    vec4 u_fwd (0.0f, 0.0f, -1.0f, 0.0f);
    vec4 u_rgt (1.0f, 0.0f, 0.0f, 0.0f);
    vec4 u_up (0.0f, 1.0f, 0.0f, 0.0f);
    
    /*---------------------------SET RENDERING DEFAULTS---------------------------*/
    glUseProgram (cube_sp);
    glUniformMatrix4fv (cube_V_location, 1, GL_FALSE, R.m);
    glUniformMatrix4fv (cube_P_location, 1, GL_FALSE, proj_mat.m);
    // unique model matrix for each sphere
    mat4 model_mat = identity_mat4 ();
    
    glEnable (GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"
    glEnable (GL_CULL_FACE); // cull face
    glCullFace (GL_BACK); // cull back face
    glFrontFace (GL_CCW); // set counter-clock-wise vertex order to mean the front
    glClearColor (0.2, 0.2, 0.2, 1.0); // grey background to help spot mistakes
    glViewport (0, 0, g_gl_width, g_gl_height);
    
    while (!glfwWindowShouldClose (window)) {
        // update timers
        static double previous_seconds = glfwGetTime ();
        double current_seconds = glfwGetTime ();
        double elapsed_seconds = current_seconds - previous_seconds;
        previous_seconds = current_seconds;
        _update_fps_counter (window);
        
        // wipe the drawing surface clear
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // render a sky-box using the cube-map texture
        glDepthMask (GL_FALSE);
        glUseProgram (cube_sp);
        glActiveTexture (GL_TEXTURE0);
        glBindTexture (GL_TEXTURE_CUBE_MAP, cube_map_texture);
        glBindVertexArray (vao);
        //glBindVertexArray (vao_sphere);
        glDrawArrays (GL_TRIANGLES, 0, 36);
        glDepthMask (GL_TRUE);
        
        // update other events like input handling
        glfwPollEvents ();
        
        // control keys
        bool cam_moved = false;
        vec3 move (0.0, 0.0, 0.0);
        float cam_yaw = 0.0f; // y-rotation in degrees
        float cam_pitch = 0.0f;
        float cam_roll = 0.0;
        if (glfwGetKey (window, GLFW_KEY_A)) {
            move.v[0] -= cam_speed * elapsed_seconds;
            cam_moved = true;
            print(move);
        }
        if (glfwGetKey (window, GLFW_KEY_D)) {
            move.v[0] += cam_speed * elapsed_seconds;
            cam_moved = true;
        }
        if (glfwGetKey (window, GLFW_KEY_Q)) {
            move.v[1] += cam_speed * elapsed_seconds;
            cam_moved = true;
        }
        if (glfwGetKey (window, GLFW_KEY_E)) {
            move.v[1] -= cam_speed * elapsed_seconds;
            cam_moved = true;
        }
        if (glfwGetKey (window, GLFW_KEY_W)) {
            move.v[2] -= cam_speed * elapsed_seconds;
            cam_moved = true;
        }
        if (glfwGetKey (window, GLFW_KEY_S)) {
            move.v[2] += cam_speed * elapsed_seconds;
            cam_moved = true;
        }
        if (glfwGetKey (window, GLFW_KEY_LEFT)) {
            cam_yaw += cam_heading_speed * elapsed_seconds;
            cam_moved = true;
            versor q_yaw = quat_from_axis_deg (
                                               cam_yaw, up.v[0], up.v[1], up.v[2]
                                               );
            q = q_yaw * q;
        }
        if (glfwGetKey (window, GLFW_KEY_RIGHT)) {
            cam_yaw -= cam_heading_speed * elapsed_seconds;
            cam_moved = true;
            versor q_yaw = quat_from_axis_deg (
                                               cam_yaw, up.v[0], up.v[1], up.v[2]
                                               );
            q = q_yaw * q;
        }
        if (glfwGetKey (window, GLFW_KEY_UP)) {
            cam_pitch += cam_heading_speed * elapsed_seconds;
            cam_moved = true;
            versor q_pitch = quat_from_axis_deg (
                                                 cam_pitch, rgt.v[0], rgt.v[1], rgt.v[2]
                                                 );
            q = q_pitch * q;
        }
        if (glfwGetKey (window, GLFW_KEY_DOWN)) {
            cam_pitch -= cam_heading_speed * elapsed_seconds;
            cam_moved = true;
            versor q_pitch = quat_from_axis_deg (
                                                 cam_pitch, rgt.v[0], rgt.v[1], rgt.v[2]
                                                 );
            q = q_pitch * q;
        }
        if (glfwGetKey (window, GLFW_KEY_Z)) {
            cam_roll -= cam_heading_speed * elapsed_seconds;
            cam_moved = true;
            versor q_roll = quat_from_axis_deg (
                                                cam_roll, fwd.v[0], fwd.v[1], fwd.v[2]
                                                );
            q = q_roll * q;
        }
        if (glfwGetKey (window, GLFW_KEY_C)) {
            cam_roll += cam_heading_speed * elapsed_seconds;
            cam_moved = true;
            versor q_roll = quat_from_axis_deg (
                                                cam_roll, fwd.v[0], fwd.v[1], fwd.v[2]
                                                );
            q = q_roll * q;
        }
        // update view matrix
        if (cam_moved) {
            cam_heading += cam_yaw;
            
            // re-calculate local axes so can move fwd in dir cam is pointing
            R = quat_to_mat4 (q);
            fwd = R * vec4 (0.0, 0.0, -1.0, 0.0);
            rgt = R * vec4 (1.0, 0.0, 0.0, 0.0);
            up = R * vec4 (0.0, 1.0, 0.0, 0.0);
            
            cam_pos = cam_pos + vec3 (fwd) * -move.v[2];
            cam_pos = cam_pos + vec3 (up) * move.v[1];
            cam_pos = cam_pos + vec3 (rgt) * move.v[0];
            mat4 T = translate (identity_mat4 (), vec3 (cam_pos));
            
            view_mat = inverse (R) * inverse (T);
            
            // cube-map view matrix has rotation, but not translation
            glUseProgram (cube_sp);
            glUniformMatrix4fv (cube_V_location, 1, GL_FALSE, inverse (R).m);
        }
        
        
        if (GLFW_PRESS == glfwGetKey (window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose (window, 1);
        }
        // put the stuff we've been drawing onto the display
        glfwSwapBuffers (window);
    }
    
    // close GL context and any other GLFW resources
    glfwTerminate();
    return 0;
}


std::string loadShader(std::string fname)
{
    std::string content;
    std::ifstream fileStream(fname, std::ios::in);
    
    if(!fileStream.is_open()) {
        std::cerr << "Could not read file " << fname << ". File does not exist." << std::endl;
        return "";
    }
    
    std::string line = "";
    while(!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");

    }

    //std::cout<<content<<"\n";
    fileStream.close();
    return content;
    //return content.c_str();
}
void create_cube_map (
                      const char* front,
                      const char* back,
                      const char* top,
                      const char* bottom,
                      const char* left,
                      const char* right,
                      GLuint* tex_cube
                      ) {
    // generate a cube-map texture to hold all the sides
    glActiveTexture (GL_TEXTURE0);
    glGenTextures (1, tex_cube);
    
    // load each image and copy into a side of the cube-map texture
    assert (load_cube_map_side (*tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, front));
    assert (load_cube_map_side (*tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, back));
    assert (load_cube_map_side (*tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, top));
    assert (load_cube_map_side (*tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, bottom));
    assert (load_cube_map_side (*tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, left));
    assert (load_cube_map_side (*tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_X, right));
    // format cube map texture
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

bool load_cube_map_side (GLuint texture, GLenum side_target, const char* file_name) {
    
    glBindTexture (GL_TEXTURE_CUBE_MAP, texture);
    
    int x, y, n;
    int force_channels = 4;
    unsigned char*  image_data = stbi_load (file_name, &x, &y, &n, force_channels);
    if (!image_data) {
        fprintf (stderr, "ERROR: could not load %s\n", file_name);
        return false;
    }
    // non-power-of-2 dimensions check
    if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0) {
        fprintf (
                 stderr, "WARNING: image %s is not power-of-2 dimensions\n", file_name
                 );
    }
    
    // copy image data into 'target' side of cube map
    glTexImage2D (
                  side_target,
                  0,
                  GL_RGBA,
                  x,
                  y,
                  0,
                  GL_RGBA,
                  GL_UNSIGNED_BYTE,
                  image_data
                  );
    free (image_data);
    return true;
}

void _update_fps_counter (GLFWwindow* window) {
    static double previous_seconds = glfwGetTime ();
    static int frame_count;
    double current_seconds = glfwGetTime ();
    double elapsed_seconds = current_seconds - previous_seconds;
    if (elapsed_seconds > 0.25) {
        previous_seconds = current_seconds;
        double fps = (double)frame_count / elapsed_seconds;
        char tmp[128];
        sprintf (tmp, "opengl @ fps: %.2f", fps);
        glfwSetWindowTitle (window, tmp);
        frame_count = 0;
    }
    frame_count++;
}