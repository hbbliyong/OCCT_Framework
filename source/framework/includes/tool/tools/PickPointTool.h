#pragma once

#include "tool/Tool.h"

#include <functional>
#include <optional>
#include <gp_Pnt.hxx>
namespace SongYun
{
	class View;

	class PickPointTool : public Tool
	{
	public:
		explicit PickPointTool(
			View* view);

		std::function<void(const gp_Pnt&)> OnPicked;

		std::function<void()> OnCancelled;

		void MousePress(
			QMouseEvent* e) override;

		void KeyPress(
			QKeyEvent* e) override;

	private:
		View* m_view{};
	};
}