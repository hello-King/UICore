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
*/

#include "UICore/UI/View/view.h"
#include "UICore/UI/View/focus_policy.h"
#include "UICore/UI/Style/style.h"
#include "UICore/Display/Window/display_window.h"
#include "UICore/Display/Window/cursor.h"
#include "UICore/Display/Window/cursor_description.h"
#include "../Animation/animation_group.h"
#include "view_layout.h"
#include "flex_layout.h"
#include <map>

namespace uicore
{
	class ViewLayout;

	class ViewLayoutCache
	{
	public:
		bool preferred_width_calculated = false;
		float preferred_width = 0.0f;
		std::map<float, float> preferred_height;
		std::map<float, float> first_baseline_offset;
		std::map<float, float> last_baseline_offset;

		bool definite_width_calculated = false;
		bool is_width_definite = false;
		float definite_width = 0.0f;

		bool definite_height_calculated = false;
		bool is_height_definite = false;
		float definite_height = 0.0f;

		void clear()
		{
			preferred_width_calculated = false;
			preferred_width = 0.0f;
			preferred_height.clear();
			first_baseline_offset.clear();
			last_baseline_offset.clear();
			definite_width_calculated = false;
			is_width_definite = false;
			definite_width = 0.0f;
			definite_height_calculated = false;
			is_height_definite = false;
			definite_height = 0.0f;
		}
	};

	class ViewImpl
	{
	public:
		ViewLayout *active_layout(View *self);

		void render(View *self, const CanvasPtr &canvas);
		void process_event(View *self, EventUI *e, bool use_capture);
		void process_event_handler(ViewEventHandler *handler, EventUI *e);
		void update_style_cascade() const;

		unsigned int find_next_tab_index(unsigned int tab_index) const;
		unsigned int find_prev_tab_index(unsigned int tab_index) const;
		unsigned int find_highest_tab_index() const;
		View *find_next_with_tab_index(unsigned int tab_index, const ViewImpl *search_from = nullptr, bool also_search_ancestors = true) const;
		View *find_prev_with_tab_index(unsigned int tab_index, const ViewImpl *search_from = nullptr, bool also_search_ancestors = true) const;

		void set_state_cascade_siblings(const std::string &name, bool value);

		void inverse_bubble(EventUI *e, const View *until_parent_view);

		View *_parent = nullptr;
		std::shared_ptr<View> _first_child, _last_child;
		std::shared_ptr<View> _next_sibling;
		std::weak_ptr<View> _prev_sibling;

		std::vector<std::shared_ptr<ViewAction>> _actions;
		ViewAction *_active_action = nullptr;

		Signal<void(PointerEvent *)> sig_pointer_press;
		Signal<void(PointerEvent *)> sig_pointer_double_click;
		Signal<void(PointerEvent *)> sig_pointer_release;
		Signal<void(PointerEvent *)> sig_pointer_move;
		Signal<void(PointerEvent *)> sig_pointer_enter;
		Signal<void(PointerEvent *)> sig_pointer_leave;
		Signal<void(PointerEvent *)> sig_pointer_proximity_change;
		Signal<void(ActivationChangeEvent *)> sig_activated;
		Signal<void(ActivationChangeEvent *)> sig_deactivated;
		Signal<void(FocusChangeEvent *)> sig_focus_gained;
		Signal<void(FocusChangeEvent *)> sig_focus_lost;
		Signal<void(KeyEvent *)> sig_key_press;
		Signal<void(KeyEvent *)> sig_key_release;
		Signal<void(CloseEvent *)> sig_close;

		unsigned int tab_index = 0;
		FocusPolicy focus_policy = FocusPolicy::reject;

		mutable StyleCascade style_cascade;
		mutable std::map<std::string, std::shared_ptr<Style>> styles;

		struct StyleState
		{
			StyleState(){}
			StyleState(bool is_inherited, bool is_enabled) : inherited(is_inherited), enabled(is_enabled){}
			bool inherited = true;	// Set to true by set_state_cascade(), else false
			bool enabled = false;
		};

		std::map<std::string, StyleState> states;
		
		ViewGeometry _geometry;
		bool hidden = false;

		Mat4f view_transform = Mat4f::identity();
		bool content_clipped = false;

		bool exception_encountered = false;

		bool needs_layout = true;

		ViewTree *view_tree = nullptr;

		AnimationGroup animation_group;

		StandardCursor cursor_type = StandardCursor::arrow;
		CursorDescription cursor_desc;
		CursorPtr cursor;
		bool is_custom_cursor = false;
		bool is_cursor_inherited = true;

		ViewLayoutCache layout_cache;

		FlexLayout flex;

	private:
		unsigned int find_prev_tab_index_helper(unsigned int tab_index) const;
	};
}
