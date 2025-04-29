#pragma once
#include "std.h"
namespace GDE
{
	template<typename T>
	class LinkingContext
	{
	public:
		static LinkingContext& getInstance();

		void addTemplate(std::string_view templateName)
		{
			_templateToIdMap.insert({ templateName, _nextAvailableId });
			_nextAvailableId++;
		}
		void removeTemplate(std::string_view templateName);
		{
			//_templateToIdMap.insert({ _nextAvailableId, templateName });
			_nextAvailableId++;
		}
		const std::string& getTemplateFromId(T id)
		{

		}

		const std::string& getIdFromTemplate(T id)
		{

		}
	private:
		T _nextAvailableId = 0;
		std::map<std::string, T> _templateToIdMap;
		std::map<T, std::string> _itToTemplateMap;
	};
}