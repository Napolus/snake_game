//
// Created by napolus on 8/24/20.
//

#include "widget.hpp"

AbstractWidget::AbstractWidget(AbstractWidget::Type type) :m_type(type),m_enabled(true)
{}

AbstractWidget::Type AbstractWidget::getType()
{
    return m_type;
}

void AbstractWidget::setSize(const sf::Vector2f &size)
{
    m_size=size;
}

bool AbstractWidget::isEnabled() const
{
    return m_enabled;
}

void AbstractWidget::setEnabled(bool enabled)
{
    m_enabled=enabled;
}

const std::string &AbstractWidget::getName() const
{
    return m_name;
}

void AbstractWidget::setName(const std::string &name)
{
    m_name=name;
}


