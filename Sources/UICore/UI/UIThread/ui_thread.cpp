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
*/

#include "UICore/precomp.h"
#include "UICore/Display/2D/canvas.h"
#include "UICore/Display/2D/image.h"
#include "UICore/Display/Font/font.h"
#include "UICore/Display/Font/font_family.h"
#include "UICore/Display/System/run_loop.h"
#include "UICore/UI/UIThread/ui_thread.h"
#include "UICore/Core/ErrorReporting/exception_dialog.h"
#include "UICore/Core/IOData/path_help.h"
#include "UICore/UI/Style/style.h"
#include <map>

namespace uicore
{
	static UIThreadImpl *ui_thread_instance = nullptr;

	class UIThreadImpl
	{
	public:
		UIThreadImpl()
		{
			if (ui_thread_instance != nullptr) throw Exception("Only one UIThread is allowed");
			ui_thread_instance = this;
		}

		~UIThreadImpl()
		{
			ui_thread_instance = nullptr;
		}

		std::string resource_path;
		std::function<void(const std::exception_ptr &)> exception_handler;

		std::map<std::string, FontFamily> font_families;
		std::map<std::string, Image> images;

		static UIThreadImpl *get_instance()
		{
			if (ui_thread_instance == nullptr) throw Exception("No UIThread created");
			return ui_thread_instance;
		}
	};

	UIThread::UIThread()
	{
	}

	UIThread::UIThread(const std::string &resource_path, const std::function<void(const std::exception_ptr &)> &exception_handler) : impl(std::make_shared<UIThreadImpl>())
	{
		impl->resource_path = resource_path;
		impl->exception_handler = exception_handler;

		if (!exception_handler)
		{
			impl->exception_handler = ExceptionDialog::show;
		}
	}

	UIThread::~UIThread()
	{
	}

	void UIThread::add_font_face(const std::string &properties, const std::string &src)
	{
		auto style = std::make_shared<Style>();
		style->set(properties);

		auto font_weight = style->declared_value("font-weight");
		auto font_style = style->declared_value("font-style");
		//auto font_variant = style->declared_value("font-variant"); // To do: needs FontDescription support
		auto font_family_name = style->declared_value("font-family-names[0]");

		FontDescription font_desc;

		if (font_weight.is_keyword("normal"))
			font_desc.set_weight(FontWeight::normal);
		else if (font_weight.is_keyword("bold") || font_weight.is_keyword("bolder"))
			font_desc.set_weight(FontWeight::bold);
		else if (font_weight.is_keyword("lighter"))
			font_desc.set_weight(FontWeight::light);
		else if (font_weight.is_number())
			font_desc.set_weight((FontWeight)(int)font_weight.number());

		if (font_style.is_keyword("normal"))
			font_desc.set_style(FontStyle::normal);
		else if (font_style.is_keyword("italic"))
			font_desc.set_style(FontStyle::italic);
		else if (font_style.is_keyword("oblique"))
			font_desc.set_style(FontStyle::oblique);

		std::string family_name;
		if (font_family_name.is_string())
			family_name = font_family_name.text();
		else if (font_family_name.is_keyword())
			family_name = font_family_name.text();
		else
			family_name = "sans-serif";

		auto &family = UIThreadImpl::get_instance()->font_families[family_name];
		if (family.is_null())
			family = FontFamily(family_name);

		family.add(font_desc, PathHelp::combine(UIThreadImpl::get_instance()->resource_path, src));
	}

	Image UIThread::get_image(Canvas &canvas, const std::string &name)
	{
		auto &images = UIThreadImpl::get_instance()->images;
		if (images.find(name) == images.end())
			images[name] = Image(canvas, PathHelp::combine(UIThreadImpl::get_instance()->resource_path, name));
		return images[name];
	}

	Font UIThread::get_font(const std::string &family, const FontDescription &desc)
	{
		auto it = UIThreadImpl::get_instance()->font_families.find(family);
		if (it != UIThreadImpl::get_instance()->font_families.end())
			return Font(it->second, desc);
		else
			return Font(family, desc);
	}

	bool UIThread::try_catch(const std::function<void()> &block)
	{
		try
		{
			block();
			return true;
		}
		catch (...)
		{
			std::exception_ptr exception = std::current_exception();
			RunLoop::main_thread_async([=]()
			{
				UIThreadImpl::get_instance()->exception_handler(exception);
			});
			return false;
		}
	}
}
