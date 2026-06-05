#include "command/commands/CreateCylinderCommand.h"

#include <AIS_Shape.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <gp_Ax2.hxx>
#include <gp_Dir.hxx>
#include <TopoDS_Shape.hxx>

CreateCylinderCommand::CreateCylinderCommand(const Handle(AIS_InteractiveContext) & context,
                                             std::function<void(const QString &)> statusCallback)
    : myContext(context), myStatusCallback(std::move(statusCallback))
{
}

bool CreateCylinderCommand::execute()
{
    if (myExecuted)
    {
        if (!myCylinder.IsNull())
        {
            myContext->Display(myCylinder, Standard_True);
            if (myStatusCallback)
                myStatusCallback("Cylinder created.");
            return true;
        }
        return false;
    }

    if (myStatusCallback)
        myStatusCallback("Click in the view to place the cylinder center.");
    return true;
}

bool CreateCylinderCommand::undo()
{
    if (myCylinder.IsNull())
        return false;

    myContext->Remove(myCylinder, false);
    myContext->UpdateCurrentViewer();
    myCylinder.Nullify();

    if (myStatusCallback)
        myStatusCallback("Cylinder creation undone.");

    return true;
}

void CreateCylinderCommand::onPickedPoint(const gp_Pnt &point)
{
    myPickedPoint = point;
    const TopoDS_Shape cylinder = BRepPrimAPI_MakeCylinder(gp_Ax2(point, gp_Dir(0, 0, 1)), 20.0, 120.0).Shape();
    myCylinder = new AIS_Shape(cylinder);
    myContext->Display(myCylinder, Standard_True);
    myExecuted = true;

    if (myStatusCallback)
        myStatusCallback("Cylinder created.");
}
