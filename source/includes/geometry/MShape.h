#pragma once

#include <cmath>
#include <memory>
#include <map>
#include <set>

#include <QString>
#include <qDebug>

#include "TColStd_HArray2OfInteger.hxx"

#include "ProjLib.hxx"
#include "ElSLib.hxx"
#include "IntTools_Tools.hxx"
#include "BRep_Tool.hxx"
#include "BRepTools.hxx"
#include "BRepLib.hxx"
#include "BOPTools_AlgoTools3D.hxx"

#include "gp_Ax2.hxx"
#include "gp_Ax3.hxx"
#include "gp_Quaternion.hxx"
#include "gp_Pnt.hxx"
#include "gp_Circ.hxx"
#include "gp_Pln.hxx"

#include "GCE2d_MakeLine.hxx"
#include "GCE2d_MakeSegment.hxx"
#include "GccAna_Circ2d2TanRad.hxx"
#include "GccEnt.hxx"
#include "GccEnt_QualifiedLin.hxx"
#include "gce_MakeCirc.hxx"
#include "GCPnts_AbscissaPoint.hxx"
#include "GC_MakeSegment.hxx"

#include "Geom2d_Line.hxx"
#include "Geom2d_Circle.hxx"
#include "Geom2d_TrimmedCurve.hxx"
#include "Geom_CartesianPoint.hxx"
#include "Geom_Line.hxx"
#include "Geom_Curve.hxx"
#include "Geom_Circle.hxx"
#include "Geom_Plane.hxx"
#include "Geom_Surface.hxx"
#include "Geom_BezierCurve.hxx"
#include "Geom_BSplineSurface.hxx"
#include "Geom_Axis2Placement.hxx"
#include "GeomAPI_Interpolate.hxx"
#include "GeomAPI_PointsToBSpline.hxx"
#include "GeomAPI_IntCS.hxx"
#include "GeomAPI_ProjectPointOnCurve.hxx"
#include "GeomAPI_ProjectPointOnSurf.hxx"
#include "GeomFill_BSplineCurves.hxx"
#include "GeomAdaptor_Curve.hxx"

#include "TopoDS.hxx"
#include "TopoDS_Edge.hxx"
#include "TopoDS_Shape.hxx"
#include "TopoDS_Face.hxx"
#include "TopExp_Explorer.hxx"
#include "TopAbs.hxx"
#include "TopAbs_ShapeEnum.hxx"

#include "BRepBndLib.hxx"
#include "BRepBuilderAPI_MakeVertex.hxx"
#include "BRepBuilderAPI_MakeEdge.hxx"
#include "BRepBuilderAPI_MakePolygon.hxx"
#include "BRepBuilderAPI_MakeWire.hxx"
#include "BRepBuilderAPI_MakeFace.hxx"
#include "BRepBuilderAPI_MakeShell.hxx"
#include "BRepBuilderAPI_MakeSolid.hxx"
#include "BRepBuilderAPI_Transform.hxx"
#include "BRepBuilderAPI_Copy.hxx"
#include "BRepBuilderAPI_Sewing.hxx"
#include "BRepPrimAPI_MakeBox.hxx"
#include "BRepPrimAPI_MakeCone.hxx"
#include "BRepPrimAPI_MakeSphere.hxx"
#include "BRepPrimAPI_MakeCylinder.hxx"
#include "BRepPrimAPI_MakeRevolution.hxx"
#include "BRepPrimAPI_MakeRevol.hxx"
#include "BRepPrimAPI_MakePrism.hxx"
#include "BRepAlgoAPI_Fuse.hxx"
#include "BRepAlgoAPI_Cut.hxx"
#include "BRepAlgoAPI_Common.hxx"
#include "BRepAlgoAPI_Section.hxx"
#include "BRepAlgoAPI_Splitter.hxx"
#include "BRepOffsetAPI_MakeOffsetShape.hxx"
#include "BRepOffsetAPI_MakeThickSolid.hxx"
#include "BRepOffsetAPI_MakePipe.hxx"
#include "BRepOffsetAPI_MakePipeShell.hxx"
#include "BRepOffsetAPI_ThruSections.hxx"
#include "BRepFilletAPI_MakeFillet.hxx"
#include "BRepFilletAPI_MakeChamfer.hxx"
#include "BRepFilletAPI_MakeFillet2d.hxx"
#include "BRepFeat_MakePrism.hxx"
#include "BRepFeat_MakePipe.hxx"
#include "BRepAlgoAPI_Defeaturing.hxx"
#include "BRepAdaptor_Surface.hxx"
#include "BRepGProp.hxx"
#include "GProp_GProps.hxx"

#include "BOPAlgo_MakePeriodic.hxx"
#include "BOPAlgo_Builder.hxx"
#include "BOPAlgo_Splitter.hxx"
#include "BOPAlgo_BOP.hxx"
#include "BOPAlgo_Section.hxx"
#include "BOPAlgo_BuilderFace.hxx"
#include "BOPAlgo_WireSplitter.hxx"
#include "BOPAlgo_WireEdgeSet.hxx"

#include "HLRBRep_Algo.hxx"
#include "HLRBRep_HLRToShape.hxx"

#include <IMeshData_Status.hxx>
#include <IMeshTools_Parameters.hxx>
#include <BRepMesh_IncrementalMesh.hxx>

// OCCT自带dump函数可打印信息
std::ostream& operator<<(std::ostream& os , const gp_Pnt& pnt);

namespace Tools
{
	double round(double theValue , int digital = 2);

	template <typename T>
	bool isIn(const std::vector<T>& check , const T value)
	{

		return std::find(check.begin() , check.end() , value) != check.end();
	}
}

Handle(Geom_BSplineCurve) createBSplineCurve(std::vector<gp_Pnt>& pnts, int mode = 0);

TopoDS_Shape correctionProfile(TopoDS_Shape& aProfile , gp_Pnt fromPnt , Handle(Geom_Curve) CPath , double toParam);

TopoDS_Wire convertFaceToWire(const TopoDS_Shape& aShape);