#pragma once

#include "selection/MAISObj.h"

#include "AIS_InteractiveObject.hxx"
#include "SelectMgr_EntityOwner.hxx"
#include "Select3D_SensitivePrimitiveArray.hxx"
#include "Select3D_SensitivePoint.hxx"
#include "Select3D_SensitiveSegment.hxx"
#include "Select3D_SensitiveFace.hxx"
#include "Select3D_SensitiveBox.hxx"
#include "StdSelect_FaceFilter.hxx"
#include "StdSelect_ViewerSelector3d.hxx"
#include "StdSelect_BRepSelectionTool.hxx"

#include "Image_AlienPixMap.hxx"

class MAisObject5;
class MAisObject6;

// HighlightingSelectionOwner
class MAisOwner5 : public SelectMgr_EntityOwner
{
	DEFINE_STANDARD_RTTI_INLINE(MAisOwner5, SelectMgr_EntityOwner)
public:
	MAisOwner5(const Handle(AIS_InteractiveObject) & theObj, int thePriority = 0)
		: SelectMgr_EntityOwner(theObj, thePriority)
	{
	}

	virtual void HilightWithColor(const Handle(PrsMgr_PresentationManager) & thePrsMgr,
								  const Handle(Prs3d_Drawer) & theStyle,
								  const Standard_Integer theMode) override;

	virtual void Unhilight(const Handle(PrsMgr_PresentationManager) & thePrsMgr,
						   const Standard_Integer theMode) override;

protected:
	Handle(Prs3d_Presentation) myPrs;
};

// add arrow
class MAisOwner6 : public SelectMgr_EntityOwner
{
	DEFINE_STANDARD_RTTI_INLINE(MAisOwner6, SelectMgr_EntityOwner)
public:
	MAisOwner6(const Handle(AIS_InteractiveObject) & theObj, int thePriority = 0)
		: SelectMgr_EntityOwner(theObj, thePriority)
	{
	}

	virtual void HilightWithColor(const Handle(PrsMgr_PresentationManager) & thePrsMgr,
								  const Handle(Prs3d_Drawer) & theStyle,
								  const Standard_Integer theMode) override;

	virtual void Unhilight(const Handle(PrsMgr_PresentationManager) & thePrsMgr,
						   const Standard_Integer theMode) override;
	virtual bool IsForcedHilight() const override
	{
		return true;
	}

protected:
	Handle(Prs3d_Presentation) myPrs;
};
