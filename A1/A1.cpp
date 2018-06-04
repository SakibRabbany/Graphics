#include "A1.hpp"
#include "cs488-framework/GlErrorCheck.hpp"

#include <iostream>

#include <imgui/imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


using namespace glm;
using namespace std;

const float PI = 3.14159265f;

struct cell_info {
    int height;
    int color;
};


static const size_t DIM = 16;
static cell_info the_grid[DIM][DIM];
//----------------------------------------------------------------------------------------
// Constructor
A1::A1()
: current_col( 0 ),
  active_x( 0 ),
  active_y( 0 ),
  rotation( 0.0f ),
  scale( 1.0f ),
  pressed(false),
  dragged(false),
  shiftActive(false),
  prevHeight(0),
  prevColour(0)
{}

//----------------------------------------------------------------------------------------
// Destructor
A1::~A1()
{}

//----------------------------------------------------------------------------------------
/*
 * Called once, at program start.
 */
void A1::init()
{
    // Set the background colour.
    glClearColor( 0.3, 0.5, 0.7, 1.0 );
    
    // Build the shader
    m_shader.generateProgramObject();
    m_shader.attachVertexShader(
                                getAssetFilePath( "VertexShader.vs" ).c_str() );
    m_shader.attachFragmentShader(
                                  getAssetFilePath( "FragmentShader.fs" ).c_str() );
    m_shader.link();
    
    // Set up the uniforms
    P_uni = m_shader.getUniformLocation( "P" );
    V_uni = m_shader.getUniformLocation( "V" );
    M_uni = m_shader.getUniformLocation( "M" );
    col_uni = m_shader.getUniformLocation( "colour" );
    
    initGrid();
    initCube();
    
    // Set up initial view and projection matrices (need to do this here,
    // since it depends on the GLFW window being set up correctly).
    view = glm::lookAt(
                       glm::vec3( 0.0f, float(DIM)*2.0*M_SQRT1_2, float(DIM)*2.0*M_SQRT1_2 ),
                       glm::vec3( 0.0f, 0.0f, 0.0f ),
                       glm::vec3( 0.0f, 1.0f, 0.0f ) );
    
    proj = glm::perspective(
                            glm::radians( 45.0f ),
                            float( m_framebufferWidth ) / float( m_framebufferHeight ),
                            1.0f, 1000.0f );
}

void A1::initGrid()
{
    size_t sz = 3 * 2 * 2 * (DIM+3);
    
    float *verts = new float[ sz ];
    size_t ct = 0;
    for( int idx = 0; idx < DIM+3; ++idx ) {
        verts[ ct ] = -1;
        verts[ ct+1 ] = 0;
        verts[ ct+2 ] = idx-1;
        verts[ ct+3 ] = DIM+1;
        verts[ ct+4 ] = 0;
        verts[ ct+5 ] = idx-1;
        ct += 6;
        
        verts[ ct ] = idx-1;
        verts[ ct+1 ] = 0;
        verts[ ct+2 ] = -1;
        verts[ ct+3 ] = idx-1;
        verts[ ct+4 ] = 0;
        verts[ ct+5 ] = DIM+1;
        ct += 6;
    }
    
    // Create the vertex array to record buffer assignments.
    glGenVertexArrays( 1, &m_grid_vao );
    glBindVertexArray( m_grid_vao );
    
    // Create the cube vertex buffer
    glGenBuffers( 1, &m_grid_vbo );
    glBindBuffer( GL_ARRAY_BUFFER, m_grid_vbo );
    glBufferData( GL_ARRAY_BUFFER, sz*sizeof(float),
                 verts, GL_STATIC_DRAW );
    
    // Specify the means of extracting the position values properly.
    GLint posAttrib = m_shader.getAttribLocation( "position" );
    glEnableVertexAttribArray( posAttrib );
    glVertexAttribPointer( posAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr );
    
    // Reset state to prevent rogue code from messing with *my*
    // stuff!
    glBindVertexArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    
    // OpenGL has the buffer now, there's no need for us to keep a copy.
    delete [] verts;
    
    CHECK_GL_ERRORS;
    
    for(int i = 0 ; i < DIM ; i++){
        for(int j = 0 ; j < DIM ; j++){
            the_grid[i][j].height = 0;
            the_grid[i][j].color = 1;
        }
    }
    
}

void A1::initCube()
{
    GLfloat vertex_buffer_data[] = {
        0.0f, 0.0f, 0.0f, // triangle 1 : begin
        1.0f, 0.0f, 0.0f, // triangle 1 : end
        0.0f, 1.0f, 0.0f,
        
        1.0f, 0.0f, 0.0f, // triangle 2 : begin
        1.0f, 1.0f, 0.0f, // triangle 2 : end
        0.0f, 1.0f, 0.0f,
        
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        
        1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 1.0f,
        
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 1.0f,
        
        0.0f, 0.0f, 1.0f, // triangle 1 : begin
        1.0f, 0.0f, 1.0f, // triangle 1 : end
        0.0f, 1.0f, 1.0f,

        1.0f, 0.0f, 1.0f, // triangle 2 : begin
        0.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, // triangle 2 : end
        
        0.0f, 0.0f, 0.0f, // triangle 1 : begin
        0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, // triangle 1 : end
        
        1.0f, 0.0f, 1.0f, // triangle 2 : begin
        0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, // triangle 2 : end
        
        0.0f, 1.0f, 0.0f, // triangle 1 : begin
        0.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 0.0f, // triangle 1 : end
        
        1.0f, 1.0f, 1.0f, // triangle 2 : begin
        0.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 0.0f, // triangle 2 : end
    };
    
    // Create the vertex array to record buffer assignments.
    glGenVertexArrays( 1, &m_cube_vao );
    glBindVertexArray( m_cube_vao );
    
    // Create the cube vertex buffer
    glGenBuffers( 1, &m_cube_vbo );
    glBindBuffer( GL_ARRAY_BUFFER, m_cube_vbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof(vertex_buffer_data),
                 vertex_buffer_data, GL_STATIC_DRAW );
    
    // Specify the means of extracting the position values properly.
    GLint posAttrib = m_shader.getAttribLocation( "position" );
    glEnableVertexAttribArray( posAttrib );
    glVertexAttribPointer( posAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr );
    
    // Reset state to prevent rogue code from messing with *my*
    // stuff!
    glBindVertexArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    
    CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, before guiLogic().
 */
void A1::appLogic()
{
    //    active_x = int(glm::clamp(active_x, 0.0f, float(DIM)));
    //    active_y = int(glm::clamp(active_y, 0.0f, float(DIM)));
    // Place per frame, application logic here ...
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after appLogic(), but before the draw() method.
 */
void A1::guiLogic()
{
    // We already know there's only going to be one window, so for
    // simplicity we'll store button states in static local variables.
    // If there was ever a possibility of having multiple instances of
    // A1 running simultaneously, this would break; you'd want to make
    // this into instance fields of A1.
    static bool showTestWindow(false);
    static bool showDebugWindow(true);
    
    ImGuiWindowFlags windowFlags(ImGuiWindowFlags_AlwaysAutoResize);
    float opacity(0.5f);
    
    ImGui::Begin("Debug Window", &showDebugWindow, ImVec2(100,100), opacity, windowFlags);
    if( ImGui::Button( "Quit Application" ) ) {
        glfwSetWindowShouldClose(m_window, GL_TRUE);
    }
    
    // Eventually you'll create multiple colour widgets with
    // radio buttons.  If you use PushID/PopID to give them all
    // unique IDs, then ImGui will be able to keep them separate.
    // This is unnecessary with a single colour selector and
    // radio button, but I'm leaving it in as an example.
    
    // Prefixing a widget name with "##" keeps it from being
    // displayed.
    
    ImGui::PushID( 0 );
    ImGui::ColorEdit3( "Colour", colour1 );
    ImGui::SameLine();
    if( ImGui::RadioButton( "Col", &current_col, 0 ) ) {
        // Select this colour.
        //active_col = colour1;
        the_grid[active_x][active_y].color = 1;
    }
    ImGui::PopID();
    
    ImGui::PushID( 1 );
    ImGui::ColorEdit3( "Colour", colour2 );
    ImGui::SameLine();
    if( ImGui::RadioButton( "Col", &current_col, 1 ) ) {
        // Select this colour.
        //active_col = colour2;
        the_grid[active_x][active_y].color = 2;
        cout << current_col << endl;
    }
    ImGui::PopID();
    
    ImGui::PushID( 2 );
    ImGui::ColorEdit3( "Colour", colour3 );
    ImGui::SameLine();
    if( ImGui::RadioButton( "Col", &current_col, 2 ) ) {
        // Select this colour.
        //active_col = colour3;
        the_grid[active_x][active_y].color = 3;
        cout << current_col << endl;
    }
    ImGui::PopID();
    
    ImGui::PushID( 3 );
    ImGui::ColorEdit3( "Colour", colour4 );
    ImGui::SameLine();
    if( ImGui::RadioButton( "Col", &current_col, 3 ) ) {
        // Select this colour.
        //active_col = colour4;
        the_grid[active_x][active_y].color = 4;
        cout << current_col << endl;
    }
    ImGui::PopID();
    
    ImGui::PushID( 4 );
    ImGui::ColorEdit3( "Colour", colour5 );
    ImGui::SameLine();
    if( ImGui::RadioButton( "Col", &current_col, 4 ) ) {
        // Select this colour.
        //active_col = colour5;
        the_grid[active_x][active_y].color = 5;
        cout << current_col << endl;
    }
    ImGui::PopID();
    
    ImGui::PushID( 5 );
    ImGui::ColorEdit3( "Colour", colour6 );
    ImGui::SameLine();
    if( ImGui::RadioButton( "Col", &current_col, 5 ) ) {
        // Select this colour.
        //active_col = colour6;
        the_grid[active_x][active_y].color = 6;
        cout << current_col << endl;
    }
    ImGui::PopID();
    
    ImGui::PushID( 6 );
    ImGui::ColorEdit3( "Colour", colour7 );
    ImGui::SameLine();
    if( ImGui::RadioButton( "Col", &current_col, 6 ) ) {
        // Select this colour.
        //active_col = colour7;
        the_grid[active_x][active_y].color = 7;
        cout << current_col << endl;
    }
    ImGui::PopID();
    
    ImGui::PushID( 7 );
    ImGui::ColorEdit3( "Colour", colour8 );
    ImGui::SameLine();
    if( ImGui::RadioButton( "Col", &current_col, 7 ) ) {
        // Select this colour.
        //active_col = colour8;
        the_grid[active_x][active_y].color = 8;
        cout << current_col << endl;
    }
    ImGui::PopID();
    
    
    // For convenience, you can uncomment this to show ImGui's massive
    // demonstration window right in your application.  Very handy for
    // browsing around to get the widget you want.  Then look in
    // shared/imgui/imgui_demo.cpp to see how it's done.
    if( ImGui::Button( "Test Window" ) ) {
        showTestWindow = !showTestWindow;
    }
    
    
    ImGui::Text( "Framerate: %.1f FPS", ImGui::GetIO().Framerate );
    
    ImGui::End();
    
    if( showTestWindow ) {
        ImGui::ShowTestWindow( &showTestWindow );
    }
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after guiLogic().
 */
void A1:: draw()
{
    // Create a global transformation for the model (centre it).
    mat4 W;
    vec3 y_axis(0.0f,1.0f,0.0f);
    
    W = glm::scale(W, vec3(scale, scale, scale));
    W = glm::rotate(W, rotation, y_axis);
    W = glm::translate( W, vec3( -float(DIM)/2.0f, 0, -float(DIM)/2.0f ) );
    
    
    float init_x = -float(DIM)/2.0f;
    float init_y = -float(DIM)/2.0f;
    
    
    m_shader.enable();
    glEnable( GL_DEPTH_TEST );
    
    glUniformMatrix4fv( P_uni, 1, GL_FALSE, value_ptr( proj ) );
    glUniformMatrix4fv( V_uni, 1, GL_FALSE, value_ptr( view ) );
    glUniformMatrix4fv( M_uni, 1, GL_FALSE, value_ptr( W ) );
    
//     Just draw the grid for now.
    glBindVertexArray( m_grid_vao );
    glUniform3f( col_uni, 1, 1, 1 );
    glDrawArrays( GL_LINES, 0, (3+DIM)*4 );
    
    glm::mat4 model;
    model = glm::scale(model, vec3(scale, scale, scale));
    model = rotate(model, rotation , y_axis);
    model = glm::translate(model, glm::vec3(init_x,0,init_y));
    glBindVertexArray( m_cube_vao );
    
    // Draw the cubes
    for(int i = 0 ; i < DIM ; i++){
        for(int j = 0 ; j < DIM ; j++){
            int num_cubes = the_grid[i][j].height;
            if (num_cubes) {
                setGlColour(i,j);
                glm::mat4 col_model = model;
                col_model = glm::translate(col_model, glm::vec3(float(i),0,float(j)));
                for (int k = 0 ; k < num_cubes ; k++){
                    col_model = glm::translate(col_model, glm::vec3(0,float(k),0));
                    glUniformMatrix4fv( M_uni, 1, GL_FALSE, value_ptr( col_model ) );
                    glDrawArrays(GL_TRIANGLES, 0, 12*3);
                    col_model = glm::translate(col_model, glm::vec3(0,-float(k),0));
                }
            }
        }
    }
    glDisable(GL_DEPTH_TEST);

    // Draw the active cell
    glm::mat4 active_model = model;
    active_model = glm::translate(active_model, vec3(active_x, the_grid[active_x][active_y].height, active_y));
    glUniform3f( col_uni, 1, 1, 1 );
    glUniformMatrix4fv( M_uni, 1, GL_FALSE, value_ptr( active_model ) );
    glDrawArrays(GL_LINES, 0, 12*3);
    
    // Highlight the active square.
    m_shader.disable();
    
    // Restore defaults
    glBindVertexArray( 0 );
    
    CHECK_GL_ERRORS;
}

void A1::reset()
{
    for (int i = 0 ; i < DIM ; i++) {
        for (int j = 0 ; j < DIM ; j++) {
            the_grid[i][j].height = 0;
            the_grid[i][j].color = 1;
        }
    }
    
    active_x = 0;
    active_y = 0;
    rotation = 0;
    scale = 1;
    current_col = 0;
}

void A1::setGlColour(int i, int j)
{
    int color = the_grid[i][j].color;
    switch (color) {
        case 1:
            glUniform3f( col_uni, colour1[0], colour1[1], colour1[2] );
            break;
        case 2:
            glUniform3f( col_uni, colour2[0], colour2[1], colour2[2] );
            break;
        case 3:
            glUniform3f( col_uni, colour3[0], colour3[1], colour3[2] );
            break;
        case 4:
            glUniform3f( col_uni, colour4[0], colour4[1], colour4[2] );
            break;
        case 5:
            glUniform3f( col_uni, colour5[0], colour5[1], colour5[2] );
            break;
        case 6:
            glUniform3f( col_uni, colour6[0], colour6[1], colour6[2] );
            break;
        case 7:
            glUniform3f( col_uni, colour7[0], colour7[1], colour7[2] );
            break;
        case 8:
            glUniform3f( col_uni, colour8[0], colour8[1], colour8[2] );
            break;
            
        default:
            break;
    }
}

//----------------------------------------------------------------------------------------
/*
 * Called once, after program is signaled to terminate.
 */
void A1::cleanup()
{}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles cursor entering the window area events.
 */
bool A1::cursorEnterWindowEvent (
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
bool A1::mouseMoveEvent(double xPos, double yPos)
{
    bool eventHandled(false);
    
    x = xPos; y = yPos;
    
    // Translate current mouse position to GL coordinates:
    m_mouse_GL_coordinate = vec2 (
                                  (2.0f * xPos) / m_windowWidth - 1.0f,
                                  1.0f - ( (2.0f * yPos) / m_windowHeight)
                                  );
    
    
    if (!ImGui::IsMouseHoveringAnyWindow()) {
        
        // Put some code here to handle rotations.  Probably need to
        // check whether we're *dragging*, not just moving the mouse.
        // Probably need some instance variables to track the current
        // rotation amount, and maybe the previous X position (so
        // that you can rotate relative to the *change* in X.
        
        if (pressed) {
            rotation += ((m_mouse_GL_coordinate.x - initial_click.x)/2)*PI;
        }
        initial_click = m_mouse_GL_coordinate;
    }
    
    return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse button events.
 */
bool A1::mouseButtonInputEvent(int button, int actions, int mods) {
    bool eventHandled(false);
    
    if (!ImGui::IsMouseHoveringAnyWindow()) {
        // The user clicked in the window.  If it's the left
        // mouse button, initiate a rotation.
        if (actions == GLFW_PRESS) {
            pressed = true;
            initial_click = m_mouse_GL_coordinate;
            cout << "clicked" << endl;
            cout << m_mouse_GL_coordinate.x << " " << m_mouse_GL_coordinate.y << endl;
            cout << x << " " << y << endl;
        }
        
        if (actions == GLFW_RELEASE) {
            pressed = false;
        }
    }
    
    return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse scroll wheel events.
 */
bool A1::mouseScrollEvent(double xOffSet, double yOffSet) {
    bool eventHandled(false);
    
    // Zoom in or out.
    scale += yOffSet * 0.01;
    if (scale < 0.2) {
        scale = 0.2;
    }
    
    return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles window resize events.
 */
bool A1::windowResizeEvent(int width, int height) {
    bool eventHandled(false);
    
    // Fill in with event handling code...
    
    return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles key input events.
 */
bool A1::keyInputEvent(int key, int action, int mods) {
    bool eventHandled(false);
    
    // Fill in with event handling code...
    if( action == GLFW_PRESS || action == GLFW_REPEAT ) {
        // Respond to some key events.
        if (key == GLFW_KEY_Q) {
            glfwSetWindowShouldClose(m_window, GL_TRUE);
        }
        if (key == GLFW_KEY_R) {
            reset();
        }
        if (key == GLFW_KEY_SPACE) {
            the_grid[active_x][active_y].height++;
        }
        if (key == GLFW_KEY_BACKSPACE) {
            if (the_grid[active_x][active_y].height)
                the_grid[active_x][active_y].height--;
        }
        if (key == GLFW_KEY_UP) {
            if (active_y > 0){
                active_y--;
                if (shiftActive) {
                    the_grid[active_x][active_y].color = prevColour;
                    the_grid[active_x][active_y].height = prevHeight;
                } else {
                    int height = the_grid[active_x][active_y].height;
                    if (height) {
                        current_col = the_grid[active_x][active_y].color - 1;
                    } else {
                        the_grid[active_x][active_y].color = current_col+1;
                    }
                }
            }
        }
        if (key == GLFW_KEY_DOWN) {
            if (active_y < DIM - 1){
                active_y++;
                if (shiftActive) {
                    the_grid[active_x][active_y].color = prevColour;
                    the_grid[active_x][active_y].height = prevHeight;
                } else {
                    int height = the_grid[active_x][active_y].height;
                    if (height) {
                        current_col = the_grid[active_x][active_y].color - 1;
                    } else {
                        the_grid[active_x][active_y].color = current_col+1;
                    }
                }
            }
        }
        if (key == GLFW_KEY_LEFT) {
            if (active_x > 0){
                active_x--;
                if (shiftActive) {
                    the_grid[active_x][active_y].color = prevColour;
                    the_grid[active_x][active_y].height = prevHeight;
                } else {
                    int height = the_grid[active_x][active_y].height;
                    if (height) {
                        current_col = the_grid[active_x][active_y].color - 1;
                    } else {
                        the_grid[active_x][active_y].color = current_col+1;
                    }
                }
            }
        }
        if (key == GLFW_KEY_RIGHT) {
            if (active_x < DIM - 1){
                active_x++;
                if (shiftActive) {
                    the_grid[active_x][active_y].color = prevColour;
                    the_grid[active_x][active_y].height = prevHeight;
                } else {
                    int height = the_grid[active_x][active_y].height;
                    if (height) {
                        current_col = the_grid[active_x][active_y].color - 1;
                    } else {
                        the_grid[active_x][active_y].color = current_col+1;
                    }
                }
            }
        }
        if (key == GLFW_KEY_EQUAL) {
            cout << "+ key pressed" << endl;
            
            // TODO - increase shape size.
            scale += 0.025;
            
        }
        if (key == GLFW_KEY_MINUS) {
            cout << "- key pressed" << endl;
            
            // TODO - decrease shape size.
            scale -= 0.025;
            
        }
        if (key == GLFW_KEY_Z) {
            
            rotation -= 0.025;
            
        }
        if (key == GLFW_KEY_X) {
            
            rotation += 0.025;
            
        }
        if (key == GLFW_KEY_A) {
            
            current_col += 1;
            if (current_col > 7) {
                current_col = 0;
            }
            the_grid[active_x][active_y].color = current_col + 1;
            
        }
        if (key == GLFW_KEY_S) {
            
            current_col -= 1;
            if (current_col < 0) {
                current_col = 7;
            }
            the_grid[active_x][active_y].color = current_col + 1;
            
        }
        if (key == GLFW_KEY_RIGHT_SHIFT || key == GLFW_KEY_LEFT_SHIFT) {
            shiftActive = true;
            prevHeight = the_grid[active_x][active_y].height;
            prevColour= the_grid[active_x][active_y].color;
        }
    }
    
    if (action == GLFW_RELEASE) {
        if (key == GLFW_KEY_RIGHT_SHIFT || key == GLFW_KEY_LEFT_SHIFT) {
            shiftActive = false;
        }
    }
    
    return eventHandled;
}

