#include "ui/project/ProjectModel.h"
#include "document/Document.h"

namespace SongYun {

	ProjectModel::ProjectModel(Document* doc, QObject* parent)
		: QAbstractItemModel(parent), m_doc(doc) {}

	// ---- 索引 ----

	QModelIndex ProjectModel::index(int row, int col, const QModelIndex& parent) const
	{
		if (!m_doc || !hasIndex(row, col, parent)) return {};
		if (!parent.isValid())
			return createIndex(row, col, quintptr(-1));        // root：项目名（1 行）
		return createIndex(row, col, quintptr(-2));            // 子：对象列表
	}

	QModelIndex ProjectModel::parent(const QModelIndex& index) const
	{
		if (!index.isValid()) return {};
		// 内部 ID -2 表示对象子节点，父节点是 root(0,0)
		if (static_cast<quintptr>(index.internalId()) == quintptr(-2))
			return createIndex(0, 0, quintptr(-1));
		return {}; // root has no parent
	}

	int ProjectModel::rowCount(const QModelIndex& parent) const
	{
		if (!m_doc) return 0;
		if (!parent.isValid())
			return 1;                            // root → 项目名（1 行）
		if (static_cast<quintptr>(parent.internalId()) == quintptr(-1))
			return m_doc->objectCount();         // 项目名下 → 对象列表
		return 0;
	}

	int ProjectModel::columnCount(const QModelIndex&) const { return 1; }

	QVariant ProjectModel::data(const QModelIndex& index, int role) const
	{
		if (!m_doc || !index.isValid()) return {};
		if (role != Qt::DisplayRole) return {};

		// 根节点 → 项目（文档）名称
		if (static_cast<quintptr>(index.internalId()) == quintptr(-1))
			return QString::fromStdString(m_doc->name());

		// 子节点 → Object #N
		int id = m_doc->objectIdAt(index.row());
		return QString("Object #%1").arg(id);
	}

	QVariant ProjectModel::headerData(int section, Qt::Orientation, int role) const
	{
		if (role == Qt::DisplayRole && section == 0) return "Project";
		return {};
	}

	// ---- 刷新 ----

	void ProjectModel::setActiveDocument(Document* doc)
	{
		beginResetModel();
		m_doc = doc;
		endResetModel();
	}

	void ProjectModel::refresh()
	{
		beginResetModel();
		endResetModel();
	}

	// ---- 查找 ----

	QModelIndex ProjectModel::indexById(int objectId) const
	{
		if (!m_doc) return {};
		for (int i = 0; i < m_doc->objectCount(); ++i)
			if (m_doc->objectIdAt(i) == objectId)
				return createIndex(i, 0, quintptr(-2));
		return {};
	}

	int ProjectModel::objectIdAt(const QModelIndex& index) const
	{
		if (!m_doc || !index.isValid()) return -1;
		if (static_cast<quintptr>(index.internalId()) != quintptr(-2))
			return -1;
		return m_doc->objectIdAt(index.row());
	}

} // namespace SongYun
