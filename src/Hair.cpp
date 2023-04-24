#include "Hair.h"

#define FONTSTASH_IMPLEMENTATION
#include <fontstash.h>
#define GLFONTSTASH_IMPLEMENTATION
#include <glfontstash.h>

#include <config/VRDataIndex.h>

Hair::Hair(int argc, char** argv) : VRApp(argc, argv)
{
	_turntable.reset(new TurntableManipulator(9.0, 0.3, 0.5));
	_turntable->setCenterPosition(vec3(0.0, 0.0, 0.0));
	_lastTime = 0.0;
}

Hair::~Hair()
{
	glfonsDelete(fs);
	shutdown();
}

void Hair::onAnalogChange(const VRAnalogEvent &event) {
    // This routine is called for all Analog_Change events.  Check event->getName()
    // to see exactly which analog input has been changed, and then access the
    // new value with event->getValue().
    
	if (event.getName() == "FrameStart") {
		_lastTime = _curFrameTime;
		_curFrameTime = event.getValue();
	}
}

void Hair::onButtonUp(const VRButtonEvent& event) {
	_turntable->onButtonUp(event);
}

void Hair::onButtonDown(const VRButtonEvent& event) {
	_turntable->onButtonDown(event);
}


void Hair::onCursorMove(const VRCursorEvent& event) {
	_turntable->onCursorMove(event);
}
    
void Hair::onRenderGraphicsContext(const VRGraphicsState &renderState) {
    // This routine is called once per graphics context at the start of the
    // rendering process.  So, this is the place to initialize textures,
    // load models, or do other operations that you only want to do once per
    // frame.
    
	// Is this the first frame that we are rendering after starting the app?
    if (renderState.isInitialRenderCall()) {

		//For windows, we need to initialize a few more things for it to recognize all of the
		// opengl calls.
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

		glEnable(GL_MULTISAMPLE);

		// This sets the background color that is used to clear the canvas
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

		// This load shaders from disk, we do it once when the program starts up.
		reloadShaders();

		// Make our model objects
		std::vector<Mesh::Vertex> cpuVertexArray;
		std::vector<int> cpuIndexArray;
		std::vector<std::shared_ptr<Texture>> textures;

		vec3 eyePos = _turntable->getPos();

		Mesh::Vertex vert;
		vert.position = vec3(0, 1, 0);
		vert.normal = -normalize(eyePos - vert.position);
		vert.texCoord0 = glm::vec2(0, 1);
		cpuVertexArray.push_back(vert);
		cpuIndexArray.push_back(0);

		vert.position = vec3(0, 0, 0);
		vert.normal = -normalize(eyePos - vert.position);
		vert.texCoord0 = glm::vec2(0, 1);
		cpuVertexArray.push_back(vert);
		cpuIndexArray.push_back(1);

		vert.position = vec3(0.5, 1, 0);
		vert.normal = -normalize(eyePos - vert.position);
		vert.texCoord0 = glm::vec2(0, 1);
		cpuVertexArray.push_back(vert);
		cpuIndexArray.push_back(2);

		vert.position = vec3(0.5, 0, 0);
		vert.normal = -normalize(eyePos - vert.position);
		vert.texCoord0 = glm::vec2(0, 1);
		cpuVertexArray.push_back(vert);
		cpuIndexArray.push_back(3);

		vert.position = vec3(1, 1, 0);
		vert.normal = -normalize(eyePos - vert.position);
		vert.texCoord0 = glm::vec2(0, 1);
		cpuVertexArray.push_back(vert);
		cpuIndexArray.push_back(4);

		vert.position = vec3(1, 0, 0);
		vert.normal = -normalize(eyePos - vert.position);
		vert.texCoord0 = glm::vec2(0, 1);
		cpuVertexArray.push_back(vert);
		cpuIndexArray.push_back(5);

		const int numVertices = cpuVertexArray.size();
		const int cpuVertexByteSize = sizeof(Mesh::Vertex) * numVertices;
		const int cpuIndexByteSize = sizeof(int) * cpuIndexArray.size();
		_mesh.reset(new Mesh(textures, GL_LINES, GL_STATIC_DRAW, cpuVertexByteSize, cpuIndexByteSize, 0, cpuVertexArray, cpuIndexArray.size(), cpuIndexByteSize, &cpuIndexArray[0]));

		initializeText();
    }
}


void Hair::onRenderGraphicsScene(const VRGraphicsState &renderState) {
    // This routine is called once per eye/camera.  This is the place to actually
    // draw the scene.
    
	// clear the canvas and other buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	

	// Setup the projection matrix so that things are rendered in perspective
	GLfloat windowHeight = renderState.index().getValue("FramebufferHeight");
	GLfloat windowWidth = renderState.index().getValue("FramebufferWidth");

	glm::mat4 view = _turntable->frame();
	vec3 eyePos = _turntable->getPos();

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), windowWidth / windowHeight, 0.01f, 100.0f);
	// When we use virtual reality, this will be replaced by:
	// projection = glm::make_mat4(renderState.getProjectionMatrix())
	
	// Setup the model matrix
	glm::mat4 model = glm::mat4(1.0);
    
	// Tell opengl we want to use this specific shader.
	_shader.use();
	
	_shader.setUniform("view_mat", view);
	_shader.setUniform("projection_mat", projection);
	
	_shader.setUniform("model_mat", model);
	_shader.setUniform("normal_mat", mat3(transpose(inverse(model))));
	_shader.setUniform("eye_world", eyePos);

	_mesh->draw(_shader);

	double deltaTime = _curFrameTime - _lastTime;
	std::string fps = "FPS: " + std::to_string(1.0/deltaTime);
	drawText(fps, 10, 10, windowHeight, windowWidth);
}

void Hair::drawText(const std::string text, float xPos, float yPos, GLfloat windowHeight, GLfloat windowWidth) {
	//float lh = 0;
	//fonsVertMetrics(fs, NULL, NULL, &lh);
	//double width = fonsTextBounds(fs, text.c_str(), NULL, NULL) + 40;
	//double height = lh + 40;

	_textShader.use();
	_textShader.setUniform("projection_mat", glm::ortho(0.f, windowWidth, windowHeight, 0.f, -1.f, 1.f));
	_textShader.setUniform("view_mat", glm::mat4(1.0));
	_textShader.setUniform("model_mat", glm::mat4(1.0));
	_textShader.setUniform("lambertian_texture", 0);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	fonsDrawText(fs, xPos, yPos, text.c_str(), NULL);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	_shader.use();

}

void Hair::reloadShaders()
{
	_shader.compileShader("hair-strand.vert", GLSLShader::VERTEX);
	_shader.compileShader("hair-strand.geom", GLSLShader::GEOMETRY);
	_shader.compileShader("hair-strand.frag", GLSLShader::FRAGMENT);
	_shader.link();
	_shader.use();
}

void Hair::initializeText() {
	int fontNormal = FONS_INVALID;
	fs = nullptr;

	fs = glfonsCreate(512, 512, FONS_ZERO_TOPLEFT);
	if (fs == NULL) {
		assert(false);//Could not create stash
	}

	fontNormal = fonsAddFont(fs, "sans", "DroidSansMono.ttf");
	if (fontNormal == FONS_INVALID) {
		assert(false);// Could not add font normal.
	}

	unsigned int white = glfonsRGBA(255, 255, 255, 255);

	fonsClearState(fs);
	fonsSetSize(fs, 20);
	fonsSetFont(fs, fontNormal);
	fonsSetColor(fs, white);
	fonsSetAlign(fs, FONS_ALIGN_LEFT | FONS_ALIGN_TOP);

	_textShader.compileShader("textRendering.vert", GLSLShader::VERTEX);
	_textShader.compileShader("textRendering.frag", GLSLShader::FRAGMENT);
	_textShader.link();
}
