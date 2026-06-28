#pragma once

#include "core/CoreMacro.hpp"
#include <QString>
#include <memory>
#include <vector>

namespace SongYun {

	class Project;

	class ProjectManager
	{
	public:
		SONGYUN_API static ProjectManager& Instance();

		SONGYUN_API Project* createProject(const QString& name);
		SONGYUN_API Project* activeProject() const;
		SONGYUN_API void closeProject(Project* p);
		SONGYUN_API const std::vector<std::unique_ptr<Project>>& projects() const;

	private:
		ProjectManager() = default;
		std::vector<std::unique_ptr<Project>> m_projects;
		Project* m_active = nullptr;
	};

} // namespace SongYun
