#pragma once

#include "core/CoreMacro.hpp"
#include "entity/cadObject.h"
#include <string>
#include <vector>
#include <memory>
#include <TopoDS_Shape.hxx>

namespace SongYun {

	class DocumentObserver;
	class Transaction;

	class Document
	{
	public:
		SONGYUN_API explicit Document(const std::string& name = "Untitled");
		SONGYUN_API virtual ~Document();

		SONGYUN_API const std::string& name() const noexcept;
		SONGYUN_API void setName(const std::string& name);
		SONGYUN_API virtual bool load(const std::string& path);
		SONGYUN_API virtual bool save(const std::string& path) const;

		// ---- 原始形状 ----
		SONGYUN_API int addObject(const TopoDS_Shape& shape);
		SONGYUN_API void removeObject(int id);
		SONGYUN_API const void* labelById(int id) const;
		SONGYUN_API void* shapeTool() const;
		SONGYUN_API int objectCount() const;
		SONGYUN_API int objectIdAt(int index) const;

		// ---- CadObject（参数化、通用） ----
		/// 仅记录，不 recompute；挂载 onDirty 回调
		SONGYUN_API void addCadObject(std::shared_ptr<CadObject> obj);
		/// 批量 recompute 所有脏对象 → 构建 ChangeSet → EventBus 发布
		SONGYUN_API void commit();
		SONGYUN_API const std::vector<std::shared_ptr<CadObject>>& cadObjects() const;
		SONGYUN_API int cadObjCount() const;
		SONGYUN_API CadObject* cadObjAt(int index) const;
		SONGYUN_API int cadObjIndex(CadObject* obj) const;

		// ---- TDF 属性存储（通用，委托给 CadObject 子类） ----
		SONGYUN_API void savePropertiesToTDF() const;
		SONGYUN_API void loadPropertiesFromTDF();

		// ---- 观察者 / 事务 ----
		SONGYUN_API void addObserver(DocumentObserver* obs);
		SONGYUN_API void removeObserver(DocumentObserver* obs);
		SONGYUN_API void setActiveTransaction(Transaction* txn);
		SONGYUN_API Transaction* activeTransaction() const;

	private:
		bool createNew();
		void notifyAdded(int id, const TopoDS_Shape& shape);
		void notifyRemoved(int id);

	private:
		std::string m_name;
		std::vector<DocumentObserver*> m_observers;
		Transaction* m_activeTxn = nullptr;

		std::vector<std::shared_ptr<CadObject>> m_cadObjects;
		std::vector<CadObject*>                 m_dirtyCadObjects;

		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};

} // namespace SongYun
