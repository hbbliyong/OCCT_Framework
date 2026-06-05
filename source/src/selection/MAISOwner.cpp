#include "selection/MAISOwner.h"

void MAisOwner5::HilightWithColor(const Handle(PrsMgr_PresentationManager) & thePrsMgr,
								  const Handle(Prs3d_Drawer) & theStyle,
								  const Standard_Integer theMode)
{
	if (myPrs.IsNull())
	{
		myPrs = new Prs3d_Presentation(thePrsMgr->StructureManager());
		MAisObject5 *anObj = dynamic_cast<MAisObject5 *>(mySelectable);
		anObj->Compute(thePrsMgr, myPrs, MAisObject5::MyDispMode_Highlight);
	}
	if (thePrsMgr->IsImmediateModeOn())
	{
		// 使用此方法获取的aShadow不能高亮显示Box
		Handle(Prs3d_PresentationShadow) aShadow =
			new Prs3d_PresentationShadow(thePrsMgr->StructureManager(), myPrs);
		// 自动设置myPrs的高亮颜色
		aShadow->SetZLayer(Graphic3d_ZLayerId_Top);
		aShadow->Highlight(theStyle);

		thePrsMgr->AddToImmediateList(aShadow);
	}
	else
	{
		myPrs->Display();
	}
}

void MAisOwner5::Unhilight(const Handle(PrsMgr_PresentationManager) & thePrsMgr, const Standard_Integer theMode)
{
	if (!myPrs.IsNull())
	{
		myPrs->Erase();
	}
}

void MAisOwner6::HilightWithColor(const Handle(PrsMgr_PresentationManager) & thePrsMgr,
								  const Handle(Prs3d_Drawer) & theStyle,
								  const Standard_Integer theMode)
{
	MAisObject6 *anObj = dynamic_cast<MAisObject6 *>(mySelectable);
	if (thePrsMgr->IsImmediateModeOn())
	{
		// 文档有误， 类型为StdSelect_ViewerSelector3d
		Handle(StdSelect_ViewerSelector3d) aSelector = anObj->InteractiveContext()->MainSelector();
		SelectMgr_SortCriterion aPickPnt;
		for (int aPickIter = 1; aPickIter <= aSelector->NbPicked(); ++aPickIter)
		{
			if (aSelector->Picked(aPickIter) == this)
			{
				aPickPnt = aSelector->PickedData(aPickIter);
				break;
			}
		}

		Handle(Prs3d_Presentation) aPrs = mySelectable->GetHilightPresentation(thePrsMgr);
		aPrs->SetZLayer(Graphic3d_ZLayerId_Top);
		aPrs->Clear();
		// 文档有误，变量名为aGroupPnt
		Handle(Graphic3d_Group) aGroupPnt = aPrs->NewGroup();
		aGroupPnt->SetGroupPrimitivesAspect(theStyle->ArrowAspect()->Aspect());
		gp_Trsf aTrsfInv = mySelectable->LocalTransformation().Inverted();
		gp_Dir aNorm(aPickPnt.Normal.x(), aPickPnt.Normal.y(), aPickPnt.Normal.z());
		Handle(Graphic3d_ArrayOfTriangles) aTris = Prs3d_Arrow::DrawShaded(
			gp_Ax1(aPickPnt.Point, aNorm).Transformed(aTrsfInv),
			1.0, 15.0,
			3.0, 4.0, 10);
		aGroupPnt->AddPrimitiveArray(aTris);
		thePrsMgr->AddToImmediateList(aPrs);
	}
}

void MAisOwner6::Unhilight(const Handle(PrsMgr_PresentationManager) & thePrsMgr, const Standard_Integer theMode)
{
	if (!myPrs.IsNull())
	{
		myPrs->Erase();
	}
}