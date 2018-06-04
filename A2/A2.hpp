#pragma once

#include "cs488-framework/CS488Window.hpp"
#include "cs488-framework/OpenGLImport.hpp"
#include "cs488-framework/ShaderProgram.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <utility>

// Set a global maximum number of vertices in order to pre-allocate VBO data
// in one shot, rather than reallocating each frame.
const GLsizei kMaxVertices = 1000;


// Convenience class for storing vertex data in CPU memory.
// Data should be copied over to GPU memory via VBO storage before rendering.
class VertexData {
public:
    VertexData();
    
    std::vector<glm::vec2> positions;
    std::vector<glm::vec3> colours;
    GLuint index;
    GLsizei numVertices;
};


class A2 : public CS488Window {
public:
    A2();
    virtual ~A2();
    
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
    
    void createShaderProgram();
    void enableVertexAttribIndices();
    void generateVertexBuffers();
    void mapVboDataToVertexAttributeLocation();
    void uploadVertexDataToVbos();
    
    void initLineData();
    
    void setLineColour(const glm::vec3 & colour);
    
    void drawLine (
                   const glm::vec2 & v0,
                   const glm::vec2 & v1
                   );
    
    ShaderProgram m_shader;
    
    GLuint m_vao;            // Vertex Array Object
    GLuint m_vbo_positions;  // Vertex Buffer Object
    GLuint m_vbo_colours;    // Vertex Buffer Object
    
    VertexData m_vertexData;
    
    glm::vec3 m_currentLineColour;
    
    // New Code
private:
    
    enum Axis {
        x_axis,
        y_axis,
        z_axis,
    };
    
    //    enum UserMode {
    //        R,
    //        T,
    //        S,
    //        O,
    //        N,
    //        P,
    //        V
    //    };
    
    // Cube Vertices
    std::vector<std::pair<glm::vec4, glm::vec4>> m_cube_lines;
    
    // Cube gnomon vertices
    std::vector<std::pair<glm::vec4, glm::vec4>> m_cube_gnomon_lines;
    
    // World gnomon vertices
    std::vector<std::pair<glm::vec4, glm::vec4>> m_world_gnomon_lines;
    
    std::vector<std::pair<glm::vec2, glm::vec2>> m_viewport;
    
    // View Matrix
    glm::mat4 V;
    
    // Model Matrix
    glm::mat4 M;
    
    // Perspective Matrix
    glm::mat4 P;
    
    // Perspective variables
    float near;
    float far;
    float field_of_view;
    float frame_buffer_width, frame_buffer_height;
    
    // interaction mode
    int user_mode;
    
    // mouse interaction
    bool left_mouse_pressed;
    bool middle_mouse_pressed;
    bool right_mouse_pressed;
    bool viewport_mode;
    bool dragged;
    
    glm::vec2 m_mouse_GL_coordinate;
    glm::vec2 initial_click;
    
    // transformation matrix;
    glm::mat4 model_translate_mat, model_rotate_mat, model_scale_mat;
    glm::mat4 view_translate_mat, view_rotate_mat;
    
    void initCube();
    void drawCube();
    void initGnomons();
    void drawWorldGnomon();
    void drawModelGnomon();
    void drawViewport();
    
    void makeViewport();
    
    void updateMatrices(glm::vec2& delta, Axis axis);
    void updateViewport();
    
    bool clip(glm::vec4& p1, glm::vec4& p2);
    
    
    glm::mat4 translate(glm::mat4 const &mat, glm::vec3 const &t);
    glm::mat4 scale(glm::mat4 const &mat, glm::vec3 const &s);
    glm::mat4 rotate(glm::mat4 const &mat, float angle, Axis axis);
    
    glm::mat4 look_at(glm::vec3 const &camera_position, glm::vec3 const &center, glm::vec3 const &up);
    
    float view_port_width;
    float view_port_height;
    glm::vec4 view_port_center;
    glm::vec2 view_port_start;
    glm::vec2 view_port_end;

    
    void reset();
    
    glm::mat4 perspective(float fov, float aspect, float near, float far);
    //    glm::vec3 cross(glm::vec3 const &v1, glm::vec3 const &v2);
    void mapWorldToViewPortAndDraw(const glm::vec2 & p1, const glm::vec2 & p2);

};

