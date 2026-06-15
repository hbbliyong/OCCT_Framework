#include "document/Document.h"
#include <TDocStd_Document.hxx>
#include <TDocStd_Application.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <TDataStd_Name.hxx>
#include <XCAFApp_Application.hxx>
#include <map>
namespace SongYun {
	struct Document::Impl
	{
		Handle(XCAFApp_Application) app;   // 单例，不用你 new
		Handle(TDocStd_Document) doc;
		Handle(XCAFDoc_ShapeTool) shapeTool;

		int nextId = 1;
		std::map<int, TDF_Label> idToLabel;
	};

	Document::Document(const std::string& name)
		: name_(name), m_impl(std::make_unique<Impl>())
	{
		createNew();
	}

	Document::~Document()
	{}

	const std::string& Document::name() const noexcept
	{
		return name_;
	}

	void Document::setName(const std::string& name)
	{
		name_ = name;
	}

	bool Document::load(const std::string& file)
	{
		m_impl->app = XCAFApp_Application::GetApplication();
		m_impl->app->Open(file.c_str(), m_impl->doc);
		m_impl->shapeTool = XCAFDoc_DocumentTool::ShapeTool(m_impl->doc->Main());
		return !m_impl->doc.IsNull();
	}

	bool Document::save(const std::string& file) const
	{
		if (m_impl->doc.IsNull()) return false;

		PCDM_StoreStatus st = m_impl->app->SaveAs(
			m_impl->doc,
			TCollection_ExtendedString(file.c_str())
		);
		return st == PCDM_SS_OK;
	}

	int Document::addObject(const TopoDS_Shape& shape)
	{
		TDF_Label label = m_impl->shapeTool->NewShape();
		m_impl->shapeTool->SetShape(label, shape);

		int id = m_impl->nextId++;
		m_impl->idToLabel[id] = label;

		return id;
	}

	int Document::createAssembly(const std::vector<int>& children)
	{
		TDF_Label assembly = m_impl->shapeTool->NewShape();

		for (int cid : children)
		{
			auto it = m_impl->idToLabel.find(cid);
			if (it != m_impl->idToLabel.end())
			{
				m_impl->shapeTool->AddComponent(assembly, it->second, TopLoc_Location());
			}
		}

		int id = m_impl->nextId++;
		m_impl->idToLabel[id] = assembly;
		return id;
	}

	void Document::removeObject(const int id)
	{
		auto it = m_impl->idToLabel.find(id);
		if (it == m_impl->idToLabel.end()) return;

		m_impl->shapeTool->RemoveShape(it->second);
		m_impl->idToLabel.erase(it);
	}

	const std::vector<TopoDS_Shape>& Document::objects() const noexcept
	{
		return std::vector<TopoDS_Shape>();
	}
	bool Document::createNew()
	{
		//Handle(TDocStd_Application) app = new TDocStd_Application();
		//m_impl->doc = new TDocStd_Document("BinOcaf");

		//app->NewDocument("MDTV-XCAF", m_impl->doc);
		//m_impl->shapeTool = XCAFDoc_DocumentTool::ShapeTool(m_impl->doc->Main());
		//m_impl->shapeTool->SetAutoNaming(Standard_False);
		//return true;

		m_impl->app = XCAFApp_Application::GetApplication();
		// ↑ 这一步内部已经完成了 Application 的创建 + 格式注册
		//    不需要你手动 BinDrivers::DefineFormat

		m_impl->app->NewDocument("MDTV-XCAF", m_impl->doc);
		m_impl->shapeTool = XCAFDoc_DocumentTool::ShapeTool(m_impl->doc->Main());
		return true;
	}
}