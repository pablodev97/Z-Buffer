
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
 * File: View.hpp
 * File Created: 2nd February 2020
 * ––––––––––––––––––––––––
 * Author: Pablo Jimeno Domínguez (pablojimenodev@gmail.com)
 */

#ifndef VIEW_HEADER
#define VIEW_HEADER

    #include <vector>
    #include "Rasterizer.hpp"
	#include "Color_Buffer_Rgb565.hpp"
    #include "Color_Buffer_Rgba8888.hpp"
	#include <Shared_Ptr.hpp>
    #include "Camera.hpp"
	#include "Model.hpp"
    #include "Light.hpp"
	
namespace example
    {

        using std::vector;

        /**
            View management
        */
        class View
        {
        private:

            typedef Color_Buffer_Rgba8888  Color_Buffer;
            typedef Color_Buffer::Color    Color;
			typedef vector< std::shared_ptr<Model> >   Model_Buffer;
            typedef vector< std::shared_ptr<Light> >   Light_Buffer;

        private:

            Camera camera;

            /**
            @brief Color buffer of the window
            */
            Color_Buffer               Color_buffer;
            /**
            @brief Rasterizer based on the color buffer
            */
            Rasterizer< Color_Buffer > rasterizer;
            /**
            @brief Model buffer to save the models of the view
            */
            Model_Buffer			   model_buffer;
            /**
            @brief Light buffer to save the lights of the view
            */
            Light_Buffer			   light_buffer;

        public:

            /**
            @brief Constructor of the class View
            @param width The window width in pixels
            @param height The window height in pixels
            */
            View(size_t width, size_t height);

            /**
            @brief Method that updates the models 
            */
            void update ();

            /**
            @brief Method that clears the window and then paints the models
            */
            void paint  ();
        };

    }

#endif
