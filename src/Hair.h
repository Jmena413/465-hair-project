#ifndef SHADYBUNNY_H
#define SHADYBUNNY_H


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

class ShadyBunny : public VRApp {
public:
    
    /** The constructor passes argc, argv, and a MinVR config file on to VRApp.
     */
	ShadyBunny(int argc, char** argv);
    virtual ~ShadyBunny();

    
    /** USER INTERFACE CALLBACKS **/
    virtual void onAnalogChange(const VRAnalogEvent &state);
    virtual void onButtonDown(const VRButtonEvent &state);
    virtual void onButtonUp(const VRButtonEvent &state);
	virtual void onCursorMove(const VRCursorEvent &state);
    
    /** RENDERING CALLBACKS **/
    virtual void onRenderGraphicsScene(const VRGraphicsState& state);
    virtual void onRenderGraphicsContext(const VRGraphicsState& state);
    
    
private:
    double _lastTime;
    double _curFrameTime;
    
	virtual void reloadShaders();

	std::shared_ptr<Texture> _diffuseRamp;
	std::shared_ptr<Texture> _specularRamp;

	GLSLProgram _shader;

	std::unique_ptr<Model> _modelMesh;
	std::shared_ptr<TurntableManipulator> _turntable;

	glm::vec4 _lightPosition;
	bool _drawLightVector;
	float _diffuseOnOff;  // 1.0 when on, 0.0 when off
	float _specularOnOff; // 1.0 when on, 0.0 when off
	float _ambientOnOff;  // 1.0 when on, 0.0 when off
};


#endif
