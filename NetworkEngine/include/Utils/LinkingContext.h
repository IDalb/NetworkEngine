#pragma once
#include "std.h"
namespace GDE
{
	template<typename T>
	class LinkingContext
	{
	public:
		static LinkingContext& getInstance()
		{
			static LinkingContext linkingContext;
			return linkingContext;
		}

		void addTemplate(std::string_view templateName)
		{
			_templateToIdMap.insert({ std::string(templateName), _nextAvailableId });
			_idToTemplateMap.insert({ _nextAvailableId,  std::string(templateName) });
			_nextAvailableId++;
		}
		void removeTemplate(std::string_view templateName)
		{
			if(_templateToIdMap.contains(std::string(templateName)))
			{
				_idToTemplateMap.erase(_templateToIdMap.at(std::string(templateName)));
				_templateToIdMap.erase(std::string(templateName));
			}
		}

		const std::string& getTemplateFromId(T id)
		{
			if (_idToTemplateMap.contains(id))
			{
				return _idToTemplateMap.at(id);
			}
			return _idToTemplateMap.at(0);
		}

		T getIdFromTemplate(const std::string& templateName)
		{
			if (_templateToIdMap.contains(templateName))
			{
				return _templateToIdMap.at(templateName);
			}
			return _templateToIdMap.at("error");
		}
	private:
		LinkingContext()
		{
			addTemplate("error");
		}

		T _nextAvailableId = 0;
		std::map<std::string, T> _templateToIdMap;
		std::map<T, std::string> _idToTemplateMap;
	};
}