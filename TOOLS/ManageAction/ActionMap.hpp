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

/// @file
/// @brief Class template thor::ActionMap

#ifndef THOR_ACTIONMAP_HPP
#define THOR_ACTIONMAP_HPP

#include "Action.hpp"
#include "ActionContext.hpp"
#include "ActionOperations.hpp"
#include "EventManager.hpp"
#include "EventBuffer.hpp"


#include <map>
#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>


namespace thor
{


/// @addtogroup Input
/// @{

/// @brief Class template that associates identifiers with dynamic actions
/// @details You can use this template to map identifiers like strings or enumerators to a specific combination of SFML events
///  (sf::Event) and real time input states (sf::Mouse, sf::Keyboard, sf::Joystick). After the initialization, isActive() provides
///  an easy way to check whether a specified identifier is associated with an active action. Furthermore, it is possible to map
///  the actions to callbacks, which can be achieved with InvokeCallbacks().
/// @tparam ActionId The type of ID you want to map to actions. This can be a string, an enum, or anything with a < operator
///  and value semantics.
template <typename ActionId>
class ActionMap : private sf::NonCopyable
{

	// ---------------------------------------------------------------------------------------------------------------------------
	// Public member functions
	public:
		/// @brief Default constructor
		/// 
									ActionMap(EventManager::EventManagerPtr manager);

		/// @brief Move constructor
		/// 
									ActionMap(ActionMap&& source);

		/// @brief Move assignment operator
		/// 
		ActionMap&					operator= (ActionMap&& source);

		/// @brief Returns the action associated with a given action identifier.
		/// @details Use this function to create new associations. If the action id hasn't been stored yet, it is inserted and
		///  an empty action is returned. Assign an object of type thor::Action to it. Example:
		/// @code
		/// thor::ActionMap<std::string> map(...);
		/// map["run"] = thor::Action(sf::Keyboard::R);
		/// @endcode
		Action&						operator[] (const ActionId& id);

		/// @brief Remove the action associated with the specified @a id.
		///
		void						removeAction(const ActionId& id);

		/// @brief Removes all actions associated with this map.
		///
		void						clearActions();

		/// @brief Returns whether the action associated with the specified identifier is currently in effect.
		/// @details To be in effect, the boolean operation of the assigned action must yield true. Note that in contrast
		///  to registered callbacks, isActive() doesn't take into account the situation where multiple events of the same
		///  type occur in a single frame.
		/// @code
		/// // If LCtrl+A or B is pressed, the action is active.
		/// thor::Action(sf::Keyboard::A) && thor::Action(sf::Keyboard::LCtrl) || thor::Action(sf::Keyboard::B);
		/// @endcode
		bool						isActive(const ActionId& id,thor::detail::EventBuffer& buffer) const;

		void                         filtreEvents(thor::detail::EventBuffer& buffer);

	// ---------------------------------------------------------------------------------------------------------------------------
	// Private types
	private:
		using Map = std::map<ID::EVENT_ID  , Action>;


	// ---------------------------------------------------------------------------------------------------------------------------
	// Private variables
	private:
		Map							                            mActionMap;
		EventManager::EventManagerPtr                           eventManagerPtr;
};

/// @}

} // namespace thor

#include "ActionMap.inl"
#endif // THOR_ACTIONMAP_HPP
