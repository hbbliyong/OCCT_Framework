#include "view/ViewManager.h"
#include "view/View.h"
#include "document/Document.h"
#include "selection/SelectionManager.h"
#include "app/App.h"

namespace SongYun {

	View* ViewManager::createView(Document* doc, QWidget* parent)
	{
		auto* view = new View(doc, parent);

		m_views.push_back(view);
		if (doc)
			m_docView[doc] = view;

		m_activeView = view;
		App::Instance().commandContext().selectionManager().SetView(view);

		return view;
	}

	void ViewManager::closeView(View* view)
	{
		if (!view) return;

		m_views.erase(std::remove(m_views.begin(), m_views.end(), view), m_views.end());

		for (auto it = m_docView.begin(); it != m_docView.end(); )
		{
			if (it->second == view)
				it = m_docView.erase(it);
			else
				++it;
		}

		if (m_activeView == view)
			m_activeView = m_views.empty() ? nullptr : m_views.front();

		view->deleteLater();
	}

	void ViewManager::setActiveView(View* view) { m_activeView = view; }
	View* ViewManager::activeView() const        { return m_activeView; }

	std::vector<View*> ViewManager::views(Document* doc) const
	{
		std::vector<View*> result;
		auto it = m_docView.find(doc);
		if (it != m_docView.end())
			result.push_back(it->second);
		return result;
	}

	std::vector<View*> ViewManager::allViews() const { return m_views; }

} // namespace SongYun
