#pragma once

#include <glm/glm.hpp>

#include "cs488-framework/CS488Window.hpp"
#include "cs488-framework/OpenGLImport.hpp"
#include "cs488-framework/ShaderProgram.hpp"

#include "grid.hpp"


class A1 : public CS488Window {
public:
    A1();
    virtual ~A1();
    
protected:
    virtual void init() override;
    virtual void appLogic() override;
    virtual void guiLogic() override;
    virtual void draw() override;
    virtual void cleanup() override;
    
    virtual bool cursorEnterWindowEvent(int entered) override;
    virtual bool mouseMoveEvent(double xPos, double yPos) override;
    virtual bool mouseButtonInputEvent(int button, int actions, int mods) override;
    virtual bool mouseScrollEvent(double xOffSet, double yOffSet) override;
    virtual bool windowResizeEvent(int width, int height) override;
    virtual bool keyInputEvent(int key, int action, int mods) override;
    
private:
    void initGrid();
    void initCube();
    
    void reset();
    
    void setGlColour(int i, int j);
    
    // Fields related to the shader and uniforms.
    ShaderProgram m_shader;
    GLint P_uni; // Uniform location for Projection matrix.
    GLint V_uni; // Uniform location for View matrix.
    GLint M_uni; // Uniform location for Model matrix.
    GLint col_uni;   // Uniform location for cube colour.
    
    // Fields related to grid geometry.
    GLuint m_grid_vao; // Vertex Array Object
    GLuint m_grid_vbo; // Vertex Buffer Object
    
    // Fields related to cube geometry.
    GLuint m_cube_vao; // Vertex Array Object
    GLuint m_cube_vbo; // Vertex Buffer Object
    
    
    // Matrices controlling the camera and projection.
    glm::mat4 proj;
    glm::mat4 view;
    
    float colour1[3] = {0.0f,0.0f,0.0f};
    float colour2[3] = {0.0f,0.0f,1.0f};
    float colour3[3] = {0.0f,1.0f,0.0f};
    float colour4[3] = {0.0f,1.0f,1.0f};
    float colour5[3] = {1.0f,0.0f,0.0f};
    float colour6[3] = {1.0f,0.0f,1.0f};
    float colour7[3] = {1.0f,1.0f,0.0f};
    float colour8[3] = {1.0f,1.0f,1.0f};
    
    int current_col;
    float active_col[3];
    
    //the transformations
    float rotation;
    float scale;
    
    //shift move
    bool shiftActive;
    int prevHeight;
    int prevColour;
    
    // Fields related to rotation and scaling
    bool pressed;
    bool dragged;
    
    glm::vec2 m_mouse_GL_coordinate;
    glm::vec2 initial_click;
    float x;
    float y;
    
    int active_x;
    int active_y;
};

