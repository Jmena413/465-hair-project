#include "Hair.h"

#define FONTSTASH_IMPLEMENTATION
#include <fontstash.h>
#define GLFONTSTASH_IMPLEMENTATION
#include <glfontstash.h>

#include <config/VRDataIndex.h>

Hair::Hair(int argc, char** argv) : VRApp(argc, argv)
{
	_turntable.reset(new TurntableManipulator(6.0, 0.3, 0.5));
	_turntable->setCenterPosition(vec3(0.0, 0.0, 0.0));
	_lastTime = 0.0;
}

Hair::~Hair()
{
	glfonsDelete(fs);
	shutdown();
}

void Hair::onAnalogChange(const VRAnalogEvent &event) {
    
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

		glEnable(GL_MULTISAMPLE);
		
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

		reloadShaders();

		std::vector<Mesh::Vertex> cpuVertexArray;
		std::vector<int> cpuIndexArray;
		std::vector<std::shared_ptr<Texture>> textures;

		vec3 eyePos = _turntable->getPos();

		Mesh::Vertex vert;
		vert.position = vec3(0.15, 1, 0);
		vert.normal = -normalize(eyePos - vert.position);
		vert.texCoord0 = glm::vec2(0.15, 1);
		cpuVertexArray.push_back(vert);
		cpuIndexArray.push_back(0);

		vert.position = vec3(0.1, 0.75, 0);
		vert.normal = -normalize(eyePos - vert.position);
		vert.texCoord0 = glm::vec2(0.1, 0.75);
		cpuVertexArray.push_back(vert);
		cpuIndexArray.push_back(1);

		vert.position = vec3(0.05, 0.5, 0);
		vert.normal = -normalize(eyePos - vert.position);
		vert.texCoord0 = glm::vec2(0.05, 0.5);
		cpuVertexArray.push_back(vert);
		cpuIndexArray.push_back(2);

		vert.position = vec3(0.05, 0, 0);
		vert.normal = -normalize(eyePos - vert.position);
		vert.texCoord0 = glm::vec2(0.05, 0);
		cpuVertexArray.push_back(vert);
		cpuIndexArray.push_back(3);

		vert.position = vec3(0.5, 1, 0);
		vert.normal = -normalize(eyePos - vert.position);
		vert.texCoord0 = glm::vec2(0.5, 1);
		cpuVertexArray.push_back(vert);
		cpuIndexArray.push_back(0);

		vert.position = vec3(0.55, 0.75, 0);
		vert.normal = -normalize(eyePos - vert.position);
		vert.texCoord0 = glm::vec2(0.55, 0.75);
		cpuVertexArray.push_back(vert);
		cpuIndexArray.push_back(1);

		vert.position = vec3(0.60, 0.5, 0);
		vert.normal = -normalize(eyePos - vert.position);
		vert.texCoord0 = glm::vec2(0.60, 0.5);
		cpuVertexArray.push_back(vert);
		cpuIndexArray.push_back(2);

		vert.position = vec3(0.60, 0, 0);
		vert.normal = -normalize(eyePos - vert.position);
		vert.texCoord0 = glm::vec2(0.60, 0);
		cpuVertexArray.push_back(vert);
		cpuIndexArray.push_back(3);

		vert.position = vec3(0.3, 1, 0);
		vert.normal = -normalize(eyePos - vert.position);
		vert.texCoord0 = glm::vec2(0.3, 1);
		cpuVertexArray.push_back(vert);
		cpuIndexArray.push_back(0);

		vert.position = vec3(0.35, 0.75, 0);
		vert.normal = -normalize(eyePos - vert.position);
		vert.texCoord0 = glm::vec2(0.35, 0.75);
		cpuVertexArray.push_back(vert);
		cpuIndexArray.push_back(1);

		vert.position = vec3(0.40, 0.5, 0);
		vert.normal = -normalize(eyePos - vert.position);
		vert.texCoord0 = glm::vec2(0.40, 0.5);
		cpuVertexArray.push_back(vert);
		cpuIndexArray.push_back(2);

		vert.position = vec3(0.40, 0, 0);
		vert.normal = -normalize(eyePos - vert.position);
		vert.texCoord0 = glm::vec2(0.40, 0);
		cpuVertexArray.push_back(vert);
		cpuIndexArray.push_back(3);

		vert.position = vec3(0.8, 1, 0);
		vert.normal = -normalize(eyePos - vert.position);
		vert.texCoord0 = glm::vec2(0.8, 1);
		cpuVertexArray.push_back(vert);
		cpuIndexArray.push_back(0);

		vert.position = vec3(1.0, 0.75, 0);
		vert.normal = -normalize(eyePos - vert.position);
		vert.texCoord0 = glm::vec2(1.0, 0.75);
		cpuVertexArray.push_back(vert);
		cpuIndexArray.push_back(1);

		vert.position = vec3(0.5, 0.25, 0);
		vert.normal = -normalize(eyePos - vert.position);
		vert.texCoord0 = glm::vec2(0.5, 0.5);
		cpuVertexArray.push_back(vert);
		cpuIndexArray.push_back(2);

		vert.position = vec3(1.0, 0, 0);
		vert.normal = -normalize(eyePos - vert.position);
		vert.texCoord0 = glm::vec2(1.0, 0);
		cpuVertexArray.push_back(vert);
		cpuIndexArray.push_back(3);

		const int numVertices = cpuVertexArray.size();
		const int cpuVertexByteSize = sizeof(Mesh::Vertex) * numVertices;
		const int cpuIndexByteSize = sizeof(int) * cpuIndexArray.size();
		_mesh.reset(new Mesh(textures, GL_PATCHES, GL_STATIC_DRAW, cpuVertexByteSize, cpuIndexByteSize, 0, cpuVertexArray, cpuIndexArray.size(), cpuIndexByteSize, &cpuIndexArray[0]));

		glPatchParameteri(GL_PATCH_VERTICES, 4);

		initializeText();
    }
}


void Hair::onRenderGraphicsScene(const VRGraphicsState &renderState) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	GLfloat windowHeight = renderState.index().getValue("FramebufferHeight");
	GLfloat windowWidth = renderState.index().getValue("FramebufferWidth");

	glm::mat4 view = _turntable->frame();
	vec3 eyePos = _turntable->getPos();

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), windowWidth / windowHeight, 0.01f, 100.0f);

	glm::mat4 model = glm::mat4(1.0);
    
	_shader.use();
	
	_shader.setUniform("view_mat", view);
	_shader.setUniform("projection_mat", projection);
	
	_shader.setUniform("model_mat", model);
	_shader.setUniform("normal_mat", mat3(transpose(inverse(model))));
	_shader.setUniform("eye_world", eyePos);

	glBindVertexArray(_mesh->getVAOID());
	glDrawArrays(GL_PATCHES, 0, 16);

	double deltaTime = _curFrameTime - _lastTime;
	std::string fps = "FPS: " + std::to_string(1.0/deltaTime);
	drawText(fps, 10, 10, windowHeight, windowWidth);
}

void Hair::drawText(const std::string text, float xPos, float yPos, GLfloat windowHeight, GLfloat windowWidth) {

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
	_shader.compileShader("hair-strand.tesc", GLSLShader::TESS_CONTROL);
	_shader.compileShader("hair-strand.tese", GLSLShader::TESS_EVALUATION);
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
