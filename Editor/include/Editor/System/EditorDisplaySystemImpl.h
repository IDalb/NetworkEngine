#pragma once
#include <System/DisplaySystemImpl.h>

namespace GDEEditor
{
	class EditorDisplaySystemImpl : public GDE::DisplaySystemImpl
	{
	public:
		void iterate(const GDE::Timing& dt) override;
	private:

	};
}