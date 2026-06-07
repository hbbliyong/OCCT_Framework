#include "selection/MAISObj.h"
namespace SongYun
{
	MAisDemo::MAisDemo(const TopoDS_Shape& shap, bool indexVisible) : myShape(shap),
		faceIndexIsVisible(indexVisible)
	{
		// 不设置的话 ,myDrawer->ShadingAspect()->SetColor(theColor)会改变所有AIS的颜色
		// 因为所有context->display调用的AIS都共用一个context的drawer(myLink)
		// 当myDrawer->myShadingAspect.IsNull() && !myLink.IsNull()时，返回myLink->ShadingAspect()而不是myDrawer自己的
		myDrawer->SetupOwnShadingAspect();
		myDrawer->SetupOwnFaceBoundaryAspect();
		myDrawer->SetOwnLineAspects();
		myDrawer->SetFaceBoundaryDraw(true);
	}

	void MAisDemo::Compute(const Handle(PrsMgr_PresentationManager)& thePrsMgr, const Handle(Prs3d_Presentation)& thePrs, const Standard_Integer theMode)
	{
		if (theMode == MDispMode_WireFrame)
		{
			StdPrs_WFShape::Add(thePrs, myShape, myDrawer);
			setFaceIndex(thePrs);
		}
		else if (theMode == MDispMode_Shaded)
		{
			StdPrs_ShadedShape::Add(thePrs, myShape, myDrawer);
			setFaceIndex(thePrs);
		}
	}

	void MAisDemo::ComputeSelection(const Handle(SelectMgr_Selection)& theSel, const Standard_Integer theMode)
	{
		TopAbs_ShapeEnum TypOfSel = SelectionType(theMode);
		Standard_Real aDefl = StdPrs_ToolTriangulatedShape::GetDeflection(myShape, myDrawer);
		StdSelect_BRepSelectionTool::Load(theSel, this, myShape, TypOfSel, aDefl,
			myDrawer->DeviationAngle(),
			myDrawer->IsAutoTriangulation());
	}

	void MAisDemo::SetColor(const Quantity_Color& theColor)
	{
		myDrawer->SetColor(theColor);
		hasOwnColor = Standard_True;

		if (myShape.ShapeType() == TopAbs_EDGE)
		{
			myDrawer->WireAspect()->SetColor(theColor);
		}
		else if (myShape.ShapeType() == TopAbs_SOLID)
		{
			myDrawer->ShadingAspect()->SetColor(theColor);
		}
	}

	void MAisDemo::SetWidth(const Standard_Real theWidth)
	{
		myOwnWidth = (Standard_ShortReal)theWidth;

		myDrawer->FaceBoundaryAspect()->SetWidth(theWidth);
		myDrawer->WireAspect()->SetWidth(theWidth);
	}

	void MAisDemo::setTypeOfLine(Aspect_TypeOfLine theType)
	{
		myDrawer->FaceBoundaryAspect()->SetTypeOfLine(theType);
		myDrawer->WireAspect()->SetTypeOfLine(theType);
	}

	void MAisDemo::setColor(const QColor& theColor)
	{
		SetColor(Quantity_Color(theColor.redF(), theColor.greenF(), theColor.blueF(), Quantity_TOC_RGB));
	}

	void MAisDemo::setFaceIndex(const Handle(Prs3d_Presentation)& thePrs)
	{

		if (faceIndexIsVisible)
		{
			int i = 1;
			for (TopExp_Explorer exp(myShape, TopAbs_FACE); exp.More(); exp.Next(), ++i)
			{
				TopoDS_Face face = TopoDS::Face(exp.Current());

				GProp_GProps props;
				BRepGProp::SurfaceProperties(face, props);
				gp_Pnt center = props.CentreOfMass(); // 获取几何中心

				Handle(Graphic3d_Group) group = thePrs->NewGroup();
				Handle(Graphic3d_AspectText3d) textAspect = new Graphic3d_AspectText3d();
				textAspect->SetColor(Quantity_NOC_BLUE);
				textAspect->SetTextZoomable(false);
				textAspect->SetFont("Consolas");
				textAspect->SetStyle(Aspect_TOST_ANNOTATION);
				group->SetPrimitivesAspect(textAspect);

				Handle(Graphic3d_Text) text = new Graphic3d_Text(20.0);
				text->SetText(std::to_string(i).c_str());
				text->SetPosition(center);
				// 不定义，始终面对摄像头。若定义，positon/朝向自动修改到该gp_Ax2位置/方向
				// text->SetOrientation(gp_Ax2(center, gp::DZ()));

				group->AddText(text);
			}
		}
	}

	MAisObject0::MAisObject0(const TopoDS_Shape& shap) : aShape(shap)
	{
		SetDisplayMode(MyDispMode_Main);
		SetHilightMode(MyDispMode_Highlight);
	}

	void MAisObject0::Compute(const Handle(PrsMgr_PresentationManager)& thePrsMgr,
		const Handle(Prs3d_Presentation)& thePrs,
		const Standard_Integer theMode)
	{
		if (theMode == MyDispMode_Main)
		{
			StdPrs_ShadedShape::Add(thePrs, aShape, myDrawer);
			StdPrs_WFShape::Add(thePrs, aShape, myDrawer);
		}
		else if (theMode == MyDispMode_Highlight)
		{
			Bnd_Box aBox;
			BRepBndLib::Add(aShape, aBox);
			Prs3d_BndBox::Add(thePrs, aBox, myDrawer);
		}
	}

	MAisObject1::MAisObject1(const TopoDS_Shape& shap) : aShape(shap)
	{
		SetDisplayMode(MyDispMode_Main);
		SetHilightMode(MyDispMode_Highlight);
	}

	void MAisObject1::Compute(const Handle(PrsMgr_PresentationManager)& thePrsMgr,
		const Handle(Prs3d_Presentation)& thePrs,
		const Standard_Integer theMode)
	{
		const double aRadius = 100.0, aHeight = 100.0;
		if (theMode == MyDispMode_Main)
		{
			StdPrs_ShadedShape::Add(thePrs, aShape, myDrawer);
			// StdPrs_WFShape::Add (thePrs, aShape, myDrawer);
			const double aRadius = 100.0, aHeight = 100.0;
			Handle(Graphic3d_ArrayOfSegments) aSegs = new Graphic3d_ArrayOfSegments(4, 4 * 2, Graphic3d_ArrayFlags_None);
			aSegs->AddVertex(gp_Pnt(0.0, -aRadius, 0.0));
			aSegs->AddVertex(gp_Pnt(0.0, -aRadius, aHeight));
			aSegs->AddVertex(gp_Pnt(0.0, aRadius, aHeight));
			aSegs->AddVertex(gp_Pnt(0.0, aRadius, 0.0));
			aSegs->AddEdges(1, 2);
			aSegs->AddEdges(2, 3);
			aSegs->AddEdges(3, 4);
			aSegs->AddEdges(4, 1);
			Handle(Graphic3d_Group) aGroupSegs = thePrs->NewGroup();
			aGroupSegs->SetGroupPrimitivesAspect(myDrawer->WireAspect()->Aspect());
			aGroupSegs->AddPrimitiveArray(aSegs);
		}
		else if (theMode == MyDispMode_Highlight)
		{
			Bnd_Box aBox;
			BRepBndLib::Add(aShape, aBox);
			Prs3d_BndBox::Add(thePrs, aBox, myDrawer);
		}
	}

	MAisObject2::MAisObject2(const TopoDS_Shape& shap) : aShape(shap)
	{
		SetHilightMode(MyDispMode_Highlight);
		myDrawer->SetupOwnShadingAspect();
		myDrawer->ShadingAspect()->SetMaterial(Graphic3d_NameOfMaterial_Silver);
		myDrawer->SetWireAspect(new Prs3d_LineAspect(Quantity_NOC_GREEN, Aspect_TOL_SOLID, 2.0));
	}

	void MAisObject2::Compute(const Handle(PrsMgr_PresentationManager)& thePrsMgr,
		const Handle(Prs3d_Presentation)& thePrs,
		const Standard_Integer theMode)
	{
		if (theMode == MyDispMode_Main)
		{
			StdPrs_ShadedShape::Add(thePrs, aShape, myDrawer);
			// StdPrs_WFShape::Add (thePrs, aShape, myDrawer);
			const double aRadius = 100.0, aHeight = 100.0;
			Handle(Graphic3d_ArrayOfSegments) aSegs = new Graphic3d_ArrayOfSegments(4, 4 * 2, Graphic3d_ArrayFlags_None);
			aSegs->AddVertex(gp_Pnt(0.0, -aRadius, 0.0));
			aSegs->AddVertex(gp_Pnt(0.0, -aRadius, aHeight));
			aSegs->AddVertex(gp_Pnt(0.0, aRadius, aHeight));
			aSegs->AddVertex(gp_Pnt(0.0, aRadius, 0.0));
			aSegs->AddEdges(1, 2);
			aSegs->AddEdges(2, 3);
			aSegs->AddEdges(3, 4);
			aSegs->AddEdges(4, 1);
			Handle(Graphic3d_Group) aGroupSegs = thePrs->NewGroup();
			aGroupSegs->SetGroupPrimitivesAspect(myDrawer->WireAspect()->Aspect());
			aGroupSegs->AddPrimitiveArray(aSegs);
		}
		else if (theMode == MyDispMode_Highlight)
		{
			Bnd_Box aBox;
			BRepBndLib::Add(aShape, aBox);
			Prs3d_BndBox::Add(thePrs, aBox, myDrawer);
		}
	}

	MAisObject3::MAisObject3(const TopoDS_Shape& shap) : aShape(shap)
	{
		SetHilightMode(MyDispMode_Highlight);
		myDrawer->SetupOwnShadingAspect();
		myDrawer->ShadingAspect()->SetMaterial(Graphic3d_NameOfMaterial_Silver);
		myDrawer->SetWireAspect(new Prs3d_LineAspect(Quantity_NOC_GREEN, Aspect_TOL_SOLID, 2.0));
	}

	void MAisObject3::Compute(const Handle(PrsMgr_PresentationManager)& thePrsMgr,
		const Handle(Prs3d_Presentation)& thePrs,
		const Standard_Integer theMode)
	{
		const double aRadius = 100.0, aHeight = 100.0;
		if (theMode == MyDispMode_Main)
		{
			Prs3d_ToolCylinder aCyl(aRadius, aRadius, aHeight, 25, 25);
			Prs3d_ToolDisk aDisk(0.0, aRadius, 25, 1);
			Handle(Graphic3d_ArrayOfTriangles) aTris =
				new Graphic3d_ArrayOfTriangles(aCyl.VerticesNb() + 2 * aDisk.VerticesNb(),
					3 * (aCyl.TrianglesNb() + 2 * aDisk.TrianglesNb()),
					Graphic3d_ArrayFlags_VertexNormal);
			aCyl.FillArray(aTris, gp_Trsf());
			aDisk.FillArray(aTris, gp_Trsf());

			gp_Trsf aDisk2Trsf;
			aDisk2Trsf.SetTransformation(gp_Ax3(gp_Pnt(0.0, 0.0, aHeight), -gp::DZ(), gp::DX()), gp::XOY());
			aDisk.FillArray(aTris, aDisk2Trsf);

			Handle(Graphic3d_Group) aGroupTris = thePrs->NewGroup();
			aGroupTris->SetGroupPrimitivesAspect(myDrawer->ShadingAspect()->Aspect());
			aGroupTris->AddPrimitiveArray(aTris);
			aGroupTris->SetClosed(true);

			Handle(Graphic3d_ArrayOfSegments) aSegs = new Graphic3d_ArrayOfSegments(4, 4 * 2, Graphic3d_CappingFlags_None);
			aSegs->AddVertex(gp_Pnt(0.0, -aRadius, 0.0));
			aSegs->AddVertex(gp_Pnt(0.0, -aRadius, aHeight));
			aSegs->AddVertex(gp_Pnt(0.0, aRadius, aHeight));
			aSegs->AddVertex(gp_Pnt(0.0, aRadius, 0.0));
			aSegs->AddEdges(1, 2);
			aSegs->AddEdges(2, 3);
			aSegs->AddEdges(3, 4);
			aSegs->AddEdges(4, 1);
			Handle(Graphic3d_Group) aGroupSegs = thePrs->NewGroup();
			aGroupSegs->SetGroupPrimitivesAspect(myDrawer->WireAspect()->Aspect());
			aGroupSegs->AddPrimitiveArray(aSegs);
		}
		else if (theMode == MyDispMode_Highlight)
		{
			Bnd_Box aBox;
			BRepBndLib::Add(aShape, aBox);
			Prs3d_BndBox::Add(thePrs, aBox, myDrawer);
		}
	}

	MAisObject4::MAisObject4(const TopoDS_Shape& shap) : aShape(shap)
	{
		SetDisplayMode(MyDispMode_Main);
		SetHilightMode(MyDispMode_Highlight);
	}

	void MAisObject4::Compute(const Handle(PrsMgr_PresentationManager)& thePrsMgr,
		const Handle(Prs3d_Presentation)& thePrs,
		const Standard_Integer theMode)
	{
		const double aRadius = 100.0, aHeight = 100.0;
		if (theMode == MyDispMode_Main)
		{
			Prs3d_ToolCylinder aCyl(aRadius, aRadius, aHeight, 25, 25);
			Prs3d_ToolDisk aDisk(0.0, aRadius, 25, 1);
			Handle(Graphic3d_ArrayOfTriangles) aTris =
				new Graphic3d_ArrayOfTriangles(aCyl.VerticesNb() + 2 * aDisk.VerticesNb(),
					3 * (aCyl.TrianglesNb() + 2 * aDisk.TrianglesNb()),
					Graphic3d_ArrayFlags_VertexNormal);
			aCyl.FillArray(aTris, gp_Trsf());
			aDisk.FillArray(aTris, gp_Trsf());

			gp_Trsf aDisk2Trsf;
			aDisk2Trsf.SetTransformation(gp_Ax3(gp_Pnt(0.0, 0.0, aHeight), -gp::DZ(), gp::DX()), gp::XOY());
			aDisk.FillArray(aTris, aDisk2Trsf);

			Handle(Graphic3d_Group) aGroupTris = thePrs->NewGroup();
			aGroupTris->SetGroupPrimitivesAspect(myDrawer->ShadingAspect()->Aspect());
			aGroupTris->AddPrimitiveArray(aTris);
			aGroupTris->SetClosed(true);

			Handle(Graphic3d_ArrayOfSegments) aSegs = new Graphic3d_ArrayOfSegments(4, 4 * 2, Graphic3d_CappingFlags_None);
			aSegs->AddVertex(gp_Pnt(0.0, -aRadius, 0.0));
			aSegs->AddVertex(gp_Pnt(0.0, -aRadius, aHeight));
			aSegs->AddVertex(gp_Pnt(0.0, aRadius, aHeight));
			aSegs->AddVertex(gp_Pnt(0.0, aRadius, 0.0));
			aSegs->AddEdges(1, 2);
			aSegs->AddEdges(2, 3);
			aSegs->AddEdges(3, 4);
			aSegs->AddEdges(4, 1);
			Handle(Graphic3d_Group) aGroupSegs = thePrs->NewGroup();
			aGroupSegs->SetGroupPrimitivesAspect(myDrawer->WireAspect()->Aspect());
			aGroupSegs->AddPrimitiveArray(aSegs);
		}
		else if (theMode == MyDispMode_Highlight)
		{
			Bnd_Box aBox;
			BRepBndLib::Add(aShape, aBox);
			Prs3d_BndBox::Add(thePrs, aBox, myDrawer);
		}
	}

	void MAisObject4::ComputeSelection(const Handle(SelectMgr_Selection)& theSel, const Standard_Integer theMode)
	{
		if (theMode != 0)
		{
			return;
		}
		Bnd_Box aBox;
		BRepBndLib::Add(aShape, aBox);
		Handle(SelectMgr_EntityOwner) anOwner = new SelectMgr_EntityOwner(this);
		Handle(Select3D_SensitiveBox) aSensBox = new Select3D_SensitiveBox(anOwner, aBox);
		theSel->Add(aSensBox);

		// Standard_Real aDefl = StdPrs_ToolTriangulatedShape::GetDeflection (aShape,myDrawer);
		// StdSelect_BRepSelectionTool::Load (theSel,this,aShape,TopAbs_SHAPE,aDefl,
		//								   myDrawer->DeviationAngle(),
		//								   myDrawer->IsAutoTriangulation());

		// const double aRadius = 100.0 , aHeight = 100.0;
		// Handle(SelectMgr_EntityOwner) anOwner = new SelectMgr_EntityOwner(this);
		// Handle(Graphic3d_ArrayOfTriangles) aTris =
		//	Prs3d_ToolCylinder::Create(aRadius , aRadius , aHeight , 25 , 25 , gp_Trsf());
		// Handle(Select3D_SensitivePrimitiveArray) aSensTri =
		//	new Select3D_SensitivePrimitiveArray(anOwner);
		// aSensTri->InitTriangulation(aTris->Attributes() , aTris->Indices() ,
		//							 TopLoc_Location());
		// theSel->Add(aSensTri);
	}

	MAisObject5::MAisObject5(const TopoDS_Shape& shap) : aShape(shap)
	{
		myDrawer->SetupOwnShadingAspect();
		myDrawer->ShadingAspect()->SetMaterial(Graphic3d_NameOfMaterial_Silver);
		myDrawer->SetWireAspect(new Prs3d_LineAspect(Quantity_NOC_GREEN, Aspect_TOL_SOLID, 2.0));
	}

	void MAisObject5::Compute(const Handle(PrsMgr_PresentationManager)& thePrsMgr,
		const Handle(Prs3d_Presentation)& thePrs,
		const Standard_Integer theMode)
	{
		const double aRadius = 100.0, aHeight = 100.0;
		if (theMode == MyDispMode_Main)
		{
			Prs3d_ToolCylinder aCyl(aRadius, aRadius, aHeight, 25, 25);
			Prs3d_ToolDisk aDisk(0.0, aRadius, 25, 1);
			Handle(Graphic3d_ArrayOfTriangles) aTris =
				new Graphic3d_ArrayOfTriangles(aCyl.VerticesNb() + 2 * aDisk.VerticesNb(),
					3 * (aCyl.TrianglesNb() + 2 * aDisk.TrianglesNb()),
					Graphic3d_ArrayFlags_VertexNormal);
			aCyl.FillArray(aTris, gp_Trsf());
			aDisk.FillArray(aTris, gp_Trsf());

			gp_Trsf aDisk2Trsf;
			aDisk2Trsf.SetTransformation(gp_Ax3(gp_Pnt(0.0, 0.0, aHeight), -gp::DZ(), gp::DX()), gp::XOY());
			aDisk.FillArray(aTris, aDisk2Trsf);

			Handle(Graphic3d_Group) aGroupTris = thePrs->NewGroup();
			aGroupTris->SetGroupPrimitivesAspect(myDrawer->ShadingAspect()->Aspect());
			aGroupTris->AddPrimitiveArray(aTris);
			aGroupTris->SetClosed(true);

			Handle(Graphic3d_ArrayOfSegments) aSegs = new Graphic3d_ArrayOfSegments(4, 4 * 2, Graphic3d_CappingFlags_None);
			aSegs->AddVertex(gp_Pnt(0.0, -aRadius, 0.0));
			aSegs->AddVertex(gp_Pnt(0.0, -aRadius, aHeight));
			aSegs->AddVertex(gp_Pnt(0.0, aRadius, aHeight));
			aSegs->AddVertex(gp_Pnt(0.0, aRadius, 0.0));
			aSegs->AddEdges(1, 2);
			aSegs->AddEdges(2, 3);
			aSegs->AddEdges(3, 4);
			aSegs->AddEdges(4, 1);
			Handle(Graphic3d_Group) aGroupSegs = thePrs->NewGroup();
			aGroupSegs->SetGroupPrimitivesAspect(myDrawer->WireAspect()->Aspect());
			aGroupSegs->AddPrimitiveArray(aSegs);
		}
		else if (theMode == MyDispMode_Highlight)
		{
			Bnd_Box aBox;
			BRepBndLib::Add(aShape, aBox);
			Prs3d_BndBox::Add(thePrs, aBox, myDrawer);
		}
	}

	void MAisObject5::ComputeSelection(const Handle(SelectMgr_Selection)& theSel, const Standard_Integer theMode)
	{
		if (theMode != 0)
		{
			return;
		}
		const double aRadius = 100.0, aHeight = 100.0;
		Handle(MAisOwner5) anOwner = new MAisOwner5(this);
		Handle(Graphic3d_ArrayOfTriangles) aTris =
			Prs3d_ToolCylinder::Create(aRadius, aRadius, aHeight, 25, 25, gp_Trsf());
		Handle(Select3D_SensitivePrimitiveArray) aSensTri =
			new Select3D_SensitivePrimitiveArray(anOwner);
		aSensTri->InitTriangulation(aTris->Attributes(), aTris->Indices(),
			TopLoc_Location());
		theSel->Add(aSensTri);
	}

	MAisObject6::MAisObject6(const TopoDS_Shape& shap) : aShape(shap)
	{
		myDrawer->SetupOwnShadingAspect();
		myDrawer->ShadingAspect()->SetMaterial(Graphic3d_NameOfMaterial_Silver);
		myDrawer->SetWireAspect(new Prs3d_LineAspect(Quantity_NOC_GREEN, Aspect_TOL_SOLID, 2.0));
	}

	void MAisObject6::Compute(const Handle(PrsMgr_PresentationManager)& thePrsMgr,
		const Handle(Prs3d_Presentation)& thePrs,
		const Standard_Integer theMode)
	{
		const double aRadius = 100.0, aHeight = 100.0;
		if (theMode == MyDispMode_Main)
		{
			Prs3d_ToolCylinder aCyl(aRadius, aRadius, aHeight, 25, 25);
			Prs3d_ToolDisk aDisk(0.0, aRadius, 25, 1);
			Handle(Graphic3d_ArrayOfTriangles) aTris =
				new Graphic3d_ArrayOfTriangles(aCyl.VerticesNb() + 2 * aDisk.VerticesNb(),
					3 * (aCyl.TrianglesNb() + 2 * aDisk.TrianglesNb()),
					Graphic3d_ArrayFlags_VertexNormal);
			aCyl.FillArray(aTris, gp_Trsf());
			aDisk.FillArray(aTris, gp_Trsf());

			gp_Trsf aDisk2Trsf;
			aDisk2Trsf.SetTransformation(gp_Ax3(gp_Pnt(0.0, 0.0, aHeight), -gp::DZ(), gp::DX()), gp::XOY());
			aDisk.FillArray(aTris, aDisk2Trsf);

			Handle(Graphic3d_Group) aGroupTris = thePrs->NewGroup();
			aGroupTris->SetGroupPrimitivesAspect(myDrawer->ShadingAspect()->Aspect());
			aGroupTris->AddPrimitiveArray(aTris);
			aGroupTris->SetClosed(true);

			Handle(Graphic3d_ArrayOfSegments) aSegs = new Graphic3d_ArrayOfSegments(4, 4 * 2, Graphic3d_CappingFlags_None);
			aSegs->AddVertex(gp_Pnt(0.0, -aRadius, 0.0));
			aSegs->AddVertex(gp_Pnt(0.0, -aRadius, aHeight));
			aSegs->AddVertex(gp_Pnt(0.0, aRadius, aHeight));
			aSegs->AddVertex(gp_Pnt(0.0, aRadius, 0.0));
			aSegs->AddEdges(1, 2);
			aSegs->AddEdges(2, 3);
			aSegs->AddEdges(3, 4);
			aSegs->AddEdges(4, 1);
			Handle(Graphic3d_Group) aGroupSegs = thePrs->NewGroup();
			aGroupSegs->SetGroupPrimitivesAspect(myDrawer->WireAspect()->Aspect());
			aGroupSegs->AddPrimitiveArray(aSegs);
		}
		else if (theMode == MyDispMode_Highlight)
		{
			Bnd_Box aBox;
			BRepBndLib::Add(aShape, aBox);
			Prs3d_BndBox::Add(thePrs, aBox, myDrawer);
		}
	}

	void MAisObject6::ComputeSelection(const Handle(SelectMgr_Selection)& theSel, const Standard_Integer theMode)
	{
		if (theMode != 0)
		{
			return;
		}
		const double aRadius = 100.0, aHeight = 100.0;
		Handle(MAisOwner6) anOwner = new MAisOwner6(this);
		Handle(Graphic3d_ArrayOfTriangles) aTris =
			Prs3d_ToolCylinder::Create(aRadius, aRadius, aHeight, 25, 25, gp_Trsf());
		Handle(Select3D_SensitivePrimitiveArray) aSensTri =
			new Select3D_SensitivePrimitiveArray(anOwner);
		aSensTri->InitTriangulation(aTris->Attributes(), aTris->Indices(),
			TopLoc_Location());
		theSel->Add(aSensTri);
	}

	myPk_IShape::myPk_IShape(const TopoDS_Shape& theShape, PrsMgr_TypeOfPresentation3d theType) : AIS_InteractiveObject(theType),
		myShape(theShape)
	{
		SetHilightMode(0);
	}

	void myPk_IShape::Compute(const Handle(PrsMgr_PresentationManager)& thePrsMgr,
		const Handle(Prs3d_Presentation)& thePrs,
		const Standard_Integer theMode)
	{
		switch (theMode)
		{
			// algo for calculation of wireframe presentation
		case 0:
			StdPrs_WFShape::Add(thePrs, myShape, myDrawer);
			return;
			// case 0:
			//{
			//	Handle(V3d_View) aView = myCTXPtr->CurrentViewer()->ActiveView();
			//	Handle(Graphic3d_Camera) aCam = aView->Camera();
			//	Handle(TopLoc_Datum3D) aLoc = new TopLoc_Datum3D();
			//	computeHLR(aCam , aLoc , thePrs);
			// }
			//  algo for calculation of shading presentation
		case 1:
			StdPrs_ShadedShape::Add(thePrs, myShape, myDrawer);
			return;
		}
	}

	void myPk_IShape::computeHLR(const Handle(Graphic3d_Camera)& theProjector,
		const Handle(TopLoc_Datum3D)& theTrsf,
		const Handle(Prs3d_Presentation)& thePrs)
	{
		// Hidden line mode calculation algorithm
		StdPrs_HLRPolyShape hlr;
		hlr.ComputeHLR(thePrs, myShape, myDrawer, theProjector);
	}

	void myPk_IShape::SetColor(const Quantity_Color& theColor)
	{
		myDrawer->ShadingAspect()->SetColor(theColor);
	}

	MAisObject7::MAisObject7()
	{}

	void MAisObject7::Compute(const Handle(PrsMgr_PresentationManager)& thePrsMgr,
		const Handle(Prs3d_Presentation)& thePrs,
		const Standard_Integer theMode)
	{
		if (theMode == MDispMode_WireFrame)
		{
			Handle(Graphic3d_ArrayOfTriangles) anArray = new Graphic3d_ArrayOfTriangles(4, 6, Graphic3d_ArrayFlags_None);
			anArray->AddVertex(-1.0, 0.0, 0.0); // vertex 1
			anArray->AddVertex(1.0, 0.0, 0.0);	// vertex 2
			anArray->AddVertex(0.0, 1.0, 0.0);	// vertex 3
			anArray->AddVertex(0.0, -1.0, 0.0); // vertex 4
			// add edges to the array
			anArray->AddEdges(1, 2, 3); // first triangle
			anArray->AddEdges(1, 2, 4); // second triangle
			// add the array to the structure
			Handle(Graphic3d_Group) aGroup = thePrs->NewGroup();
			aGroup->AddPrimitiveArray(anArray);
			aGroup->SetGroupPrimitivesAspect(myDrawer->ShadingAspect()->Aspect());
		}
	}

	void MAisObject7::ComputeSelection(const Handle(SelectMgr_Selection)& theSel, const Standard_Integer theMode)
	{}

	void MAisObject8::Compute(const Handle(PrsMgr_PresentationManager)& thePrsMgr,
		const Handle(Prs3d_Presentation)& thePrs, const Standard_Integer theMode)
	{
		Handle(Graphic3d_Group) aGroup = thePrs->NewGroup();
		// change the text aspect
		Handle(Graphic3d_AspectText3d) aTextAspect = new Graphic3d_AspectText3d();
		aTextAspect->SetTextZoomable(true);
		// aTextAspect->SetTextAngle(45.0);
		aTextAspect->SetFont("Consolas");
		aGroup->SetPrimitivesAspect(aTextAspect);

		// add a text primitive to the structure
		Handle(Graphic3d_Text) aText = new Graphic3d_Text(100.0f);
		aText->SetText("Text你好啊");
		aText->SetPosition(gp_Pnt(10, 10, 10));
		aGroup->AddText(aText);
	}

	MAisObject9::MAisObject9(TopoDS_Shape& shap) : myShape(shap)
	{
		myDrawer->SetFaceBoundaryDraw(true); // 要放开头，其余样式设置位置随便
		myDrawer->SetFaceBoundaryAspect(new Prs3d_LineAspect(Quantity_NOC_BLACK, Aspect_TOL_SOLID, 1.0));
	}

	void MAisObject9::Compute(const Handle(PrsMgr_PresentationManager)& thePrsMgr,
		const Handle(Prs3d_Presentation)& thePrs,
		const Standard_Integer theMode)
	{
		if (theMode == MDispMode_WireFrame)
		{
			StdPrs_ShadedShape::Add(thePrs, myShape, myDrawer);

			Bnd_Box aBox;
			BRepBndLib::Add(myShape, aBox);
			Prs3d_BndBox::Add(thePrs, aBox, myDrawer);
		}
		else if (theMode == MDispMode_Shaded)
		{
			StdPrs_ShadedShape::Add(thePrs, myShape, myDrawer);

			Bnd_OBB aBox;
			BRepBndLib::AddOBB(myShape, aBox);
			Prs3d_BndBox::Add(thePrs, aBox, myDrawer);
		}
	}

	void MAisObject9::ComputeSelection(const Handle(SelectMgr_Selection)& theSel, const Standard_Integer theMode)
	{
		Bnd_Box aBox;
		BRepBndLib::Add(myShape, aBox);

		Handle(SelectMgr_EntityOwner) anOwner = new SelectMgr_EntityOwner(this);
		Handle(Select3D_SensitiveBox) aSensBox = new Select3D_SensitiveBox(anOwner, aBox);
		theSel->Add(aSensBox);
	}

	MDataSource::MDataSource(std::vector<PointXYZ> CoordData, std::vector<std::vector<int>> Ele2NodeData)
	{
		myNodeCoords = new NCollection_HArray2<double>(1, CoordData.size(), 1, 3);
		for (size_t i = 1; i <= CoordData.size(); i++)
		{
			myNodes.Add(i);
			myNodeCoords->SetValue(i, 1, CoordData[i - 1].X);
			myNodeCoords->SetValue(i, 2, CoordData[i - 1].Y);
			myNodeCoords->SetValue(i, 3, CoordData[i - 1].Z);
		}
		myElemNormals = new NCollection_HArray2<double>(1, Ele2NodeData.size(), 1, 3);
		myElemNodes = new NCollection_HArray2<int>(1, Ele2NodeData.size(), 1, 3);

		for (size_t i = 1; i <= Ele2NodeData.size(); i++)
		{
			myElements.Add(i);
			for (int j = 0; j < 3; j++)
			{
				Standard_Integer index = Ele2NodeData[i - 1][j];
				myElemNodes->SetValue(i, j + 1, index);
			}
			myElemNormals->SetValue(i, 1, 0);
			myElemNormals->SetValue(i, 2, 0);
			myElemNormals->SetValue(i, 3, 1);
		}
	}

	bool MDataSource::GetGeom(const int ID, const bool IsElement, NCollection_Array1<double>& Coords, int& NbNodes, MeshVS_EntityType& Type) const
	{
		if (IsElement)
		{
			if (ID >= 1 && ID <= myElements.Extent())
			{
				Type = MeshVS_ET_Face;
				NbNodes = 3;

				for (Standard_Integer i = 1, k = 1; i <= 3; i++)
				{
					Standard_Integer IdxNode = myElemNodes->Value(ID, i);
					for (Standard_Integer j = 1; j <= 3; j++, k++)
						Coords(k) = myNodeCoords->Value(IdxNode, j);
				}

				return Standard_True;
			}
			else
				return Standard_False;
		}
		else if (ID >= 1 && ID <= myNodes.Extent())
		{
			Type = MeshVS_ET_Node;
			NbNodes = 1;

			Coords(1) = myNodeCoords->Value(ID, 1);
			Coords(2) = myNodeCoords->Value(ID, 2);
			Coords(3) = myNodeCoords->Value(ID, 3);
			return Standard_True;
		}
		else
			return Standard_False;
	}

	bool MDataSource::GetGeomType(const int ID, const bool IsElement, MeshVS_EntityType& Type) const
	{
		if (IsElement)
		{
			Type = MeshVS_ET_Face;
			return Standard_True;
		}
		else
		{
			Type = MeshVS_ET_Node;
			return Standard_True;
		}
	}

	Standard_Address MDataSource::GetAddr(const int ID, const bool IsElement) const
	{
		return NULL;
	}

	bool MDataSource::GetNodesByElement(const int ID, NCollection_Array1<int>& NodeIDs, int& NbNodes) const
	{
		if (ID >= 1 && ID <= myElements.Extent() && NodeIDs.Length() >= 3)
		{
			Standard_Integer aLow = NodeIDs.Lower();
			NodeIDs(aLow) = myElemNodes->Value(ID, 1);
			NodeIDs(aLow + 1) = myElemNodes->Value(ID, 2);
			NodeIDs(aLow + 2) = myElemNodes->Value(ID, 3);
			return Standard_True;
		}
		return Standard_False;
	}

	const TColStd_PackedMapOfInteger& MDataSource::GetAllNodes() const
	{
		return myNodes;
	}

	const TColStd_PackedMapOfInteger& MDataSource::GetAllElements() const
	{
		return myElements;
	}

	bool MDataSource::GetNormal(const int Id, const int Max, double& nx, double& ny, double& nz) const
	{
		if (Id >= 1 && Id <= myElements.Extent() && Max >= 3)
		{
			nx = myElemNormals->Value(Id, 1);
			ny = myElemNormals->Value(Id, 2);
			nz = myElemNormals->Value(Id, 3);
			return Standard_True;
		}
		else
			return Standard_False;
	}
}