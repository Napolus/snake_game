//
// Created by napolus on 8/15/20.
//

#ifndef SNAKE_GAME_EVENTBUFFER_HPP
#define SNAKE_GAME_EVENTBUFFER_HPP

#include <utility>


#include "ActionContext.hpp"
namespace thor::detail
{

// Class that stores events and provides methods for lookup
// Note: Since there are at most a few dozens of events per frame (at a decent framerate not even that), std::vector and linear search is fine
    class EventBuffer
    {
    public:
        using Buffer = std::vector<sf::Event>;
        EventBuffer() :mEvents(),mRealtimeEnabled(true),mWindow(nullptr)
        {}

        // Modifiers
        void pollEvents(thor::ActionContext::WindowPtr window);

        void pushEvent(const sf::Event &event);

        void clearEvents();

        // Accessors
        bool isRealtimeInputEnabled() const;

        const Buffer&     getBuffer()const;

        thor::ActionContext::WindowPtr getWindowProviderEvent();

    private:
        Buffer mEvents;
        bool mRealtimeEnabled;
        thor::ActionContext::WindowPtr mWindow;
    };

    inline void EventBuffer::pushEvent(const sf::Event& event)
    {
        // Generate realtime actions only if window has the focus. For example, key presses don't invoke callbacks when the
        // window is not focused.
        switch (event.type)
        {
            case sf::Event::GainedFocus:
                mRealtimeEnabled = true;
                break;

            case sf::Event::LostFocus:
                mRealtimeEnabled = false;
                break;
            default:
                break;
        }

        // Store event
        mEvents.push_back(event);
    }

    inline void EventBuffer::clearEvents()
    {
        mEvents.clear();
    }


    inline bool EventBuffer::isRealtimeInputEnabled() const
    {
        return mRealtimeEnabled;
    }

    inline void EventBuffer::pollEvents(thor::ActionContext::WindowPtr window)
    {

        mWindow = std::move(window);
        sf::Event event;

        while (mWindow->pollEvent(event))
        {
            pushEvent(event);
        }

        mRealtimeEnabled=mWindow->hasFocus();

    }

    inline const EventBuffer::Buffer &EventBuffer::getBuffer() const
    {
        return mEvents;
    }

    inline thor::ActionContext::WindowPtr EventBuffer::getWindowProviderEvent()
    {
        return mWindow;
    }
}
#endif //SNAKE_GAME_EVENTBUFFER_HPP
