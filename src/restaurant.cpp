#include "restaurant.h"

Restaurant::Restaurant()
    : memoryHandler(),
      serverHandler(memoryHandler),
      menu(memoryHandler.fetchMenu()),
      waiters(memoryHandler.fetchWaiters()),
      tables(memoryHandler.fetchTables())
{
    if (menu.empty())
        throw std::runtime_error("Menu cannot be empty");
    if (waiters.empty())
        throw std::runtime_error("Waiters list cannot be empty");

    currentWaiter = &waiters[0];
}

RemoteOrder& Restaurant::newRemoteOrder(Remote& remote)
{
    orders.push_back(std::make_unique<RemoteOrder>(remote));
    return dynamic_cast<RemoteOrder&>(*orders.back().get());
}

LocalOrder& Restaurant::newLocalOrder(Table& table)
{
    orders.push_back(std::make_unique<LocalOrder>(table));
    return dynamic_cast<LocalOrder&>(*orders.back().get());
}

void Restaurant::closeRestaurant()
{
    if (!orders.empty())
        throw std::runtime_error("cannot close restaurant, when some orders are still in progress");
}

void Restaurant::changeCurrentWaiter(Waiter *waiter)
{
    currentWaiter = waiter;
}

Waiter const *Restaurant::getCurrentWaiter() const
{
    return currentWaiter;
}

std::vector<Waiter> const &Restaurant::getWaiters() const
{
    return waiters;
}
std::vector<Table> const &Restaurant::getTables() const
{
    return tables;
}
Menu const &Restaurant::getMenu() const
{
    return menu;
}