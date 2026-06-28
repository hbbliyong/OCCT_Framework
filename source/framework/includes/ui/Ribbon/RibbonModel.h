#pragma once

#include "core/CoreMacro.hpp"
#include <QString>
#include <QIcon>
#include <vector>

namespace SongYun {

	struct RibbonItem
	{
		QString text;
		const char* commandId = nullptr;
	};

	struct RibbonGroup
	{
		QString name;
		std::vector<RibbonItem> items;
	};

	struct RibbonPage
	{
		QString name;
		std::vector<RibbonGroup> groups;
	};

	/// @brief Ribbon 数据模型，定义命令在 Ribbon 中的组织方式
	class RibbonModel
	{
	public:
		SONGYUN_API void addPage(const RibbonPage& page);
		SONGYUN_API const std::vector<RibbonPage>& pages() const;

	private:
		std::vector<RibbonPage> m_pages;
	};

} // namespace SongYun
