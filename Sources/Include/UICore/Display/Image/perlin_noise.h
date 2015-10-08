/*
**  UICore
**  Copyright (c) 1997-2015 The UICore Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Mark Page
*/

#pragma once

#include <memory>
#include "pixel_buffer.h"

namespace uicore
{
	/// \brief Perlin Noise Generator class
	class PerlinNoise
	{
	public:
		/// \brief Constructor
		static std::shared_ptr<PerlinNoise> create();

		/// \brief Create the perlin noise
		///
		/// Remember to call set_size() with the height set to 1.\n
		/// The period of these parameters are from 0 to 255
		///
		/// \param start_x = Start x position of the noise
		/// \param end_x = End x position of the noise
		/// \param start_y = Start y position of the noise
		/// \param end_y = End y position of the noise
		virtual PixelBufferPtr create_noise1d(float start_x, float end_x) = 0;

		/// \brief Create the perlin noise
		///
		/// The period of these parameters are from 0 to 255
		///
		/// \param start_x = Start x position of the noise
		/// \param end_x = End x position of the noise
		/// \param start_y = Start y position of the noise
		/// \param end_y = End y position of the noise
		virtual PixelBufferPtr create_noise2d(float start_x, float end_x, float start_y, float end_y) = 0;

		/// \brief Create the perlin noise
		///
		/// The period of these parameters are from 0 to 255
		///
		/// \param start_x = Start x position of the noise
		/// \param end_x = End x position of the noise
		/// \param start_y = Start y position of the noise
		/// \param end_y = End y position of the noise
		/// \param z_position = The z position of the noise
		virtual PixelBufferPtr create_noise3d(float start_x, float end_x, float start_y, float end_y, float z_position) = 0;

		/// \brief Create the perlin noise
		///
		/// The period of these parameters are from 0 to 255
		///
		/// \param start_x = Start x position of the noise
		/// \param end_x = End x position of the noise
		/// \param start_y = Start y position of the noise
		/// \param end_y = End y position of the noise
		/// \param z_position = The z position of the noise
		/// \param w_position = The w position of the noise
		virtual PixelBufferPtr create_noise4d(float start_x, float end_x, float start_y, float end_y, float z_position, float w_position) = 0;

		/// \brief Get the size of the output pixelbuffer
		virtual Size size() const = 0;

		/// \brief Get the format of the output pixelbuffer
		virtual TextureFormat format() const = 0;

		/// \brief Get the amplitude of the perlin noise
		virtual float amplitude() const = 0;

		/// \brief Get the number of octaves of the perlin noise
		virtual int octaves() const = 0;

		/// \brief Set the permutation table
		///
		/// If this function is not used, this class uses rand() to create a permutation table instead
		///
		/// \param table = The permutation table
		/// \param size = The permutation table size (2,4,8,16,32,64,128,256). Usually 256 (the maximum) unless a lower periodic table is required
		virtual void set_permutations(const unsigned char *table, unsigned int size = 256) = 0;

		/// \brief Set the size of the output pixelbuffer
		///
		/// If this function is not used, the size defaults to 256x256
		///
		/// \param width = Output pixelbuffer width
		/// \param height = Output pixelbuffer height
		void set_size(int width = 256, int height = 256) { set_size(Size(width, height)); }

		/// \brief Set the size of the output pixelbuffer
		///
		/// If this function is not used, the size defaults to 256x256
		///
		/// \param width = Output pixelbuffer width
		/// \param height = Output pixelbuffer height
		virtual void set_size(const Size &size) = 0;

		/// \brief Set the format of the output pixelbuffer
		///
		/// If this function is not used, the format defaults to tf_rgb8\n
		/// These following formats are supported:\n
		///		tf_rgba8, tf_rgb8, tf_r8, tf_r32f
		///
		/// \param format = The specified format
		virtual void set_format(TextureFormat texture_format = tf_rgb8) = 0;

		/// \brief Set the amplitude of the perlin noise
		///
		/// If this function is not used, the amplitude defaults to 1.0f
		///
		/// \param amplitude = The amplitude to set
		virtual void set_amplitude(float amplitude = 1.0f) = 0;

		/// \brief Set the number of octaves of the perlin noise
		///
		/// If this function is not used, the octaves count defaults to 1
		///
		/// \param octaves = The number of octaves to set
		virtual void set_octaves(int octaves = 1) = 0;
	};
}
