/////////////////////////////////////////////////////////////////////////////////
//
// Thor C++ Library
// Copyright (c) 2011-2015 Jan Haller
// 
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
// 
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////


namespace thor
{

template <typename ActionId>
ActionMap<ActionId>::ActionMap(EventManager::EventManagerPtr manager):eventManagerPtr(manager){}

template <typename ActionId>
ActionMap<ActionId>::ActionMap(ActionMap&& source)
: mActionMap(std::move(source.mActionMap)),
  eventManagerPtr(std::move(source.eventManagerPtr))

{
}

template <typename ActionId>
ActionMap<ActionId>& ActionMap<ActionId>::operator= (ActionMap&& source)
{
	mActionMap = std::move(source.mActionMap);
    eventManagerPtr=std::move(source.eventManagerPtr);
	return *this;
}

template <typename ActionId>
Action& ActionMap<ActionId>::operator[] (const ActionId& id)
{
	return mActionMap[ID::getEventId(id)];
}

template <typename ActionId>
void ActionMap<ActionId>::removeAction(const ActionId& id)
{
	mActionMap.erase(ID::getEventId(id));
}

template <typename ActionId>
void ActionMap<ActionId>::clearActions()
{
	mActionMap.clear();
}

template <typename ActionId>
bool ActionMap<ActionId>::isActive(const ActionId& id,thor::detail::EventBuffer& buffer) const
{
	typename Map::const_iterator action = mActionMap.find(ID::getEventId(id));
	if (action == mActionMap.end())
		return false;

	return action->second.isActive(buffer);
}

template<typename ActionId>
void ActionMap<ActionId>::filtreEvents(thor::detail::EventBuffer& buffer)
{
    for(const auto &pair:mActionMap)
    {
        detail::ActionResult result;
        if(!pair.second.isActive(buffer,result))
            continue;

        for(const sf::Event &event: result.eventContainer)
            eventManagerPtr->SendEvent(thor::ActionContext(pair.first,buffer.getWindowProviderEvent(),std::make_shared<sf::Event>(event)));

        if(result.nbRealtimeTriggers>0 && result.eventContainer.empty())
            eventManagerPtr->SendEvent(thor::ActionContext(pair.first,buffer.getWindowProviderEvent()));
    }
}
} // namespace thor
