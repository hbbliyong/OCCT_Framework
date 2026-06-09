#pragma once

#include "tool/Tool.h"

#include <functional>
#include <optional>
#include <gp_Pnt.hxx>
namespace SongYun
{
	class OccView;

	class PickPointTool : public Tool
	{
	public:
		explicit PickPointTool(
			OccView* view);

		std::function<void(const gp_Pnt&)> OnPicked;

		std::function<void()> OnCancelled;

		void MousePress(
			QMouseEvent* e) override;

		void KeyPress(
			QKeyEvent* e) override;

	private:
		OccView* m_view{};
	};
}