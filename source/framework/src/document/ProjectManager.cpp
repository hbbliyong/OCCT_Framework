#include "document/ProjectManager.h"
#include "document/Project.h"

namespace SongYun {

	ProjectManager& ProjectManager::Instance()
	{
		static ProjectManager instance;
		return instance;
	}

	Project* ProjectManager::createProject(const QString& name)
	{
		auto p = std::make_unique<Project>(name);
		auto* ptr = p.get();
		m_projects.push_back(std::move(p));
		m_active = ptr;
		return ptr;
	}

	Project* ProjectManager::activeProject() const { return m_active; }

	void ProjectManager::closeProject(Project* p)
	{
		m_projects.erase(
			std::remove_if(m_projects.begin(), m_projects.end(),
				[p](const auto& u) { return u.get() == p; }),
			m_projects.end());
		if (m_active == p)
			m_active = m_projects.empty() ? nullptr : m_projects.front().get();
	}

	const std::vector<std::unique_ptr<Project>>& ProjectManager::projects() const
	{
		return m_projects;
	}

} // namespace SongYun
