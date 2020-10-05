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


#include <iterator>
#include <algorithm>


namespace thor::detail
{

	// ---------------------------------------------------------------------------------------------------------------------------


	inline ActionNode::~ActionNode()
	{
	}

	// ---------------------------------------------------------------------------------------------------------------------------


	inline EventNode::EventNode()
	: mEvent()
	{
	}

	inline bool EventNode::isActionActive(const EventBuffer& buffer) const
	{
		return containsEvent(buffer);
	}

	inline bool EventNode::isActionActive(const EventBuffer& buffer, ActionResult& out) const
	{
		return filterEvents(buffer, out.eventContainer);
	}

    inline bool EventNode::containsEvent(const EventBuffer& buffer) const
    {
        std::vector<sf::Event> unused;
        return filterEvents(buffer, unused);
    }

    inline bool EventNode::filterEvents(const EventBuffer& buffer, std::vector<sf::Event>& out) const
    {
        // Variable to check if something was actually inserted (don't look at range, it's not filtered yet)
        std::size_t oldSize = out.size();

        // Copy events that fulfill filter criterion to the end of the output vector
        std::copy_if(buffer.getBuffer().cbegin(),
                     buffer.getBuffer().cend(), std::back_inserter(out),
                     [this](const auto &event) {return isEventActive(event);});
        return oldSize != out.size();
    }
	// ---------------------------------------------------------------------------------------------------------------------------


	inline bool RealtimeNode::isActionActive(const EventBuffer& buffer) const
	{
		return buffer.isRealtimeInputEnabled() && isRealtimeActive();
	}

	inline bool RealtimeNode::isActionActive(const EventBuffer& buffer, ActionResult& out) const
	{
		if (isActionActive(buffer))
		{
			++out.nbRealtimeTriggers;
			return true;
		}
		else
		{
			return false;
		}
	}

	// ---------------------------------------------------------------------------------------------------------------------------


	inline RealtimeKeyLeaf::RealtimeKeyLeaf(sf::Keyboard::Key key)
	: RealtimeNode()
	, mKey(key)
	{
	}

	inline bool RealtimeKeyLeaf::isRealtimeActive() const
	{
		return sf::Keyboard::isKeyPressed(mKey);
	}

	// ---------------------------------------------------------------------------------------------------------------------------


	inline EventKeyLeaf::EventKeyLeaf(sf::Keyboard::Key key, bool pressed)
	: EventNode()
	{
		mEvent.type = pressed ? sf::Event::KeyPressed : sf::Event::KeyReleased;
		mEvent.key.code = key;
	}

	inline bool EventKeyLeaf::isEventActive(const sf::Event& event) const
	{
		return event.type == mEvent.type && event.key.code == mEvent.key.code;
	}

	// ---------------------------------------------------------------------------------------------------------------------------


	inline RealtimeMouseLeaf::RealtimeMouseLeaf(sf::Mouse::Button mouseButton)
	: RealtimeNode()
	, mMouseButton(mouseButton)
	{
	}

	inline bool RealtimeMouseLeaf::isRealtimeActive() const
	{
		return sf::Mouse::isButtonPressed(mMouseButton);
	}

	// ---------------------------------------------------------------------------------------------------------------------------


	inline EventMouseLeaf::EventMouseLeaf(sf::Mouse::Button mouseButton, bool pressed)
	: EventNode()
	{
		mEvent.type = pressed ? sf::Event::MouseButtonPressed : sf::Event::MouseButtonReleased;
		mEvent.mouseButton.button = mouseButton;
	}

	inline bool EventMouseLeaf::isEventActive(const sf::Event& event) const
	{
		return event.type == mEvent.type && event.mouseButton.button == mEvent.mouseButton.button;
	}


	// ---------------------------------------------------------------------------------------------------------------------------


	inline MiscEventLeaf::MiscEventLeaf(sf::Event::EventType eventType)
	: EventNode()
	{
		mEvent.type = eventType;
	}

	inline bool MiscEventLeaf::isEventActive(const sf::Event& event) const
	{
		return mEvent.type == event.type;
	}

	// ---------------------------------------------------------------------------------------------------------------------------


	inline CustomEventLeaf::CustomEventLeaf(std::function<bool(const sf::Event&)> filter)
	: mFilter(std::move(filter))
	{
	}

	inline bool CustomEventLeaf::isEventActive(const sf::Event& event) const
	{
		return mFilter(event);
	}

	// ---------------------------------------------------------------------------------------------------------------------------


	inline CustomRealtimeLeaf::CustomRealtimeLeaf(std::function<bool()> filter)
	: mFilter(std::move(filter))
	{
	}

	inline bool CustomRealtimeLeaf::isRealtimeActive() const
	{
		return mFilter();
	}

	// ---------------------------------------------------------------------------------------------------------------------------


	inline OrNode::OrNode(ActionNode::ActionNodePtr lhs,ActionNode::ActionNodePtr rhs)
	: ActionNode()
	, mLhs(std::move(lhs))
	, mRhs(std::move(rhs))
	{
	}

	inline bool OrNode::isActionActive(const EventBuffer& buffer) const
	{
		return mLhs->isActionActive(buffer) || mRhs->isActionActive(buffer);
	}

	inline bool OrNode::isActionActive(const EventBuffer& buffer, ActionResult& out) const
	{
		// Prevent shortcut semantics of logical OR: If first operand is true, the second's vector isn't filled.
		bool lhsValue = mLhs->isActionActive(buffer, out);
		bool rhsValue = mRhs->isActionActive(buffer, out);

		return lhsValue || rhsValue;
	}

	// ---------------------------------------------------------------------------------------------------------------------------


	inline AndNode::AndNode(ActionNode::ActionNodePtr lhs,ActionNode::ActionNodePtr rhs)
	: ActionNode()
	, mLhs(std::move(lhs))
	, mRhs(std::move(rhs))
	{
	}

	inline bool AndNode::isActionActive(const EventBuffer& buffer) const
	{
		return mLhs->isActionActive(buffer) && mRhs->isActionActive(buffer);
	}

	inline bool AndNode::isActionActive(const EventBuffer& buffer, ActionResult& out) const
	{
		// Don't modify out if whole logical AND expression is false -> use temporary result state
		ActionResult tmpResult;

		// If both calls return true, copy events and realtime counter
		if (mLhs->isActionActive(buffer, tmpResult) && mRhs->isActionActive(buffer, tmpResult))
		{
			out.eventContainer.insert(out.eventContainer.end(), tmpResult.eventContainer.begin(), tmpResult.eventContainer.end());
			out.nbRealtimeTriggers += tmpResult.nbRealtimeTriggers;
			return true;
		}
		else
		{
			return false;
		}
	}

	// ---------------------------------------------------------------------------------------------------------------------------


	inline NotNode::NotNode(ActionNode::ActionNodePtr action)
	: ActionNode()
	, mAction(std::move(action))
	{
	}

	inline bool NotNode::isActionActive(const EventBuffer& buffer) const
	{
		return !mAction->isActionActive(buffer);
	}

	inline bool NotNode::isActionActive(const EventBuffer& buffer, ActionResult& out) const
	{
		// Don't modify if action is active -> use temporary result state
		ActionResult tmpResult;

		if (!mAction->isActionActive(buffer, tmpResult))
		{
			out.eventContainer.insert(out.eventContainer.end(), tmpResult.eventContainer.begin(), tmpResult.eventContainer.end());
			out.nbRealtimeTriggers += tmpResult.nbRealtimeTriggers;
			return true;
		}
		else
		{
			return false;
		}
	}

} // namespace thor::detail
