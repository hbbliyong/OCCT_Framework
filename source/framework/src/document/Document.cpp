#include "document/Document.h"
#include "document/DocumentObserver.h"
#include "command/Transaction.h"
#include <TDocStd_Document.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <XCAFApp_Application.hxx>
#include <TDataStd_Name.hxx>
#include <TDF_Label.hxx>
#include <map>

namespace SongYun {

	struct Document::Impl
	{
		Handle(XCAFApp_Application) app;
		Handle(TDocStd_Document) doc;
		Handle(XCAFDoc_ShapeTool) shapeTool;
		int nextId = 1;
		std::map<int, TDF_Label> idToLabel;
	};

	Document::Document(const std::string& name)
		: name_(name), m_impl(std::make_unique<Impl>()) { createNew(); }

	Document::~Document() = default;

	const std::string& Document::name() const noexcept { return name_; }
	void Document::setName(const std::string& name) { name_ = name; }

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
		return m_impl->app->SaveAs(m_impl->doc,
			TCollection_ExtendedString(file.c_str())) == PCDM_SS_OK;
	}

	// ================================================================
	// addObject / removeObject
	// ================================================================

	int Document::addObject(const TopoDS_Shape& shape)
	{
		TDF_Label label = m_impl->shapeTool->NewShape();
		m_impl->shapeTool->SetShape(label, shape);

		int id = m_impl->nextId++;
		m_impl->idToLabel[id] = label;

		// 事务记录（正常执行期间；undo/redo 期间 activeTxn 为空）
		if (m_activeTxn)
			m_activeTxn->recordAdd(id, shape);

		notifyAdded(id, shape);
		return id;
	}

	void Document::removeObject(int id)
	{
		auto it = m_impl->idToLabel.find(id);
		if (it == m_impl->idToLabel.end()) return;

		TopoDS_Shape shape;
		m_impl->shapeTool->GetShape(it->second, shape);

		m_impl->shapeTool->RemoveShape(it->second);
		m_impl->idToLabel.erase(it);

		if (m_activeTxn)
			m_activeTxn->recordRemove(id, shape);

		notifyRemoved(id);
	}

	const void* Document::labelById(int id) const
	{
		auto it = m_impl->idToLabel.find(id);
		return (it != m_impl->idToLabel.end()) ? &it->second : nullptr;
	}

	void* Document::shapeTool() const { return m_impl->shapeTool.operator->(); }

	int Document::createAssembly(const std::vector<int>& children)
	{
		TDF_Label assembly = m_impl->shapeTool->NewShape();
		for (int cid : children)
		{
			auto it = m_impl->idToLabel.find(cid);
			if (it != m_impl->idToLabel.end())
				m_impl->shapeTool->AddComponent(assembly, it->second, TopLoc_Location());
		}
		int id = m_impl->nextId++;
		m_impl->idToLabel[id] = assembly;
		return id;
	}

	// ================================================================
	// 观察者
	// ================================================================

	void Document::addObserver(DocumentObserver* obs)    { m_observers.push_back(obs); }
	void Document::removeObserver(DocumentObserver* obs)
	{
		m_observers.erase(std::remove(m_observers.begin(), m_observers.end(), obs), m_observers.end());
	}

	void Document::notifyAdded(int id, const TopoDS_Shape& shape)
	{
		for (auto* obs : m_observers) obs->onObjectAdded(id, shape);
	}

	void Document::notifyRemoved(int id)
	{
		for (auto* obs : m_observers) obs->onObjectRemoved(id);
	}

	// ================================================================
	// 事务
	// ================================================================

	void Document::setActiveTransaction(Transaction* txn) { m_activeTxn = txn; }
	Transaction* Document::activeTransaction() const      { return m_activeTxn; }

	// ================================================================
	// XCAF
	// ================================================================

	bool Document::createNew()
	{
		m_impl->app = XCAFApp_Application::GetApplication();
		m_impl->app->NewDocument("MDTV-XCAF", m_impl->doc);
		m_impl->shapeTool = XCAFDoc_DocumentTool::ShapeTool(m_impl->doc->Main());
		return true;
	}

} // namespace SongYun
