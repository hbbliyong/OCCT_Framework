#include "selection/PickService.h"
namespace SongYun {
	PickService& PickService::instance()
	{
		static PickService service;
		return service;
	}

	void PickService::initialize(View* view, const Handle(AIS_InteractiveContext)& context)
	{
		myView = view;
		myContext = context;
	}

	bool PickService::isInitialized() const noexcept
	{
		return myView != nullptr && !myContext.IsNull();
	}

	gp_Pnt PickService::pickPoint(int x, int y) const
	{
		if (!isInitialized())
			return gp_Pnt();

		return myView->convertScreenToAt({ x, y });
	}

	PickResult PickService::pickObject(int x, int y) const
	{
		PickResult result;
		if (!isInitialized())
			return result;

		myContext->MoveTo(x, y, myView->getV3dView(), Standard_True);
		if (myContext->MoreDetected())
		{
			myContext->SelectDetected(AIS_SelectionScheme_Replace);
			result.object = myContext->DetectedInteractive();
			result.shape = myContext->DetectedShape();
			result.worldPoint = pickPoint(x, y);
		}
		return result;
	}
}