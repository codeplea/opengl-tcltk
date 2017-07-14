/*
 * OpenGL example with C and Tcl/Tk
 * https://codeplea.com/opengl-with-c-and-tcl-tk
 *
 * Copyright (c) 2015-2017 Lewis Van Winkle
 * Linux x11 code provided by Todd Norland
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgement in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */


#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#elif __linux__
#include <X11/Xlib.h>
#include <GL/glx.h>
#endif

#include <tcl.h>
#include <tk.h>
#include <GL/gl.h>


/* OpenGL Hooks*/
#ifdef _WIN32
  HDC dc;
#elif __linux__
  Display *__glDisplay__ = (Display *)NULL;
  Window __glWindow__;
#endif



int SetRenderWindow(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
#ifdef _WIN32

    /* Grab the HWND from Tcl. */
    HWND hwnd;
    Tcl_GetIntFromObj(interp, objv[2], (int*)&hwnd);

    /* Setup OpenGL. */
    dc = GetDC(hwnd);


    /* Windows code, setup OpenGL. */
    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0, sizeof(pfd));
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    GLuint pixelFormat = ChoosePixelFormat(dc, &pfd);
    SetPixelFormat(dc, pixelFormat, &pfd);

    HGLRC rc = wglCreateContext(dc);

    wglMakeCurrent(dc, rc);

#elif __linux__

    Tk_Window    __glTkwin__;
    XVisualInfo *__glVisinfo__ = (XVisualInfo *)NULL;
    int dummy;
    int attribs[] =
        {GLX_USE_GL, GLX_RGBA, GLX_DOUBLEBUFFER, GLX_DEPTH_SIZE, 0};
    GLXContext __glGLXContext__ = (void *)NULL;

    __glTkwin__ = Tk_NameToWindow(interp, Tcl_GetString (objv[1]), Tk_MainWindow(interp));
    if (__glTkwin__ == NULL) {
        fprintf(stderr, "__glTkwin__ is NULL\n" );
        exit(-1);
    }
    Tk_MakeWindowExist( __glTkwin__);

    /* setup X11 implementation of OpenGL */
    __glDisplay__ = Tk_Display(__glTkwin__);
    if (__glDisplay__ == NULL) {
        fprintf(stderr, "__glDisplay__ is NULL\n" );
        exit(-1);
    }
    if (!glXQueryExtension(__glDisplay__, &dummy, &dummy)) {
        fprintf(stderr, "require GLX Extesion\n" );
        exit(-1);
    }
    __glWindow__ = Tk_WindowId( __glTkwin__ );

    __glVisinfo__ =
         glXChooseVisual( __glDisplay__, DefaultScreen(__glDisplay__), attribs);

    if (__glVisinfo__ == NULL) {
        fprintf(stderr, "__glVisinfo__ is NULL\n" );
        exit(-1);
    }

    __glGLXContext__ =
         glXCreateContext(__glDisplay__, __glVisinfo__, 0, GL_TRUE);
    if (__glGLXContext__ == NULL) {
        fprintf(stderr, "__glGLXContext__ is NULL\n" );
        exit(-1);
    }
    glXMakeCurrent( __glDisplay__, __glWindow__, __glGLXContext__ );

#endif

    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(1.0, 1.0, 1.0, 0.0f);
    glClearDepth(1.0);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glMatrixMode(GL_PROJECTION);

    return TCL_OK;
}


int Resize(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    int w, h;
    Tcl_GetIntFromObj(interp, objv[1], &w);
    Tcl_GetIntFromObj(interp, objv[2], &h);
    /* printf("Resized to: %d x %d.\n", w, h); */

    float dx = (float)w/h;
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-2*dx, 2*dx, -2, 2, -2, 2);

    return TCL_OK;
}


void Loop(ClientData cd)
{
    Tcl_Interp *interp = (Tcl_Interp*)cd;
    Tcl_CreateTimerHandler(25, Loop, interp);

    static float rot = 0;
    rot += .5;

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(rot, 1.0f, 1.0f, 0.5f);


    /* Draw a simple cube. */
    glBegin(GL_QUADS);

    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f( 1.0f, 1.0f, 1.0f);
    glVertex3f( 1.0f, 1.0f,-1.0f);
    glVertex3f(-1.0f, 1.0f,-1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    glColor3f(1.0f, 0.5f, 0.0f);
    glVertex3f( 1.0f,-1.0f, 1.0f);
    glVertex3f(-1.0f,-1.0f, 1.0f);
    glVertex3f(-1.0f,-1.0f,-1.0f);
    glVertex3f( 1.0f,-1.0f,-1.0f);

    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f( 1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f,-1.0f, 1.0f);
    glVertex3f( 1.0f,-1.0f, 1.0f);

    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f( 1.0f, 1.0f,-1.0f);
    glVertex3f( 1.0f,-1.0f,-1.0f);
    glVertex3f(-1.0f,-1.0f,-1.0f);
    glVertex3f(-1.0f, 1.0f,-1.0f);

    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f,-1.0f);
    glVertex3f(-1.0f,-1.0f,-1.0f);
    glVertex3f(-1.0f,-1.0f, 1.0f);

    glColor3f(1.0f, 0.0f, 1.0f);
    glVertex3f( 1.0f, 1.0f, 1.0f);
    glVertex3f( 1.0f,-1.0f, 1.0f);
    glVertex3f( 1.0f,-1.0f,-1.0f);
    glVertex3f( 1.0f, 1.0f,-1.0f);

    glEnd();

    glFlush();

#ifdef _WIN32
    SwapBuffers(dc);
#elif __linux__
    glXSwapBuffers(__glDisplay__, __glWindow__);
#endif
}


void Cleanup(ClientData cd)
{
    //Any cleanup code here.
    printf("Done\n");
}


int Init(Tcl_Interp *interp)
{
    Tcl_Init(interp);
    Tk_Init(interp);

    Tcl_CreateObjCommand(interp, "SetRenderWindow", SetRenderWindow, 0, 0);
    Tcl_CreateObjCommand(interp, "Resize", Resize, 0, 0);

    Tcl_CreateTimerHandler(10, Loop, interp);
    Tcl_CreateExitHandler(Cleanup, 0);

    return TCL_OK;
}


int main(int argc, char *argv[])
{
    char *p[] = {"OpenGL Test", "gui.tcl"};
    Tk_Main(2, p, Init);

    return 0;
}
