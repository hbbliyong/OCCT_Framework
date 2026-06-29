#pragma once

#include "core/CoreMacro.hpp"
#include "document/DocumentObserver.h"
#include <QWidget>

class QTreeView;

namespace SongYun {

	class ProjectModel;
	class Document;
	class View;
	class CadObject;

	class ProjectPanel : public QWidget, public DocumentObserver
	{
		Q_OBJECT
	public:
		SONGYUN_API explicit ProjectPanel(QWidget* parent = nullptr);
		SONGYUN_API void setDocument(Document* doc);
		SONGYUN_API void setView(View* view);

		// DocumentObserver（原始形状仍用）
		void onObjectAdded(int id, const TopoDS_Shape& shape) override;
		void onObjectRemoved(int id) override;

	private:
		void refreshTree();

		QTreeView*    m_tree     = nullptr;
		ProjectModel* m_model    = nullptr;
		Document*     m_doc      = nullptr;
		View*         m_occView  = nullptr;
	};

} // namespace SongYun
