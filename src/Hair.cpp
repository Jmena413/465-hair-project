#include "Hair.h"
using namespace basicgraphics;

#include <config/VRDataIndex.h>

Hair::Hair(int argc, char** argv) : VRApp(argc, argv)
{
    _turntable.reset(new TurntableManipulator(3, 0.3, 0.5));
    _turntable->setCenterPosition(vec3(-0.3, 0.8, 0));

    _drawLightVector = true;
    _ambientOnOff = 1.0;
    _diffuseOnOff = 1.0;
    _specularOnOff = 1.0;
    _lastTime = 0.0;
    _curFrameTime = 0.0;

}

Hair::~Hair()
{
	shutdown();
}

void Hair::onAnalogChange(const VRAnalogEvent &state) {
    // This routine is called for all Analog_Change events.  Check event->getName()
    // to see exactly which analog input has been changed, and then access the
    // new value with event->getValue().
    
	if (state.getName() == "FrameStart") {
		_lastTime = _curFrameTime;
		_curFrameTime = state.getValue();
		// Calculate model matrix based on time
		float scaledTime = 0.5f*_curFrameTime;

		// Make the light orbit around the bunny so we can see the lighting change in response to the light position
		float radius = 5.0;
		_lightPosition = vec4(cos(scaledTime*0.6)*sin(scaledTime*0.5)*radius,
			cos(scaledTime*0.3)*sin(scaledTime*0.2)*radius,
			cos(scaledTime*0.1)*sin(scaledTime*0.4)*radius,
			1.0);
	}
}


void Hair::onButtonDown(const VRButtonEvent &event) {
    _turntable->onButtonDown(event);
    string name = event.getName();
    if (name == "KbdL_Down") {
        _drawLightVector = !_drawLightVector; // Toggle drawing the vector to the light on or off
    }
    // Press S to toggle specular lighting on/off
    else if (name == "KbdS_Down") {
        if (_specularOnOff == 1.0) {
            _specularOnOff = 0.0;
        }
        else {
            _specularOnOff = 1.0;
        }
    }
    // Press D to toggle diffuse lighting on/off
    else if (name == "KbdD_Down") {
        if (_diffuseOnOff == 1.0) {
            _diffuseOnOff = 0.0;
        }
        else {
            _diffuseOnOff = 1.0;
        }
    }
    // Press A to toggle ambient lighting on/off
    else if (name == "KbdA_Down") {
        if (_ambientOnOff == 1.0) {
            _ambientOnOff = 0.0;
        }
        else {
            _ambientOnOff = 1.0;
        }
    }
}


void Hair::onButtonUp(const VRButtonEvent &event) {
    _turntable->onButtonUp(event);
}


void Hair::onCursorMove(const VRCursorEvent &event) {
    _turntable->onCursorMove(event);
}

void Hair::reloadShaders()
{
	_shader.compileShader("BlinnPhong.vert", GLSLShader::VERTEX);
	_shader.compileShader("BlinnPhong.frag", GLSLShader::FRAGMENT);
	_shader.link();
	_shader.use();
}
    
void Hair::onRenderGraphicsContext(const VRGraphicsState &renderState) {
    // This routine is called once per graphics context at the start of the
    // rendering process.  So, this is the place to initialize textures,
    // load models, or do other operations that you only want to do once per
    // frame when in stereo mode.
    
    if (renderState.isInitialRenderCall()) {

#ifndef __APPLE__
		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			std::cout << "Error initializing GLEW." << std::endl;
		}
#endif     


        glEnable(GL_DEPTH_TEST);
        glClearDepth(1.0f);
        glDepthFunc(GL_LEQUAL);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

		// This load shaders from disk, we do it once when the program starts up.
		reloadShaders();

		// This loads the model from a file and initializes an instance of the model class to store it
		_modelMesh.reset(new Model("bunny.obj", 1.0, vec4(1.0)));

		//Loading textures
		_diffuseRamp = Texture::create2DTextureFromFile("lightingToon.jpg");
		_diffuseRamp->setTexParameteri(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		_diffuseRamp->setTexParameteri(GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		_specularRamp = Texture::create2DTextureFromFile("lightingToon.jpg");
		_specularRamp->setTexParameteri(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		_specularRamp->setTexParameteri(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
}

void Hair::onRenderGraphicsScene(const VRGraphicsState &renderState) {
    // This routine is called once per eye.  This is the place to actually
    // draw the scene.
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
	// Setup the camera with a good initial position and view direction to see the table
	glm::mat4 model(1.0);
	_shader.use(); // Tell opengl we want to use this specific shader.
	
	GLfloat windowHeight = renderState.index().getValue("FramebufferHeight");
	GLfloat windowWidth = renderState.index().getValue("FramebufferWidth");

    glm::mat4 view = _turntable->frame();;
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), windowWidth / windowHeight, 0.1f, 100.0f);
	_shader.setUniform("view_mat", view);
	_shader.setUniform("projection_mat", projection);
	
	_shader.setUniform("model_mat", model);
	_shader.setUniform("normal_mat", mat3(transpose(inverse(model))));
	vec3 eyePosition = _turntable->getPos();
	_shader.setUniform("eye_world", eyePosition);


	// Properties of the material the model is made out of (the "K" terms in the equations discussed in class)
	// These values should make the model look like it is made out of a metal, like brass
	/*
	vec3 ambientReflectionCoeff = vec3(0.4125, 0.275, 0.0375);
	vec3 diffuseReflectionCoeff = vec3(0.78, 0.57, 0.11);
	vec3 specularReflectionCoeff = vec3(0.99, 0.94, 0.80);
	float specularExponent = 27.9;
	*/
	

	// For toon shading, you want all the color to come from the texture, so you can just use a white bunny like this:

	vec3 ambientReflectionCoeff = vec3(1, 1, 1);
	vec3 diffuseReflectionCoeff = vec3(1, 1, 1);
	vec3 specularReflectionCoeff = vec3(1, 1, 1);
	float specularExponent = 50.0;


	// Properties of the light source (the "I" terms in the equations discussed in class)
	// These values are for a white light so the r,g,b intensities are all the same
	// Note: lightPosition is another important property of the light; it is set at the top of the method
	vec3 ambientLightIntensity = vec3(0.4, 0.4, 0.4);
	vec3 diffuseLightIntensity = vec3(0.6, 0.6, 0.6);
	vec3 specularLightIntensity = vec3(1.0, 1.0, 1.0);

	// Multiply these light intensities by the OnOff terms below to turn each commonent on the lighting on/off based on keystrokes
	ambientLightIntensity *= _ambientOnOff;
	diffuseLightIntensity *= _diffuseOnOff;
	specularLightIntensity *= _specularOnOff;


	// TODO: Pass these parameters into your shader programs... in shader programs these are called "uniform variables"

	_shader.setUniform("ambientReflectionCoeff", ambientReflectionCoeff);
	_shader.setUniform("diffuseReflectionCoeff", diffuseReflectionCoeff);
	_shader.setUniform("specularReflectionCoeff", specularReflectionCoeff);
	_shader.setUniform("specularExponent", specularExponent);
    
	_shader.setUniform("lightPosition", _lightPosition);
	_shader.setUniform("ambientLightIntensity", ambientLightIntensity);
	_shader.setUniform("diffuseLightIntensity", diffuseLightIntensity);
	_shader.setUniform("specularLightIntensity", specularLightIntensity);

	/*_shader.setUniform("diffuseRamp", _diffuseRamp);
	_shader.setUniform("specularRamp", _specularRamp);*/

    
    
    
    
    

	// Draw the model
	_modelMesh->draw(_shader);

	
	// For debugging purposes, let's draw a sphere to reprsent each "light bulb" in the scene, that way
	// we can make sure the lighting on the bunny makes sense given the position of each light source.
	Sphere s(vec3(_lightPosition), 0.1f, vec4(1.0f, 1.0f, 0.0f, 1.0f));
	s.draw(_shader, glm::mat4(1.0));

	// Another useful aid for debugging: draw vectors to the light sources
	if (_drawLightVector) {
		Cylinder l(vec3(-0.3, 0.8, 0), vec3(_lightPosition), 0.01f, vec4(1.0f, 1.0f, 0.0f, 1.0f));
		l.draw(_shader, glm::mat4(1.0));
	}
}
