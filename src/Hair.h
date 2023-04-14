#ifndef HAIR_H
#define HAIR_H

#include <api/MinVR.h>
using namespace MinVR;

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "TurntableManipulator.h"

#ifdef _WIN32
#include "GL/glew.h"
#include "GL/wglew.h"
#elif (!defined(__APPLE__))
#include "GL/glxew.h"
#endif

// OpenGL Headers
#if defined(WIN32)
#define NOMINMAX
#include <windows.h>
#include <GL/gl.h>
#elif defined(__APPLE__)
#define GL_GLEXT_PROTOTYPES
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#else
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#endif

#include <BasicGraphics.h>
using namespace basicgraphics;

class Hair : public VRApp {
public:
    
    /** The constructor passes argc, argv, and a MinVR config file on to VRApp.
     */
	Hair(int argc, char** argv);
    virtual ~Hair();

    
    /** USER INTERFACE CALLBACKS **/
    virtual void onAnalogChange(const VRAnalogEvent &state);
    virtual void onButtonUp(const VRButtonEvent &state);
    virtual void onButtonDown(const VRButtonEvent& event);
	virtual void onCursorMove(const VRCursorEvent &state);
    
    /** RENDERING CALLBACKS **/
    virtual void onRenderGraphicsScene(const VRGraphicsState& state);
    virtual void onRenderGraphicsContext(const VRGraphicsState& state);
    
    
private:

    std::unique_ptr<basicgraphics::Mesh> _mesh;

	double _lastTime;
	double _curFrameTime;

	virtual void reloadShaders();
	GLSLProgram _shader;

    std::shared_ptr<TurntableManipulator> _turntable;

	void initializeText();
	void drawText(const std::string text, float xPos, float yPos, GLfloat windowHeight, GLfloat windowWidth);
	struct FONScontext* fs;
	GLSLProgram _textShader;
};


#endif //HAIR_H
