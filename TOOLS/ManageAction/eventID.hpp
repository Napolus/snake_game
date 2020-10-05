//
// Created by napolus on 7/29/20.
//

#ifndef SNAKE_GAME_EVENTID_HPP
#define SNAKE_GAME_EVENTID_HPP

#include <unordered_map>


class ID
{
public:
    using EVENT_ID = size_t ;

    template<typename EVENT>
    static EVENT_ID getEventId(EVENT type);

private:
    static EVENT_ID  m_nextEventId;
};

inline ID::EVENT_ID ID::m_nextEventId=0;

template<typename EVENT>
inline ID::EVENT_ID ID::getEventId(EVENT type)
{
    static std::unordered_map<EVENT,EVENT_ID> map_to_id;

    if(map_to_id.find(type)==map_to_id.end())
        map_to_id.insert(std::make_pair(type,m_nextEventId++));

    return map_to_id[type];
}
#endif //SNAKE_GAME_EVENTID_HPP
