#include "ui/project/ProjectPanel.h"
#include "ui/project/ProjectModel.h"
#include "document/Document.h"
#include "view/View.h"
#include "common/EventBus.h"
#include "app/App.h"

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

		m_tree = new QTreeView(this);
		m_tree->setHeaderHidden(true);
		m_tree->setSelectionMode(QAbstractItemView::SingleSelection);
		layout->addWidget(m_tree);
	}

	void ProjectPanel::setDocument(Document* doc)
	{
		if (m_doc) m_doc->removeObserver(this);
		m_doc = doc;

		delete m_model;
		m_model = new ProjectModel(m_doc, this);
		m_tree->setModel(m_model);
		m_tree->expandAll();

		if (m_doc) m_doc->addObserver(this);
	}

	void ProjectPanel::setView(View* view)
	{
		m_occView = view;
		if (!m_occView) return;

		// View 选中 → Tree 高亮
		connect(m_occView, &View::selectionChanged, this, [this]() {
			if (!m_occView || !m_model) return;
			auto objs = m_occView->selectedCadObjects();
			if (!objs.empty())
			{
				auto idx = m_model->indexOf(objs.front());
				if (idx.isValid()) m_tree->setCurrentIndex(idx);
			}
		});

		// Tree 点击 → View 选中
		connect(m_tree, &QTreeView::clicked, this, [this](const QModelIndex& idx) {
			if (!m_occView || !m_model) return;
			auto* obj = m_model->objectAt(idx);
			if (obj) m_occView->selectCadObject(obj);
		});

		// Commit → 刷新树
		App::Instance().eventBus().subscribe("document.changed",
			[this](const std::any& data) {
				auto* cs = std::any_cast<ChangeSet>(&data);
				if (cs && !cs->empty())
					QTimer::singleShot(0, this, [this]() { refreshTree(); });
			});
	}

	void ProjectPanel::refreshTree()
	{
		if (m_model) { m_model->refresh(); m_tree->expandAll(); }
	}

	// ---- DocumentObserver（原始 TopoDS 形状） ----
	void ProjectPanel::onObjectAdded(int, const TopoDS_Shape&)
	{
		QTimer::singleShot(0, this, [this]() { refreshTree(); });
	}

	void ProjectPanel::onObjectRemoved(int)
	{
		QTimer::singleShot(0, this, [this]() { refreshTree(); });
	}

} // namespace SongYun
