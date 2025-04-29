#pragma once
#include "std.h"
namespace GDE
{
	template<typename T>
	class LinkingContext
	{
	public:
		static LinkingContext& getInstance();

		void addTemplate(std::string_view templateName);

		const std::string& getTemplateFromId(T id)
		{

		}

		const std::string& getIdFromTemplate(T id)
		{

		}
	private:

		std::map<std::string, T> _templateToIdMap;
		std::map<T, std::string> _itToTemplateMap;
	};
}