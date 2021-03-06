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
**    Magnus Norddahl
**    Mark Page
*/

#include "UICore/precomp.h"
#include "path_cache.h"
#include "FontEngine/font_engine.h"
#include "UICore/Core/System/databuffer.h"
#include "UICore/Core/Text/string_format.h"
#include "UICore/Core/Math/color.h"
#include "UICore/Display/Image/pixel_buffer.h"
#include "UICore/Display/2D/texture_group.h"
#include "UICore/Display/2D/canvas.h"
#include "UICore/Display/Render/texture.h"
#include "UICore/Display/Font/font_metrics.h"
#include "UICore/Display/Font/glyph_metrics.h"
#include "UICore/Core/Text/text.h"
#include "UICore/Core/Text/utf8_reader.h"
#include "UICore/Display/2D/render_batch_triangle.h"

namespace uicore
{
	PathCache::PathCache()
	{
		glyph_list.reserve(256);
	}

	PathCache::~PathCache()
	{
		for (auto & elem : glyph_list)
			delete elem;
	}

	Font_PathGlyph *PathCache::get_glyph(const CanvasPtr &canvas, FontEngine *font_engine, unsigned int glyph)
	{
		std::vector< Font_PathGlyph * >::size_type size = glyph_list.size();
		for (int cnt = 0; cnt < size; cnt++)
		{
			if (glyph_list[cnt]->glyph == glyph)
				return &(*glyph_list[cnt]);
		}

		auto font_glyph = new Font_PathGlyph();
		glyph_list.push_back(font_glyph);
		font_glyph->glyph = glyph;
		font_engine->load_glyph_path(glyph, font_glyph->path, font_glyph->metrics);

		// Search for the glyph again
		size = glyph_list.size();
		for (int cnt = 0; cnt < size; cnt++)
		{
			if (glyph_list[cnt]->glyph == glyph)
				return &(*glyph_list[cnt]);
		}

		return nullptr;
	}

	GlyphMetrics PathCache::get_metrics(FontEngine *font_engine, const CanvasPtr &canvas, unsigned int glyph)
	{
		Font_PathGlyph *gptr = get_glyph(canvas, font_engine, glyph);
		if (gptr)
		{
			return gptr->metrics;
		}
		return GlyphMetrics();
	}
}
