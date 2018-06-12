#include "A3.hpp"
#include "scene_lua.hpp"
using namespace std;

#include "cs488-framework/GlErrorCheck.hpp"
#include "cs488-framework/MathUtils.hpp"
#include "GeometryNode.hpp"
#include "JointNode.hpp"

#include <imgui/imgui.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <math.h>

using namespace glm;

static bool show_gui = true;

const size_t CIRCLE_PTS = 48;


//----------------------------------------------------------------------------------------
// Constructor
A3::A3(const std::string & luaSceneFile)
	: m_luaSceneFile(luaSceneFile),
	  m_positionAttribLocation(0),
	  m_normalAttribLocation(0),
	  m_vao_meshData(0),
	  m_vbo_vertexPositions(0),
	  m_vbo_vertexNormals(0),
	  m_vao_arcCircle(0),
	  m_vbo_arcCircle(0)
{

}

//----------------------------------------------------------------------------------------
// Destructor
A3::~A3()
{

}

//----------------------------------------------------------------------------------------
/*
 * Called once, at program start.
 */
void A3::init()
{
	// Set the background colour.
	glClearColor(0.35, 0.35, 0.35, 1.0);

	createShaderProgram();

	glGenVertexArrays(1, &m_vao_arcCircle);
	glGenVertexArrays(1, &m_vao_meshData);
	enableVertexShaderInputSlots();

	processLuaSceneFile(m_luaSceneFile);

	// Load and decode all .obj files at once here.  You may add additional .obj files to
	// this list in order to support rendering additional mesh types.  All vertex
	// positions, and normals will be extracted and stored within the MeshConsolidator
	// class.
	unique_ptr<MeshConsolidator> meshConsolidator (new MeshConsolidator{
			getAssetFilePath("cube.obj"),
			getAssetFilePath("sphere.obj"),
			getAssetFilePath("suzanne.obj")
	});


	// Acquire the BatchInfoMap from the MeshConsolidator.
	meshConsolidator->getBatchInfoMap(m_batchInfoMap);

	// Take all vertex data within the MeshConsolidator and upload it to VBOs on the GPU.
	uploadVertexDataToVbos(*meshConsolidator);

	mapVboDataToVertexShaderInputLocations();

	initPerspectiveMatrix();

	initViewMatrix();

	initLightSources();
    
    puppet_model_matrix = mat4(1);

    m_rootNode->updateWorldMatrix(puppet_model_matrix);
    
    frontface_culling = false;
    backface_culling = false;
    z_buffer = true;
    circle = false;
    user_mode = 0;
    
    left_mouse_pressed = false;
    middle_mouse_pressed = false;
    right_mouse_pressed = false;
    picking_enabled = false;
    
    puppet_translation = vec3(0,0,0);
    
    findJointNode("neck_joint", m_rootNode.get());
    
    initial_torso_transformation = torso->trans;
    
    updateParentPointers(nullptr, m_rootNode.get());
    
    populateNodeVector(m_rootNode.get());
    
    rot_ang = 0.3;

	// Exiting the current scope calls delete automatically on meshConsolidator freeing
	// all vertex data resources.  This is fine since we already copied this data to
	// VBOs on the GPU.  We have no use for storing vertex data on the CPU side beyond
	// this point.
}

//----------------------------------------------------------------------------------------
void A3::processLuaSceneFile(const std::string & filename) {
	// This version of the code treats the Lua file as an Asset,
	// so that you'd launch the program with just the filename
	// of a puppet in the Assets/ directory.
	// std::string assetFilePath = getAssetFilePath(filename.c_str());
	// m_rootNode = std::shared_ptr<SceneNode>(import_lua(assetFilePath));

	// This version of the code treats the main program argument
	// as a straightforward pathname.
	m_rootNode = std::shared_ptr<SceneNode>(import_lua(filename));
	if (!m_rootNode) {
		std::cerr << "Could not open " << filename << std::endl;
	}
}

//----------------------------------------------------------------------------------------
void A3::createShaderProgram()
{
	m_shader.generateProgramObject();
	m_shader.attachVertexShader( getAssetFilePath("VertexShader.vs").c_str() );
	m_shader.attachFragmentShader( getAssetFilePath("FragmentShader.fs").c_str() );
	m_shader.link();

	m_shader_arcCircle.generateProgramObject();
	m_shader_arcCircle.attachVertexShader( getAssetFilePath("arc_VertexShader.vs").c_str() );
	m_shader_arcCircle.attachFragmentShader( getAssetFilePath("arc_FragmentShader.fs").c_str() );
	m_shader_arcCircle.link();
}

//----------------------------------------------------------------------------------------
void A3::enableVertexShaderInputSlots()
{
	//-- Enable input slots for m_vao_meshData:
	{
		glBindVertexArray(m_vao_meshData);

		// Enable the vertex shader attribute location for "position" when rendering.
		m_positionAttribLocation = m_shader.getAttribLocation("position");
		glEnableVertexAttribArray(m_positionAttribLocation);

		// Enable the vertex shader attribute location for "normal" when rendering.
		m_normalAttribLocation = m_shader.getAttribLocation("normal");
		glEnableVertexAttribArray(m_normalAttribLocation);

		CHECK_GL_ERRORS;
	}


	//-- Enable input slots for m_vao_arcCircle:
	{
		glBindVertexArray(m_vao_arcCircle);

		// Enable the vertex shader attribute location for "position" when rendering.
		m_arc_positionAttribLocation = m_shader_arcCircle.getAttribLocation("position");
		glEnableVertexAttribArray(m_arc_positionAttribLocation);

		CHECK_GL_ERRORS;
	}

	// Restore defaults
	glBindVertexArray(0);
}

//----------------------------------------------------------------------------------------
void A3::uploadVertexDataToVbos (
		const MeshConsolidator & meshConsolidator
) {
	// Generate VBO to store all vertex position data
	{
		glGenBuffers(1, &m_vbo_vertexPositions);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexPositions);

		glBufferData(GL_ARRAY_BUFFER, meshConsolidator.getNumVertexPositionBytes(),
				meshConsolidator.getVertexPositionDataPtr(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		CHECK_GL_ERRORS;
	}

	// Generate VBO to store all vertex normal data
	{
		glGenBuffers(1, &m_vbo_vertexNormals);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexNormals);

		glBufferData(GL_ARRAY_BUFFER, meshConsolidator.getNumVertexNormalBytes(),
				meshConsolidator.getVertexNormalDataPtr(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		CHECK_GL_ERRORS;
	}

	// Generate VBO to store the trackball circle.
	{
		glGenBuffers( 1, &m_vbo_arcCircle );
		glBindBuffer( GL_ARRAY_BUFFER, m_vbo_arcCircle );

		float *pts = new float[ 2 * CIRCLE_PTS ];
		for( size_t idx = 0; idx < CIRCLE_PTS; ++idx ) {
			float ang = 2.0 * M_PI * float(idx) / CIRCLE_PTS;
			pts[2*idx] = cos( ang );
			pts[2*idx+1] = sin( ang );
		}

		glBufferData(GL_ARRAY_BUFFER, 2*CIRCLE_PTS*sizeof(float), pts, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		CHECK_GL_ERRORS;
	}
}

//----------------------------------------------------------------------------------------
void A3::mapVboDataToVertexShaderInputLocations()
{
	// Bind VAO in order to record the data mapping.
	glBindVertexArray(m_vao_meshData);

	// Tell GL how to map data from the vertex buffer "m_vbo_vertexPositions" into the
	// "position" vertex attribute location for any bound vertex shader program.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexPositions);
	glVertexAttribPointer(m_positionAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	// Tell GL how to map data from the vertex buffer "m_vbo_vertexNormals" into the
	// "normal" vertex attribute location for any bound vertex shader program.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexNormals);
	glVertexAttribPointer(m_normalAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	//-- Unbind target, and restore default values:
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	CHECK_GL_ERRORS;

	// Bind VAO in order to record the data mapping.
	glBindVertexArray(m_vao_arcCircle);

	// Tell GL how to map data from the vertex buffer "m_vbo_arcCircle" into the
	// "position" vertex attribute location for any bound vertex shader program.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_arcCircle);
	glVertexAttribPointer(m_arc_positionAttribLocation, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	//-- Unbind target, and restore default values:
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
void A3::initPerspectiveMatrix()
{
	float aspect = ((float)m_windowWidth) / m_windowHeight;
	m_perpsective = glm::perspective(degreesToRadians(60.0f), aspect, 0.1f, 100.0f);
}


//----------------------------------------------------------------------------------------
void A3::initViewMatrix() {
	m_view = glm::lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f),
			vec3(0.0f, 1.0f, 0.0f));
}

//----------------------------------------------------------------------------------------
void A3::initLightSources() {
	// World-space position
	m_light.position = vec3(-2.0f, 5.0f, 0.5f);
	m_light.rgbIntensity = vec3(0.8f); // White light
}

//----------------------------------------------------------------------------------------
void A3::uploadCommonSceneUniforms() {
	m_shader.enable();
	{
		//-- Set Perpsective matrix uniform for the scene:
		GLint location = m_shader.getUniformLocation("Perspective");
		glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(m_perpsective));
        
        location = m_shader.getUniformLocation("picking_enabled");
        glUniform1i(location, picking_enabled ? 1 : 0);

        
		CHECK_GL_ERRORS;

        if (!picking_enabled) {
            //-- Set LightSource uniform for the scene:
            {
                location = m_shader.getUniformLocation("light.position");
                glUniform3fv(location, 1, value_ptr(m_light.position));
                location = m_shader.getUniformLocation("light.rgbIntensity");
                glUniform3fv(location, 1, value_ptr(m_light.rgbIntensity));
                CHECK_GL_ERRORS;
            }
            
            //-- Set background light ambient intensity
            {
                location = m_shader.getUniformLocation("ambientIntensity");
                vec3 ambientIntensity(0.05f);
                glUniform3fv(location, 1, value_ptr(ambientIntensity));
                CHECK_GL_ERRORS;
            }
        }
		
	}
	m_shader.disable();
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, before guiLogic().
 */
void A3::appLogic()
{
	// Place per frame, application logic here ...

	uploadCommonSceneUniforms();
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after appLogic(), but before the draw() method.
 */
void A3::guiLogic()
{
	if( !show_gui ) {
		return;
	}

	static bool firstRun(true);
	if (firstRun) {
		ImGui::SetNextWindowPos(ImVec2(50, 50));
		firstRun = false;
	}

	static bool showDebugWindow(true);
	ImGuiWindowFlags windowFlags(ImGuiWindowFlags_AlwaysAutoResize);
	float opacity(0.5f);

	ImGui::Begin("Properties", &showDebugWindow, ImVec2(100,100), opacity,
			windowFlags);

    
    ImGui::BeginMainMenuBar();
        if (ImGui::BeginMenu("Application")) {
            if (ImGui::MenuItem("Reset Position", "I")) {
                resetPosition();
            }
            if (ImGui::MenuItem("Reset Orientation", "O")) {
                resetOrientation();
            }
            if (ImGui::MenuItem("Reset Joints", "N")) {
            }
            if (ImGui::MenuItem("Reset All", "A")) {
                resetOrientation();
                resetPosition();
            }
            if (ImGui::MenuItem("Quit", "Q")) {
                glfwSetWindowShouldClose(m_window, GL_TRUE);
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Options")) {
            if (ImGui::MenuItem("Circle", "C", &circle, true)) {
            }
            if (ImGui::MenuItem("Z-buffer", "Z", &z_buffer, true)) {
            }
            if (ImGui::MenuItem("Backface Culling", "B", &backface_culling, true)) {
            }
            if (ImGui::MenuItem("Frontface Culling", "F", &frontface_culling, true)) {
            }
            ImGui::EndMenu();
        }
    ImGui::EndMainMenuBar();
		// Add more gui elements here here ...


		// Create Button, and check if it was clicked:
		if( ImGui::Button( "Quit Application" ) ) {
			glfwSetWindowShouldClose(m_window, GL_TRUE);
		}

		ImGui::Text( "Framerate: %.1f FPS", ImGui::GetIO().Framerate );
    
    ImGui::Text("Position/Orientation   (R):");
    ImGui::SameLine();
    ImGui::PushID( 0 );
    if( ImGui::RadioButton( "##   Rotate:", &user_mode, 0 ) ) {
        // Select this colour.
        //active_col = colour1;
    }
    ImGui::PopID();
    ImGui::Text("Joints                 (J):");
    ImGui::SameLine();
    ImGui::PushID( 1 );
    if( ImGui::RadioButton( "##   Translate:", &user_mode, 1 ) ) {
        // Select this colour.
        //active_col = colour2;
    }
    ImGui::PopID();

	ImGui::End();
}

//----------------------------------------------------------------------------------------
// Update mesh specific shader uniforms:
static void updateShaderUniforms(
		const ShaderProgram & shader,
		const GeometryNode & node,
		const glm::mat4 & viewMatrix,
        bool picking_enabled
) {

	shader.enable();
	{
		//-- Set ModelView matrix:
		GLint location = shader.getUniformLocation("ModelView");
        
        mat4 modelView = mat4(1);
        
        modelView = viewMatrix * node.world_mat;
		glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(modelView));
		CHECK_GL_ERRORS;

		//-- Set NormMatrix:
		location = shader.getUniformLocation("NormalMatrix");
		mat3 normalMatrix = glm::transpose(glm::inverse(mat3(modelView)));
		glUniformMatrix3fv(location, 1, GL_FALSE, value_ptr(normalMatrix));
		CHECK_GL_ERRORS;


		//-- Set Material values:
		location = shader.getUniformLocation("material.kd");
        
        vec3 kd;
        if (picking_enabled) {
            kd = node.idToRGB();
        } else {
            kd = node.material.kd;

        }

		glUniform3fv(location, 1, value_ptr(kd));
        
		CHECK_GL_ERRORS;
		location = shader.getUniformLocation("material.ks");
		vec3 ks = node.material.ks;
		glUniform3fv(location, 1, value_ptr(ks));
		CHECK_GL_ERRORS;
		location = shader.getUniformLocation("material.shininess");
		glUniform1f(location, node.material.shininess);
		CHECK_GL_ERRORS;

	}
	shader.disable();

}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after guiLogic().
 */
void A3::draw() {

    
    if (z_buffer)
        glEnable( GL_DEPTH_TEST );
    
    performFaceCulling();

    
//    update(m_rootNode.get());

	renderSceneGraph(*m_rootNode);

    finishFaceCulling();

    if (z_buffer)
        glDisable( GL_DEPTH_TEST );
    
    if (circle)
        renderArcCircle();
}

//----------------------------------------------------------------------------------------
void A3::renderSceneGraph(const SceneNode & root) {

	// Bind the VAO once here, and reuse for all GeometryNode rendering below.
	glBindVertexArray(m_vao_meshData);

	// This is emphatically *not* how you should be drawing the scene graph in
	// your final implementation.  This is a non-hierarchical demonstration
	// in which we assume that there is a list of GeometryNodes living directly
	// underneath the root node, and that we can draw them in a loop.  It's
	// just enough to demonstrate how to get geometry and materials out of
	// a GeometryNode and onto the screen.

	// You'll want to turn this into recursive code that walks over the tree.
	// You can do that by putting a method in SceneNode, overridden in its
	// subclasses, that renders the subtree rooted at every node.  Or you
	// could put a set of mutually recursive functions in this class, which
	// walk down the tree from nodes of different types.
//    update(m_rootNode.get());
    m_rootNode->updateWorldMatrix(mat4(1));
    drawPuppet(*m_rootNode);
    
	glBindVertexArray(0);
	CHECK_GL_ERRORS;
}

void A3::update(SceneNode* root){
    if (root->m_name == "root") {
        root->rotate('y', rot_ang);
    }
//    if (root->m_name == "left_shoulder") {
//        root->rotate('x', rot_ang);
//        return;
//    }
    for (SceneNode* child: root->children) {
        update(child);
    }
}


void A3::drawPuppet(const SceneNode& root) {
//    if(root.m_nodeType == NodeType::JointNode) {
//        cout << root.m_name << endl;
//        cout << root.trans << endl;
//    }
    
    for (const SceneNode * node : root.children) {

        if (node->m_nodeType == NodeType::GeometryNode){
            const GeometryNode * geometryNode = static_cast<const GeometryNode *>(node);
            
            updateShaderUniforms(m_shader, *geometryNode, m_view, picking_enabled);
            
            // Get the BatchInfo corresponding to the GeometryNode's unique MeshId.
            BatchInfo batchInfo = m_batchInfoMap[geometryNode->meshId];
            
            //-- Now render the mesh:
            m_shader.enable();
            glDrawArrays(GL_TRIANGLES, batchInfo.startIndex, batchInfo.numIndices);
            m_shader.disable();
        }
        drawPuppet(*node);
    }
}

//----------------------------------------------------------------------------------------
// Draw the trackball circle.
void A3::renderArcCircle() {
	glBindVertexArray(m_vao_arcCircle);

	m_shader_arcCircle.enable();
		GLint m_location = m_shader_arcCircle.getUniformLocation( "M" );
		float aspect = float(m_framebufferWidth)/float(m_framebufferHeight);
		glm::mat4 M;
		if( aspect > 1.0 ) {
			M = glm::scale( glm::mat4(), glm::vec3( 0.5/aspect, 0.5, 1.0 ) );
		} else {
			M = glm::scale( glm::mat4(), glm::vec3( 0.5, 0.5*aspect, 1.0 ) );
		}
		glUniformMatrix4fv( m_location, 1, GL_FALSE, value_ptr( M ) );
		glDrawArrays( GL_LINE_LOOP, 0, CIRCLE_PTS );
	m_shader_arcCircle.disable();

	glBindVertexArray(0);
	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
/*
 * Called once, after program is signaled to terminate.
 */
void A3::cleanup()
{

}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles cursor entering the window area events.
 */
bool A3::cursorEnterWindowEvent (
		int entered
) {
	bool eventHandled(false);

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse cursor movement events.
 */
bool A3::mouseMoveEvent (
		double xPos,
		double yPos
) {
	bool eventHandled(false);
    
    m_mouse_GL_coordinate = vec2 ((2.0f * xPos) / m_windowWidth - 1.0f,
                                  1.0f - ( (2.0f * yPos) / m_windowHeight));

    
    rotation_click = vec2(xPos, yPos);
    
    vec2 delta = m_mouse_GL_coordinate - initial_click;
    if (user_mode == 0) {
        if (left_mouse_pressed) {
            puppet_translation.x += delta.x;
            puppet_translation.y += delta.y;
            m_rootNode.get()->translate(vec3(delta.x, delta.y, 0));
            updateSceneGraph();
        }
        if (middle_mouse_pressed) {
            puppet_translation.z -= delta.y;
            m_rootNode->translate(vec3(0, 0, -delta.y));
            updateSceneGraph();
        }
        if (right_mouse_pressed) {
            rotateTorso(initial_rotation_click.x, rotation_click.x, initial_rotation_click.y, rotation_click.y);
            updateSceneGraph();
        }
    }
    
    if (user_mode == 1) {
        if (left_mouse_pressed) {
            // picking
        }
        if (middle_mouse_pressed) {
            rotateSelectedjoints(delta.y*10);
            updateSceneGraph();
        }
        if (right_mouse_pressed) {
            cout << "rotating joints " << endl;
            
            
            neck_joint->rotate('x', -delta.y*10);
            neck_joint->rotate('y', delta.x*10);

            updateSceneGraph();
        }
    }
    
    initial_click = m_mouse_GL_coordinate;
    initial_rotation_click = rotation_click;
    
	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse button events.
 */
bool A3::mouseButtonInputEvent (
		int button,
		int actions,
		int mods
) {
	bool eventHandled(false);

	
    if (actions == GLFW_PRESS) {
        initial_click = m_mouse_GL_coordinate;
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            if ( user_mode == 1 ) {
                selectJoint();
            } else {
                left_mouse_pressed = true;
            }
            left_mouse_pressed = true;

        }
        if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
            middle_mouse_pressed = true;
        }
        if (button == GLFW_MOUSE_BUTTON_RIGHT) {
            right_mouse_pressed = true;
            initial_rotation_click = rotation_click;
        }
    }
    
    if (actions == GLFW_RELEASE) {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            left_mouse_pressed = false;
        }
        if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
            middle_mouse_pressed = false;
        }
        if (button == GLFW_MOUSE_BUTTON_RIGHT) {
            right_mouse_pressed = false;
        }
    }
    eventHandled = true;
	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse scroll wheel events.
 */
bool A3::mouseScrollEvent (
		double xOffSet,
		double yOffSet
) {
	bool eventHandled(false);

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles window resize events.
 */
bool A3::windowResizeEvent (
		int width,
		int height
) {
	bool eventHandled(false);
	initPerspectiveMatrix();
	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles key input events.
 */
bool A3::keyInputEvent (
		int key,
		int action,
		int mods
) {
	bool eventHandled(false);

	if( action == GLFW_PRESS ) {
		if( key == GLFW_KEY_M ) {
			show_gui = !show_gui;
			eventHandled = true;
		}
        if( key == GLFW_KEY_Q ) {
            glfwSetWindowShouldClose(m_window, GL_TRUE);
            eventHandled = true;
        }
        if( key == GLFW_KEY_C ) {
            circle = !circle;
            eventHandled = true;
        }
        if( key == GLFW_KEY_Z ) {
            z_buffer = !z_buffer;
            eventHandled = true;
        }
        if( key == GLFW_KEY_B ) {
            backface_culling = !backface_culling;
            eventHandled = true;
        }
        if( key == GLFW_KEY_F ) {
            frontface_culling = !frontface_culling;
            eventHandled = true;
        }
        if( key == GLFW_KEY_J ) {
            user_mode = 1;
            eventHandled = true;
        }
        if( key == GLFW_KEY_R ) {
            user_mode = 0;
            eventHandled = true;
        }
        if( key == GLFW_KEY_I ) {
            resetPosition();
            eventHandled = true;
        }
        if( key == GLFW_KEY_O ) {
            resetOrientation();
            eventHandled = true;
        }
        if( key == GLFW_KEY_A ) {
            resetOrientation();
            resetPosition();
            eventHandled = true;
        }
        if( key == GLFW_KEY_L ) {
//            if (picking_enabled == 1) {
//                picking_enabled = 0;
//            } else {
//                picking_enabled = 1;
//            }
            picking_enabled = !picking_enabled;
            eventHandled = true;
        }
	}
	// Fill in with event handling code...

	return eventHandled;
}

void A3::performFaceCulling(){
    glEnable(GL_CULL_FACE);
    if (frontface_culling and backface_culling) {
        glCullFace(GL_FRONT_AND_BACK);
    } else if (frontface_culling) {
        glCullFace(GL_FRONT);
    } else if (backface_culling){
        glCullFace(GL_BACK);
    } else {
        glDisable(GL_CULL_FACE);
    }
}

void A3::finishFaceCulling(){
    glDisable(GL_CULL_FACE);
}

void A3::updateSceneGraph () {    
    m_rootNode->updateWorldMatrix(mat4(1));
}

void A3::resetPosition() {
    m_rootNode->translate(-puppet_translation);
    updateSceneGraph();
    puppet_translation = vec3(0,0,0);
}

void A3::findJointNode(std::string joint_name, SceneNode* root) {
    if (root->m_name == "neck_joint") {
        neck_joint = (JointNode*)root;
    }
    if (root->m_name == "torso") {
        torso = root;
    }
    for (SceneNode* child : root->children) {
        findJointNode(joint_name, child);
    }
}


void A3::resetOrientation(){
    torso->trans = initial_torso_transformation;
    updateSceneGraph();
}

void A3::updateParentPointers(SceneNode* parent, SceneNode* root){
    root->parent = parent;
    for(SceneNode* child: root->children) {
        updateParentPointers(root, child);
    }
}

void A3::rotateSelectedjoints(float angle){
    for (SceneNode* node : nodes) {
        if (node->isSelected){
            node->rotate('x', angle);
        }
    }
}

void A3::selectJoint(){
    
    int id;
//    if (m_rootNode.get()->parent != nullptr) {
//        cout << "root er parent ase" << endl;
//    } else {
//        cout << "root er parent nai" << endl;
//    }
//
    cout << torso->parent->m_name << endl;
    glFlush();
    glFinish();
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    // Read the pixel at the center of the screen.
    // You can also use glfwGetMousePos().
    // Ultra-mega-over slow too, even for 1 pixel,
    // because the framebuffer is on the GPU.
    unsigned char data[4];
    
    double mx, my;
    
    glfwGetCursorPos(m_window, &mx, &my);
    
    glReadPixels(mx*2, m_framebufferHeight - (my*2) ,1,1, GL_RGBA, GL_UNSIGNED_BYTE, data);
    
    id = colorToId(data);
    
    SceneNode* node = findNodeWithId(id);
    
    if (node){
        cout << node->m_name << endl;
        SceneNode* jnode = findJoint(node);
        if (jnode) {
            jnode->isSelected = !jnode->isSelected;
            cout << jnode->m_name << endl;
        }
        else {
            cout << "nothing picked" << endl;
        }
    } else {
        cout << "nothing picked" << endl;
    }
    
}

SceneNode* A3::findJoint(SceneNode *node) {
    SceneNode* p;
    p = node;
    while (p != nullptr and p->m_nodeType != NodeType::JointNode) {
        p = p->parent;
    }
    return p;
}

void A3::populateNodeVector(SceneNode *root){
    nodes.push_back(root);
    for (SceneNode* child : root->children) {
        populateNodeVector(child);
    }
}

SceneNode* A3::findNodeWithId(int id) {
    for (SceneNode* node : nodes) {
        
        if (node->m_nodeId == id) {
            return node;
        }
    }
    return nullptr;
}

int A3:: colorToId(unsigned char *arr) {
    int id = arr[0] + arr[1] * 256 + arr[2] * 256 * 256;
    return id;
}

// trackball

/*******************************************************
 *
 * void vCalcRotVec(float fNewX, float fNewY,
 *                  float fOldX, float fOldY,
 *                  float fDiameter,
 *                  float *fVecX, float *fVecY, float *fVecZ);
 *
 *    Calculates a rotation vector based on mouse motion over
 *    a virtual trackball.
 *
 *    The fNew and fOld mouse positions
 *    should be in 'trackball' space. That is, they have been
 *    transformed into a coordinate system centered at the middle
 *    of the trackball. fNew, fOld, and fDiameter must all be specified
 *    in the same units (pixels for example).
 *
 * Parameters: fNewX, fNewY - New mouse position in trackball space.
 *                            This is the second point along direction
 *                            of rotation.
 *             fOldX, fOldY - Old mouse position in trackball space.
 *                            This is the first point along direction
 *                            of rotation.
 *             fDiameter - Diameter of the trackball. This should
 *                         be specified in the same units as fNew and fOld.
 *                         (ie, usually pixels if fNew and fOld are transformed
 *                         mouse positions)
 *             fVec - The output rotation vector. The length of the vector
 *                    is proportional to the angle of rotation.
 *
 *******************************************************/
void A3::vCalcRotVec(float fNewX, float fNewY,
                 float fOldX, float fOldY,
                 float fDiameter,
                 float *fVecX, float *fVecY, float *fVecZ) {
    long  nXOrigin, nYOrigin;
    float fNewVecX, fNewVecY, fNewVecZ,        /* Vector corresponding to new mouse location */
    fOldVecX, fOldVecY, fOldVecZ,        /* Vector corresponding to old mouse location */
    fLength;
    
    /* Vector pointing from center of virtual trackball to
     * new mouse position
     */
    fNewVecX    = fNewX * 2.0 / fDiameter;
    fNewVecY    = fNewY * 2.0 / fDiameter;
    fNewVecZ    = (1.0 - fNewVecX * fNewVecX - fNewVecY * fNewVecY);
    
    /* If the Z component is less than 0, the mouse point
     * falls outside of the trackball which is interpreted
     * as rotation about the Z axis.
     */
    if (fNewVecZ < 0.0) {
        fLength = sqrt(1.0 - fNewVecZ);
        fNewVecZ  = 0.0;
        fNewVecX /= fLength;
        fNewVecY /= fLength;
    } else {
        fNewVecZ = sqrt(fNewVecZ);
    }
    
    /* Vector pointing from center of virtual trackball to
     * old mouse position
     */
    fOldVecX    = fOldX * 2.0 / fDiameter;
    fOldVecY    = fOldY * 2.0 / fDiameter;
    fOldVecZ    = (1.0 - fOldVecX * fOldVecX - fOldVecY * fOldVecY);
    
    /* If the Z component is less than 0, the mouse point
     * falls outside of the trackball which is interpreted
     * as rotation about the Z axis.
     */
    if (fOldVecZ < 0.0) {
        fLength = sqrt(1.0 - fOldVecZ);
        fOldVecZ  = 0.0;
        fOldVecX /= fLength;
        fOldVecY /= fLength;
    } else {
        fOldVecZ = sqrt(fOldVecZ);
    }
    
    /* Generate rotation vector by calculating cross product:
     *
     * fOldVec x fNewVec.
     *
     * The rotation vector is the axis of rotation
     * and is non-unit length since the length of a crossproduct
     * is related to the angle between fOldVec and fNewVec which we need
     * in order to perform the rotation.
     */
    *fVecX = fOldVecY * fNewVecZ - fNewVecY * fOldVecZ;
    *fVecY = fOldVecZ * fNewVecX - fNewVecZ * fOldVecX;
    *fVecZ = fOldVecX * fNewVecY - fNewVecX * fOldVecY;
}

/*******************************************************
 * void vAxisRotMatrix(float fVecX, float fVecY, float fVecZ, Matrix mNewMat)
 *
 *    Calculate the rotation matrix for rotation about an arbitrary axis.
 *
 *    The axis of rotation is specified by (fVecX,fVecY,fVecZ). The length
 *    of the vector is the amount to rotate by.
 *
 * Parameters: fVecX,fVecY,fVecZ - Axis of rotation
 *             mNewMat - Output matrix of rotation in column-major format
 *                       (ie, translation components are in column 3 on rows
 *                       0,1, and 2).
 *
 *******************************************************/
void A3::vAxisRotMatrix(float fVecX, float fVecY, float fVecZ, mat4& mNewMat) {
    float fRadians, fInvLength, fNewVecX, fNewVecY, fNewVecZ;
    
    /* Find the length of the vector which is the angle of rotation
     * (in radians)
     */
    fRadians = sqrt(fVecX * fVecX + fVecY * fVecY + fVecZ * fVecZ);
    
    /* If the vector has zero length - return the identity matrix */
    if (fRadians > -0.000001 && fRadians < 0.000001) {
        mNewMat = mat4(1);
        return;
    }
    
    /* Normalize the rotation vector now in preparation for making
     * rotation matrix.
     */
    fInvLength = 1 / fRadians;
    fNewVecX   = fVecX * fInvLength;
    fNewVecY   = fVecY * fInvLength;
    fNewVecZ   = fVecZ * fInvLength;
    
    /* Create the arbitrary axis rotation matrix */
    double dSinAlpha = sin(fRadians);
    double dCosAlpha = cos(fRadians);
    double dT = 1 - dCosAlpha;
    
    mNewMat[0][0] = dCosAlpha + fNewVecX*fNewVecX*dT;
    mNewMat[0][1] = fNewVecX*fNewVecY*dT + fNewVecZ*dSinAlpha;
    mNewMat[0][2] = fNewVecX*fNewVecZ*dT - fNewVecY*dSinAlpha;
    mNewMat[0][3] = 0;
    
    mNewMat[1][0] = fNewVecX*fNewVecY*dT - dSinAlpha*fNewVecZ;
    mNewMat[1][1] = dCosAlpha + fNewVecY*fNewVecY*dT;
    mNewMat[1][2] = fNewVecY*fNewVecZ*dT + dSinAlpha*fNewVecX;
    mNewMat[1][3] = 0;
    
    mNewMat[2][0] = fNewVecZ*fNewVecX*dT + dSinAlpha*fNewVecY;
    mNewMat[2][1] = fNewVecZ*fNewVecY*dT - dSinAlpha*fNewVecX;
    mNewMat[2][2] = dCosAlpha + fNewVecZ*fNewVecZ*dT;
    mNewMat[2][3] = 0;
    
    mNewMat[3][0] = 0;
    mNewMat[3][1] = 0;
    mNewMat[3][2] = 0;
    mNewMat[3][3] = 1;
}

void A3::rotateTorso(float oldx, float newx, float oldy, float newy){
    float  fRotVecX, fRotVecY, fRotVecZ;
    mat4 mNewMat;
    
    /*
     * Track ball rotations are being used.
     */
    
    float fDiameter;
    int iCenterX, iCenterY;
    float fNewModX, fNewModY, fOldModX, fOldModY;
    
    /* vCalcRotVec expects new and old positions in relation to the center of the
     * trackball circle which is centered in the middle of the window and
     * half the smaller of nWinWidth or nWinHeight.
     */
//    fDiameter = (nWinWidth < nWinHeight) ? nWinWidth * 0.5 : nWinHeight * 0.5;
    fDiameter = (m_windowWidth < m_windowHeight) ? m_windowWidth * 0.5 : m_windowHeight * 0.5;
    iCenterX = m_windowWidth/2;
    iCenterY = m_windowHeight/2;
    fOldModX = oldx - iCenterX;
    fOldModY = oldy - iCenterY;
    fNewModX = newx - iCenterX;
    fNewModY = newy - iCenterY;
    
    vCalcRotVec(fNewModX, fNewModY,
                fOldModX, fOldModY,
                fDiameter,
                &fRotVecX, &fRotVecY, &fRotVecZ);
    /* Negate Y component since Y axis increases downwards
     * in screen space and upwards in OpenGL.
     */
    vAxisRotMatrix(fRotVecX, -fRotVecY, fRotVecZ, mNewMat);
    
    // Since all these matrices are meant to be loaded
    // into the OpenGL matrix stack, we need to transpose the
    // rotation matrix (since OpenGL wants rows stored
    // in columns)
    mNewMat = transpose(mNewMat);
    torso->rotate(mNewMat);
}

