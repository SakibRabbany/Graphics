#pragma once

#include "cs488-framework/CS488Window.hpp"
#include "cs488-framework/OpenGLImport.hpp"
#include "cs488-framework/ShaderProgram.hpp"
#include "cs488-framework/MeshConsolidator.hpp"

#include "SceneNode.hpp"
#include "JointNode.hpp"

#include <glm/glm.hpp>
#include <memory>

struct LightSource {
	glm::vec3 position;
	glm::vec3 rgbIntensity;
};


class A3 : public CS488Window {
public:
	A3(const std::string & luaSceneFile);
	virtual ~A3();

protected:
	virtual void init() override;
	virtual void appLogic() override;
	virtual void guiLogic() override;
	virtual void draw() override;
	virtual void cleanup() override;

	//-- Virtual callback methods
	virtual bool cursorEnterWindowEvent(int entered) override;
	virtual bool mouseMoveEvent(double xPos, double yPos) override;
	virtual bool mouseButtonInputEvent(int button, int actions, int mods) override;
	virtual bool mouseScrollEvent(double xOffSet, double yOffSet) override;
	virtual bool windowResizeEvent(int width, int height) override;
	virtual bool keyInputEvent(int key, int action, int mods) override;

	//-- One time initialization methods:
	void processLuaSceneFile(const std::string & filename);
	void createShaderProgram();
	void enableVertexShaderInputSlots();
	void uploadVertexDataToVbos(const MeshConsolidator & meshConsolidator);
	void mapVboDataToVertexShaderInputLocations();
	void initViewMatrix();
	void initLightSources();

	void initPerspectiveMatrix();
	void uploadCommonSceneUniforms();
	void renderSceneGraph(const SceneNode &node);
	void renderArcCircle();

	glm::mat4 m_perpsective;
	glm::mat4 m_view;

	LightSource m_light;

	//-- GL resources for mesh geometry data:
	GLuint m_vao_meshData;
	GLuint m_vbo_vertexPositions;
	GLuint m_vbo_vertexNormals;
	GLint m_positionAttribLocation;
	GLint m_normalAttribLocation;
	ShaderProgram m_shader;

	//-- GL resources for trackball circle geometry:
	GLuint m_vbo_arcCircle;
	GLuint m_vao_arcCircle;
	GLint m_arc_positionAttribLocation;
	ShaderProgram m_shader_arcCircle;

	// BatchInfoMap is an associative container that maps a unique MeshId to a BatchInfo
	// object. Each BatchInfo object contains an index offset and the number of indices
	// required to render the mesh with identifier MeshId.
	BatchInfoMap m_batchInfoMap;

	std::string m_luaSceneFile;

	std::shared_ptr<SceneNode> m_rootNode;
    
    ///////////////
    // options
    
    bool frontface_culling;
    bool backface_culling;
    bool z_buffer;
    bool circle;
    
    void performFaceCulling();
    void finishFaceCulling();
    
    // user interaction mode
    int user_mode;
    
    // mouse interaction
    bool left_mouse_pressed;
    bool middle_mouse_pressed;
    bool right_mouse_pressed;
    
    glm::vec2 left_initial;
    glm::vec2 middle_initial;
    glm::vec2 right_initial;
    
    glm::vec2 initial_click;
    
    glm::vec2 m_mouse_GL_coordinate;

    glm::vec3 puppet_translation;
    
    glm::mat4 puppet_model_matrix;
    
    JointNode* neck_joint;
    SceneNode* torso;
    
    void resetPosition();
    void resetOrientation();
    
    void updateSceneGraph();
    
    void findJointNode(std::string joint_name, SceneNode* root);
    
    float rot_ang;
    
    void drawPuppet(const SceneNode &node);
    void update(SceneNode* root);
    void updateParentPointers(SceneNode* parent);
    
    //// trackball
    
    glm::vec2 rotation_click;
    glm::vec2 initial_rotation_click;
    
    glm::mat4 initial_torso_transformation;

    
    void vCalcRotVec(float fNewX, float fNewY,
                     float fOldX, float fOldY,
                     float fDiameter,
                     float *fVecX, float *fVecY, float *fVecZ);
    void vAxisRotMatrix(float fVecX, float fVecY, float fVecZ, glm::mat4& mNewMat);
    
    void rotateTorso(float oldx, float newx, float oldy, float newy);

};
