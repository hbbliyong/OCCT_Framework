#include "cmds/CreatePolylineCommand.h"

#include <AIS_Shape.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <TopoDS_Shape.hxx>
#include <gp_Pnt.hxx>
#include "app/App.h"
#include "selection/SelectionManager.h"
#include "document/DocumentManager.h"
#include <BRepPrimAPI_MakeCylinder.hxx>
namespace Samples
{
	bool CreatePolylineCommand::execute()
	{

		auto result = context().selectionManager().PickPoint("Select a point on the screen:");
		if (result.has_value()) {
			const TopoDS_Shape cylinder = BRepPrimAPI_MakeCylinder(gp_Ax2(result.value(), gp_Dir(0, 0, 1)), 20.0, 120.0).Shape();
			this->context().documentManager().activeDocument()->addObject(cylinder);
		}

		return false;
	}

	bool CreatePolylineCommand::undo()
	{
		if (myPolyline.IsNull())
			return false;

		cleanupMarkers();
		myContext->Remove(myPolyline, false);
		myContext->UpdateCurrentViewer();
		myPolyline.Nullify();
		myPoints.clear();
		myExecuted = false;

		if (myStatusCallback)
			myStatusCallback("Polyline creation undone.");

		return true;
	}

	void CreatePolylineCommand::onPickedPoint(const gp_Pnt& point)
	{
		const TopoDS_Shape markerShape = BRepPrimAPI_MakeSphere(point, 2.0).Shape();
		Handle(AIS_Shape) marker = new AIS_Shape(markerShape);
		myContext->Display(marker, Standard_True);
		myMarkers.push_back(marker);
		myPoints.push_back(point);

		if (myStatusCallback)
		{
			myStatusCallback(QString("Added point %1. Right-click to finish.").arg(myPoints.size()));
		}
	}

	void CreatePolylineCommand::onFinish()
	{
		if (myPoints.size() < 2)
		{
			if (myStatusCallback)
			{
				myStatusCallback("Need at least two points to create a polyline.");
			}
			cleanupMarkers();
			myPoints.clear();
			return;
		}

		buildPolyline();
	}

	void CreatePolylineCommand::buildPolyline()
	{
		BRepBuilderAPI_MakeWire wireMaker;
		for (int i = 1; i < static_cast<int>(myPoints.size()); ++i)
		{
			wireMaker.Add(BRepBuilderAPI_MakeEdge(myPoints[i - 1], myPoints[i]));
		}

		const TopoDS_Shape wireShape = wireMaker.Wire();
		myPolyline = new AIS_Shape(wireShape);
		myContext->Display(myPolyline, Standard_True);
		cleanupMarkers();
		myPoints.clear();
		myExecuted = true;

		if (myStatusCallback)
		{
			myStatusCallback("Polyline created.");
		}
	}

	void CreatePolylineCommand::cleanupMarkers()
	{
		for (auto& marker : myMarkers)
		{
			if (!marker.IsNull())
			{
				myContext->Remove(marker, false);
			}
		}
		myMarkers.clear();
		myContext->UpdateCurrentViewer();
	}

	REGISTER_COMMAND(CreatePolylineCommand, "Samples.CreatePolyline");
}