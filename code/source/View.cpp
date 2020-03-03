
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                             *
 *  Started by Ángel on december of 2013                                       *
 *                                                                             *
 *  This is free software released into the public domain.                     *
 *                                                                             *
 *  angel.rodriguez@esne.edu                                                   *
 *                                                                             *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
 * File: View.cpp
 * File Created: 2nd February 2020
 * ––––––––––––––––––––––––
 * Author: Pablo Jimeno Domínguez (pablojimenodev@gmail.com)
 */

#include "View.hpp"
#include "Light.hpp"

using namespace toolkit;

namespace example
{

    View::View(size_t width, size_t height)
    :
        Color_buffer(width, height),
        rasterizer  (Color_buffer )
    {
        Projection3f projection(5, 15, 20, float(width) / float(height));
        Transform3f transform_camera;

        transform_camera.set_translation({ 0, 0, 3 });
        transform_camera.set_rotation({ 0.2, 0, 0 });

        camera.set_projection(projection);
        camera.set_transform(transform_camera);

        // Colors of every model, where each mesh gets one color

        vector<Color> colors_land;
        colors_land.push_back({ 196, 124, 52 });    // Brown

        vector<Color> colors_clouds;
        colors_clouds.push_back({ 248, 249, 216 }); // White
        colors_clouds.push_back({ 248, 249, 216 }); // White
        colors_clouds.push_back({ 248, 249, 216 }); // White

        vector<Color> colors_trees;
        colors_trees.push_back({ 186,  30, 30 });   // Red
        colors_trees.push_back({ 186,  30, 30 });   // Red
        colors_trees.push_back({ 120, 156, 65 });   // Green
        colors_trees.push_back({ 120, 156, 65 });   // Green
        colors_trees.push_back({ 120, 156, 65 });   // Green
        colors_trees.push_back({ 120, 156, 65 });   // Green
        colors_trees.push_back({ 139,  69, 19 });   // Brown
        colors_trees.push_back({ 139,  69, 19 });   // Brown

        vector<Color> colors_mill;
        colors_mill.push_back({ 200, 200, 200 });   // Gray
        colors_mill.push_back({ 200, 200, 200 });   // Gray
        colors_mill.push_back({   0,   0,   0 });   // Black
        colors_mill.push_back({ 139,  69,  19 });   // Brown
        colors_mill.push_back({ 139,  69,  19 });   // Brown

        // Creates the models with the .obj path and the color's vector

        // Land will be the parent model of the scene

		std::shared_ptr<Model> land(new Model("..\\..\\binaries\\assets\\land.obj", colors_land));
        land->get_transform().set_pivot(Transform3f::PivotRotation::Y);
        land->get_transform().set_translation({ 0, 2.4, -10 });
        land->get_transform().set_rotation({ 0.f, 0.f, 0.f });
        land->get_transform().set_scale(0.05f);
        land->set_speed_rotation(0.005f);

        // Clouds and trees will be children

        std::shared_ptr<Model> clouds(new Model("..\\..\\binaries\\assets\\clouds.obj", colors_clouds));
        clouds->get_transform().set_pivot(Transform3f::PivotRotation::Y);
        clouds->get_transform().set_translation({ -2, 7.5, 0 });
        clouds->get_transform().set_rotation({ 0.f, 0.f, 0.f });
        clouds->get_transform().set_scale(1.f);
        clouds->set_speed_rotation(-0.01f);
        clouds->set_parent(land);

        std::shared_ptr<Model> trees(new Model("..\\..\\binaries\\assets\\trees.obj", colors_trees));
        trees->get_transform().set_pivot(Transform3f::PivotRotation::Y);
        trees->get_transform().set_translation({ 0, 0, 0 });
        trees->get_transform().set_rotation({ 0.f, 0.f, 0.f });
        trees->get_transform().set_scale(1.f);
        trees->set_speed_rotation(0.f);
        trees->set_parent(land);

        std::shared_ptr<Model> mill(new Model("..\\..\\binaries\\assets\\Molino.obj", colors_mill));
        mill->get_transform().set_pivot(Transform3f::PivotRotation::Y);
        mill->get_transform().set_translation({ 0, 0, 0});
        mill->get_transform().set_rotation({ 0.f, 0.f, 0.f });
        mill->get_transform().set_scale(1.f);
        mill->set_speed_rotation(0.f);
        mill->set_parent(land);

        // All of them are push back to the model_buffer

		model_buffer.push_back(land);
        model_buffer.push_back(clouds);
        model_buffer.push_back(trees);
        model_buffer.push_back(mill);
        
        // Creates the light

        Point3f pos({ 1, 1, 1 });           // Light's position
        Color color = { 220, 220, 220 };    // Light's color

        std::shared_ptr<Light> light(new Light(pos, color));

        light_buffer.push_back(light);
    }

    void View::update ()
    {	
        // Call of every model's update		

        for (auto model : model_buffer)
		{
			model->update(camera.get_inverse(), camera.get_projection(), light_buffer[0]);
		}
    }

    void View::paint ()
    {
        // Clear the framebuffer 

        rasterizer.clear();

        // Call of every model's render

        for (auto& model : model_buffer)
        {
            model->render(rasterizer);
        }
    }

}
