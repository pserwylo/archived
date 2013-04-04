/*
 * LayerCanvas.h
 *
 *  Created on: 12/05/2009
 *      Author: pete
 */

#ifndef LAYERCANVAS_H_
#define LAYERCANVAS_H_

#include <fox/fx.h>
#include "core/fox/FoxOutput.h"

class LayerCanvas : public FXHorizontalFrame
{
public:
	LayerCanvas(DataView* data);
	virtual ~LayerCanvas();

	virtual void update();

	enum
	{
		ID_TOGGLE_VISIBLE = FXHorizontalFrame::ID_LAST,
		ID_LAST
	};

protected:
	LayerCanvas(){};
	FXDECLARE(LayerCanvas);

private:
	DataView* data;
};

#endif /* LAYERCANVAS_H_ */
