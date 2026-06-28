#pragma once

#include "core/CoreMacro.hpp"
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

		/// 添加对象 → XCAF + 通知观察者 + 事务记录
		SONGYUN_API int addObject(const TopoDS_Shape& shape);
		/// 移除对象 → XCAF + 通知观察者
		SONGYUN_API void removeObject(int id);
		/// 根据 ID 获取 Label 指针（View 创建 AIS 用）
		SONGYUN_API const void* labelById(int id) const;
		/// 获取内部 XCAF ShapeTool
		SONGYUN_API void* shapeTool() const;

		/// 对象遍历（ProjectModel 用）
		SONGYUN_API int objectCount() const;
		SONGYUN_API int objectIdAt(int index) const;

		SONGYUN_API int createAssembly(const std::vector<int>& children);

		// 观察者
		SONGYUN_API void addObserver(DocumentObserver* obs);
		SONGYUN_API void removeObserver(DocumentObserver* obs);

		// 事务
		SONGYUN_API void setActiveTransaction(Transaction* txn);
		SONGYUN_API Transaction* activeTransaction() const;

	private:
		bool createNew();
		void notifyAdded(int id, const TopoDS_Shape& shape);
		void notifyRemoved(int id);

	private:
		std::string name_;
		std::vector<DocumentObserver*> m_observers;
		Transaction* m_activeTxn = nullptr;

		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};

} // namespace SongYun
