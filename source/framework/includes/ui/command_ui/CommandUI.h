#pragma once

#include "core/CoreMacro.hpp"
#include "UISection.h"
#include <vector>
#include <QString>

namespace SongYun {

	/// @brief 命令 UI DSL 入口 — 命令通过 buildUI(CommandUI&) 声明参数面板
	class CommandUI
	{
	public:
		UISection& group(const QString& name)
		{
			m_sections.emplace_back(name);
			return m_sections.back();
		}

		const std::vector<UISection>& sections() const { return m_sections; }

	private:
		std::vector<UISection> m_sections;
	};

} // namespace SongYun
