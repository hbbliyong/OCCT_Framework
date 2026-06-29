#include "document/Document.h"
#include "document/DocumentObserver.h"
#include "command/Transaction.h"
#include "entity/cadObject.h"
#include "common/EventBus.h"
#include "app/App.h"

#include <TDocStd_Document.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <TDataStd_Real.hxx>
#include <TDataStd_Name.hxx>
#include <TDF_ChildIterator.hxx>
#include <TDF_Label.hxx>
#include <XCAFApp_Application.hxx>
#include <map>

namespace SongYun {

	struct Document::Impl
	{
		Handle(XCAFApp_Application) app;
		Handle(TDocStd_Document) doc;
		Handle(XCAFDoc_ShapeTool) shapeTool;
		int nextId = 1;
		std::map<int, TDF_Label> idToLabel;
		TDF_Label cadRoot;
	};

	Document::Document(const std::string& name)
		: m_name(name), m_impl(std::make_unique<Impl>()) { createNew(); }

	Document::~Document() = default;

	const std::string& Document::name() const noexcept { return m_name; }
	void Document::setName(const std::string& name) { m_name = name; }

	bool Document::load(const std::string& file)
	{
		m_impl->app = XCAFApp_Application::GetApplication();
		m_impl->app->Open(file.c_str(), m_impl->doc);
		m_impl->shapeTool = XCAFDoc_DocumentTool::ShapeTool(m_impl->doc->Main());
		m_impl->cadRoot   = m_impl->doc->Main().FindChild(1, Standard_False);
		return !m_impl->doc.IsNull();
	}

	bool Document::save(const std::string& file) const
	{
		if (m_impl->doc.IsNull()) return false;
		return m_impl->app->SaveAs(m_impl->doc,
			TCollection_ExtendedString(file.c_str())) == PCDM_SS_OK;
	}

	// ================================================================
	// 原始形状
	// ================================================================

	int Document::addObject(const TopoDS_Shape& shape)
	{
		TDF_Label label = m_impl->shapeTool->NewShape();
		m_impl->shapeTool->SetShape(label, shape);
		int id = m_impl->nextId++;
		m_impl->idToLabel[id] = label;
		if (m_activeTxn) m_activeTxn->recordAdd(id, shape);
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
		if (m_activeTxn) m_activeTxn->recordRemove(id, shape);
		notifyRemoved(id);
	}

	const void* Document::labelById(int id) const
	{
		auto it = m_impl->idToLabel.find(id);
		return (it != m_impl->idToLabel.end()) ? &it->second : nullptr;
	}

	void* Document::shapeTool() const { return m_impl->shapeTool.operator->(); }
	int  Document::objectCount() const { return static_cast<int>(m_impl->idToLabel.size()); }
	int  Document::objectIdAt(int index) const
	{
		if (index < 0 || index >= objectCount()) return -1;
		auto it = m_impl->idToLabel.begin();
		std::advance(it, index);
		return it->first;
	}

	// ================================================================
	// CadObject — 通用、不依赖具体实体类型
	// ================================================================

	void Document::addCadObject(std::shared_ptr<CadObject> obj)
	{
		if (!obj) return;

		obj->onDirty = [this](CadObject* o) {
			m_dirtyCadObjects.push_back(o);
		};

		m_cadObjects.push_back(obj);

		// 属性可能在 addCadObject 之前已设置（如 line->start.set(p1)）
		// 此时 onDirty 尚未挂载，需手动补充到 dirty 列表
		if (obj->state() != CadObject::State::Clean)
			m_dirtyCadObjects.push_back(obj.get());
	}

	void Document::commit()
	{
		ChangeSet cs;

		for (auto* obj : m_dirtyCadObjects)
		{
			obj->recompute();                              // 子类写入 m_shape
			cs.modified.push_back(obj);
		}
		m_dirtyCadObjects.clear();

		if (!cs.empty())
			App::Instance().eventBus().publish("document.changed", std::move(cs));
	}

	const std::vector<std::shared_ptr<CadObject>>& Document::cadObjects() const
	{
		return m_cadObjects;
	}

	int Document::cadObjCount() const { return static_cast<int>(m_cadObjects.size()); }
	CadObject* Document::cadObjAt(int index) const
	{
		return (index >= 0 && index < cadObjCount()) ? m_cadObjects[index].get() : nullptr;
	}
	int Document::cadObjIndex(CadObject* obj) const
	{
		for (size_t i = 0; i < m_cadObjects.size(); ++i)
			if (m_cadObjects[i].get() == obj) return static_cast<int>(i);
		return -1;
	}

	// ================================================================
	// TDF 属性存储 — 通用，委托给 CadObject 虚函数
	// ================================================================

	void Document::savePropertiesToTDF() const
	{
		for (TDF_ChildIterator it(m_impl->cadRoot); it.More(); it.Next())
			it.Value().ForgetAllAttributes();

		int idx = 1;
		for (auto& obj : m_cadObjects)
		{
			TDF_Label lbl = m_impl->cadRoot.FindChild(idx, Standard_True);
			obj->saveToTDF(&lbl);
			++idx;
		}
	}

	void Document::loadPropertiesFromTDF()
	{
		m_cadObjects.clear();
		m_dirtyCadObjects.clear();

		// 由子类工厂完成（外部注册）
		// 此处只从 TDF 读取并构造
		for (TDF_ChildIterator it(m_impl->cadRoot); it.More(); it.Next())
		{
			TDF_Label lbl = it.Value();
			// 尝试用 TDataStd_Real 读取类型标识
			Handle(TDataStd_Real) typeAttr;
			if (!lbl.FindAttribute(TDataStd_Real::GetID(), typeAttr)) continue;
			int typeInt = static_cast<int>(typeAttr->Get());

			std::shared_ptr<CadObject> obj;
			switch (static_cast<EntityType>(typeInt))
			{
			// 需要注册外部工厂，此处仅保留框架
			default: break;
			}
			if (obj) { obj->loadFromTDF(&lbl); addCadObject(obj); }
		}
	}

	// ================================================================
	// 观察者 / 事务
	// ================================================================

	void Document::addObserver(DocumentObserver* obs) { m_observers.push_back(obs); }
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

	void Document::setActiveTransaction(Transaction* txn) { m_activeTxn = txn; }
	Transaction* Document::activeTransaction() const      { return m_activeTxn; }

	bool Document::createNew()
	{
		m_impl->app = XCAFApp_Application::GetApplication();
		m_impl->app->NewDocument("MDTV-XCAF", m_impl->doc);
		m_impl->shapeTool = XCAFDoc_DocumentTool::ShapeTool(m_impl->doc->Main());
		m_impl->cadRoot = m_impl->doc->Main().NewChild();
		TDataStd_Name::Set(m_impl->cadRoot, TCollection_ExtendedString("CadObjects"));
		return true;
	}

} // namespace SongYun
