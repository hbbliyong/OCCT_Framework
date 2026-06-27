#pragma once

#include "core/CoreMacro.hpp"
#include <vector>
#include <map>

class QWidget;

namespace SongYun {

	class View;
	class Document;

	/// @brief 视图管理器 — 跟踪 View-Document 关系，生命周期由 Qt 父子链管理
	class ViewManager
	{
	public:
		SONGYUN_API ViewManager() = default;

		/// 创建视图（parent 由 Qt 管理生命周期）
		SONGYUN_API View* createView(Document* doc, QWidget* parent = nullptr);

		/// 关闭视图
		SONGYUN_API void closeView(View* view);

		SONGYUN_API View* activeView() const;
		SONGYUN_API void setActiveView(View* view);

		SONGYUN_API std::vector<View*> views(Document* doc) const;
		SONGYUN_API std::vector<View*> allViews() const;

	private:
		View* m_activeView = nullptr;
		std::vector<View*> m_views;
		std::map<Document*, View*> m_docView;   // Document → View (一对一)
	};

} // namespace SongYun
