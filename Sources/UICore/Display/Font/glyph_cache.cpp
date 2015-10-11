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
**    Magnus Norddahl
**    Mark Page
*/

#include "UICore/precomp.h"
#include "glyph_cache.h"
#include "FontEngine/font_engine.h"
#include "UICore/Display/Image/pixel_buffer.h"
#include "UICore/Display/2D/subtexture.h"
#include "UICore/Display/2D/texture_group.h"
#include "UICore/Display/2D/subtexture.h"
#include "UICore/Display/2D/canvas.h"
#include "UICore/Display/Render/texture.h"
#include "UICore/Display/Font/font_metrics.h"
#include "UICore/Display/Font/glyph_metrics.h"
#include "UICore/Core/Math/color.h"
#include "UICore/Core/System/databuffer.h"
#include "UICore/Core/Text/string_format.h"
#include "UICore/Core/Text/text.h"
#include "UICore/Core/Text/utf8_reader.h"
#include "UICore/Display/2D/render_batch_triangle.h"

namespace uicore
{
	GlyphCache::GlyphCache()
	{
		glyph_list.reserve(256);
	}

	GlyphCache::~GlyphCache()
	{
	}

	Font_TextureGlyph *GlyphCache::get_glyph(const CanvasPtr &canvas, FontEngine *font_engine, unsigned int glyph)
	{
		auto size = glyph_list.size();
		for (int cnt = 0; cnt < size; cnt++)
		{
			if (glyph_list[cnt]->glyph == glyph)
				return &(*glyph_list[cnt]);
		}

		// If glyph does not exist, create one automatically
		FontPixelBuffer pb = font_engine->get_font_glyph(glyph);
		if (pb.glyph)	// Ignore invalid glyphs
			insert_glyph(canvas, pb);

		// Search for the glyph again
		size = glyph_list.size();
		for (int cnt = 0; cnt < size; cnt++)
		{
			if (glyph_list[cnt]->glyph == glyph)
				return &(*glyph_list[cnt]);
		}

		return nullptr;
	}

	void GlyphCache::set_texture_group(TextureGroup &new_texture_group)
	{
		texture_group = new_texture_group;
	}

	GlyphMetrics GlyphCache::get_metrics(FontEngine *font_engine, const CanvasPtr &canvas, unsigned int glyph)
	{
		Font_TextureGlyph *gptr = get_glyph(canvas, font_engine, glyph);
		if (gptr)
		{
			return gptr->metrics;
		}
		return GlyphMetrics();
	}

	void GlyphCache::insert_glyph(const CanvasPtr &canvas, FontPixelBuffer &pb)
	{
		auto font_glyph = std::unique_ptr<Font_TextureGlyph>(new Font_TextureGlyph());

		font_glyph->glyph = pb.glyph;
		font_glyph->offset = pb.offset;
		font_glyph->metrics = pb.metrics;

		if (!pb.empty_buffer)
		{
			PixelBufferPtr buffer_with_border = PixelBuffer::add_border(pb.buffer, glyph_border_size, pb.buffer_rect);
			GraphicContextPtr gc = canvas->gc();
			Subtexture sub_texture = texture_group.add(gc, buffer_with_border->size());
			font_glyph->texture = sub_texture.get_texture();
			font_glyph->geometry = Rect(sub_texture.get_geometry().left + glyph_border_size, sub_texture.get_geometry().top + glyph_border_size, pb.buffer_rect.get_size());
			font_glyph->size = pb.size;
			sub_texture.get_texture()->set_subimage(gc, sub_texture.get_geometry().left, sub_texture.get_geometry().top, buffer_with_border, buffer_with_border->size());
		}

		glyph_list.push_back(std::move(font_glyph));
	}

	void GlyphCache::insert_glyph(const CanvasPtr &canvas, unsigned int glyph, Subtexture &sub_texture, const Pointf &offset, const Sizef &size, const GlyphMetrics &glyph_metrics)
	{
		auto font_glyph = std::unique_ptr<Font_TextureGlyph>(new Font_TextureGlyph());

		font_glyph->glyph = glyph;
		font_glyph->offset = offset;
		font_glyph->metrics = glyph_metrics;
		font_glyph->size = size;

		if ((sub_texture.get_geometry().get_width() > 0) && (sub_texture.get_geometry().get_height() > 0))
		{
			font_glyph->texture = sub_texture.get_texture();
			font_glyph->geometry = sub_texture.get_geometry();
		}

		glyph_list.push_back(std::move(font_glyph));
	}
}
