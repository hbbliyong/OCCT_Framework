#pragma once

#include "core/CoreMacro.hpp"

class QMouseEvent;
class QKeyEvent;
namespace SongYun
{

	class Tool
	{
	public:
		virtual SONGYUN_API ~Tool() = default;

		virtual SONGYUN_API void OnEnter() {}

		virtual SONGYUN_API void OnLeave() {}

		virtual SONGYUN_API void MouseMove(QMouseEvent* event) {}

		virtual SONGYUN_API void MousePress(QMouseEvent* event) {}

		virtual SONGYUN_API void MouseRelease(QMouseEvent* event) {}
		virtual void KeyPress(
			QKeyEvent* e) {};
	};

} // namespace SongYun
