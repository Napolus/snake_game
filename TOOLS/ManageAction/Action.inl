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

#include <algorithm>
#include <cassert>


namespace thor
{

inline Action::Action()
: mOperation()
{
}

inline Action::Action(sf::Keyboard::Key key, ActionType action)
: mOperation()
{
	switch (action)
	{
		case Hold:
			mOperation =std::make_shared<detail::RealtimeKeyLeaf>(key);
			break;

		case PressOnce:
		case ReleaseOnce:
			mOperation = std::make_shared<detail::EventKeyLeaf>(key, action == PressOnce);
			break;
	}
}

inline Action::Action(sf::Mouse::Button mouseButton, ActionType action)
: mOperation()
{
	switch (action)
	{
		case Hold:
			mOperation = std::make_shared<detail::RealtimeMouseLeaf>(mouseButton);
			break;

		case PressOnce:
		case ReleaseOnce:
			mOperation = std::make_shared<detail::EventMouseLeaf>(mouseButton, action == PressOnce);
			break;
	}
}


inline Action::Action(sf::Event::EventType eventType)
: mOperation(std::make_shared<detail::MiscEventLeaf>(eventType))
{
}

inline bool Action::isActive(const detail::EventBuffer& buffer) const
{
	return mOperation->isActionActive(buffer);
}

inline bool Action::isActive(const detail::EventBuffer& buffer, detail::ActionResult& out) const
{
	return mOperation->isActionActive(buffer, out);
}

inline Action::Action(detail::ActionNode::ActionNodePtr operation)
{
	mOperation = std::move(operation);
}

// TODO: Wait until aurora::CopiedPtr implements move derived-to-base more efficiently
inline Action operator|| (const Action& lhs, const Action& rhs)
{
	return Action( std::make_shared<detail::OrNode>(lhs.mOperation, rhs.mOperation) );
}

inline Action operator&& (const Action& lhs, const Action& rhs)
{
	return Action( std::make_shared<detail::AndNode>(lhs.mOperation, rhs.mOperation) );
}

inline Action operator! (const Action& action)
{
	return Action( std::make_shared<detail::NotNode>(action.mOperation) );
}

inline Action eventAction(std::function<bool(const sf::Event&)> filter)
{
	return Action( std::make_shared<detail::CustomEventLeaf>(std::move(filter)) );
}

inline Action realtimeAction(std::function<bool()> filter)
{
	return Action( std::make_shared<detail::CustomRealtimeLeaf>(std::move(filter)) );
}

} // namespace thor
