#pragma once

class QMenuBar;
class QToolBar;

namespace SongYun { class ActionManager; }

namespace Samples
{
	class MenuBarSetup
	{
	public:
		MenuBarSetup() = delete;
		static void initialize(QMenuBar* menuBar, QToolBar* toolBar,
							   SongYun::ActionManager& am);
	};

} // namespace Samples
