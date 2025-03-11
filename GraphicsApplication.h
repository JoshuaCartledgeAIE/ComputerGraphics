#pragma once
#include "Application.h"

class GraphicsApplication : public Application
{
public:
	virtual bool startup() override;
	virtual bool update() override;
	virtual void draw() override;
	virtual void shutdown() override;

protected:
	const int windowWidth = 1280;
	const int windowHeight = 720;
};

