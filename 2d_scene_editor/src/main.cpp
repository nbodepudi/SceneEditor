// This example is heavily based on the tutorial at https://open.gl

// OpenGL Helpers to reduce the clutter
#include "Helpers.h"
#include <iostream>
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
// GLFW is necessary to handle the OpenGL context
#include <GLFW/glfw3.h>
#else
// GLFW is necessary to handle the OpenGL context
#include <GLFW/glfw3.h>
#endif

// OpenGL Mathematics Library
#include <glm/glm.hpp> // glm::vec3
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

// Timer
#include <chrono>

// VertexBufferObject wrapper
VertexBufferObject VBO;
VertexBufferObject color_VBO;

VertexBufferObject line_VBO;
VertexBufferObject line_color_VBO;

// Contains the vertex positions
std::vector<glm::vec2> V(0);
std::vector<glm::vec4> color(0);

glm::vec4 temp_color; // Variable for assigning color to a vertex

std::vector<glm::vec2> line_v(0);
std::vector<glm::vec4> line_color(0);

glm::mat2x2  vscale;
glm::vec2 vpos;

glm::mat4x4 rotation;
glm::mat4x4 scaling;
glm::mat4x4 toOrignal, toBack;
glm::mat4x4 translation = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
glm::mat4x4 applied_translation;


int state_flag = 0; // Based on the mode state flag changes
int translation_flag = 0; // enables when one of the key is pressed

int mouse_clicks = 0; // In the insertion mode.
int mouse_flag = 0;


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

class point{
public:
    double x;
    double y;
    point(){
        x = 0.0;
        y = 0.0;
    }

    point(double x1, double y1){
        x = x1;
        y = y1;
    }

};

class triangle{
public:
    std::vector<point> vert;
    bool clicked = false;
    int start_index = -1;
};

triangle selected_traingle;
int v_clicked = 0;

point start_click;

int animation_step_flag = 0;
int j = 0;
std::vector<glm::vec2> animation_V(0);
std::vector<int> vector_index(0);
std::chrono::time_point<std::chrono::high_resolution_clock> start_time;


glm::vec2 find_barycenter(point a, point b, point c){
    return glm::vec2((a.x + b.x + c.x) / 3, (a.y + b.y + c.y) / 3);
}


void transform_triangle_without(int mode){
    glm::vec2 bary = find_barycenter(selected_traingle.vert[0], selected_traingle.vert[1], selected_traingle.vert[2]);
    float clockwise = (float)(-10*3.141592f/180);
    float anti_clockwise = (float)(10*3.141592f/180);
    float scale_up = 1.25, scale_down = 0.75;

    glm::vec2 temp_vert1, temp_vert2, temp_vert3;

    temp_vert1 = {selected_traingle.vert[0].x - bary[0], selected_traingle.vert[0].y-bary[1]};
    temp_vert2 = {selected_traingle.vert[1].x - bary[0], selected_traingle.vert[1].y-bary[1]};
    temp_vert3 = {selected_traingle.vert[2].x - bary[0], selected_traingle.vert[2].y-bary[1]};

    glm::mat2x2 trans = {1,0,0,1};
    if(mode == 1){
        trans = {cos(anti_clockwise), sin(anti_clockwise), -sin(anti_clockwise), cos(anti_clockwise)};
        temp_vert1 = trans*temp_vert1;
        temp_vert2 = trans*temp_vert2;
        temp_vert3 = trans*temp_vert3;
    }
    else if(mode == 2){
        trans = {cos(clockwise), sin(clockwise), -sin(clockwise), cos(clockwise)};
        temp_vert1 = trans*temp_vert1;
        temp_vert2 = trans*temp_vert2;
        temp_vert3 = trans*temp_vert3;
    }
    else if(mode == 3){
        trans = {scale_up, 0, 0, scale_up};
        temp_vert1 = trans*temp_vert1;
        temp_vert2 = trans*temp_vert2;
        temp_vert3 = trans*temp_vert3;
    }
    else if(mode == 4){
        trans = {scale_down, 0, 0, scale_down};
        temp_vert1 = trans*temp_vert1;
        temp_vert2 = trans*temp_vert2;
        temp_vert3 = trans*temp_vert3;
    }

    temp_vert1 = temp_vert1 + bary;
    temp_vert2 = temp_vert2 + bary;
    temp_vert3 = temp_vert3 + bary;

    selected_traingle.vert = {point(temp_vert1[0],temp_vert1[1]), point(temp_vert2[0], temp_vert2[1]), point(temp_vert3[0], temp_vert3[1])};
    V[selected_traingle.start_index] = temp_vert1;
    V[selected_traingle.start_index + 1] = temp_vert2;
    V[selected_traingle.start_index + 2] = temp_vert3;
    line_v[0] = temp_vert1;
    line_v[1] = temp_vert2;
    line_v[2] = temp_vert3;
    VBO.update(V);
    line_VBO.update(line_v);
}

void transform_triangle(int mode){
    // Constant for rotation and scaling
    float clockwise = (float)(-10*3.141592f/180);
    float anti_clockwise = (float)(10*3.141592f/180);
    float scale_up = 1.25, scale_down = 0.75;

    glm::vec2 bary;
    if (mode == 1){
        bary = find_barycenter(selected_traingle.vert[0], selected_traingle.vert[1], selected_traingle.vert[2]);
        rotation = {cos(clockwise), -sin(clockwise), 0, 0, sin(clockwise), cos(clockwise),  0, 0, 0, 0, 1, 0, 0,0,0, 1};
        toOrignal = { 1, 0, 0, -bary[0], 0, 1, 0, -bary[1], 0, 0, 1, 0, 0, 0, 0, 1};
        toBack = { 1, 0, 0, bary[0], 0, 1, 0, bary[1], 0, 0, 1, 0, 0, 0, 0, 1};
        applied_translation =  toBack * rotation * toOrignal;
        translation_flag = 1;
        VBO.update(V);
    }
    else if(mode == 2){
        bary = find_barycenter(selected_traingle.vert[0], selected_traingle.vert[1], selected_traingle.vert[2]);
        rotation = {cos(anti_clockwise), -sin(anti_clockwise), 0, 0, sin(anti_clockwise), cos(anti_clockwise),  0, 0, 0, 0, 1, 0, 0,0,0, 1};
        toOrignal = { 1, 0, 0, -bary[0], 0, 1, 0, -bary[1], 0, 0, 1, 0, 0, 0, 0, 1};
        toBack = { 1, 0, 0, bary[0], 0, 1, 0, bary[1], 0, 0, 1, 0, 0, 0, 0, 1};
        applied_translation =  toBack * rotation * toOrignal;
        translation_flag = 1;
    }
    else if(mode == 3){
        bary = find_barycenter(selected_traingle.vert[0], selected_traingle.vert[1], selected_traingle.vert[2]);
        scaling = {scale_up, 0, 0, 0, 0, scale_up,  0, 0, 0, 0, 1, 0, 0,0,0, 1};
        toOrignal = { 1, 0, 0, -bary[0], 0, 1, 0, -bary[1], 0, 0, 1, 0, 0, 0, 0, 1};
        toBack = { 1, 0, 0, bary[0], 0, 1, 0, bary[1], 0, 0, 1, 0, 0, 0, 0, 1};
        applied_translation =  toBack * scaling * toOrignal;
        translation_flag = 1;
    }
    else if(mode == 4){
        bary = find_barycenter(selected_traingle.vert[0], selected_traingle.vert[1], selected_traingle.vert[2]);
        scaling = {scale_down, 0, 0, 0, 0, scale_down,  0, 0, 0, 0, 1, 0, 0,0,0, 1};
        toOrignal = { 1, 0, 0, -bary[0], 0, 1, 0, -bary[1], 0, 0, 1, 0, 0, 0, 0, 1};
        toBack = { 1, 0, 0, bary[0], 0, 1, 0, bary[1], 0, 0, 1, 0, 0, 0, 0, 1};
        applied_translation =  toBack * scaling * toOrignal;
        translation_flag = 1;
    }
}

point get_pos(GLFWwindow* window)
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    int width, height;
    glfwGetWindowSize(window, &width, &height);

    double xworld = ((xpos/double(width))*2)-1;
    double yworld = (((height-1-ypos)/double(height))*2)-1;

    glm::mat2x2 inverse_scale;
    inverse_scale = {1/vscale[0][0], 0, 0, 1/ vscale[1][1]};

    glm::vec2 temp_vec = {(float) xworld, (float) yworld};
    temp_vec = inverse_scale*temp_vec - vpos;
    return point((double) temp_vec[0], (double) temp_vec[1]);
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos){
    point screen_click = get_pos(window);

    if (state_flag == 1){
        if (mouse_clicks > 0){
            line_v[mouse_clicks] = (glm::vec2(screen_click.x, screen_click.y));
            line_color[mouse_clicks] = glm::vec4(1.0f,1.0f,1.0f,1.0f);
            //line_c(g
            line_VBO.update(line_v);
            line_color_VBO.update(line_color);
            mouse_flag = 1;
        }
    }
    else if (state_flag == 2){
        if(selected_traingle.clicked){
            glm::vec2 new1 = {screen_click.x - start_click.x , screen_click.y - start_click.y};
            for(unsigned i=0;i<3;i++){
                V[selected_traingle.start_index+i][0] = selected_traingle.vert[i].x + new1[0];
                V[selected_traingle.start_index+i][1] = selected_traingle.vert[i].y + new1[1];
                line_v[i] = glm::vec2{selected_traingle.vert[i].x + new1[0], selected_traingle.vert[i].y + new1[1]};
            }
            VBO.update(V);
            line_VBO.update(line_v);
        }
    }
    else if (state_flag == 5){
        if(selected_traingle.clicked){
            glm::vec2 new1 = {screen_click.x - start_click.x , screen_click.y - start_click.y};
            for(unsigned i=0;i<3;i++){
                vector_index.push_back(selected_traingle.start_index);
               // animation_V[selected_traingle.start_index+i][0] = selected_traingle.vert[i].x + new1[0];
                //animation_V[selected_traingle.start_index+i][1] = selected_traingle.vert[i].y + new1[1];
                line_v[i] = glm::vec2{selected_traingle.vert[i].x + new1[0], selected_traingle.vert[i].y + new1[1]};
            }
            VBO.update(V);
            line_VBO.update(line_v);
        }
    }
}

// Thanks to stackoverflow for this https://stackoverflow.com/questions/2049582/how-to-determine-if-a-point-is-in-a-2d-triangle
float sign (point p1, point p2, point p3)
{
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool check_point (point click_point, std::vector<point> test)
{
    float d1, d2, d3;
    bool has_neg, has_pos;

    d1 = sign(click_point, test[0], test[1]);
    d2 = sign(click_point, test[1], test[2]);
    d3 = sign(click_point, test[2], test[0]);

    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    // Get the position of the mouse in the window

    point screen_click = get_pos(window);

    // Update the position of the first vertex if the left button is pressed
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        {
            if (state_flag == 1){
               if (mouse_clicks == 0){
                   line_v = std::vector<glm::vec2>(0);
                   line_color = std::vector<glm::vec4>(0);

                   line_v.push_back(glm::vec2(screen_click.x,screen_click.y));
                   line_color.push_back(glm::vec4(1.0f,1.0f,1.0f,1.0f));

                   line_v.push_back(glm::vec2(screen_click.x,screen_click.y)); // This will be replaced by the mousecursor
                   line_color.push_back(glm::vec4(1.0f,1.0f,1.0f,1.0f));

                   line_VBO.update(line_v);
                   line_color_VBO.update(line_color);
                   mouse_clicks += 1;
                   mouse_flag = 0;

                   //std::cout << "click 0 - "  << line_v.size() << std::endl;
               }
               else if(mouse_clicks == 1){

                   line_v[mouse_clicks] = glm::vec2(screen_click.x,screen_click.y);
                   line_v.push_back( glm::vec2(screen_click.x,screen_click.y));
                   line_color.push_back(glm::vec4(1.0f,1.0f,1.0f,1.0f));
                   line_v.push_back(line_v[0]); //For displaying the complete traingle
                   line_color.push_back(glm::vec4(1.0f,1.0f,1.0f,1.0f));

                   line_VBO.update(line_v);
                   line_color_VBO.update(line_color);
                   //std::cout << "click 1 -" << line_v.size() << std::endl;
                   mouse_clicks++;
                   mouse_flag = 0;
               }
               else if(mouse_clicks == 2){
                   for(unsigned i=0 ; i<3; i++){
                       V.push_back(line_v[i]);
                       color.push_back(glm::vec4(1.0f,0.0f,0.0f,0.0f));// Pushing red for vertices
                   }

                   VBO.update(V);
                   color_VBO.update(color);

                   line_v = std::vector<glm::vec2>(0);
                   line_color = std::vector<glm::vec4>(0);
                   mouse_clicks = 0;
                   mouse_flag = 0;
               }
               //V.emplace_back(glm::vec2(screen_click.x, screen_click.y));
            }
            else if (state_flag == 4){
               // std::cout << "came inside here" << std::endl;
                float dist;
                float min_dist = std::numeric_limits<float>::infinity();
                for(unsigned i=0; i<V.size(); i++){
                    dist = (V[i][0] - screen_click.x)*(V[i][0] - screen_click.x) + (V[i][1] - screen_click.y)*(V[i][1] - screen_click.y);
                    if(dist < min_dist){
                        min_dist = dist;
                        v_clicked = i;
                    }
                }
                color[v_clicked] = {0.25f, 0.25f, 0.25f, 1.0f};
                color_VBO.update(color);
                mouse_clicks++;
            }
            else if (state_flag == 2){
                if (mouse_clicks == 0){
                    //std::cout << "came here" << std::endl;
                    for(unsigned i=0;i<V.size();i+=3){

                        std::vector<point> temp;
                        temp.push_back(point(V[i][0], V[i][1]));
                        temp.push_back(point(V[i+1][0], V[i+1][1]));
                        temp.push_back(point(V[i+2][0], V[i+2][1]));
                        if (check_point(screen_click, temp)){
                            //std::cout << "found the point " << std::endl;
                            selected_traingle.vert = temp;
                            selected_traingle.clicked = true;
                            selected_traingle.start_index = i;
                            line_v = std::vector<glm::vec2>(0);
                            line_color = std::vector<glm::vec4>(0);
                            line_v.push_back(V[i]);
                            line_color.push_back({1.0f, 1.0f, 1.0f, 1.0f});
                            line_v.push_back(V[i+1]);
                            line_color.push_back({1.0f, 1.0f, 1.0f, 1.0f});
                            line_v.push_back(V[i+2]);
                            line_color.push_back({1.0f, 1.0f, 1.0f, 1.0f});
                            start_click = screen_click;
                            mouse_clicks ++;

                            line_VBO.update(line_v);
                            line_color_VBO.update(line_color);
                            break;
                        }

                    }
                }
                else{
                    selected_traingle.clicked = false;
                    selected_traingle.start_index = -1;
                    mouse_clicks = 0;

                    applied_translation = translation;
                    translation_flag = 0;
                    line_v = std::vector<glm::vec2>(0);
                    line_color = std::vector<glm::vec4>(0);
                }
            }
            else if(state_flag == 3){
                if (mouse_clicks == 0){
                   // std::cout << "came here" << std::endl;
                    for(unsigned i=0;i<V.size();i+=3){

                        std::vector<point> temp;
                        temp.push_back(point(V[i][0], V[i][1]));
                        temp.push_back(point(V[i+1][0], V[i+1][1]));
                        temp.push_back(point(V[i+2][0], V[i+2][1]));
                        if (check_point(screen_click, temp)){
                           // std::cout << "found the point " << std::endl;
                            V.erase(V.begin()+i, V.begin()+i+3);
                            if(V.size() > 0){
                                VBO.update(V);
                            }
                            break;
                        }

                    }
                }
            }
            else if(state_flag == 5){
                //Animation mode
                if (mouse_clicks == 0 && animation_step_flag == 1){
                    //std::cout << "came here" << std::endl;
                    for(unsigned i=0;i<V.size();i+=3){

                        std::vector<point> temp;
                        temp.push_back(point(V[i][0], V[i][1]));
                        temp.push_back(point(V[i+1][0], V[i+1][1]));
                        temp.push_back(point(V[i+2][0], V[i+2][1]));
                        if (check_point(screen_click, temp)){
                            //std::cout << "found the point " << std::endl;
                            selected_traingle.vert = temp;
                            selected_traingle.clicked = true;
                            selected_traingle.start_index = i;
                            line_v = std::vector<glm::vec2>(0);
                            line_color = std::vector<glm::vec4>(0);
                            line_v.push_back(V[i]);
                            line_color.push_back({1.0f, 1.0f, 1.0f, 1.0f});
                            line_v.push_back(V[i+1]);
                            line_color.push_back({1.0f, 1.0f, 1.0f, 1.0f});
                            line_v.push_back(V[i+2]);
                            line_color.push_back({1.0f, 1.0f, 1.0f, 1.0f});
                            start_click = screen_click;
                            mouse_clicks ++;

                            line_VBO.update(line_v);
                            line_color_VBO.update(line_color);
                            break;
                        }

                    }
                }
                else{
                    if (animation_step_flag == 1){
                        std::cout << "key frame successfully added" << std::endl;
                        animation_V[selected_traingle.start_index] = line_v[0];
                        animation_V[selected_traingle.start_index+1] = line_v[1];
                        animation_V[selected_traingle.start_index+2] = line_v[2];

                        selected_traingle.clicked = false;
                        selected_traingle.start_index = -1;
                        mouse_clicks = 0;

                        //applied_translation = translation;
                        //translation_flag = 0;
                        line_v = std::vector<glm::vec2>(0);
                        line_color = std::vector<glm::vec4>(0);
                    }
                }
            }

        }
}



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Update the position of the first vertex if the keys 1,2, or 3 are pressed
    if (action == GLFW_RELEASE){
        switch (key)
        {
            case  GLFW_KEY_I:
                state_flag = 1;
                mouse_clicks = 0;
                std::cout << "Insertion Mode" << std::endl;
                break;
            case GLFW_KEY_O:
                state_flag = 2;
                mouse_clicks = 0;
                std::cout << "Translation Mode" << std::endl;
                break;
            case GLFW_KEY_P:
                state_flag = 3;
                mouse_clicks = 0;
                std::cout << "Deletetion Mode" << std::endl;
                break;
            case GLFW_KEY_W:
                vpos = vpos - glm::vec2(0,1)*(float) (0.2*2/vscale[1][1]);
                mouse_clicks = 0;
                break;
            case GLFW_KEY_S:
                vpos = vpos + glm::vec2(0,1)*(float) (0.2*2/vscale[1][1]);
                mouse_clicks = 0;
                break;
            case GLFW_KEY_A:
                vpos = vpos + glm::vec2(1,0)*(float) (0.2*2/vscale[0][0]);
                mouse_clicks = 0;
                break;
            case GLFW_KEY_D:
                vpos = vpos - glm::vec2(1,0)*(float) (0.2*2/vscale[0][0]);
                mouse_clicks = 0;
                break;
            case GLFW_KEY_EQUAL:
                vscale = vscale + glm::mat2x2(1,0,0,1)*0.2f;
                mouse_clicks = 0;
                break;
            case GLFW_KEY_MINUS:
                vscale = vscale - glm::mat2x2(1,0,0,1)*0.2f;
                mouse_clicks = 0;
                break;
            case GLFW_KEY_C:
                state_flag = 4;
                mouse_clicks = 0;
                std:: cout << "Vertex color mode" << std::endl;
                break;
            case GLFW_KEY_F:
                state_flag = 5;
                mouse_clicks = 0;
                std::cout << "Keyframe mode selected" << std::endl;
                animation_step_flag = 1;
                animation_V = V;
                std::cout << "Enabling translation mode to selected Key frame" << std::endl;
                std::cout << "Press Enter when done" << std::endl;
                break;
            case GLFW_KEY_ENTER:
                if (state_flag == 5){
                    animation_step_flag = 2;
                    std::cout << "Playing animation. (Enter space to stop)" << std::endl;
                    start_time = std::chrono::high_resolution_clock::now();
                }
                break;
            case GLFW_KEY_SPACE:
                if (state_flag == 5){
                    animation_step_flag = 0;
                    animation_V = V;
                    std::cout << "Animation stopped." << std:: endl;
                    std::cout << "Translation mode enabled" << std::endl;
                    state_flag = 2;
                    mouse_clicks = 0;
                }
                break;
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GL_TRUE);
                std::cout << "Bye!" << std::endl;
                break;
            default:
                break;
        }
        //128.0f/255.0f, 178.0f/255.0f, 1,            1,             1, 124.0f/255.0f, 0,             30.0f/255.0f,  188.0f/255.0f,
        //                         0,             34.0f/255.0f,  69.0f/255.0f, 165.0f/255.0f, 1, 252.0f/255.0f, 1,             144.0f/255.0f, 143.0f/255.0f,
        //                         0,             34.0f/255.0f,  0,            0,             0, 0,             127.0f/255.0f, 1,             143.0f/255.0f;
        if (state_flag == 4){
            if(mouse_clicks > 0){

                switch (key)
                {
                    case  GLFW_KEY_1:
                        temp_color = {128.0f/255.0f, 178.0f/255.0f, 1.0f,1.0f};
                        break;
                    case  GLFW_KEY_2:
                        temp_color = {1.0f, 124.0f/255.0f, 0.0f, 1.0f};
                        break;
                    case  GLFW_KEY_3:
                        temp_color = {1.0f,1.0f,1.0f,1.0f};
                        break;
                    case  GLFW_KEY_4:
                        temp_color = {0.0f,0.0f,0.5f,1.0f};
                        break;
                    case  GLFW_KEY_5:
                        temp_color = {0.5f,1.0f,0.0f,1.0f};
                        break;
                    case  GLFW_KEY_6:
                        temp_color = {0.5f,0.0f,0.5f,1.0f};
                        break;
                    case  GLFW_KEY_7:
                        temp_color = {0.25f,1.0f,0.25f,1.0f};
                        break;
                    case  GLFW_KEY_8:
                        temp_color = {0.0f,0.25f,1.0f,1.0f};
                        break;
                    case  GLFW_KEY_9:
                        temp_color = {0.0f,0.75f,0.25f,1.0f};
                        break;
                    default:
                        break;
                }
                color[v_clicked] = temp_color;
                color_VBO.update(color);
            }
        }
        if (selected_traingle.clicked){
            switch(key){
                case GLFW_KEY_H:
                    transform_triangle_without(1);
                    break;
                case GLFW_KEY_J:
                    transform_triangle_without(2);
                    break;
                case GLFW_KEY_K:
                    transform_triangle_without(3);
                    break;
                case GLFW_KEY_L:
                    transform_triangle_without(4);
                    break;
            }
        }
    }


}

int main(void)
{
    GLFWwindow* window;

    // Initialize the library
    if (!glfwInit())
        return -1;

    // Activate supersampling
    glfwWindowHint(GLFW_SAMPLES, 8);

    // Ensure that we get at least a 3.2 context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

    // On apple we have to load a core profile with forward compatibility
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(640, 480, "2d Graphics", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    #ifndef __APPLE__
      glewExperimental = true;
      GLenum err = glewInit();
      if(GLEW_OK != err)
      {
        /* Problem: glewInit failed, something is seriously wrong. */
       fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
      }
      glGetError(); // pull and savely ignonre unhandled errors like GL_INVALID_ENUM
      fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
    #endif

    int major, minor, rev;
    major = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
    minor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
    rev = glfwGetWindowAttrib(window, GLFW_CONTEXT_REVISION);
    printf("OpenGL version recieved: %d.%d.%d\n", major, minor, rev);
    printf("Supported OpenGL is %s\n", (const char*)glGetString(GL_VERSION));
    printf("Supported GLSL is %s\n", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
    std::cout<< "Enter you commands!" << std::endl;

    // Initialize the VAO
    // A Vertex Array Object (or VAO) is an object that describes how the vertex
    // attributes are stored in a Vertex Buffer Object (or VBO). This means that
    // the VAO is not the actual object storing the vertex data,
    // but the descriptor of the vertex data.
    VertexArrayObject VAO;
    VAO.init();
    VAO.bind();

    // Initialize the VBO with the vertices data
    // A VBO is a data container that lives in the GPU memory
    VBO.init();
    color_VBO.init();

    line_VBO.init();
    line_color_VBO.init();

    vscale = {1,0,0,1};
    vpos = {0,0};

    // Initialize the OpenGL Program
    // A program controls the OpenGL pipeline and it must contains
    // at least a vertex shader and a fragment shader to be valid
    Program program;
    const GLchar* vertex_shader =
            "#version 150 core\n"
                    "in vec2 position;"
                    "in vec4 color;"
                    "uniform mat2 view;"
                    "uniform vec2 trans;"
                    "uniform mat4 translation;"
                    "out vec4 f_color;"
                    "void main()"
                    "{"
                    "    gl_Position = translation*vec4(view * (position + trans) , 0.0, 1.0);"
                    "     f_color = color;"
                    "}";
    const GLchar* fragment_shader =
            "#version 150 core\n"
                    "in vec4 f_color;"
                    "out vec4 outColor;"
                    "uniform vec4 triangleColor;"
                    "void main()"
                    "{"
                    "    outColor = vec4(f_color);"
                    "}";

    // Compile the two shaders and upload the binary to the GPU
    // Note that we have to explicitly specify that the output "slot" called outColor
    // is the one that we want in the fragment buffer (and thus on screen)
    program.init(vertex_shader,fragment_shader,"outColor");
    program.bind();

    // Register the keyboard callback
    glfwSetKeyCallback(window, key_callback);

    auto t_start = std::chrono::high_resolution_clock::now();

    // Register the mouse callback
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    // glfwSetCursorEnterCallback( window, cursorEnterCallback );
    glfwSetCursorPosCallback(window, cursor_position_callback);
    // Update viewport
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        // Bind your VAO (not necessary if you have only one)
        VAO.bind();

        // Bind your program
        program.bind();

        glUniformMatrix2fv(program.uniform("view"), 1, GL_FALSE, &vscale[0][0]);
        glUniform2fv(program.uniform("trans"),1, &vpos[0]);

        // Clear the framebuffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        if (V.size() > 0){
            VBO.bind();
            color_VBO.bind();
            program.bindVertexAttribArray("position",VBO);
            program.bindVertexAttribArray("color", color_VBO);
            for(int i=0;i< V.size()/3; i++){
                //glUniform4f(program.uniform("triangleColor"), 1.0f, 0.0f, 0.0f, 1.0f);
                if (selected_traingle.clicked && selected_traingle.start_index == i*3 ){
                    glUniformMatrix4fv(program.uniform("translation"), 1, GL_FALSE, &applied_translation[0][0]);
                    glDrawArrays(GL_TRIANGLES,i*3, 3);
                }
                else{
                    glUniformMatrix4fv(program.uniform("translation"), 1, GL_FALSE, &translation[0][0]);
                    glDrawArrays(GL_TRIANGLES,i*3, 3);
                }

                }

            }



        if (state_flag == 1){
            if (mouse_clicks > 0){
                line_VBO.bind();
                program.bindVertexAttribArray("position",line_VBO);
                program.bindVertexAttribArray("color", line_color_VBO);
                if (mouse_flag == 1){
                    glUniformMatrix4fv(program.uniform("translation"), 1, GL_FALSE, &translation[0][0]);
                    glDrawArrays(GL_LINE_STRIP,0, line_v.size());
                }
                else{
                    glUniformMatrix4fv(program.uniform("translation"), 1, GL_FALSE, &translation[0][0]);
                    glDrawArrays(GL_LINE_STRIP,0, line_v.size()-1);
                }
            }

        }

        if (state_flag == 2){
            if (selected_traingle.clicked && mouse_clicks > 0){
                line_VBO.bind();
                line_color_VBO.bind();
                program.bindVertexAttribArray("position",line_VBO);
                program.bindVertexAttribArray("color", line_color_VBO);
                if (translation_flag == 1)
                {
                    glUniformMatrix4fv(program.uniform("translation"), 1, GL_FALSE, &applied_translation[0][0]);
                    glDrawArrays(GL_LINE_LOOP, 0, line_v.size());
                }
                else{
                    glUniformMatrix4fv(program.uniform("translation"), 1, GL_FALSE, &translation[0][0]);
                    glDrawArrays(GL_LINE_LOOP, 0, line_v.size());
                }

            }
        }

        if (state_flag == 5){
            if(animation_step_flag == 1 && mouse_clicks > 0){
                line_VBO.bind();
                line_color_VBO.bind();
                program.bindVertexAttribArray("position",line_VBO);
                program.bindVertexAttribArray("color", line_color_VBO);
                glUniformMatrix4fv(program.uniform("translation"), 1, GL_FALSE, &translation[0][0]);
                glDrawArrays(GL_LINE_LOOP, 0, line_v.size());
            }
            else if(animation_step_flag == 2){
                auto time_now = std::chrono::high_resolution_clock::now();
                float time = std::chrono::duration_cast<std::chrono::duration<float>>(time_now - start_time).count();
                // Play animation
                if (time > 0){
                    double time_step = 0.2;
                    j = floor(time/time_step);
                    for(int i=0;i< V.size()/3; i++){
                        V[3*i][0] += (animation_V[3*i][0]-V[3*i][0])*j*0.1;
                        V[3*i][1] += (animation_V[3*i][1]-V[3*i][1])*j*0.1;
                        V[3*i + 1][0] += (animation_V[3*i + 1][0]-V[3*i + 1][0])*j*0.1;
                        V[3*i + 1][1] += (animation_V[3*i + 1][1]-V[3*i + 1][1])*j*0.1;
                        V[3*i + 2][0] += (animation_V[3*i + 2][0]-V[3*i + 2][0])*j*0.1;
                        V[3*i + 2][1] += (animation_V[3*i + 2][1]-V[3*i + 2][1])*j*0.1;
                    }
                    VBO.update(V);
                    VBO.bind();
                    color_VBO.bind();
                    program.bindVertexAttribArray("position",VBO);
                    program.bindVertexAttribArray("color", color_VBO);
                    glUniformMatrix4fv(program.uniform("translation"), 1, GL_FALSE, &translation[0][0]);
                    glDrawArrays(GL_TRIANGLES,0, V.size());
                    if (j == 4) {
                        time = 0;
                        animation_step_flag = 0;
                        state_flag = 2;
                        mouse_clicks = 0;
                        std::cout << "Animation Finished, Translation mode enabled" << std::endl;
                    }
                }

            }

        }



        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Deallocate opengl memory
    program.free();
    VAO.free();
    VBO.free();

    // Deallocate glfw internals
    glfwTerminate();
    return 0;
}
