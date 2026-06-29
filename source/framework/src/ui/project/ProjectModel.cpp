#include "ui/project/ProjectModel.h"
#include "document/Document.h"
#include "entity/cadObject.h"

namespace SongYun {

	static QString entityTypeLabel(EntityType t)
	{
		switch (t) {
		case EntityType::Circle:             return "Circle";
		case EntityType::Line:               return "Line";
		case EntityType::Arc:                return "Arc";
		case EntityType::Polyline:           return "Polyline";
		case EntityType::Plane:              return "Plane";
		case EntityType::CylindricalSurface: return "CylSurface";
		case EntityType::ConicalSurface:     return "ConSurface";
		case EntityType::SphericalSurface:   return "SphSurface";
		case EntityType::ToroidalSurface:    return "TorSurface";
		case EntityType::Cylinder:           return "Cylinder";
		case EntityType::Cone:               return "Cone";
		case EntityType::Sphere:             return "Sphere";
		case EntityType::Torus:              return "Torus";
		}
		return "Unknown";
	}

	ProjectModel::ProjectModel(Document* doc, QObject* parent)
		: QAbstractItemModel(parent), m_doc(doc) {}

	QModelIndex ProjectModel::index(int row, int col, const QModelIndex& parent) const
	{
		if (!m_doc || !hasIndex(row, col, parent)) return {};
		if (!parent.isValid())
			return createIndex(row, col, quintptr(-1));     // root：项目名
		return createIndex(row, col, quintptr(-2));         // 子：CadObject 列表
	}

	QModelIndex ProjectModel::parent(const QModelIndex& index) const
	{
		if (!index.isValid()) return {};
		if (static_cast<quintptr>(index.internalId()) == quintptr(-2))
			return createIndex(0, 0, quintptr(-1));
		return {};
	}

	int ProjectModel::rowCount(const QModelIndex& parent) const
	{
		if (!m_doc) return 0;
		if (!parent.isValid())
			return 1;                                // root
		if (static_cast<quintptr>(parent.internalId()) == quintptr(-1))
			return m_doc->cadObjCount();             // CadObject 数量
		return 0;
	}

	int ProjectModel::columnCount(const QModelIndex&) const { return 1; }

	QVariant ProjectModel::data(const QModelIndex& index, int role) const
	{
		if (!m_doc || !index.isValid()) return {};
		if (role != Qt::DisplayRole) return {};

		// root → 项目名称
		if (static_cast<quintptr>(index.internalId()) == quintptr(-1))
			return QString::fromStdString(m_doc->name());

		// 子 → EntityType 名称 + 序号
		auto* obj = m_doc->cadObjAt(index.row());
		if (obj)
			return QString("%1 #%2").arg(entityTypeLabel(obj->entityType())).arg(index.row() + 1);
		return "???";
	}

	QVariant ProjectModel::headerData(int section, Qt::Orientation, int role) const
	{
		if (role == Qt::DisplayRole && section == 0) return "Project";
		return {};
	}

	void ProjectModel::refresh()
	{
		beginResetModel();
		endResetModel();
	}

	QModelIndex ProjectModel::indexOf(CadObject* obj) const
	{
		if (!m_doc) return {};
		int idx = m_doc->cadObjIndex(obj);
		if (idx < 0) return {};
		return createIndex(idx, 0, quintptr(-2));
	}

	CadObject* ProjectModel::objectAt(const QModelIndex& index) const
	{
		if (!m_doc || !index.isValid()) return nullptr;
		if (static_cast<quintptr>(index.internalId()) != quintptr(-2))
			return nullptr;
		return m_doc->cadObjAt(index.row());
	}

} // namespace SongYun
