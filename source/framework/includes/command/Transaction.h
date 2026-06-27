#pragma once

#include "core/CoreMacro.hpp"
#include <TopoDS_Shape.hxx>
#include <vector>

namespace SongYun {

	class Document;

	/// @brief 事务 — 记录 Document 操作，支持 undo/redo
	class Transaction
	{
		struct Op
		{
			enum Type : char { Add, Remove };
			Type type;
			int objectId;
			TopoDS_Shape shape;
		};

	public:
		SONGYUN_API explicit Transaction(Document* doc);
		SONGYUN_API ~Transaction();

		SONGYUN_API void recordAdd(int id, const TopoDS_Shape& shape);
		SONGYUN_API void recordRemove(int id, const TopoDS_Shape& shape);

		SONGYUN_API void undo();
		SONGYUN_API void redo();

		SONGYUN_API bool empty() const;

	private:
		Document* m_doc;
		std::vector<Op> m_ops;
	};

} // namespace SongYun
