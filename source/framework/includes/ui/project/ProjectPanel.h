#pragma once

#include "core/CoreMacro.hpp"
#include "document/DocumentObserver.h"
#include <QWidget>

class QTreeView;

namespace SongYun {

	class ProjectModel;
	class Document;
	class View;

	class ProjectPanel : public QWidget, public DocumentObserver
	{
		Q_OBJECT
	public:
		SONGYUN_API explicit ProjectPanel(QWidget* parent = nullptr);
		SONGYUN_API void setDocument(Document* doc);

		/// 关联 View 以实现双向选择
		SONGYUN_API void setView(View* view);

		/// 高亮指定 ID 的对象
		SONGYUN_API void highlightById(int id);

		// DocumentObserver
		void onObjectAdded(int id, const TopoDS_Shape& shape) override;
		void onObjectRemoved(int id) override;

	private:
		QTreeView*    m_view  = nullptr;
		ProjectModel* m_model = nullptr;
		Document*     m_doc   = nullptr;
		View*         m_occView = nullptr;
	};

} // namespace SongYun
