#pragma once

namespace SongYun { class MainWindow; }

namespace Samples
{
	class MenuBarSetup
	{
	public:
		MenuBarSetup() = delete;
		static void initialize(SongYun::MainWindow& mainWindow);
	};

} // namespace Samples
