#pragma once

#include "core/CoreMacro.hpp"
#include "geometry/MShape.h"
#include "selection/MAISOwner.h"

#include <QColor>

#include "V3d_Viewer.hxx"
#include "V3d_View.hxx"
#include "AIS_InteractiveObject.hxx"
#include "AIS_InteractiveContext.hxx"
#include "AIS_Point.hxx"
#include "AIS_Axis.hxx"
#include "AIS_Line.hxx"
#include "AIS_Circle.hxx"
#include "AIS_Plane.hxx"
#include "AIS_PlaneTrihedron.hxx"
#include "AIS_PointCloud.hxx"
#include "AIS_TexturedShape.hxx"
#include "AIS_TextLabel.hxx"

#include "PrsDim_ParallelRelation.hxx"
#include "PrsDim_PerpendicularRelation.hxx"
#include "PrsDim_LengthDimension.hxx"

#include "StdPrs_ShadedShape.hxx"
#include "StdPrs_WFShape.hxx"
#include "StdPrs_ToolTriangulatedShape.hxx"
#include "StdPrs_HLRPolyShape.hxx"

#include "Prs3d_ShadingAspect.hxx"
#include "Prs3d_ArrowAspect.hxx"
#include "Prs3d_Arrow.hxx"
#include "Prs3d_BndBox.hxx"
#include "Prs3d_ToolCylinder.hxx"
#include "Prs3d_ToolDisk.hxx"
#include "Prs3d_PresentationShadow.hxx"

#include "Graphic3d_ArrayOfPoints.hxx"
#include "Graphic3d_Text.hxx"

#include "MeshVS_DataSource.hxx"
#include "MeshVS_Mesh.hxx"
#include "MeshVS_MeshPrsBuilder.hxx"
#include "MeshVS_Drawer.hxx"
#include "MeshVS_DrawerAttribute.hxx"
#include "NCollection_HArray2.hxx"
namespace SongYun
{
	// 添加边界线条
	class MAisDemo : public AIS_InteractiveObject
	{
		DEFINE_STANDARD_RTTI_INLINE(MAisDemo, AIS_InteractiveObject)
	public:
		enum MDispMode
		{
			MDispMode_WireFrame,
			MDispMode_Shaded
		};

		MAisDemo(const TopoDS_Shape& shap, bool indexVisible = false);

		virtual  void Compute(const Handle(PrsMgr_PresentationManager)& thePrsMgr,
			const Handle(Prs3d_Presentation)& thePrs,
			const Standard_Integer theMode) override;

		virtual  void ComputeSelection(const Handle(SelectMgr_Selection)& theSel,
			const Standard_Integer theMode) override;

		virtual  bool AcceptDisplayMode(const Standard_Integer theMode) const override
		{
			return theMode == MDispMode_WireFrame || theMode == MDispMode_Shaded;
		}

		/**
		 * @brief 设置color， 此函数需override , 因为比如point , edge不是设置ShadingAspect
		 * @param theColor
		 */
		virtual  void SetColor(const Quantity_Color& theColor) override;

		/**
		 * @brief 设置线宽
		 * @param theWidth
		 */
		virtual  void SetWidth(const Standard_Real theWidth) override;

		/**
		 * @brief 设置线型
		 * @param theType
		 */
		void setTypeOfLine(Aspect_TypeOfLine theType = Aspect_TOL_SOLID);

		void setColor(const QColor& theColor);

		TopoDS_Shape Shape() const
		{
			return myShape;
		}

		void setShape(TopoDS_Shape& theShape)
		{
			myShape = theShape;
		}

		void setFaceIndex(const Handle(Prs3d_Presentation)& thePrs);

		static  TopAbs_ShapeEnum SelectionType(const Standard_Integer theSelMode)
		{
			switch (theSelMode)
			{
			case 1:
				return TopAbs_VERTEX;
			case 2:
				return TopAbs_EDGE;
			case 3:
				return TopAbs_WIRE;
			case 4:
				return TopAbs_FACE;
			case 5:
				return TopAbs_SHELL;
			case 6:
				return TopAbs_SOLID;
			case 7:
				return TopAbs_COMPSOLID;
			case 8:
				return TopAbs_COMPOUND;
			case 0:
				return TopAbs_SHAPE;
			}
			return TopAbs_SHAPE;
		}

		static  Standard_Integer SelectionMode(const TopAbs_ShapeEnum theShapeType)
		{
			switch (theShapeType)
			{
			case TopAbs_VERTEX:
				return 1;
			case TopAbs_EDGE:
				return 2;
			case TopAbs_WIRE:
				return 3;
			case TopAbs_FACE:
				return 4;
			case TopAbs_SHELL:
				return 5;
			case TopAbs_SOLID:
				return 6;
			case TopAbs_COMPSOLID:
				return 7;
			case TopAbs_COMPOUND:
				return 8;
			case TopAbs_SHAPE:
				return 0;
			}
			return 0;
		}

	private:
		TopoDS_Shape myShape;
		bool faceIndexIsVisible;
	};

	// Presentation builders
	class MAisObject0 : public AIS_InteractiveObject
	{
		DEFINE_STANDARD_RTTI_INLINE(MAisObject0, AIS_InteractiveObject)
	public:
		enum MyDispMode
		{
			MyDispMode_Main = 0,
			MyDispMode_Highlight = 1
		};

		MAisObject0(const TopoDS_Shape& shap);

		virtual void Compute(const Handle(PrsMgr_PresentationManager)& thePrsMgr,
			const Handle(Prs3d_Presentation)& thePrs,
			const Standard_Integer theMode) override;

		virtual void ComputeSelection(const Handle(SelectMgr_Selection)& theSel,
			const Standard_Integer theMode) override {}

		virtual bool AcceptDisplayMode(const Standard_Integer theMode) const override
		{
			return theMode == 0 || theMode == 1;
		}

		const TopoDS_Shape& Shape() const
		{
			return aShape;
		}

		void SetShape(const TopoDS_Shape& theShape)
		{
			aShape = theShape;
		}

	private:
		TopoDS_Shape aShape;
	};

	// Primitive arrays
	class MAisObject1 : public AIS_InteractiveObject
	{
		DEFINE_STANDARD_RTTI_INLINE(MAisObject1, AIS_InteractiveObject)
	public:
		enum MyDispMode
		{
			MyDispMode_Main = 0,
			MyDispMode_Highlight = 1
		};

		MAisObject1(const TopoDS_Shape& shap);

		virtual void Compute(const Handle(PrsMgr_PresentationManager)& thePrsMgr,
			const Handle(Prs3d_Presentation)& thePrs,
			const Standard_Integer theMode) override;

		virtual void ComputeSelection(const Handle(SelectMgr_Selection)& theSel,
			const Standard_Integer theMode) override {}

		virtual bool AcceptDisplayMode(const Standard_Integer theMode) const override
		{
			return theMode == 0 || theMode == 1;
		}

		const TopoDS_Shape& Shape() const
		{
			return aShape;
		}

		void SetShape(const TopoDS_Shape& theShape)
		{
			aShape = theShape;
		}

	private:
		TopoDS_Shape aShape;
	};

	// Primitive aspects
	class MAisObject2 : public AIS_InteractiveObject
	{
		DEFINE_STANDARD_RTTI_INLINE(MAisObject2, AIS_InteractiveObject)
	public:
		enum MyDispMode
		{
			MyDispMode_Main = 0,
			MyDispMode_Highlight = 1
		};

		MAisObject2(const TopoDS_Shape& shap);

		virtual void Compute(const Handle(PrsMgr_PresentationManager)& thePrsMgr,
			const Handle(Prs3d_Presentation)& thePrs,
			const Standard_Integer theMode) override;

		virtual void ComputeSelection(const Handle(SelectMgr_Selection)& theSel,
			const Standard_Integer theMode) override {}

		virtual bool AcceptDisplayMode(const Standard_Integer theMode) const override
		{
			return theMode == 0 || theMode == 1;
		}

		const TopoDS_Shape& Shape() const
		{
			return aShape;
		}

		void SetShape(const TopoDS_Shape& theShape)
		{
			aShape = theShape;
		}

	private:
		TopoDS_Shape aShape;
	};

	// Quadric builders
	class MAisObject3 : public AIS_InteractiveObject
	{
		DEFINE_STANDARD_RTTI_INLINE(MAisObject3, AIS_InteractiveObject)
	public:
		enum MyDispMode
		{
			MyDispMode_Main = 0,
			MyDispMode_Highlight = 1
		};

		MAisObject3(const TopoDS_Shape& shap);

		virtual void Compute(const Handle(PrsMgr_PresentationManager)& thePrsMgr,
			const Handle(Prs3d_Presentation)& thePrs,
			const Standard_Integer theMode) override;

		virtual void ComputeSelection(const Handle(SelectMgr_Selection)& theSel,
			const Standard_Integer theMode) override {}

		virtual bool AcceptDisplayMode(const Standard_Integer theMode) const override
		{
			return theMode == 0 || theMode == 1;
		}

		const TopoDS_Shape& Shape() const
		{
			return aShape;
		}

		void SetShape(const TopoDS_Shape& theShape)
		{
			aShape = theShape;
		}

	private:
		TopoDS_Shape aShape;
	};

	// Computing selection
	class MAisObject4 : public AIS_InteractiveObject
	{
		DEFINE_STANDARD_RTTI_INLINE(MAisObject4, AIS_InteractiveObject)
	public:
		enum MyDispMode
		{
			MyDispMode_Main = 0,
			MyDispMode_Highlight = 1
		};

		MAisObject4(const TopoDS_Shape& shap);

		virtual void Compute(const Handle(PrsMgr_PresentationManager)& thePrsMgr,
			const Handle(Prs3d_Presentation)& thePrs,
			const Standard_Integer theMode) override;

		virtual void ComputeSelection(const Handle(SelectMgr_Selection)& theSel,
			const Standard_Integer theMode) override;

		virtual bool AcceptDisplayMode(const Standard_Integer theMode) const override
		{
			return theMode == 0 || theMode == 1;
		}

		const TopoDS_Shape& Shape() const
		{
			return aShape;
		}

		void SetShape(const TopoDS_Shape& theShape)
		{
			aShape = theShape;
		}

	private:
		TopoDS_Shape aShape;
	};

	// HighlightingSelectionOwner
	class MAisObject5 : public AIS_InteractiveObject
	{
		DEFINE_STANDARD_RTTI_INLINE(MAisObject5, AIS_InteractiveObject)
	public:
		enum MyDispMode
		{
			MyDispMode_Main = 0,
			MyDispMode_Highlight = 1
		};

		MAisObject5(const TopoDS_Shape& shap);

		virtual void Compute(const Handle(PrsMgr_PresentationManager)& thePrsMgr,
			const Handle(Prs3d_Presentation)& thePrs,
			const Standard_Integer theMode) override;

		virtual void ComputeSelection(const Handle(SelectMgr_Selection)& theSel,
			const Standard_Integer theMode) override;

		virtual bool AcceptDisplayMode(const Standard_Integer theMode) const override
		{
			return theMode == 0 || theMode == 1;
		}

		const TopoDS_Shape& Shape() const
		{
			return aShape;
		}

		void SetShape(const TopoDS_Shape& theShape)
		{
			aShape = theShape;
		}

	private:
		TopoDS_Shape aShape;
	};

	// add arrow
	class MAisObject6 : public AIS_InteractiveObject
	{
		DEFINE_STANDARD_RTTI_INLINE(MAisObject6, AIS_InteractiveObject)
	public:
		enum MyDispMode
		{
			MyDispMode_Main = 0,
			MyDispMode_Highlight = 1
		};

		MAisObject6(const TopoDS_Shape& shap);

		virtual void Compute(const Handle(PrsMgr_PresentationManager)& thePrsMgr,
			const Handle(Prs3d_Presentation)& thePrs,
			const Standard_Integer theMode) override;

		virtual void ComputeSelection(const Handle(SelectMgr_Selection)& theSel,
			const Standard_Integer theMode) override;

		virtual bool AcceptDisplayMode(const Standard_Integer theMode) const override
		{
			return theMode == 0 || theMode == 1;
		}

		const TopoDS_Shape& Shape() const
		{
			return aShape;
		}

		void SetShape(const TopoDS_Shape& theShape)
		{
			aShape = theShape;
		}

	private:
		TopoDS_Shape aShape;
	};

	// HLR
	class myPk_IShape : public AIS_InteractiveObject
	{
		DEFINE_STANDARD_RTTI_INLINE(myPk_IShape, AIS_InteractiveObject)
	public:
		myPk_IShape(const TopoDS_Shape& theShape, PrsMgr_TypeOfPresentation3d theType);

		virtual void Compute(const Handle(PrsMgr_PresentationManager)& thePrsMgr,
			const Handle(Prs3d_Presentation)& thePrs,
			const Standard_Integer theMode) override;

		virtual void computeHLR(const Handle(Graphic3d_Camera)& theProjector,
			const Handle(TopLoc_Datum3D)& theTrsf,
			const Handle(Prs3d_Presentation)& thePrs) override;

		virtual void ComputeSelection(const Handle(SelectMgr_Selection)& theSel,
			const Standard_Integer theMode) override {}

		virtual bool AcceptDisplayMode(const Standard_Integer theMode) const override
		{
			return theMode == 0 || theMode == 1;
		}

		virtual void SetColor(const Quantity_Color& theColor) override;

	private:
		TopoDS_Shape myShape;
	};

	// Primitive arrays
	class MAisObject7 : public AIS_InteractiveObject
	{
		DEFINE_STANDARD_RTTI_INLINE(MAisObject7, AIS_InteractiveObject)
	public:
		enum MDispMode
		{
			MDispMode_WireFrame,
			MDispMode_Shaded
		};

		MAisObject7();

		virtual void Compute(const Handle(PrsMgr_PresentationManager)& thePrsMgr,
			const Handle(Prs3d_Presentation)& thePrs,
			const Standard_Integer theMode) override;

		virtual void ComputeSelection(const Handle(SelectMgr_Selection)& theSel,
			const Standard_Integer theMode) override;

		virtual bool AcceptDisplayMode(const Standard_Integer theMode) const override
		{
			return theMode == MDispMode_WireFrame || theMode == MDispMode_Shaded;
		}

	private:
	};

	// Text primitive
	class MAisObject8 : public AIS_InteractiveObject
	{
		DEFINE_STANDARD_RTTI_INLINE(MAisObject8, AIS_InteractiveObject)
	public:
		enum MDispMode
		{
			MDispMode_WireFrame,
			MDispMode_Shaded
		};

		MAisObject8() {};

		virtual void Compute(const Handle(PrsMgr_PresentationManager)& thePrsMgr,
			const Handle(Prs3d_Presentation)& thePrs,
			const Standard_Integer theMode) override;

		virtual void ComputeSelection(const Handle(SelectMgr_Selection)& theSel,
			const Standard_Integer theMode) override {};

		virtual bool AcceptDisplayMode(const Standard_Integer theMode) const override
		{
			return theMode == MDispMode_WireFrame || theMode == MDispMode_Shaded;
		}

	private:
	};

	// Bounding boxes
	class MAisObject9 : public AIS_InteractiveObject
	{
		DEFINE_STANDARD_RTTI_INLINE(MAisObject9, AIS_InteractiveObject)
	public:
		enum MDispMode
		{
			MDispMode_WireFrame,
			MDispMode_Shaded
		};

		MAisObject9(TopoDS_Shape& shap);

		virtual void Compute(const Handle(PrsMgr_PresentationManager)& thePrsMgr,
			const Handle(Prs3d_Presentation)& thePrs,
			const Standard_Integer theMode) override;

		virtual void ComputeSelection(const Handle(SelectMgr_Selection)& theSel,
			const Standard_Integer theMode) override;

		virtual bool AcceptDisplayMode(const Standard_Integer theMode) const override
		{
			return theMode == MDispMode_WireFrame || theMode == MDispMode_Shaded;
		}

	private:
		TopoDS_Shape myShape;
	};

	struct PointXYZ
	{
		double X;
		double Y;
		double Z;
		PointXYZ(double x, double y, double z)
		{
			X = x;
			Y = y;
			Z = z;
		}
	};

	class MDataSource : public MeshVS_DataSource
	{
		DEFINE_STANDARD_RTTI_INLINE(MDataSource, MeshVS_DataSource)

	public:
		MDataSource(std::vector<PointXYZ> CoordData, std::vector<std::vector<int>> Ele2NodeData);

		virtual  bool GetGeom(const int ID, const bool IsElement, NCollection_Array1<double>& Coords, int& NbNodes, MeshVS_EntityType& Type) const override;

		virtual  bool GetGeomType(const int ID, const bool IsElement, MeshVS_EntityType& Type) const override;

		virtual  Standard_Address GetAddr(const int ID, const bool IsElement) const override;

		virtual  bool GetNodesByElement(const int ID, NCollection_Array1<int>& NodeIDs, int& NbNodes) const override;

		virtual  const TColStd_PackedMapOfInteger& GetAllNodes() const override;

		virtual  const TColStd_PackedMapOfInteger& GetAllElements() const override;

		virtual  bool GetNormal(const int Id, const int Max, double& nx, double& ny, double& nz) const override;

	private:
		TColStd_PackedMapOfInteger myNodes;
		TColStd_PackedMapOfInteger myElements;
		Handle(NCollection_HArray2<int>) myElemNodes;
		Handle(NCollection_HArray2<double>) myNodeCoords;
		Handle(NCollection_HArray2<double>) myElemNormals;
	};
}