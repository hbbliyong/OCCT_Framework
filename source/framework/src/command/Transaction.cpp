#include "command/Transaction.h"
#include "document/Document.h"

namespace SongYun {

	Transaction::Transaction(Document* doc) : m_doc(doc) {}
	Transaction::~Transaction() = default;

	void Transaction::recordAdd(int id, const TopoDS_Shape& shape)
	{
		m_ops.push_back({ Op::Add, id, shape });
	}

	void Transaction::recordRemove(int id, const TopoDS_Shape& shape)
	{
		m_ops.push_back({ Op::Remove, id, shape });
	}

	void Transaction::undo()
	{
		for (auto it = m_ops.rbegin(); it != m_ops.rend(); ++it)
		{
			if (it->type == Op::Add)
				m_doc->removeObject(it->objectId);
			else
				m_doc->addObject(it->shape);
		}
	}

	void Transaction::redo()
	{
		for (auto& op : m_ops)
		{
			if (op.type == Op::Add)
				m_doc->addObject(op.shape);
			else
				m_doc->removeObject(op.objectId);
		}
	}

	bool Transaction::empty() const { return m_ops.empty(); }

} // namespace SongYun
