#include "ui/project/ProjectPanel.h"
#include "ui/project/ProjectModel.h"
#include "document/Document.h"
#include "view/View.h"
#include <QTreeView>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QTimer>

namespace SongYun {

	ProjectPanel::ProjectPanel(QWidget* parent)
		: QWidget(parent)
	{
		auto* layout = new QVBoxLayout(this);
		layout->setContentsMargins(0, 0, 0, 0);

		m_view  = new QTreeView(this);
		m_view->setHeaderHidden(true);
		m_view->setSelectionMode(QAbstractItemView::SingleSelection);
		layout->addWidget(m_view);
	}

	void ProjectPanel::setDocument(Document* doc)
	{
		if (m_doc) m_doc->removeObserver(this);

		m_doc = doc;
		delete m_model;
		m_model = new ProjectModel(m_doc, this);
		m_view->setModel(m_model);
		m_view->expandAll();

		if (m_doc) m_doc->addObserver(this);
	}

	void ProjectPanel::setView(View* view)
	{
		m_occView = view;
		if (!m_occView) return;

		// View 选中 → Tree 高亮
		connect(m_occView, &View::selectionChanged, this, [this]() {
			if (!m_occView || !m_model) return;
			auto ids = m_occView->selectedObjectIds();
			if (!ids.empty())
			{
				auto idx = m_model->indexById(ids.front());
				if (idx.isValid())
					m_view->setCurrentIndex(idx);
			}
		});

		// Tree 点击 → AIS 选中
		connect(m_view, &QTreeView::clicked, this, [this](const QModelIndex& idx) {
			if (!m_occView || !m_model) return;
			int id = m_model->objectIdAt(idx);
			if (id < 0) return;
			m_occView->selectObjectById(id);
		});
	}

	void ProjectPanel::highlightById(int id)
	{
		if (!m_model) return;
		auto idx = m_model->indexById(id);
		if (idx.isValid())
			m_view->setCurrentIndex(idx);
	}

	// ============ DocumentObserver ============

	void ProjectPanel::onObjectAdded(int /*id*/, const TopoDS_Shape& /*shape*/)
	{
		// 延迟到下一事件循环，避免 addObject 回调中直接操作 Model 导致 reentry
		QTimer::singleShot(0, this, [this]() {
			if (m_model) {
				m_model->refresh();
				m_view->expandAll();
			}
		});
	}

	void ProjectPanel::onObjectRemoved(int /*id*/)
	{
		QTimer::singleShot(0, this, [this]() {
			if (m_model) {
				m_model->refresh();
				m_view->expandAll();
			}
		});
	}

} // namespace SongYun
