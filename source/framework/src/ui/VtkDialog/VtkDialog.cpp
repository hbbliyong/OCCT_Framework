#include "ui/VtkDialog/VtkDialog.h"
#include <QVBoxLayout>
#include <vtkConeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkGenericOpenGLRenderWindow.h>

#include <QVTKOpenGLNativeWidget.h>

#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <TopoDS_Shape.hxx>
#include <IVtkOCC_Shape.hxx>
#include <IVtkTools_ShapeDataSource.hxx>
#include <Geom_CylindricalSurface.hxx>
#include <gp_Ax2.hxx>
#include <gp_Pnt.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <GC_MakeArcOfCircle.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
namespace SongYun {
	TopoDS_Face displaySurface()
	{
		// 圆柱参数
		Standard_Real radius = 50.0;
		Standard_Real height = 100.0;

		// 1. 构建圆柱面
		gp_Ax2 axis(gp_Pnt(0, 0, 0), gp_Dir(0, 0, 1)); // Z 轴为圆柱方向
		Handle(Geom_CylindricalSurface) cyl = new Geom_CylindricalSurface(axis, radius);

		// 2. 构造左右半圆竖边（在圆柱表面）
		// 左半圆弧 U = [-pi/2, pi/2]
		Standard_Real uLeftStart = -M_PI / 2.0;
		Standard_Real uLeftEnd = M_PI / 2.0;
		Handle(Geom_TrimmedCurve) leftEdge =
			new Geom_TrimmedCurve(cyl->UIso(uLeftStart), 0.0, height);

		// 右半圆弧 U = [pi/2, 3*pi/2]
		Standard_Real uRightStart = M_PI / 2.0;
		Standard_Real uRightEnd = 3 * M_PI / 2.0;
		Handle(Geom_TrimmedCurve) rightEdge =
			new Geom_TrimmedCurve(cyl->UIso(uRightStart), 0.0, height);

		// 3. 构造上下水平边（顶边和底边）
		gp_Pnt topStart(cyl->Value(uLeftStart, height));
		gp_Pnt topEnd(cyl->Value(uRightEnd, height));
		TopoDS_Edge topEdge = BRepBuilderAPI_MakeEdge(topStart, topEnd);

		gp_Pnt bottomStart(cyl->Value(uLeftStart, 0.0));
		gp_Pnt bottomEnd(cyl->Value(uRightEnd, 0.0));
		TopoDS_Edge bottomEdge = BRepBuilderAPI_MakeEdge(bottomStart, bottomEnd);

		// 4. 构造 Wire 并闭合
		BRepBuilderAPI_MakeWire wireMaker;
		wireMaker.Add(BRepBuilderAPI_MakeEdge(leftEdge));
		wireMaker.Add(topEdge);
		wireMaker.Add(BRepBuilderAPI_MakeEdge(rightEdge));
		wireMaker.Add(bottomEdge);
		TopoDS_Wire wire = wireMaker.Wire();

		// 5. 构造圆柱面片
		TopoDS_Face face = BRepBuilderAPI_MakeFace(cyl, wire);

		return face;
	}
	VtkDialog::VtkDialog(QWidget* parent) : QDialog(parent)
	{
		// 设置弹窗基本属性
		setWindowTitle("VTK 3D Viewer");
		resize(800, 600);

		// 创建 QVTKOpenGLNativeWidget 实例,创建 Qt 容器
		auto vtkWidget = new QVTKOpenGLNativeWidget(this);

		// 配置布局（关键：必须占满弹窗客户区）
		auto layout = new QVBoxLayout(this);
		layout->setContentsMargins(0, 0, 0, 0); // 清除边距避免黑边
		layout->addWidget(vtkWidget);

		// 初始化 VTK 渲染（见下文）
		this->setupVtkWidget(vtkWidget);
	}

	VtkDialog::~VtkDialog()
	{
		// if (vtkWidget) {
		//	vtkWidget->setRenderWindow(nullptr); // 断开 VTK 渲染窗口与 Qt 控件的关联)
		//	vtkWidget->update(); // 强制刷新 Qt 控件，确保 VTK 资源被正确释放)
		// }
	}

	void VtkDialog::setupVtkWidget(QVTKOpenGLNativeWidget* vtkWidget)
	{
		// 1. 创建 VTK 渲染窗口
		renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
		renderer = vtkSmartPointer<vtkRenderer>::New();
		renderWindow->AddRenderer(renderer);

		// 2.
		vtkWidget->setRenderWindow(renderWindow);

		// 3. 创建一个圆锥体
		auto cone = vtkSmartPointer<vtkConeSource>::New();
		cone->SetHeight(3.0);
		cone->SetRadius(1.0);
		cone->SetResolution(128);

		TopoDS_Shape cylinder = BRepPrimAPI_MakeCylinder(50.0, 100.0);
		TopoDS_Shape cylinder1 = BRepPrimAPI_MakeCylinder(100.0, 50.0);

		double deflection = 0.5;
		BRepMesh_IncrementalMesh mesh(cylinder, deflection);
		mesh.Perform();
		if (!mesh.IsDone())
		{
			std::cerr << "Mesh generation failed!" << std::endl;
		}
		// Step 3: 将 OCCT 形状转换为 VTK PolyData
		// vtkSmartPointer<IVtkOCC_Shape> ivtkShape =
		//	vtkSmartPointer<IVtkOCC_Shape>::New(cylinder);
		// Handle(IVtkOCC_Shape) aShapeImpl = new IVtkOCC_Shape(cylinder);
		Handle(IVtkOCC_Shape) aShapeImpl1 = new IVtkOCC_Shape(displaySurface());
		std::vector<Handle(IVtkOCC_Shape)> shapes = { aShapeImpl1 };
		for (const auto& shape : shapes)
		{
			vtkSmartPointer<IVtkTools_ShapeDataSource> dataSource =
				vtkSmartPointer<IVtkTools_ShapeDataSource>::New();

			dataSource->SetShape(shape);
			dataSource->Update();

			auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();

			mapper->SetInputConnection(dataSource->GetOutputPort());
			auto actor = vtkSmartPointer<vtkActor>::New();
			actor->SetMapper(mapper);

			// 4. 添加到场景
			renderer->AddActor(actor);
		}

		renderer->SetBackground(0.1, 0.2, 0.4);
		renderer->ResetCamera();
	}
}