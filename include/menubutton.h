#pragma once
#include <ncurses.h>
#include <string>

#include "consoleuiobject.h"
#include "waiter.h"
// #include "popuphandler.h"

class PopUpHandler;
class Restaurant;

class MenuButton : public TerminalUIObject
{
protected:
    std::string title;
    bool selected = false;
    PopUpHandler *popUpHandler;

public:
    MenuButton(int height, int width, int yPosition, int xPosition, std::string title, PopUpHandler *popUpHandler, bool selected = false) // not passed by reference due to nature of initialization of buttons
        : TerminalUIObject(height, width, yPosition, xPosition), title(title), selected(selected), popUpHandler(popUpHandler)
    {
    }
    void draw() override
    {
        box(window, 0, 0);
        if (selected)
            wattr_on(window, A_REVERSE, nullptr);
        mvwprintw(window, 1, 1, title.c_str());
        wattr_off(window, A_REVERSE, nullptr);

        wrefresh(window);
    }
    void activate()
    {
        selected = true;
    }

    void deactivate()
    {
        selected = false;
    }

    virtual void pressed() = 0;
};

class CloseButton : public MenuButton
{
public:
    CloseButton(int height, int width, int yPosition, int xPosition, PopUpHandler *popUpHandler, bool selected = false)
        : MenuButton(height, width, yPosition, xPosition, std::string("Close"), popUpHandler, selected)
    {
    }

    void pressed() override;
};

class NewLocalOrderButton : public MenuButton
{
    Restaurant *restaurant;
    Table *table;

public:
    NewLocalOrderButton(int height, int width, int yPosition, int xPosition, PopUpHandler *popUpHandler, Restaurant *restaurant, Table *table, bool selected = false)
        : MenuButton(height, width, yPosition, xPosition, std::string("Crete new Local Order"), popUpHandler, selected), restaurant(restaurant), table(table)
    {
    }

    void pressed() override;
};

class ChangeWaiterButton : public MenuButton
{
    Waiter *waiter;
    Restaurant *restaurant;

public:
    ChangeWaiterButton(int height, int width, int yPosition, int xPosition, PopUpHandler *popUpHandler, Restaurant *restaurant, Waiter *waiter, bool selected = false)
        : MenuButton(height, width, yPosition, xPosition, waiter->toString(), popUpHandler, selected), waiter(waiter), restaurant(restaurant)
    {
    }

    void pressed() override;
};

class OrderItemButton : public MenuButton
{
    OrderItem *orderItem;

public:
    OrderItemButton(int height, int width, int yPosition, int xPosition, PopUpHandler *popUpHandler, OrderItem *orderItem, bool selected = false)
        : MenuButton(height, width, yPosition, xPosition, orderItem->toString(), popUpHandler, selected), orderItem(orderItem)
    {
    }

    void pressed() override;
};

class MenuItemButton : public MenuButton
{
    MenuItem const &menuItem;

public:
    MenuItemButton(int height, int width, int yPosition, int xPosition, PopUpHandler *popUpHandler, MenuItem const &menuItem, bool selected = false)
        : MenuButton(height, width, yPosition, xPosition, menuItem.name, popUpHandler, selected), menuItem(menuItem)
    {
    }

    void pressed() override;
};

class AddOrderItemButton : public MenuButton // button used to bring up menu, to add order item to order
{
    Order *order;

public:
    AddOrderItemButton(int height, int width, int yPosition, int xPosition, PopUpHandler *popUpHandler, Order *order, bool selected = false)
        : MenuButton(height, width, yPosition, xPosition, "Add item to order", popUpHandler, selected), order(order)
    {
    }
    void pressed() override;
};

class AddOrderItemToOrderButton : public MenuButton // represents specyfic menu item, when clicked it will be added to order
{
    Order *order;
    MenuItem const &menuItem;

public:
    AddOrderItemToOrderButton(int height, int width, int yPosition, int xPosition, PopUpHandler *popUpHandler, Order *order, MenuItem const &menuItem, bool selected = false)
        : MenuButton(height, width, yPosition, xPosition, menuItem.name, popUpHandler, selected), order(order), menuItem(menuItem)
    {
    }

    void pressed() override;
};

class CloseOrderButton : public MenuButton
{
    Order *order;

public:
    CloseOrderButton(int height, int width, int yPosition, int xPosition, PopUpHandler *popUpHandler, Order *order, bool selected = false)
        : MenuButton(height, width, yPosition, xPosition, "Close order", popUpHandler, selected), order(order)
    {
    }
    void pressed() override;
};
