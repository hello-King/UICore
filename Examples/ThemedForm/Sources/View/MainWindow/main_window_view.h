
#pragma once

#include "View/Theme/theme_form_view.h"
#include "View/Theme/theme_button_view.h"
#include "View/Theme/theme_checkbox_view.h"
#include "View/Theme/theme_radio_button_view.h"
#include "View/Theme/theme_form_view.h"
#include "View/Theme/theme_image_view.h"
#include "View/Theme/theme_label_view.h"
#include "View/Theme/theme_listbox_view.h"
#include "View/Theme/theme_scroll_view.h"
#include "View/Theme/theme_slider_view.h"
#include "View/Theme/theme_spin_view.h"
#include "View/Theme/theme_text_block_view.h"
#include "View/Theme/theme_text_view.h"
#include "View/Theme/theme_text_field_view.h"

class MainWindowView : public ThemeFormView
{
public:
	MainWindowView()
	{
		auto headline = add_child<ThemeHeadlineView>();
		headline->style()->set("margin-top: 0");
		headline->add_text("Form example with themed views");

		auto p = add_child<ThemeParagraphView>();
		p->style()->set("margin-top: 0");
		p->add_text("This is an example of how to create a form using themed UICore standard views.");

		auto button = add_child<ThemeButtonView>();
		button->label()->set_text("Test Button");
		button->style()->set("margin-right: auto");

		auto slider = add_child<ThemeSliderView>();
		slider->style()->set("width: 300px; margin-right: auto; margin-top: 5px");

		auto absolute = add_child<View>();
		absolute->style()->set(R"(
			position: absolute;
			right: 10%;
			top: 10%;
			width: 80%;
			height: 80%;
			background: rgba(160,255,160,0.5);
			)");
	}
};
