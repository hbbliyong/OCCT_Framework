#ifndef OCCTWINDOW_H
#define OCCTWINDOW_H

#include "core/CoreMacro.hpp"
// Qt headers
#include <QWidget>

// occ headers
#include <Aspect_Window.hxx>

namespace SongYun {

class OcctWindow : public Aspect_Window
{
	DEFINE_STANDARD_RTTIEXT(OcctWindow, Aspect_Window)

public:

	// constructor
	SONGYUN_API OcctWindow(QWidget* parent = nullptr, const Quantity_NameOfColor theBackColor = Quantity_NOC_MATRAGRAY);

	// destructor
	virtual SONGYUN_API ~OcctWindow() { _myWidget = nullptr; }

	SONGYUN_API void setDPR(double theDPR);

	// Returns native Window handle
	virtual SONGYUN_API Aspect_Drawable NativeHandle() const override;

	// Returns parent of native Window handle.
	virtual SONGYUN_API Aspect_Drawable NativeParentHandle() const override;

	// Applies the resizing to the window <me>
	virtual SONGYUN_API Aspect_TypeOfResize DoResize() override;

	// Returns True if the window <me> is opened
	// and False if the window is closed.
	virtual SONGYUN_API Standard_Boolean IsMapped() const override;

	// Apply the mapping change to the window <me>
	// and returns TRUE if the window is mapped at screen.
	virtual SONGYUN_API Standard_Boolean DoMapping() const override { return Standard_True; }

	// Opens the window <me>.
	virtual SONGYUN_API void Map() const override;

	//! Closes the window <me>.
	virtual SONGYUN_API void Unmap() const override;

	virtual SONGYUN_API void Position(Standard_Integer& theX1,
		Standard_Integer& theY1,
		Standard_Integer& theX2,
		Standard_Integer& theY2
	) const override;

	// Returns the window RATIO equal to the physical
	// WIDTH/HEIGHT dimensions.
	virtual SONGYUN_API Standard_Real Ratio() const override;

	virtual SONGYUN_API void Size(Standard_Integer& theWidth, Standard_Integer& theHeight) const override;

	virtual SONGYUN_API Aspect_FBConfig NativeFBConfig() const override { return NULL; }

protected:
	Standard_Integer _xLeft;
	Standard_Integer _yTop;
	Standard_Integer _xRight;
	Standard_Integer _yBottom;
	double dpr;
	QWidget* _myWidget;
};

} // namespace SongYun

#endif // OCCTWINDOW_H
