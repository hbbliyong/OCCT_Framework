#pragma once

#include "core/CoreMacro.hpp"
#include <QAbstractItemModel>
#include <QString>

namespace SongYun {

	class Document;
	class CadObject;

	class ProjectModel : public QAbstractItemModel
	{
		Q_OBJECT

	public:
		SONGYUN_API explicit ProjectModel(Document* doc, QObject* parent = nullptr);

		QModelIndex index(int row, int col, const QModelIndex& parent) const override;
		QModelIndex parent(const QModelIndex& index) const override;
		int rowCount(const QModelIndex& parent) const override;
		int columnCount(const QModelIndex& parent) const override;
		QVariant data(const QModelIndex& index, int role) const override;
		QVariant headerData(int section, Qt::Orientation, int role) const override;

		SONGYUN_API void refresh();
		SONGYUN_API QModelIndex indexOf(CadObject* obj) const;
		SONGYUN_API CadObject* objectAt(const QModelIndex& index) const;

	private:
		Document* m_doc = nullptr;
	};

} // namespace SongYun
