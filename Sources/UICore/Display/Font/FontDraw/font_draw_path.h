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
**  Note: Some of the libraries UICore may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Mark Page
*/

#pragma once

#include "font_draw.h"

namespace uicore
{
	class PathCache;

	class Font_DrawPath : public Font_Draw
	{
	public:
		void init(PathCache *cache, FontEngine *engine, float new_scaled_height);

		GlyphMetrics get_metrics(const CanvasPtr &canvas, unsigned int glyph) override;
		void draw_text(const CanvasPtr &canvas, const Pointf &position, const std::string &text, const Colorf &color, float line_spacing) override;

	private:
		PathCache *path_cache = nullptr;
		FontEngine *font_engine = nullptr;
		float scaled_height = 1.0f;
	};
}
