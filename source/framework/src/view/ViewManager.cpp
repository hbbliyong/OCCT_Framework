#include "view/ViewManager.h"
#include "view/View.h"
namespace SongYun {
	ViewManager::ViewManager(QObject* parent)
		: QObject(parent), activeView_(nullptr)
	{}

	void ViewManager::setActiveView(View* view)
	{
		activeView_ = view;
	}

	View* ViewManager::activeView() const
	{
		return activeView_;
	}
}