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

//Other libraries added
#include <fstream>
#include <cmath>

VertexBufferObject mesh_VBO;
VertexBufferObject mesh_color_VBO;


std::vector<glm::vec3> mesh_v(0);
std::vector<glm::vec3> mesh_color(0);

glm::vec3 eyes(0,0,1);


// View transformation matrices
glm::mat4x4 MVP;
glm::mat4x4 Mat_ortho;
glm::mat4x4 Mat_proj;
glm::mat4x4 Mat_cam;
glm::mat4x4 Mat_comb;
glm::mat4x4 Mat_normal;


// Static variables for performing view transformations
float near = -1;
float far = -10;
float left = -1.0;
float right = 1.0;
float top = 1.0;
float bottom = -1.0;


float clockwise = (float)(-10*3.141592f/180);
float anti_clockwise = (float)(10*3.141592f/180);
glm::mat4x4 clockwise_rotation = {cos(clockwise), -sin(clockwise), 0, 0, sin(clockwise), cos(clockwise),  0, 0, 0, 0, 1, 0, 0,0,0, 1};
glm::mat4x4 anti_rotation = {cos(anti_clockwise), -sin(anti_clockwise), 0, 0, sin(anti_clockwise), cos(anti_clockwise),  0, 0, 0, 0, 1, 0, 0,0,0, 1};


glm::mat4x4 up_scale = {1.25f, 0, 0, 0, 0, 1.25f,  0, 0, 0, 0, 1.25, 0, 0,0,0, 1};
glm::mat4x4 down_scale = {0.75f, 0, 0, 0, 0, 0.75f,  0, 0, 0, 0, 0.75, 0, 0,0,0, 1};
glm::mat4x4 trans_left = {1, 0, 0, -0.2, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
glm::mat4x4 trans_right = {1, 0, 0, 0.2, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
glm::mat4x4 trans_up = {1, 0, 0, 0, 0, 1, 0, -0.2, 0, 0, 1, 0, 0, 0, 0, 1};
glm::mat4x4 trans_down = {1, 0, 0, 0.1, 0, 1, 0, 0.2, 0, 0, 1, 0, 0, 0, 0, 1};
glm::mat4x4 translation = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
glm::mat4x4 applied_translation;


int state_flag = 0; // Based on the mode state flag changes
int translation_flag = 0; // enables when one of the key is pressed

int mouse_clicks = 0; // In the insertion mode.
int mouse_flag = 0;


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    float aspect_x = fmin(float(height)/float(width),1.);
    float aspect_y = fmin(float(width)/float(height),1.);
    MVP = {aspect_x, 0, 0, 0, 0, aspect_y, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};

}

class point{
public:
    double x;
    double y;
    double z;

    point(double x1, double y1, double z1){
        x = x1;
        y = y1;
        z = z1;
    }

};

struct light{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 intensity;
    glm::vec3 ambientIntensity;
};

struct hit_check{
    bool hit;
    double dist;
};


class triangle_mesh{
public:
    std::vector<glm::vec3> mesh_vertices;
    std::vector<glm::vec3> mesh_faces;
    std::vector<glm::vec3> mesh_f_normals;
    glm::mat4x4 M_model;
    glm::vec3 diff_color;
    glm::vec3 ka;
    int num_vertices;
    char shader_type;
    float bound_radius;
    glm::vec3 bound_center;

    glm::vec3 bound_center_clicked;
    glm::mat4x4 M_model_clicked;
    bool clicked = false;
    int clicked_index;
};

struct ray{
    glm::vec3 e;
    glm::vec3 d;
};

struct sphere{
    glm::vec3 center;
    float radius;
};

glm::vec3 find_barycenter(triangle_mesh model_selected){
    float xsum =0, ysum =0, zsum = 0;
    for(int i=0; i < model_selected.num_vertices; i++){
        xsum += model_selected.mesh_vertices[i][0];
        ysum += model_selected.mesh_vertices[i][1];
        zsum += model_selected.mesh_vertices[i][2];
    }
    return glm::vec3(xsum/model_selected.num_vertices, ysum/model_selected.num_vertices, zsum/model_selected.num_vertices);
}


triangle_mesh selected_mesh;
std::vector<triangle_mesh> meshes(0);


point start_click(0,0,0);


std::vector<std::string> divide_strings(std::string line){
    std::vector<int> temp_index;
    std::vector<std::string> temp_strings;

    for (int i=0; i<line.size(); i++){
        if(line[i] == ' ') temp_index.push_back(i);
    }

    int split_start = 0;
    int temp_len = 0;
    for(int i=0;i<temp_index.size() + 1; i++){
        if(i<temp_index.size()) temp_len = temp_index[i] - split_start;
        else temp_len = temp_index.size() - split_start;

        temp_strings.push_back(line.substr(split_start, temp_len));
        if (i < temp_index.size()) split_start = temp_index[i] + 1;

    }
    return temp_strings;
}

triangle_mesh load_mesh(std::string file_path, glm::vec3 pos, double scale){
    triangle_mesh new_mesh;

    int num_v;
    int num_f;

    std::ifstream input_file(file_path);

    char str[255];
    bool line_read = false;

    std::vector<std::string> substrings;
    input_file.getline(str, 255);
    if (std::string(str) == "OFF"){
        while(line_read == false &&  input_file.getline(str, 255)){
            if(str[0] != '#'){
                substrings = divide_strings(std::string(str));
                num_v = std::stoi(substrings[0]);
                num_f = std::stoi(substrings[1]);
                line_read = true;
            }
        }
        new_mesh.num_vertices = num_v;
        for (int i=0;i<num_v;i++){
            input_file.getline(str, 255);
            substrings = divide_strings(std::string(str));
            new_mesh.mesh_vertices.emplace_back(glm::vec3(std::stof(substrings[0])*scale+pos[0], std::stof(substrings[1])*scale+pos[1], std::stof(substrings[2])*scale+pos[2]));
        }
        for(int i=0;i< num_f;i++){
            input_file.getline(str, 255);
            substrings = divide_strings(std::string(str));

            glm::vec3 v1 = new_mesh.mesh_vertices[std::stoi(substrings[1])];
            glm::vec3 v2 = new_mesh.mesh_vertices[std::stoi(substrings[2])];
            glm::vec3 v3 = new_mesh.mesh_vertices[std::stoi(substrings[3])];

            glm::vec3 normal_face = glm::cross(v2-v1, v3-v1);
            new_mesh.mesh_faces.emplace_back(glm::vec3(std::stoi(substrings[1]), std::stoi(substrings[2]), std::stoi(substrings[3])));
            new_mesh.mesh_f_normals.emplace_back(glm::vec3(normal_face[0], normal_face[1], normal_face[2]));
        }

       // new_mesh.bound_center = glm::vec3(0,0,0);
        new_mesh.bound_center = new_mesh.bound_center* float(1/num_v);
        new_mesh.bound_radius = 0.0;
        for (unsigned i=0; i < new_mesh.mesh_vertices.size(); i++){
            glm::vec3 temp = new_mesh.mesh_vertices[i];
            float temp_radius = glm::length(temp-new_mesh.bound_center);
            if (temp_radius > new_mesh.bound_radius){
                new_mesh.bound_radius = temp_radius;
            }
        }

        new_mesh.diff_color = glm::vec3(255,255,255);
        new_mesh.ka = new_mesh.diff_color*0.15f;
        new_mesh.shader_type = 'f'; //1 flat shading
        new_mesh.M_model = glm::mat4x4(1.0);
        return new_mesh;
    }
    else{
        std::cout << "Not a valid OFF file" << std::endl;
        return new_mesh;
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

    glm::vec4 adjust = {xworld, yworld, 0, 1};
    glm::mat4x4 inv_proj = glm::inverse(Mat_proj);
    glm::mat4x4 inv_cam = glm::inverse(Mat_cam);
    adjust = inv_cam*inv_proj*adjust;
    return point(adjust[0], adjust[1], adjust[2]);
}

/*
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos){
    point screen_click = get_pos(window);
   if(selected_mesh.clicked){
       glm::vec3 pos = {screen_click.x - start_click.x, screen_click.y-start_click.y, screen_click.z - start_click.z};
       glm::mat4x4  temp_mat = {1, 0, 0, pos[0], 0, 1, 0, pos[1], 0, 0, 1, pos[2], 0, 0, 0, 1};

       selected_mesh.M_model = temp_mat*selected_mesh.M_model_clicked;
       selected_mesh.bound_center = selected_mesh.bound_center_clicked + pos;
       meshes[selected_mesh.clicked_index] = selected_mesh;
   }
}
*/

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


hit_check check_sphere(ray r, sphere test)
{
    hit_check check;
    glm::vec3 ec = r.e - test.center;

    double a = glm::dot(r.d,r.d);
    double b = glm::dot(r.d,ec);
    double c = glm::dot(ec, ec) - test.radius*test.radius;

    double det = b*b - a*c;
    if (det == 0){
        check.hit = true;
        check.dist = -b/(a);
    }

    else if(det > 0){
        check.hit = true;
        check.dist = (-b - sqrt(det))/(a);
        if (check.dist < 0){
            check.dist = (-b+sqrt(det))/(a);
        }
        else{
            double temp = (-b+sqrt(det))/(a);
            if (temp > 0 && temp < check.dist){
                check.dist = temp;
            }
        }
    }
    return check;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    // Get the position of the mouse in the window

    point screen_click = get_pos(window);
    // Update the position of the first vertex if the left button is pressed
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        if (!selected_mesh.clicked){
            ray new_ray;
            new_ray.e = glm::vec3(screen_click.x, screen_click.y, screen_click.z);
            new_ray.d = glm::vec3(0,0,-1);

            glm::vec4 temp_d = {new_ray.d[0], new_ray.d[1], new_ray.d[2], 0};
            glm::mat4x4 inv_proj = glm::inverse(Mat_proj);
            glm::mat4x4 inv_cam = glm::inverse(Mat_cam);
            temp_d = inv_cam*inv_proj*temp_d;
            new_ray.d = glm::normalize(glm::vec3(temp_d[0], temp_d[1], temp_d[2]));
            float min_dist = std::numeric_limits<float>::infinity();
            for(unsigned i=0; i<meshes.size(); i++)
            {
                sphere test_sphere;
                test_sphere.center = meshes[i].bound_center;
                test_sphere.radius = meshes[i].bound_radius;
                hit_check check = check_sphere(new_ray,test_sphere);
                if(check.hit)
                {
                    if(check.dist < min_dist)
                    {
                        min_dist = check.dist;
                        selected_mesh = meshes[i];
                        selected_mesh.clicked_index = i;
                        selected_mesh.clicked = true;
                        //std::cout << i << std::endl;
                    }
                    start_click = point(screen_click.x, screen_click.y, screen_click.z);

                }
                else{
                    selected_mesh.clicked_index = 0;
                    selected_mesh.clicked = false;
                }
            }
            if(min_dist == std::numeric_limits<float>::infinity())
            {
                selected_mesh.clicked_index = 0;
                selected_mesh.clicked = false;
            }
            else
            {
                selected_mesh.M_model_clicked = selected_mesh.M_model;
                selected_mesh.bound_center_clicked = glm::vec3( screen_click.x, screen_click.y, screen_click.z);
            }
        }
        else{
            selected_mesh.clicked_index = 0;
            selected_mesh.clicked = false;
        }


    }

}

glm::mat4x4 init_camera(){
    glm::vec3 g = glm::vec3(0,0,0) - eyes;
    glm::vec3 up = glm::vec3(0,1,0);

    glm::vec3 camu, camv, camw;
    camw = -(glm::normalize(g));
    camu = glm::normalize(glm::cross(up, camw));
    camv = glm::cross(camw, camu);

    glm::mat4x4 temp_camera;
    temp_camera = {camu[0], camv[0], camw[0], eyes[0], camu[1], camv[1], camw[1], eyes[1], camu[2], camv[2], camw[2], eyes[2], 0, 0, 0, 1};

    temp_camera = glm::inverse(temp_camera);
    return temp_camera;
}


void meshes_updates(triangle_mesh new_mesh) {
    for (unsigned i=0; i< new_mesh.mesh_faces.size(); i ++) {
        mesh_v.push_back(new_mesh.mesh_vertices[new_mesh.mesh_faces[i][0]]);
        mesh_v.push_back(new_mesh.mesh_vertices[new_mesh.mesh_faces[i][1]]);
        mesh_v.push_back(new_mesh.mesh_vertices[new_mesh.mesh_faces[i][2]]);
        mesh_color.push_back(new_mesh.mesh_f_normals[i]);
        mesh_color.push_back(new_mesh.mesh_f_normals[i]);
        mesh_color.push_back(new_mesh.mesh_f_normals[i]);
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Update the position of the first vertex if the keys 1,2, or 3 are pressed
    if (action == GLFW_RELEASE){
        switch (key){
            case GLFW_KEY_1:{
                triangle_mesh cube = load_mesh("../data/cube.off", glm::vec3(0,0,0), 1);
                meshes.push_back(cube);
                meshes_updates(cube);
                mesh_VBO.update(mesh_v);
                mesh_color_VBO.update(mesh_color);
                std::cout << "Unit cube at origin inserted" << std::endl;
                break;
            }
            case GLFW_KEY_2:{
                //check scale part of the code
                triangle_mesh bumpy = load_mesh("../data/bumpy_cube.off", glm::vec3(0,0,0), 0.11343);
                meshes.push_back(bumpy);
                meshes_updates(bumpy);
                mesh_VBO.update(mesh_v);
                mesh_color_VBO.update(mesh_color);
                std::cout << "Bumpy Cube inserted" << std::endl;
                break;
            }
            case GLFW_KEY_3:{
                //check scale part of the code
                triangle_mesh bunny = load_mesh("../data/bunny.off", glm::vec3(0.1207,-0.4054,-0.0351), 5*0.86);
                meshes.push_back(bunny);
                meshes_updates(bunny);
                mesh_VBO.update(mesh_v);
                mesh_color_VBO.update(mesh_color);
                std::cout << "Bunny inserted" << std::endl;
                break;
            }
            case GLFW_KEY_9:
                Mat_proj = {2*near/(right-left), 0, (right+left)/(right-left), 0, 0, 2*near/(top-bottom), (top+bottom)/(top-bottom), 0,0, 0, (near+far)/(near-far), 2*(near * far)/(near-far), 0, 0, -1, 0};
                break;
            case GLFW_KEY_0:
                Mat_proj = Mat_ortho;
                break;
            case  GLFW_KEY_UP:
                eyes = eyes - glm::vec3(0,1,0);
                Mat_cam = init_camera();
                break;
            case  GLFW_KEY_DOWN:
                eyes = eyes + glm::vec3(0,1,0);
                Mat_cam = init_camera();
                break;
            case  GLFW_KEY_LEFT:
                eyes = eyes + glm::vec3(1,0,0);
                Mat_cam = init_camera();
                break;
            case  GLFW_KEY_RIGHT:
                eyes = eyes - glm::vec3(1,0,0);
                Mat_cam = init_camera();
                break;
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GL_TRUE);
                std::cout << "Bye!" << std::endl;
                break;
            default:
                break;
        }
        if (selected_mesh.clicked) {
            switch(key){
                case GLFW_KEY_W:
                    std::cout << "Wire frame selected" << std::endl;
                    selected_mesh.shader_type = 'w';
                    meshes[selected_mesh.clicked_index].shader_type = 'w';
                    break;
                case GLFW_KEY_F:
                    //flat shading
                    selected_mesh.shader_type = 'f';
                    meshes[selected_mesh.clicked_index].shader_type = 'f';
                    break;
                case GLFW_KEY_P:
                    selected_mesh.shader_type = 'p';
                    meshes[selected_mesh.clicked_index].shader_type = 'p';
                    // add some code here
                    break;
                case GLFW_KEY_EQUAL:
                    //meshes[selected_mesh.clicked_index].M_model = meshes[selected_mesh.clicked_index].M_model*1.25f;
                    // std::cout << "zoom in" << std::endl;
                    meshes[selected_mesh.clicked_index].M_model = up_scale*meshes[selected_mesh.clicked_index].M_model;
                    break;
                case GLFW_KEY_MINUS:
                    //meshes[selected_mesh.clicked_index].M_model = meshes[selected_mesh.clicked_index].M_model*0.75f;
                    // std::cout << "zoom out" << std::endl;
                    meshes[selected_mesh.clicked_index].M_model = down_scale*meshes[selected_mesh.clicked_index].M_model;
                    break;
                case GLFW_KEY_J:
                    // std::cout << "Rotate 10* Clockwise" << std::endl;
                    meshes[selected_mesh.clicked_index].M_model = anti_rotation*meshes[selected_mesh.clicked_index].M_model;
                    break;
                case GLFW_KEY_K:
                    // std::cout << "Rotate 10* Anti-Clockwise" << std::endl;
                    meshes[selected_mesh.clicked_index].M_model = clockwise_rotation*meshes[selected_mesh.clicked_index].M_model;
                    break;
                case GLFW_KEY_H:
                    meshes[selected_mesh.clicked_index].M_model = trans_up*meshes[selected_mesh.clicked_index].M_model;
                    break;
                case GLFW_KEY_N:
                    meshes[selected_mesh.clicked_index].M_model = trans_down*meshes[selected_mesh.clicked_index].M_model;
                    break;
                case GLFW_KEY_B:
                    meshes[selected_mesh.clicked_index].M_model = trans_left*meshes[selected_mesh.clicked_index].M_model;
                    break;
                case GLFW_KEY_M:
                    meshes[selected_mesh.clicked_index].M_model = trans_right*meshes[selected_mesh.clicked_index].M_model;
                    break;
                case GLFW_KEY_DELETE:
                {
                    std::cout << "delete" << std::endl;
                    selected_mesh.clicked = false;
                    unsigned i;
                    for(i = 0;i < selected_mesh.clicked_index; i++){
                        i += meshes[i].mesh_vertices.size();
                    }
                    mesh_v.erase(mesh_v.begin() + i, mesh_v.begin() + i + selected_mesh.mesh_vertices.size());
                    mesh_color.erase(mesh_v.begin() + i, mesh_v.begin() + i + selected_mesh.mesh_vertices.size());
                    meshes.erase(meshes.begin() + selected_mesh.clicked_index, meshes.begin() + selected_mesh.clicked_index+1);
                    std::cout<< meshes.size()<< std::endl;
                    if (mesh_v.size() > 0){
                        mesh_VBO.update(mesh_v);
                        mesh_color_VBO.update(mesh_color);
                    }

                }

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
    window = glfwCreateWindow(640, 480, "3d Graphics", NULL, NULL);
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

    mesh_VBO.init();
    mesh_color_VBO.init();

    // Initialize the VBO with the vertices data
    // A VBO is a data container that lives in the GPU memory


    light spotlight;
    spotlight.position = glm::vec3(0,1,1);
    spotlight.intensity = glm::vec3(1,1,1);
    spotlight.color = glm::vec3(1,1,1);
    spotlight.ambientIntensity = glm::vec3(1,1,1);

    float aspect_x = fmin(float(640)/float(480),1.);
    float aspect_y = fmin(float(640)/float(480),1.);
    MVP = {aspect_x, 0, 0, 0, 0, aspect_y, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
    Mat_ortho = {2/(right-left), 0, 0, -(right+left)/(right-left), 0, 2/(top-bottom), 0, -(top+bottom)/(top-bottom), 0, 0, 2/(far-near), -(near+far)/(near-far), 0, 0, 0, 1};
    Mat_proj = Mat_ortho;
    Mat_cam = init_camera();

    // Initialize the OpenGL Program
    // A program controls the OpenGL pipeline and it must contains
    // at least a vertex shader and a fragment shader to be valid
    Program program;
    const GLchar* vertex_shader =
            "#version 330 core\n"
            "in vec3 position;"
            "in vec3 inNormal;"
            "out vec4 normal;"
            "out vec3 halfVec;"
            "out vec3 lightDir;"
            "uniform mat4 viewportMatrix;"
            "uniform mat4 projMatrix;"
            "uniform mat4 camMatrix;"
            "uniform mat4 modelMatrix;"
            "uniform mat4 normalMatrix;"
            "uniform int shaderMode;"
            "uniform vec3 lightPosition;"
            "void main()"
            "{"
            "    vec4 pos = camMatrix * modelMatrix * vec4(position, 1.0);"
            "    if(shaderMode != 0){"
            "       vec4 lightPos = camMatrix * vec4(lightPosition, 1.0);"
            "       normal = normalMatrix * vec4(inNormal, 0.0);"
            "       vec3 v = normalize(-pos.xyz);"
            "       lightDir = normalize(lightPos.xyz - pos.xyz);"
            "       halfVec = normalize(v + lightDir);"
            "    }"
            "    gl_Position = viewportMatrix * projMatrix * pos;"
            "}";
    const GLchar* fragment_shader =
            "#version 330 core\n"
            "in vec4 normal;"
            "in vec3 halfVec;"
            "in vec3 lightDir;"
            "uniform vec3 lightIntensity;"
            "uniform int shaderMode;"
            "uniform vec3 ambientIntensity;"
            "uniform vec3 kambient;"
            "uniform vec3 kdiffuse;"
            ""
            "out vec4 outColor;"
            "void main()"
            "{"
            "    float kspecular = 0.5f;"
            "    float phongExp = 64;"
            "    if (shaderMode == 0) {"
            "       outColor = vec4(kdiffuse, 1.0);"
            "    }"
            "    else {"
            "       if(shaderMode == 1){"
            "       vec3 n = normalize(normal.xyz);"
            "       vec3 l = normalize(lightDir);"
            "       float diffuse =  max(0.0, dot(n,l));"
            "       vec3 intensity = kambient * ambientIntensity + kdiffuse * lightIntensity * (diffuse);"
            "       outColor = vec4(intensity, 1.0);"
            "       }"
            "       else{"
            "       vec3 n = normalize(normal.xyz);"
            "       vec3 h = normalize(halfVec);"
            "       vec3 l = normalize(lightDir);"
            "       float diffuse =  max(0.0, dot(n,l));"
            "       float specular = kspecular * pow(max(0.0,dot(n,h)),phongExp);"
            "       vec3 intensity = kambient * ambientIntensity + kdiffuse * lightIntensity * (diffuse + specular);"
            "       outColor = vec4(intensity, 1.0);"
            "       }"
            "    }"
            "}";

    // Compile the two shaders and upload the binary to the GPU
    // Note that we have to explicitly specify that the output "slot" called outColor
    // is the one that we want in the fragment buffer (and thus on screen)
    program.init(vertex_shader,fragment_shader,"outColor");
    program.bind();

    // Register the keyboard callback
    glfwSetKeyCallback(window, key_callback);

    // Register the mouse callback
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    // glfwSetCursorEnterCallback( window, cursorEnterCallback );
   // glfwSetCursorPosCallback(window, cursor_position_callback);
    // Update viewport
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    auto t_start = std::chrono::high_resolution_clock::now();
    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        // Bind your VAO (not necessary if you have only one)
        VAO.bind();

        // Bind your program
        program.bind();

        glUniformMatrix4fv(program.uniform("viewportMatrix"), 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(program.uniform("camMatrix"), 1, GL_FALSE, &Mat_cam[0][0]);
        glUniformMatrix4fv(program.uniform("projMatrix"), 1, GL_FALSE, &Mat_proj[0][0]);

        glUniform3fv(program.uniform("lightPosition"), 1, &spotlight.position[0]);
        glUniform3fv(program.uniform("lightIntensity"), 1, &spotlight.intensity[0]);
        glUniform3fv(program.uniform("ambientIntensity"), 1, &spotlight.ambientIntensity[0]);

        glUniform1i(program.uniform("ShaderMode"), 0);


        // Clear the framebuffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (meshes.size() > 0){
            mesh_VBO.bind();
            mesh_color_VBO.bind();
            long start = 0;

            for(unsigned i = 0; i<meshes.size(); i++)
            {
                Mat_comb = Mat_cam * meshes[i].M_model;
                Mat_normal = glm::transpose(glm::inverse(Mat_comb));

                glUniformMatrix4fv(program.uniform("modelMatrix"),1, GL_FALSE, &meshes[i].M_model[0][0]);
                glUniformMatrix4fv(program.uniform("normalMatrix"),1, GL_FALSE, &Mat_normal[0][0]);

                glm::vec3 color_gl = {1,1,1};
                glm::vec3 kd = {1, 0, 0   };
                if(selected_mesh.clicked && selected_mesh.clicked_index == i)
                {
                    glUniform3fv(program.uniform("kdiffuse"),1, &kd[0]);
                }
                else
                {
                    glUniform3fv(program.uniform("kdiffuse"),1, &color_gl[0]);
                }

                float scalar = 1.0f/255;
                for(unsigned j=0; j<meshes[i].mesh_faces.size(); j++)
                {
                    //draw triangles
                    if(meshes[i].shader_type == 'w')
                    {
                        glDrawArrays(GL_LINE_LOOP,start + j*3,3);
                        glUniform1i(program.uniform("shaderMode"),0);
                    }

                    else if (meshes[i].shader_type == 'f'){
                        glUniform1i(program.uniform("shaderMode"),1);
                        glm::vec3 ka_gl = meshes[i].ka*scalar;
                        glUniform3fv(program.uniform("kambient"),1, &ka_gl[0]);

                        program.bindVertexAttribArray("position",mesh_VBO);
                        program.bindVertexAttribArray("inNormal", mesh_color_VBO);
                        glDrawArrays(GL_TRIANGLES, start + j*3, 3);

                        kd = {1,1,1};
                        glUniform3fv(program.uniform("kd"),1, &kd[0]);
                        glUniform1i(program.uniform("shaderMode"),0);
                        glDrawArrays(GL_LINE_LOOP,j*3,3);

                    }
                    else if(meshes[i].shader_type == 'p'){
                        glUniform1i(program.uniform("shaderMode"),2);
                        glm::vec3 ka_gl = meshes[i].ka*scalar;
                        glUniform3fv(program.uniform("kambient"),1, &ka_gl[0]);

                        program.bindVertexAttribArray("position",mesh_VBO);
                        program.bindVertexAttribArray("inNormal", mesh_color_VBO);
                        glDrawArrays(GL_TRIANGLES, start + j*3, 3);

                    }

                }

                start+= meshes[i].mesh_faces.size()*3;
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
    mesh_VBO.free();
    mesh_color_VBO.free();

    // Deallocate glfw internals
    glfwTerminate();
    return 0;
}
