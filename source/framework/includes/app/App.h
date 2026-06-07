#pragma once

#include "core/CoreMacro.hpp"
#include <QString>

class QApplication;

namespace SongYun {

	class SONGYUN_API App
	{
	public:
		static App& Instance();

		bool initialize(QApplication& app, const QString& appName = "VtkQtMinimal");
		void shutdown();
		bool isInitialized() const noexcept;
		QApplication* application() const noexcept;
		const QString& applicationName() const noexcept;

	private:
		App();

		QApplication* application_ = nullptr;
		QString applicationName_;
		bool initialized_ = false;
	};

} // namespace SongYun
