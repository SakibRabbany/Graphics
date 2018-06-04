#include "A2.hpp"
#include "cs488-framework/GlErrorCheck.hpp"

#include <iostream>
using namespace std;

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>

using namespace glm;

//----------------------------------------------------------------------------------------
// Constructor
VertexData::VertexData()
	: numVertices(0),
	  index(0)
{
	positions.resize(kMaxVertices);
	colours.resize(kMaxVertices);
}


//----------------------------------------------------------------------------------------
// Constructor
A2::A2()
	: m_currentLineColour(vec3(0.0f))
{}

//----------------------------------------------------------------------------------------
// Destructor
A2::~A2()
{

}

//----------------------------------------------------------------------------------------
/*
 * Called once, at program start.
 */
void A2::init()
{
	// Set the background colour.
	glClearColor(0.3, 0.5, 0.7, 1.0);

	createShaderProgram();

	glGenVertexArrays(1, &m_vao);

	enableVertexAttribIndices();

	generateVertexBuffers();

	mapVboDataToVertexAttributeLocation();
    
    initCube();
    initGnomons();
    
    model_translate_mat = model_rotate_mat = model_scale_mat = mat4(1);
    view_translate_mat = view_rotate_mat = mat4(1);
    
    far = 25.0f;
    near = 10.0f;
    field_of_view = 45.0f;
    frame_buffer_width = frame_buffer_height = 2;
    
    user_mode = 0;
    
    float aspect = m_framebufferWidth/m_framebufferHeight;
    
    M = mat4(1.0f);
    
    model_translate_mat = translate(model_translate_mat, vec3(0.4,0,0));
    M = translate(M, vec3(0.4,0,0));
    
    V = look_at(vec3(0, 0, 5),     // camera pos
                vec3(0, 0, 0),      // center
                vec3(0, 1, 0));     // up
    
//    V = inverse(glm::mat4(vec4(1, 0, 0, 0), vec4(0, 1, 0, 0), vec4(0, 0, -1, 0), vec4(0, 0, 10, 1)));

    
//    V = inverse(glm::mat4(vec4(1, 0, 0, 0), vec4(0, 1, 0, 0), vec4(0, 0, -1, 0), vec4(0, 0, 10, 1)));
    
    cout << V << endl;

    
    P = perspective(field_of_view, aspect, near, far);
    
//    M = scale(M, vec3(0.4, 0.4, 0.4));

    
    cout << "Model Matrix: " << endl;
    cout << M << endl;
    
    cout << "View Matrix: " << endl;
    cout << V << endl;
    
    cout << "Perspective Matrix: " << endl;
    cout << P << endl;
    
    
}

//----------------------------------------------------------------------------------------
void A2::createShaderProgram()
{
	m_shader.generateProgramObject();
	m_shader.attachVertexShader( getAssetFilePath("VertexShader.vs").c_str() );
	m_shader.attachFragmentShader( getAssetFilePath("FragmentShader.fs").c_str() );
	m_shader.link();
}

//----------------------------------------------------------------------------------------
void A2::enableVertexAttribIndices()
{
	glBindVertexArray(m_vao);

	// Enable the attribute index location for "position" when rendering.
	GLint positionAttribLocation = m_shader.getAttribLocation( "position" );
	glEnableVertexAttribArray(positionAttribLocation);

	// Enable the attribute index location for "colour" when rendering.
	GLint colourAttribLocation = m_shader.getAttribLocation( "colour" );
	glEnableVertexAttribArray(colourAttribLocation);

	// Restore defaults
	glBindVertexArray(0);

	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
void A2::generateVertexBuffers()
{
	// Generate a vertex buffer to store line vertex positions
	{
		glGenBuffers(1, &m_vbo_positions);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_positions);

		// Set to GL_DYNAMIC_DRAW because the data store will be modified frequently.
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * kMaxVertices, nullptr,
				GL_DYNAMIC_DRAW);


		// Unbind the target GL_ARRAY_BUFFER, now that we are finished using it.
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		CHECK_GL_ERRORS;
	}

	// Generate a vertex buffer to store line colors
	{
		glGenBuffers(1, &m_vbo_colours);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_colours);

		// Set to GL_DYNAMIC_DRAW because the data store will be modified frequently.
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * kMaxVertices, nullptr,
				GL_DYNAMIC_DRAW);


		// Unbind the target GL_ARRAY_BUFFER, now that we are finished using it.
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		CHECK_GL_ERRORS;
	}
}

//----------------------------------------------------------------------------------------
void A2::mapVboDataToVertexAttributeLocation()
{
	// Bind VAO in order to record the data mapping.
	glBindVertexArray(m_vao);

	// Tell GL how to map data from the vertex buffer "m_vbo_positions" into the
	// "position" vertex attribute index for any bound shader program.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_positions);
	GLint positionAttribLocation = m_shader.getAttribLocation( "position" );
	glVertexAttribPointer(positionAttribLocation, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	// Tell GL how to map data from the vertex buffer "m_vbo_colours" into the
	// "colour" vertex attribute index for any bound shader program.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_colours);
	GLint colorAttribLocation = m_shader.getAttribLocation( "colour" );
	glVertexAttribPointer(colorAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	//-- Unbind target, and restore default values:
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	CHECK_GL_ERRORS;
}

//---------------------------------------------------------------------------------------
void A2::initLineData()
{
	m_vertexData.numVertices = 0;
	m_vertexData.index = 0;
}

//---------------------------------------------------------------------------------------
void A2::setLineColour (
		const glm::vec3 & colour
) {
	m_currentLineColour = colour;
}

//---------------------------------------------------------------------------------------
void A2::drawLine(
		const glm::vec2 & v0,   // Line Start (NDC coordinate)
		const glm::vec2 & v1    // Line End (NDC coordinate)
) {

	m_vertexData.positions[m_vertexData.index] = v0;
	m_vertexData.colours[m_vertexData.index] = m_currentLineColour;
	++m_vertexData.index;
	m_vertexData.positions[m_vertexData.index] = v1;
	m_vertexData.colours[m_vertexData.index] = m_currentLineColour;
	++m_vertexData.index;

	m_vertexData.numVertices += 2;
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, before guiLogic().
 */
void A2::appLogic()
{
	// Place per frame, application logic here ...

	// Call at the beginning of frame, before drawing lines:
	initLineData();

	// Draw outer square:
//    setLineColour(vec3(1.0f, 0.7f, 0.8f));
//    drawLine(vec2(-0.5f, -0.5f), vec2(0.5f, -0.5f));
//    drawLine(vec2(0.5f, -0.5f), vec2(0.5f, 0.5f));
//    drawLine(vec2(0.5f, 0.5f), vec2(-0.5f, 0.5f));
//    drawLine(vec2(-0.5f, 0.5f), vec2(-0.5f, -0.5f));
//
//
//    // Draw inner square:
//    setLineColour(vec3(0.2f, 1.0f, 1.0f));
//    drawLine(vec2(-0.25f, -0.25f), vec2(0.25f, -0.25f));
//    drawLine(vec2(0.25f, -0.25f), vec2(0.25f, 0.25f));
//    drawLine(vec2(0.25f, 0.25f), vec2(-0.25f, 0.25f));
//    drawLine(vec2(-0.25f, 0.25f), vec2(-0.25f, -0.25f));
    
    
//    M = rotate(M, 0.005, z_axis);
    
//    M = translate(M, vec3(0, 0.005, 0));
    
    drawCube();
    drawModelGnomon();
    drawWorldGnomon();
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after appLogic(), but before the draw() method.
 */
void A2::guiLogic()
{
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


    ImGui::Text("Model:");
    ImGui::Text("   Rotate      (R):");
    ImGui::SameLine();
    ImGui::PushID( 0 );
    if( ImGui::RadioButton( "##   Rotate:", &user_mode, 0 ) ) {
        // Select this colour.
        //active_col = colour1;
        cout << "User Mode: " << user_mode << endl;
    }
    ImGui::PopID();
    ImGui::Text("   Translate   (T):");
    ImGui::SameLine();
    ImGui::PushID( 1 );
    if( ImGui::RadioButton( "##   Translate:", &user_mode, 1 ) ) {
        // Select this colour.
        //active_col = colour2;
        cout << "User Mode: " << user_mode << endl;
    }
    ImGui::PopID();

    ImGui::Text("   Scale       (S):");
    ImGui::SameLine();
    ImGui::PushID( 2 );
    if( ImGui::RadioButton( "##   Scale:", &user_mode, 2 ) ) {
        // Select this colour.
        //active_col = colour3;
        cout << "User Mode: " << user_mode << endl;
    }
    ImGui::PopID();


    ImGui::Text("View:");
    ImGui::Text("   Rotate      (O):");
    ImGui::SameLine();
    ImGui::PushID( 3 );
    if( ImGui::RadioButton( "##   Rotate:", &user_mode, 3 ) ) {
        // Select this colour.
        //active_col = colour4;
        cout << "User Mode: " << user_mode << endl;
    }
    ImGui::PopID();

    ImGui::PushID( 4 );
    ImGui::Text("   Translate   (N):");
    ImGui::SameLine();
    if( ImGui::RadioButton( "##   Translate:", &user_mode, 4 ) ) {
        // Select this colour.
        //active_col = colour5;
        cout << "User Mode: " << user_mode << endl;
    }
    ImGui::PopID();
    
    ImGui::Text("Perspective    (P):");
    ImGui::SameLine();
    ImGui::PushID( 5 );
    if( ImGui::RadioButton( "##Perspective", &user_mode, 5 ) ) {
        // Select this colour.
        //active_col = colour6;
        cout << "User Mode: " << user_mode << endl;
    }
    ImGui::PopID();

    ImGui::Text("Viewport       (V):");
    ImGui::SameLine();
    ImGui::PushID( 6 );
    if( ImGui::RadioButton( "##Viewport", &user_mode, 6 ) ) {
        // Select this colour.
        //active_col = colour7;
        cout << "User Mode: " << user_mode << endl;
    }
    ImGui::PopID();


    // Create Button, and check if it was clicked:
    if( ImGui::Button( "Quit Application" ) ) {
        glfwSetWindowShouldClose(m_window, GL_TRUE);
    }

    ImGui::Text( "Framerate: %.1f FPS", ImGui::GetIO().Framerate );

    ImGui::End();
}

//----------------------------------------------------------------------------------------
void A2::uploadVertexDataToVbos() {

	//-- Copy vertex position data into VBO, m_vbo_positions:
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_positions);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec2) * m_vertexData.numVertices,
				m_vertexData.positions.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		CHECK_GL_ERRORS;
	}

	//-- Copy vertex colour data into VBO, m_vbo_colours:
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_colours);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3) * m_vertexData.numVertices,
				m_vertexData.colours.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		CHECK_GL_ERRORS;
	}
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after guiLogic().
 */
void A2::draw()
{
	uploadVertexDataToVbos();

	glBindVertexArray(m_vao);

	m_shader.enable();
		glDrawArrays(GL_LINES, 0, m_vertexData.numVertices);
	m_shader.disable();

	// Restore defaults
	glBindVertexArray(0);

	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
/*
 * Called once, after program is signaled to terminate.
 */
void A2::cleanup()
{

}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles cursor entering the window area events.
 */
bool A2::cursorEnterWindowEvent (
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
bool A2::mouseMoveEvent (
		double xPos,
		double yPos
) {
	bool eventHandled(false);
    
    if (!ImGui::IsMouseHoveringAnyWindow()) {
        m_mouse_GL_coordinate = vec2 ((2.0f * xPos) / m_windowWidth - 1.0f,
                                      1.0f - ( (2.0f * yPos) / m_windowHeight));
        vec2 delta = m_mouse_GL_coordinate - initial_click;
        if (left_mouse_pressed) {
            updateMatrices(delta, x_axis);
        }
        if (middle_mouse_pressed) {
            updateMatrices(delta, y_axis);
        }
        if (right_mouse_pressed) {
            updateMatrices(delta, z_axis);
        }
//        cout << "x: " << m_mouse_GL_coordinate.x << " y: " << m_mouse_GL_coordinate.y << endl;
        initial_click = m_mouse_GL_coordinate;

    }
    

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse button events.
 */
bool A2::mouseButtonInputEvent (
		int button,
		int actions,
		int mods
) {
	bool eventHandled(false);
    
    
    if (actions == GLFW_PRESS) {
        
        initial_click = m_mouse_GL_coordinate;
        
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            left_mouse_pressed = true;
            cout << "Left Mouse Button clicked:" << endl;
            cout << m_mouse_GL_coordinate.x << " " << m_mouse_GL_coordinate.y << endl;
        }
        if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
            middle_mouse_pressed = true;
            cout << "Middle Mouse Button clicked:" << endl;
            cout << m_mouse_GL_coordinate.x << " " << m_mouse_GL_coordinate.y << endl;
        }
        if (button == GLFW_MOUSE_BUTTON_RIGHT) {
            right_mouse_pressed = true;
            cout << "Right Mouse Button clicked:" << endl;
            cout << m_mouse_GL_coordinate.x << " " << m_mouse_GL_coordinate.y << endl;
        }
    }
    
    if (actions == GLFW_RELEASE) {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            left_mouse_pressed = false;
            cout << "Left Mouse Button released:" << endl;
            cout << m_mouse_GL_coordinate.x << " " << m_mouse_GL_coordinate.y << endl;
        }
        if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
            middle_mouse_pressed = false;
            cout << "Middle Mouse Button released:" << endl;
            cout << m_mouse_GL_coordinate.x << " " << m_mouse_GL_coordinate.y << endl;
        }
        if (button == GLFW_MOUSE_BUTTON_RIGHT) {
            right_mouse_pressed = false;
            cout << "Right Mouse Button released:" << endl;
            cout << m_mouse_GL_coordinate.x << " " << m_mouse_GL_coordinate.y << endl;
        }
    }
    
    
    eventHandled = true;


	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse scroll wheel events.
 */
bool A2::mouseScrollEvent (
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
bool A2::windowResizeEvent (
		int width,
		int height
) {
	bool eventHandled(false);

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles key input events.
 */
bool A2::keyInputEvent (
		int key,
		int action,
		int mods
) {
	bool eventHandled(false);

	// Fill in with event handling code...
    if( action == GLFW_PRESS) {
        if (key == GLFW_KEY_Q) {
            glfwSetWindowShouldClose(m_window, GL_TRUE);
        }
        if (key == GLFW_KEY_R) {
            user_mode = 0;
        }
        if (key == GLFW_KEY_T) {
            user_mode = 1;
        }
        if (key == GLFW_KEY_S) {
            user_mode = 2;
        }
        if (key == GLFW_KEY_O) {
            user_mode = 3;
        }
        if (key == GLFW_KEY_N) {
            user_mode = 4;
        }
        if (key == GLFW_KEY_P) {
            user_mode = 5;
        }
        if (key == GLFW_KEY_V) {
            user_mode = 6;
        }
    }
	return eventHandled;
}

void A2::initCube()
{
    // back face lower horizontal line
    m_cube_lines.push_back(make_pair(vec4(-1.0f, -1.0f, -1.0f, 1.0f),
                                     vec4( 1.0f, -1.0f, -1.0f, 1.0f)));
    
    // back face upper horizontal line
    m_cube_lines.push_back(make_pair(vec4(-1.0f,  1.0f, -1.0f, 1.0f),
                                     vec4( 1.0f,  1.0f, -1.0f, 1.0f)));
    
    // front face lower horizontal line
    m_cube_lines.push_back(make_pair(vec4(-1.0f, -1.0f,  1.0f, 1.0f),
                                     vec4( 1.0f, -1.0f,  1.0f, 1.0f)));
    
    // front face upper horizontal line
    m_cube_lines.push_back(make_pair(vec4(-1.0f,  1.0f,  1.0f, 1.0f),
                                     vec4( 1.0f,  1.0f,  1.0f, 1.0f)));
    
    // back face left vertical line
    m_cube_lines.push_back(make_pair(vec4(-1.0f, -1.0f, -1.0f, 1.0f),
                                     vec4(-1.0f,  1.0f, -1.0f, 1.0f)));
    
    // back face right vertical line
    m_cube_lines.push_back(make_pair(vec4( 1.0f, -1.0f, -1.0f, 1.0f),
                                     vec4( 1.0f,  1.0f, -1.0f, 1.0f)));
    
    // front face left vertical line
    m_cube_lines.push_back(make_pair(vec4(-1.0f, -1.0f,  1.0f, 1.0f),
                                     vec4(-1.0f,  1.0f,  1.0f, 1.0f)));
    
    // front face right vertical line
    m_cube_lines.push_back(make_pair(vec4( 1.0f, -1.0f,  1.0f, 1.0f),
                                     vec4( 1.0f,  1.0f,  1.0f, 1.0f)));
    
    // left face bottom horizontal line
    m_cube_lines.push_back(make_pair(vec4(-1.0f, -1.0f,  1.0f, 1.0f),
                                     vec4(-1.0f, -1.0f, -1.0f, 1.0f)));
    
    // left face top horizontal line
    m_cube_lines.push_back(make_pair(vec4(-1.0f,  1.0f,  1.0f, 1.0f),
                                     vec4(-1.0f,  1.0f, -1.0f, 1.0f)));
    
    // right face bottom horizontal line
    m_cube_lines.push_back(make_pair(vec4( 1.0f, -1.0f,  1.0f, 1.0f),
                                     vec4( 1.0f, -1.0f, -1.0f, 1.0f)));
    
    // right face top horizontal line
    m_cube_lines.push_back(make_pair(vec4( 1.0f,  1.0f,  1.0f, 1.0f),
                                     vec4( 1.0f,  1.0f, -1.0f, 1.0f)));
}


void A2::initGnomons()
{
    // cube gnomon
    // x axis
    m_cube_gnomon_lines.push_back(make_pair(vec4( 0.0f,  0.0f,  0.0f, 1.0f),
                                            vec4( 1.0f,  0.0f,  0.0f, 1.0f)));
    
    // y axis
    m_cube_gnomon_lines.push_back(make_pair(vec4( 0.0f,  0.0f,  0.0f, 1.0f),
                                            vec4( 0.0f,  1.0f,  0.0f, 1.0f)));
    
    // z axis
    m_cube_gnomon_lines.push_back(make_pair(vec4( 0.0f,  0.0f,  0.0f, 1.0f),
                                            vec4( 0.0f,  0.0f,  1.0f, 1.0f)));
    
    // world gnomon
    // x axis
    m_world_gnomon_lines.push_back(make_pair(vec4( 0.0f,  0.0f,  0.0f, 1.0f),
                                             vec4( 1.0f,  0.0f,  0.0f, 1.0f)));
    
    // y axis
    m_world_gnomon_lines.push_back(make_pair(vec4( 0.0f,  0.0f,  0.0f, 1.0f),
                                             vec4( 0.0f,  1.0f,  0.0f, 1.0f)));
    
    // z axis
    m_world_gnomon_lines.push_back(make_pair(vec4( 0.0f,  0.0f,  0.0f, 1.0f),
                                             vec4( 0.0f,  0.0f,  1.0f, 1.0f)));
}


glm::mat4 A2::translate(const glm::mat4 &mat, const glm::vec3 &t)
{
    vec4 column0 = vec4(  1,   0,   0,   0);
    vec4 column1 = vec4(  0,   1,   0,   0);
    vec4 column2 = vec4(  0,   0,   1,   0);
    vec4 column3 = vec4(t.x, t.y, t.z,   1);
    
    mat4 translate_matrix = mat4(column0, column1, column2, column3);
    
    return translate_matrix * mat;
}

glm::mat4 A2::scale(const glm::mat4 &mat, const glm::vec3 &s)
{
    vec4 column0 = vec4(s.x,   0,   0,   0);
    vec4 column1 = vec4(  0, s.y,   0,   0);
    vec4 column2 = vec4(  0,   0, s.z,   0);
    vec4 column3 = vec4(  0,   0,   0,   1);
    
    mat4 scale_matrix = mat4(column0, column1, column2, column3);
    
    return  scale_matrix * mat;
}

glm::mat4 A2::rotate(const glm::mat4 &mat, float angle, A2::Axis axis)
{
    vec4 column0, column1, column2, column3;
    
    double c = cos(angle);
    double s = sin(angle);
    
    switch (axis) {
        case Axis::x_axis:
            column0 = vec4(  1,   0,   0,   0);
            column1 = vec4(  0,   c,   s,   0);
            column2 = vec4(  0,  -s,   c,   0);
            column3 = vec4(  0,   0,   0,   1);
            break;
            
        case Axis::y_axis:
            column0 = vec4(  c,   0,  -s,   0);
            column1 = vec4(  0,   1,   0,   0);
            column2 = vec4(  s,   0,   c,   0);
            column3 = vec4(  0,   0,   0,   1);
            break;
            
        case Axis::z_axis:
            column0 = vec4(  c,   s,   0,   0);
            column1 = vec4( -s,   c,   0,   0);
            column2 = vec4(  0,   0,   1,   0);
            column3 = vec4(  0,   0,   0,   1);
            break;
        default:
            break;
    }
    
    mat4 rotate_matrix = mat4(column0, column1, column2, column3);
    
    return   rotate_matrix * mat;

}

glm::mat4 A2::look_at(const glm::vec3 &camera_position, const glm::vec3 &center, const glm::vec3 &up)
{
    vec3 z_direction = normalize(center - camera_position);
    vec3 u = normalize(up);
    vec3 x_direction = normalize(cross(u, z_direction));
    
    vec3 y = cross(x_direction, z_direction);
    
    vec4 column0 = vec4(x_direction.x, y.x, z_direction.x,   0);
    vec4 column1 = vec4(x_direction.y, y.y, z_direction.y,   0);
    vec4 column2 = vec4(x_direction.z, y.z, z_direction.z,   0);
    vec4 column3 = vec4(  0,   0,   0,   1);
    
    mat4 R = mat4(column0, column1, column2, column3);
    mat4 T = translate(mat4(1.0f), -camera_position);
    
    return R * T;
}


glm::mat4 A2::perspective(float fov, float aspect, float near, float far) {
    
    float rad = glm::radians(fov/2);
    float cot = 1/tan((double)rad);
    
    vec4 column0 = vec4(cot/aspect,  0,         0,                      0);
    vec4 column1 = vec4(    0,      cot,        0,                      0);
    vec4 column2 = vec4(    0,       0, -(far + near)/(far - near),    -1);
    vec4 column3 = vec4(    0,       0, (- 2 * far * near)/(far - near),0);
    
    mat4 pers = mat4(column0, column1, column2, column3);
    
    return pers;
}


void A2::drawCube() {
    vec4 p1, p2;
    for (const pair<vec4, vec4>& line : m_cube_lines) {
        p1 = line.first;
        p1 = P * V * M * p1;
        p2 = line.second;
        p2 = P * V * M * p2;
        
//        if (clip(p1, p2)){
////            p1 = p1/p1.w;
////            p2 = p2/p2.w;
////            vec2 v1 = vec2(p1.x, p1.y);
////            vec2 v2 = vec2(p2.x, p2.y);
//            cout << "drawing point" << endl;
//            draw_in_viewport(vec2(p1.x, p1.y), vec2(p2.x, p2.y));
//        }
        
        p1 = p1/p1.w;
        p2 = p2/p2.w;
        //        cout << "p1" <<  p1 << "  p2  " << p2 << endl;
        drawLine(vec2(p1.x, p1.y), vec2(p2.x, p2.y));
    }
}

void A2::drawModelGnomon() {
    vec4 p1, p2;
    mat4 scale_vec = scale(mat4(1), vec3(0.4));
    for (const pair<vec4, vec4>& line : m_cube_gnomon_lines) {
        p1 = line.first;
        p1 = P * V * (model_translate_mat * scale_vec * model_rotate_mat) * p1;
        p1 = p1/p1.w;
        p2 = line.second;
        p2 = P * V * (model_translate_mat * scale_vec * model_rotate_mat) * p2;
        p2 = p2/p2.w;
        
        //        cout << "p1" <<  p1 << "  p2  " << p2 << endl;
        drawLine(vec2(p1.x, p1.y), vec2(p2.x, p2.y));
    }
}


void A2::drawWorldGnomon(){
    vec4 p1, p2;
    for (const pair<vec4, vec4>& line : m_world_gnomon_lines) {
        p1 = line.first;
        p1 = P * V * p1;
        p1 = p1/p1.w;
        p2 = line.second;
        p2 = P * V  * p2;
        p2 = p2/p2.w;
        
        //        cout << "p1" <<  p1 << "  p2  " << p2 << endl;
        drawLine(vec2(p1.x, p1.y), vec2(p2.x, p2.y));
    }
}



void A2::updateMatrices(vec2& delta, Axis axis) {
    M = mat4(1.0f);
    
    
    if (user_mode == 0) {
        model_rotate_mat = rotate(model_rotate_mat, delta.x, axis);
    }
    if (user_mode == 1) {
        vec3 translate_vec;
        if (axis == Axis::x_axis) {
            translate_vec = vec3(delta.x, 0, 0);
        }
        if (axis == Axis::y_axis) {
            translate_vec = vec3(0, delta.x, 0);
        }
        if (axis == Axis::z_axis) {
            translate_vec = vec3(0, 0, delta.x);
        }
        model_translate_mat = translate(model_translate_mat, translate_vec);
    }
    if (user_mode == 2) {
        vec3 scale_vec;
        if (axis == Axis::x_axis) {
            scale_vec = vec3(delta.x+1, 1, 1);
        }
        if (axis == Axis::y_axis) {
            scale_vec = vec3(1, delta.x+1, 1);
        }
        if (axis == Axis::z_axis) {
            scale_vec = vec3(1, 1, delta.x+1);
        }
        model_scale_mat = scale(model_scale_mat, scale_vec);
    }
    
    M = model_translate_mat * model_scale_mat * model_rotate_mat;
    
    if (user_mode == 3) {
        V =  V * (rotate(mat4(1), delta.x, axis)) ;
    }
    
    if (user_mode == 4) {
        vec3 translate_vec;
        if (axis == Axis::x_axis) {
            translate_vec = vec3(delta.x, 0, 0);
        }
        if (axis == Axis::y_axis) {
            translate_vec = vec3(0, delta.x, 0);
        }
        if (axis == Axis::z_axis) {
            translate_vec = vec3(0, 0, delta.x);
        }
        V = (translate(V, translate_vec));
    }
    
//    V = inverse(view_translate_mat) * inverse(view_rotate_mat) * V;
}


void A2::initViewport() {
    
}

bool A2::clip(glm::vec4 &p1, glm::vec4 &p2) {
    
    cout << p1 << endl;
    cout << p2 << endl;
    float bl1, bl2, br1, br2, bb1, bb2, bt1, bt2, bn1, bn2, bf1, bf2;
    
    unsigned int c1, c2;
    
    c1 = c2 = 0;
    
    bl1 = p1.w + p1.x;
    if (bl1 < 0)
        c1 = c1 << 1 | 1;
    else
        c1 = c1 << 1 | 0;
    
    bl2 = p2.w + p2.x;
    if (bl2 < 0)
        c2 = c2 << 1 | 1;
    else
        c2 = c2 << 1 | 0;
    
    
    br1 = p1.w - p1.x;
    if (br1 < 0)
        c1 = c1 << 1 | 1;
    else
        c1 = c1 << 1 | 0;
    
    br2 = p2.w - p2.x;
    if (br2 < 0)
        c2 = c2 << 1 | 1;
    else
        c2 = c2 << 1 | 0;
    
    
    bb1 = p1.w + p1.y;
    if (bb1 < 0)
        c1 = c1 << 1 | 1;
    else
        c1 = c1 << 1 | 0;
    
    bb2 = p2.w + p2.y;
    if (bb2 < 0)
        c2 = c2 << 1 | 1;
    else
        c2 = c2 << 1 | 0;
    
    
    bt1 = p1.w - p1.y;
    if (bt1 < 0)
        c1 = c1 << 1 | 1;
    else
        c1 = c1 << 1 | 0;
    
    bt2 = p2.w - p2.y;
    if (bt2 < 0)
        c2 = c2 << 1 | 1;
    else
        c2 = c2 << 1 | 0;
    
    
    bn1 = p1.w + p1.z;
    if (bn1 < 0)
        c1 = c1 << 1 | 1;
    else
        c1 = c1 << 1 | 0;
    
    bn2 = p2.w + p2.z;
    if (bn2 < 0)
        c2 = c2 << 1 | 1;
    else
        c2 = c2 << 1 | 0;
    
    
    bf1 = p1.w - p1.z;
    if (bf1 < 0)
        c1 = c1 << 1 | 1;
    else
        c1 = c1 << 1 | 0;
    
    bf2 = p2.w - p2.z;
    if (bf2 < 0)
        c2 = c2 << 1 | 1;
    else
        c2 = c2 << 1 | 0;
    
    
    if ((c1 | c2) == 0) {
        return true;
    }
    
    if (c1 & c2) {
        std::cout << std::bitset<8> ((c1&c2))  << std::endl;
        std::cout << std::bitset<8> ((c1))  << std::endl;
        std::cout << std::bitset<8> ((c2))  << std::endl;

        return false;
    }
    
    
    cout << "non trivial" << endl;
    float a;
    
    a = bl1 / (bl1 - bl2);
    if (bl1 < 0) p1 = ((1 - a) * p1) + (a * p2);
    if (bl2 < 0) p2 = ((1 - a) * p1) + (a * p2);

    
    a = br1 / (br1 - br2);
    if (br1 < 0) p1 = ((1 - a) * p1) + (a * p2);
    if (br2 < 0) p2 = ((1 - a) * p1) + (a * p2);
   
    
    a = bb1 / (bb1 - bb2);
    if (bb1 < 0) p1 = ((1 - a) * p1) + (a * p2);
    if (bb2 < 0) p2 = ((1 - a) * p1) + (a * p2);
    
    
    a = bt1 / (bt1 - bt2);
    if (bt1 < 0) p1 = ((1 - a) * p1) + (a * p2);
    if (bt2 < 0) p2 = ((1 - a) * p1) + (a * p2);
 
    
    a = bn1 / (bn1 - bn2);
    if (bn1 < 0) p1 = ((1 - a) * p1) + (a * p2);
    if (bn2 < 0) p2 = ((1 - a) * p1) + (a * p2);
    
    
    a = bf1 / (bf1 - bf2);
    if (bf1 < 0) p1 = ((1 - a) * p1) + (a * p2);
    if (bf2 < 0) p2 = ((1 - a) * p1) + (a * p2);

    return true;
}





