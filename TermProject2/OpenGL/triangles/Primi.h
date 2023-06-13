//
//  HGU <Computer Graphics>
//  primi.h
//

#pragma once

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <time.h>

typedef std::vector<GLfloat> GLvec;


void get_box_3d(
                std::vector<GLfloat>& p,
                GLfloat lx,
                GLfloat ly,
                GLfloat lz);

void get_cone_3d(
                 std::vector<GLfloat>& p,
                 std::vector<size_t>& side_idx,
                 std::vector<size_t>& bottom_idx,
                 GLfloat radius,
                 GLfloat height,
                 GLint subdivs);

void get_cylinder_3d(
                     std::vector<GLfloat>& p,
                     std::vector<size_t>& side_idx,
                     std::vector<size_t>& top_idx,
                     std::vector<size_t>& bottom_idx,
                     GLfloat radius,
                     GLfloat height,
                     GLint subdivs);

void get_torus_3d(
                  std::vector<GLfloat>& p,
                  std::vector<std::vector<size_t>>& side_idx,
                  GLfloat r0,
                  GLfloat r1,
                  GLint longs,
                  GLint lats);

void get_sphere_3d(
                   std::vector<GLfloat>& p,
                   GLfloat r,
                   GLint lats,
                   GLint longs);

void get_color_3d_by_pos(
                         GLvec& c,
                         GLvec& p,
                         GLfloat offset = 0);

void get_box_random_color(std::vector<GLfloat>& color);
void get_spe_random_color(std::vector<GLfloat>& color);
void get_cone_random_color(std::vector<GLfloat>& color);
void get_torus_random_color(std::vector<GLfloat>& color);
void get_cylinder_random_color(std::vector<GLfloat>& color);

void bind_buffer(
                 GLint buffer,
                 std::vector<GLfloat>& vec,
                 int program,
                 const GLchar* attri_name,
                 GLint attri_size);

void get_vertex_color(GLvec& color, GLuint n, GLfloat r, GLfloat g, GLfloat b);

#define COLOR_OFFSET 0.0

class Model{
public:
    GLuint vao, buffers[2];
    GLvec vtx_pos;
    GLvec vtx_clrs;
    
    virtual void init(GLint program)
    {
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        
        const GLchar* attri_name[2] = {"vPosition", "vColor"};
        GLvec* vtx_list[2]={&vtx_pos, &vtx_clrs};
        
        glGenBuffers(2, buffers);
        for (int i = 0; i < 2; ++i){
            bind_buffer(buffers[i], *vtx_list[i], program, attri_name[i], 3);
        }
    }
    
    virtual void draw(glm::vec3 pos, GLfloat theta, GLfloat right, GLfloat left) = 0;
    virtual void draw_wire(glm::vec3 pos, GLfloat theta, GLfloat right, GLfloat left){
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        draw(pos, theta,right,left);
    }
    virtual void draw_points(glm::vec3 pos, GLfloat theta, GLfloat right, GLfloat left){
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        draw(pos,theta,right,left);
    }
};

struct CubePrimitive:public Model
{
    CubePrimitive(GLfloat lx, GLfloat ly, GLfloat lz)
    {
        get_box_3d(vtx_pos, lx, ly, lz);
        get_color_3d_by_pos(vtx_clrs, vtx_pos, COLOR_OFFSET);
        //get_box_random_color(vtx_clrs);
        //get_vertex_color(vtx_clrs, vtx_pos.size() / 2, 0.0f, 0.0f, 1.0f);
    }
    
    virtual void draw(glm::vec3 pos, GLfloat theta, GLfloat right, GLfloat left)
    {
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, vtx_pos.size()/3);
    }
};

struct ConePrimitive:public Model
{
    GLuint element_buffs[2];
    std::vector<size_t> idx_list[2];
    
    ConePrimitive(GLfloat radius, GLfloat height, GLint subdivs)
    {
        get_cone_3d(vtx_pos, idx_list[0], idx_list[1], radius, height, subdivs);
        //get_cone_random_color(vtx_clrs);
        get_color_3d_by_pos(vtx_clrs, vtx_pos, COLOR_OFFSET);
        //(1.0f, 0.0f, 0.0f);
    }
    
    virtual void init(GLint program)
    {
        Model::init(program);
        
        glGenBuffers(2, element_buffs);
        for (int i = 0; i < 2; ++i){
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffs[i]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                         sizeof(size_t)*idx_list[i].size(), idx_list[i].data(), GL_STATIC_DRAW);
        }
    }
    
    virtual void draw(glm::vec3 pos, GLfloat theta, GLfloat right, GLfloat left)
    {
        glBindVertexArray(vao);
        for (int i = 0; i < 2; ++i){
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffs[i]);
            glDrawElements(GL_TRIANGLE_FAN, idx_list[i].size(),
                           GL_UNSIGNED_INT, NULL);
        }
    }
};

struct CylinderPrimitive:public Model
{
    GLuint element_buffs[3];
    std::vector<size_t> idx_list[3];
    GLuint drawing_mode[3]= {GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN,
        GL_TRIANGLE_FAN};
    
    CylinderPrimitive(GLfloat radius, GLfloat height, GLint subdivs)
    {
        get_cylinder_3d(vtx_pos, idx_list[0], idx_list[1], idx_list[2], radius, height,
                        subdivs);
        //get_cylinder_random_color(vtx_clrs);
        get_color_3d_by_pos(vtx_clrs, vtx_pos, COLOR_OFFSET);
        //(0.0f, 1.0f, 0.0f);
    }
    
    virtual void init(GLint program)
    {
        Model::init(program);
        
        glGenBuffers(3, element_buffs);
        for (int i = 0; i < 3; ++i){
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffs[i]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                         sizeof(size_t)*idx_list[i].size(), idx_list[i].data(), GL_STATIC_DRAW);
        }
    }
    
    virtual void draw(glm::vec3 pos, GLfloat theta, GLfloat right, GLfloat left)
    {
        GLuint drawing_mode[3] = {GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLE_FAN};
        glBindVertexArray(vao);
        for (int i = 0; i < 3; ++i){
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffs[i]);
            glDrawElements(drawing_mode[i], idx_list[i].size(),
                           GL_UNSIGNED_INT, NULL);
        }
    }
};

struct TorusPrimitive:public Model
{
    std::vector<GLuint> element_buffs;
    std::vector<std::vector<size_t>> idx_list;
    
    TorusPrimitive(GLfloat radius0, GLfloat radius1, GLint longs, GLint lats)
    {
        get_torus_3d(vtx_pos, idx_list, radius0, radius1, longs, lats);
        //get_torus_random_color(vtx_clrs);
        get_color_3d_by_pos(vtx_clrs, vtx_pos, COLOR_OFFSET);
        //(1.0f, 0.0f, 0.0f);
        
        element_buffs.resize(idx_list.size());
    }
    
    virtual void init(GLint program)
    {
        Model::init(program);
        
        size_t n = element_buffs.size();
        glGenBuffers(n, element_buffs.data());
        for (int i = 0; i < n; ++i){
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffs[i]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                         sizeof(size_t)*idx_list[i].size(), idx_list[i].data(), GL_STATIC_DRAW);
        }
    }
    
    virtual void draw(glm::vec3 pos, GLfloat theta, GLfloat right, GLfloat left)
    {
        glBindVertexArray(vao);
        size_t n = element_buffs.size();
        for (int i = 0; i < n; ++i)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffs[i]);
            glDrawElements(GL_TRIANGLE_STRIP, idx_list[i].size(),
                           GL_UNSIGNED_INT, NULL);
        }
    }
};

struct SpherePrimitive:public Model
{
    SpherePrimitive(GLfloat radius, GLint lats, GLint longs)
    {
        get_sphere_3d(vtx_pos, radius, lats, longs);
        //get_spe_random_color(vtx_clrs);
        get_color_3d_by_pos(vtx_clrs, vtx_pos, COLOR_OFFSET);
        //(1.0f, 1.0f, 0.0f);
    }
    
    virtual void draw(glm::vec3 pos, GLfloat theta, GLfloat right, GLfloat left)
    {
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, vtx_pos.size()/3);
    }
};

struct CarModel:public Model
{
    CubePrimitive* cube;
    SpherePrimitive* sphere;
    ConePrimitive* cone;
    CylinderPrimitive* cylinder;
    TorusPrimitive* torus;
    
    CarModel(
             CubePrimitive* cube,
             SpherePrimitive* sphere,
             ConePrimitive* cone,
             CylinderPrimitive* cylinder,
             TorusPrimitive* torus)
    {
        this->cube = cube;
        this->torus = torus;
        this->sphere = sphere;
        this->cone = cone;
        this->cylinder = cylinder;
    }
    
    virtual void init(GLint program)
    {
        // do nothing.
    }
    
    glm::mat4 transf(
        glm::vec3 pos,
                     GLfloat sx, GLfloat sy, GLfloat sz,
                     GLfloat tx, GLfloat ty, GLfloat tz,
                     glm::mat4*T_pre = NULL,
                     glm::mat4*T_post = NULL,
                     
                     bool set_uniform = true)
    {
        using namespace glm;
        glm::mat4 T(1.0f);
        T = translate(T, pos);
        T = translate(T, vec3(tx, ty, tz));
        T = scale(T, vec3(sx, sy, sz));
        if (T_pre) T = (*T_pre)*T;
        if (T_post) T = T*(*T_post);
        
        if (set_uniform){
            glUniformMatrix4fv(1, 1, GL_FALSE, value_ptr(T));
        }
        return T;
    }
    
    virtual void draw(glm::vec3 pos, GLfloat theta, GLfloat right, GLfloat left)
    {
        using namespace glm;
        
        mat4 standard(1.0f);
        GLfloat thetas = 0.001f * clock();
        mat4 Rz = rotate(mat4(1.0f), -2*theta, vec3(0.0f, 0.0f, 1.0f));
        mat4 Ry = rotate(mat4(1.0f), -theta, vec3(0.0f, 1.0f, 0.0f));

        mat4 Rzs = rotate(mat4(1.0f), -2 * right, vec3(0.0f, 0.0f, 1.0f));
        mat4 Rys = rotate(mat4(1.0f), -right, vec3(0.0f, 1.0f, 0.0f));
        mat4 Lzs = rotate(mat4(1.0f), -2 * left, vec3(0.0f, 0.0f, 1.0f));
        mat4 Lys = rotate(mat4(1.0f), -left, vec3(0.0f, 1.0f, 0.0f));
        
        // car main body
        transf(pos, 1.2f, 0.4f, 0.6f, +0.0f, -0.2f, 0.0f, &Ry);
        cube->draw(pos, theta, right,left);
        
        // car upper body
        transf(pos,0.6f, 0.6f, 0.6f, -0.3f, +0.3f, 0.0f, &Ry);
        cube->draw(pos, theta, right, left);
        
        // car front body
        mat4 R_fb = rotate(mat4(1.0f), radians(90.0f), vec3(0, 0, 1)) * Ry;
        transf(pos,0.5f, 0.5f, 0.5f, +0.25f, 0.0f, 0.0f, &Ry, &R_fb);
        cylinder->draw(pos, theta, right, left);
        
        // car roof
        transf(pos,1.0f, 0.2f, 1.0f, -0.3f, +0.7f, 0.0f, &Ry);
        cone->draw(pos, theta, right, left);
        
        // car front-right light
        transf(pos, 0.1f, 0.1f, 0.1f, +0.6f, -0.2f, -0.2f, &Ry);
        sphere->draw(pos, theta, right, left);
        // car front-left light
        transf(pos, 0.1f, 0.1f, 0.1f, +0.6f, -0.2f, +0.2f, &Ry);
        sphere->draw(pos, theta, right, left);
        
        // fornt left tire
        mat4 R_tire = Rzs * rotate(mat4(1.0f), radians(-90.0f), vec3(1, 0, 0));
        mat4 L_tire = Lzs * rotate(mat4(1.0f), radians(-90.0f), vec3(1, 0, 0));
        transf(pos, 0.3f, 0.3f, 0.3f, +0.3f, -0.4f, -0.4f, &Ry, &L_tire);
        torus->draw(pos, theta, right, left);

        // fornt rightt tirez
        transf(pos, 0.3f, 0.3f, 0.3f, +0.3f, -0.4f, +0.4f, &Ry, &R_tire);
        torus->draw(pos, theta, right, left);
        
        // rear left tire
        transf(pos, 0.3f, 0.3f, 0.3f, -0.3f, -0.4f, -0.4f, &Ry, &L_tire);
        torus->draw(pos, theta, right, left);

        // rear right tire
        transf(pos, 0.3f, 0.3f, 0.3f, -0.3f, -0.4f, +0.4f, &Ry, &R_tire);
        torus->draw(pos, theta, right, left);
        
        // front shaft
        mat4 R_shaft = Rz * rotate(mat4(1.0f), radians(90.0f), vec3(1, 0, 0));
        transf(pos, 0.12f, 0.12f, 0.9f,  +0.3f, -0.4f, +0.0f, &Ry, &R_shaft);
        cylinder->draw(pos, theta, right, left);
        
        // rear shaft
        transf(pos, 0.12f, 0.12f, 0.9f,  -0.3f, -0.4f, +0.0f, &Ry, &R_shaft);
        cylinder->draw(pos, theta, right, left);

    }
};
