#include "app/App.h"
#include <QApplication>
#include <QSurfaceFormat>
namespace SongYun {

	App& App::Instance()
	{
		static App instance;
		return instance;
	}

	App::App() = default;

	bool App::initialize(QApplication& app, const QString& appName)
	{


		if (initialized_)
			return false;

		application_ = &app;
		applicationName_ = appName;
		initialized_ = true;
		return true;
	}

	void App::shutdown()
	{
		initialized_ = false;
		application_ = nullptr;
		applicationName_.clear();
	}

	bool App::isInitialized() const noexcept
	{
		return initialized_;
	}

	QApplication* App::application() const noexcept
	{
		return application_;
	}

	const QString& App::applicationName() const noexcept
	{
		return applicationName_;
	}

} // namespace SongYun
