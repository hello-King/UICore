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

#pragma once

#include "UICore/Display/Render/graphic_context_provider.h"
#include "UICore/Display/Render/blend_state_description.h"
#include "UICore/Core/System/comptr.h"

namespace uicore
{
	class D3DBlendState : public BlendState
	{
	public:
		D3DBlendState(const ComPtr<ID3D11Device> &device, const BlendStateDescription &desc);

		ComPtr<ID3D11BlendState> state;

	private:
		static D3D11_BLEND_OP to_d3d_blend_op(BlendEquation op);
		static D3D11_BLEND to_d3d_blend_func(BlendFunc func);
	};
}