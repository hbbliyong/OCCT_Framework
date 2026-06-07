#include "geometry/MShape.h"
namespace SongYun
{
	std::ostream& operator<<(std::ostream& os, const gp_Pnt& pnt)
	{
		os << pnt.X() << ", " << pnt.Y() << ", " << pnt.Z();
		return os;
	}

	double Tools::round(double theValue, int digital)
	{
		if (digital < 0)
			digital = 0;
		const double scale = std::pow(10.0, static_cast<double>(digital));
		return std::round(theValue * scale) / scale;
	}

	Handle(Geom_BSplineCurve) createBSplineCurve(std::vector<gp_Pnt>& pnts, int mode)
	{
		int pntSize = pnts.size();
		Handle(NCollection_HArray1<gp_Pnt>) pntsArr = new NCollection_HArray1<gp_Pnt>(1, pntSize);
		for (int i = 0; i < pntSize; ++i)
		{
			TopoDS_Vertex aVertex = BRepBuilderAPI_MakeVertex(pnts[i]);
			pntsArr->SetValue(i + 1, pnts[i]);
		}

		Handle(Geom_BSplineCurve) curve;
		if (mode == 0)
		{
			GeomAPI_Interpolate Interp(pntsArr, false, Precision::Confusion());
			Interp.Perform();
			curve = Interp.Curve();
		}
		else if (mode == 1)
		{
			GeomAPI_PointsToBSpline Approx(pntsArr->Array1());
			curve = Approx.Curve();
		}

		return curve;
	}

	TopoDS_Shape correctionProfile(TopoDS_Shape& aProfile, gp_Pnt fromPnt, Handle(Geom_Curve) CPath, double toParam)
	{
		TopoDS_Face aFace;
		if (aProfile.ShapeType() == TopAbs_FACE)
		{
			aFace = TopoDS::Face(aProfile);
		}
		else if (aProfile.ShapeType() == TopAbs_WIRE)
		{
			aFace = BRepBuilderAPI_MakeFace(TopoDS::Wire(aProfile));
		}

		gp_Pnt toPoint;
		gp_Vec derivative;
		CPath->D1(toParam, toPoint, derivative);
		// Handle(Geom_Surface) surface = BRep_Tool::Surface(aFace);
		BRepAdaptor_Surface surface(aFace);
		gp_Vec direc = surface.Plane().Axis().Direction();

		gp_Trsf T;
		T.SetTranslation(gp_Vec(fromPnt, gp_Pnt(0, 0, 0)));
		BRepBuilderAPI_Transform theTrsf(aProfile, T);

		gp_Quaternion quater(direc, derivative);
		gp_Vec vecTranslate = gp_Vec(gp_Pnt(0, 0, 0), toPoint);
		gp_Trsf TRet;
		TRet.SetTransformation(quater, vecTranslate);
		BRepBuilderAPI_Transform theTrsfRet(theTrsf.Shape(), TRet);

		return theTrsfRet.Shape();
	}

	TopoDS_Wire convertFaceToWire(const TopoDS_Shape& aShape)
	{
		BRepBuilderAPI_MakeWire wireBuilder;
		for (TopExp_Explorer anEdgeExp(aShape, TopAbs_EDGE); anEdgeExp.More(); anEdgeExp.Next())
		{
			const TopoDS_Edge& anEdge = TopoDS::Edge(anEdgeExp.Current());
			wireBuilder.Add(anEdge);
		}
		return wireBuilder.Wire();
	}
}