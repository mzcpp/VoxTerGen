#ifndef EVENT_DISPATCHER_HPP
#define EVENT_DISPATCHER_HPP

#include "render/events/ChunkEvents.hpp"

#include "threading/ThreadSafeQueue.hpp"

#include <functional>
#include <unordered_map>
#include <vector>
#include <typeindex>
#include <optional>
#include <variant>

template <typename EventType>
class EventDispatcher
{
private:
    using GenericHandler = std::function<void(const EventType&)>;

    std::unordered_map<std::type_index, std::vector<GenericHandler>> handlers_;

public:
    template <typename Event>
    using Handler = std::function<void(const Event&)>;

    template <typename Event>
    void SubscribeEvent(Handler<Event> handler)
    {
        handlers_[std::type_index(typeid(Event))].push_back(
            [handler](const EventType& event)
            {
                handler(std::get<Event>(event));
            }
        );
    }

    void DispatchEvents(ThreadSafeQueue<EventType>& event_queue)
    {
        while (!event_queue.Empty())
        {
            const std::optional<EventType> event_opt = event_queue.TryPop();

            if (!event_opt.has_value())
            {
                continue;
            }

            std::visit(
                [this, &event_opt](const auto& event)
                {
                    const auto it = handlers_.find(std::type_index(typeid(event)));

                    if (it == handlers_.end())
                    {
                        return;
                    }

                    for (const auto& handler : it->second)
                    {
                        handler(*event_opt);
                    }
                }, 
                *event_opt
            );
        }
    }
};

#endif // EVENT_DISPATCHER_HPP