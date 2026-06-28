#include "ui/ribbon/RibbonModel.h"

namespace SongYun {

	void RibbonModel::addPage(const RibbonPage& page) { m_pages.push_back(page); }
	const std::vector<RibbonPage>& RibbonModel::pages() const { return m_pages; }

} // namespace SongYun
