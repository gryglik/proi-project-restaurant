#include <gtest/gtest.h>
#include <unistd.h>
#include "address.h"
#include "destination.h"
#include "menuitem.h"
#include "orderitem.h"
#include "menu.h"
#include "serverhandler.h"

TEST(AddressTest, create_typical)
{
    Address adr("Olsztyn", "10-555", "Baltycka", "4");
    ASSERT_EQ(adr.str(), "Baltycka 4\n10-555 Olsztyn");
}

TEST(AddressTest, create_with_additional_info)
{
    Address adr("Olsztyn", "10-555", "Baltycka", "4", "Klatka H6");
    ASSERT_EQ(adr.str(), "Baltycka 4\n10-555 Olsztyn\nAI: Klatka H6");
}

TEST(TableTest, create_typical)
{
    Table tbl(Table::Position(3, 5, 0), 4);
    ASSERT_EQ(tbl.position.x, 3);
    ASSERT_EQ(tbl.position.y, 5);
    ASSERT_EQ(tbl.position.level, 0);
    ASSERT_EQ(tbl.seats, 4);
    ASSERT_EQ(tbl.occupied, false);
}

TEST(TableTest, occupy)
{
    Table tbl(Table::Position(3, 5, 0), 4);
    ASSERT_EQ(tbl.occupied, false);
    tbl.occupied = true;
    ASSERT_EQ(tbl.occupied, true);
}

TEST(TableTest, get_typical)
{
    Table tbl(Table::Position(3, 5, 0), 4);
    ASSERT_EQ(&tbl, &(tbl.get()));
}

TEST(RemoteTest, create_typical)
{
    Address adr("Olsztyn", "10-555", "Baltycka", "4", "Klatka H6");
    Remote remote("Elzbieta Kopyto", "123456789", adr);
    ASSERT_EQ(remote.name, "Elzbieta Kopyto");
    ASSERT_EQ(remote.phoneNumber, "123456789");
    ASSERT_EQ(remote.address.city, "Olsztyn");
}

TEST(RemoteTest, get_typical)
{
    Address adr("Olsztyn", "10-555", "Baltycka", "4", "Klatka H6");
    Remote remote("Elzbieta Kopyto", "123456789", adr);
    ASSERT_EQ(&remote, &(remote.get()));
}

TEST(DestinationTest, cast_table)
{
    Table tbl(Table::Position(3, 5, 0), 4);
    Destination &dst = tbl;
    ASSERT_EQ(typeid(dst), typeid(tbl));
    ASSERT_EQ(dynamic_cast<Table &>(dst).position.x, 3);
}

TEST(DestinationTest, cast_remote)
{
    Remote remote("Elzbieta Kopyto", "123456789", Address("Olsztyn", "10-555", "Baltycka", "4", "Klatka H6"));
    Destination &dst = remote;
    ASSERT_EQ(typeid(dst), typeid(remote));
    ASSERT_EQ(dynamic_cast<Remote &>(dst).address.postalCode, "10-555");
}

TEST(DishTest, initialization)
{
    std::string name = "Pierogi";
    std::string description = "Ręcznnie lepione pierogi z mięsem, smaożone na maśle";
    unsigned int price = 1999;
    std::string ingredients = "mięso, mąka, woda, cebula, przyprawy";
    unsigned int volume = 300;
    MenuItem::CATEGORY category = MenuItem::CATEGORY::mainCourse;

    Dish pierogi(name, description, category, price, ingredients, volume);

    ASSERT_EQ(pierogi.name, name);
    ASSERT_EQ(pierogi.description, description);
    ASSERT_EQ(pierogi.price, price);
    ASSERT_EQ(pierogi.category, category);
    ASSERT_EQ(pierogi.ingredients, ingredients);
    ASSERT_EQ(pierogi.unit, MenuItem::UNIT::g);
    ASSERT_EQ(pierogi.volume, volume);
}

TEST(BeverageTest, initialization)
{
    MenuItem::CATEGORY category = MenuItem::CATEGORY::coldBeverage;
    std::string name = "Woda";
    std::string description = "Woda mineralna niegazowana";
    unsigned int price = 299;
    unsigned int alcoholPercentage = 0;
    unsigned int volume = 500;

    Beverage woda(name, description, category, price, alcoholPercentage, volume);

    ASSERT_EQ(woda.name, name);
    ASSERT_EQ(woda.description, description);
    ASSERT_EQ(woda.category, category);
    ASSERT_EQ(woda.price, price);
    ASSERT_EQ(woda.alcoholPercentage, alcoholPercentage);
    ASSERT_EQ(woda.unit, MenuItem::UNIT::ml);
    ASSERT_EQ(woda.volume, volume);
}

TEST(MenuItemTest, cast_dish)
{
    MenuItem::CATEGORY category = MenuItem::CATEGORY::mainCourse;
    Dish pierogi("Pierogi", "Ręcznnie lepione pierogi z mięsem, smaożone na maśle", category, 1999, "mięso, mąka, woda, cebula, przyprawy", 300);
    MenuItem &item = pierogi;
    ASSERT_EQ(typeid(item), typeid(pierogi));
    ASSERT_EQ(dynamic_cast<Dish &>(item).ingredients, "mięso, mąka, woda, cebula, przyprawy");
}

TEST(MenuItemTest, cast_beverage)
{
    MenuItem::CATEGORY category = MenuItem::CATEGORY::coldBeverage;
    Beverage woda("Woda", "Woda mineralna niegazowana", category, 299, 0, 500);
    MenuItem &item = woda;
    ASSERT_EQ(typeid(item), typeid(woda));
    ASSERT_EQ(dynamic_cast<Beverage &>(item).alcoholPercentage, 0);
}

TEST(MenuTest, initialization)
{
    std::vector<std::unique_ptr<MenuItem>> items;

    MenuItem::CATEGORY category1 = MenuItem::CATEGORY::coldBeverage;
    items.push_back(std::make_unique<Beverage>("Coca Cola", "Refreshing cola drink", category1, 199, 0, 330));
    items.push_back(std::make_unique<Beverage>("Orange Juice", "Freshly squeezed orange juice", category1, 299, 0, 250));

    MenuItem::CATEGORY category2 = MenuItem::CATEGORY::pizza;
    MenuItem::CATEGORY category3 = MenuItem::CATEGORY::burger;
    items.push_back(std::make_unique<Dish>("Pizza", "Pizza Neapoletana", category2, 1499, "dough, tomato sauce, cheese, toppings", 500));
    items.push_back(std::make_unique<Dish>("Burger", "Burger with fries and vegetables", category3, 1299, "beef patty, cheese, lettuce, tomato, onion", 300));

    Menu menu(std::move(items));
    ASSERT_EQ(menu.menuItems.size(), 4);
}

TEST(OrderItemTest, create_typical)
{
    Dish pierogi("Pierogi", "Ręcznnie lepione pierogi z mięsem, smaożone na maśle", MenuItem::CATEGORY::mainCourse, 1999, "mięso, mąka, woda, cebula, przyprawy", 300);
    OrderItem orderit1(pierogi, 1);
    ASSERT_EQ(orderit1.menuItem.name, "Pierogi");
    ASSERT_EQ(orderit1.quantity, 1);
    ASSERT_EQ(orderit1.getDiscount(), 0);
    ASSERT_EQ(orderit1.getComment(), "");
    ASSERT_EQ(orderit1.getStatus(), ItemStatus::created);
}

TEST(OrderItemTest, create_with_comment)
{
    Dish pierogi("Pierogi", "Ręcznnie lepione pierogi z mięsem, smaożone na maśle", MenuItem::CATEGORY::mainCourse, 1999, "mięso, mąka, woda, cebula, przyprawy", 300);
    OrderItem orderit1(pierogi, 1, "Bez masla");
    ASSERT_EQ(orderit1.getComment(), "Bez masla");
    std::string com;
    for (int i = 1; i <= 255; i++)
        com += "a";
    OrderItem orderit2(pierogi, 1, com);
    ASSERT_EQ(orderit2.getComment(), com);
}

TEST(OrderItemTest, create_with_discount)
{
    Dish pierogi("Pierogi", "Ręcznnie lepione pierogi z mięsem, smaożone na maśle", MenuItem::CATEGORY::mainCourse, 1999, "mięso, mąka, woda, cebula, przyprawy", 300);
    OrderItem orderit1(pierogi, 1, "", 20);
    ASSERT_EQ(orderit1.getDiscount(), 20);
    OrderItem orderit2(pierogi, 1, "", 100);
    ASSERT_EQ(orderit2.getDiscount(), 100);
}

TEST(OrderItemTest, create_too_long_comment)
{
    Dish pierogi("Pierogi", "Ręcznnie lepione pierogi z mięsem, smaożone na maśle", MenuItem::CATEGORY::mainCourse, 1999, "mięso, mąka, woda, cebula, przyprawy", 300);
    std::string com;
    for (int i = 1; i <= 256; i++)
        com += "a";
    EXPECT_THROW(OrderItem(pierogi, 1, com), std::invalid_argument);
}

TEST(OrderItemTest, create_too_big_discount)
{
    Dish pierogi("Pierogi", "Ręcznnie lepione pierogi z mięsem, smaożone na maśle", MenuItem::CATEGORY::mainCourse, 1999, "mięso, mąka, woda, cebula, przyprawy", 300);
    EXPECT_THROW(OrderItem(pierogi, 1, "", 101), std::invalid_argument);
}

TEST(OrderItemTest, create_zero_quantity)
{
    Dish pierogi("Pierogi", "Ręcznnie lepione pierogi z mięsem, smaożone na maśle", MenuItem::CATEGORY::mainCourse, 1999, "mięso, mąka, woda, cebula, przyprawy", 300);
    EXPECT_THROW(OrderItem(pierogi, 0), std::invalid_argument);
}

TEST(OrderItemTest, addComment_typical)
{
    Dish pierogi("Pierogi", "Ręcznnie lepione pierogi z mięsem, smaożone na maśle", MenuItem::CATEGORY::mainCourse, 1999, "mięso, mąka, woda, cebula, przyprawy", 300);
    OrderItem orderit1(pierogi, 1);
    orderit1.addComment("Połowa porcji");
    ASSERT_EQ(orderit1.getComment(), "Połowa porcji");
    std::string com;
    for (int i = 1; i <= 255; i++)
        com += "a";
    orderit1.addComment(com);
    ASSERT_EQ(orderit1.getComment(), com);
    orderit1.addComment("");
    ASSERT_EQ(orderit1.getComment(), "");
}

TEST(OrderItemTest, addComment_too_long)
{
    Dish pierogi("Pierogi", "Ręcznnie lepione pierogi z mięsem, smaożone na maśle", MenuItem::CATEGORY::mainCourse, 1999, "mięso, mąka, woda, cebula, przyprawy", 300);
    OrderItem orderit1(pierogi, 1);
    std::string com;
    for (int i = 1; i <= 256; i++)
        com += "a";
    EXPECT_THROW(orderit1.addComment(com), std::invalid_argument);
}

TEST(OrderItemTest, addComment_move_typical)
{
    Dish pierogi("Pierogi", "Ręcznnie lepione pierogi z mięsem, smaożone na maśle", MenuItem::CATEGORY::mainCourse, 1999, "mięso, mąka, woda, cebula, przyprawy", 300);
    OrderItem orderit1(pierogi, 1);
    orderit1.addComment(std::move("Połowa porcji"));
    ASSERT_EQ(orderit1.getComment(), "Połowa porcji");
    orderit1.addComment(std::move(""));
    ASSERT_EQ(orderit1.getComment(), "");
}

TEST(OrderItemTest, add_discount_typical)
{
    Dish pierogi("Pierogi", "Ręcznnie lepione pierogi z mięsem, smaożone na maśle", MenuItem::CATEGORY::mainCourse, 1999, "mięso, mąka, woda, cebula, przyprawy", 300);
    OrderItem orderit1(pierogi, 1);
    orderit1.setDiscount(40);
    ASSERT_EQ(orderit1.getDiscount(), 40);
    orderit1.setDiscount(100);
    ASSERT_EQ(orderit1.getDiscount(), 100);
    orderit1.setDiscount(0);
    ASSERT_EQ(orderit1.getDiscount(), 0);
}

TEST(OrderItemTest, add_discount_too_big)
{
    Dish pierogi("Pierogi", "Ręcznnie lepione pierogi z mięsem, smaożone na maśle", MenuItem::CATEGORY::mainCourse, 1999, "mięso, mąka, woda, cebula, przyprawy", 300);
    OrderItem orderit(pierogi, 1);
    EXPECT_THROW(orderit.setDiscount(101), std::invalid_argument);
}

TEST(OrderItemTest, changeStatus_typical)
{
    Dish pierogi("Pierogi", "Ręcznnie lepione pierogi z mięsem, smaożone na maśle", MenuItem::CATEGORY::mainCourse, 1999, "mięso, mąka, woda, cebula, przyprawy", 300);
    OrderItem orderit(pierogi, 1);

    ASSERT_EQ(orderit.getStatus(), ItemStatus::created);

    orderit.changeStatus(ItemStatus::ordered);
    ASSERT_EQ(orderit.getStatus(), ItemStatus::ordered);

    orderit.changeStatus(ItemStatus::inPreparation);
    ASSERT_EQ(orderit.getStatus(), ItemStatus::inPreparation);

    orderit.changeStatus(ItemStatus::readyToDeliver);
    ASSERT_EQ(orderit.getStatus(), ItemStatus::readyToDeliver);

    orderit.changeStatus(ItemStatus::delivered);
    ASSERT_EQ(orderit.getStatus(), ItemStatus::delivered);
}

TEST(OrderItemTest, changeStatus_decrease)
{
    Dish pierogi("Pierogi", "Ręcznnie lepione pierogi z mięsem, smaożone na maśle", MenuItem::CATEGORY::mainCourse, 1999, "mięso, mąka, woda, cebula, przyprawy", 300);
    OrderItem orderit(pierogi, 1);

    orderit.changeStatus(ItemStatus::ordered);
    ASSERT_EQ(orderit.getStatus(), ItemStatus::ordered);
    EXPECT_THROW(orderit.changeStatus(ItemStatus::created), std::runtime_error);

    orderit.changeStatus(ItemStatus::inPreparation);
    ASSERT_EQ(orderit.getStatus(), ItemStatus::inPreparation);
    EXPECT_THROW(orderit.changeStatus(ItemStatus::created), std::runtime_error);
    EXPECT_THROW(orderit.changeStatus(ItemStatus::ordered), std::runtime_error);

    orderit.changeStatus(ItemStatus::readyToDeliver);
    ASSERT_EQ(orderit.getStatus(), ItemStatus::readyToDeliver);
    EXPECT_THROW(orderit.changeStatus(ItemStatus::created), std::runtime_error);
    EXPECT_THROW(orderit.changeStatus(ItemStatus::ordered), std::runtime_error);
    EXPECT_THROW(orderit.changeStatus(ItemStatus::inPreparation), std::runtime_error);

    orderit.changeStatus(ItemStatus::delivered);
    ASSERT_EQ(orderit.getStatus(), ItemStatus::delivered);
    EXPECT_THROW(orderit.changeStatus(ItemStatus::created), std::runtime_error);
    EXPECT_THROW(orderit.changeStatus(ItemStatus::ordered), std::runtime_error);
    EXPECT_THROW(orderit.changeStatus(ItemStatus::inPreparation), std::runtime_error);
    EXPECT_THROW(orderit.changeStatus(ItemStatus::readyToDeliver), std::runtime_error);
    EXPECT_THROW(orderit.changeStatus(ItemStatus::canceled), std::runtime_error);
}

TEST(OrderItemTest, changeStatus_cancel)
{
    Dish pierogi("Pierogi", "Ręcznnie lepione pierogi z mięsem, smaożone na maśle", MenuItem::CATEGORY::mainCourse, 1999, "mięso, mąka, woda, cebula, przyprawy", 300);
    OrderItem orderit(pierogi, 1);

    ASSERT_EQ(orderit.getStatus(), ItemStatus::created);
    orderit.changeStatus(ItemStatus::canceled);
    ASSERT_EQ(orderit.getStatus(), ItemStatus::canceled);

    EXPECT_THROW(orderit.changeStatus(ItemStatus::created), std::runtime_error);
    EXPECT_THROW(orderit.changeStatus(ItemStatus::ordered), std::runtime_error);
    EXPECT_THROW(orderit.changeStatus(ItemStatus::inPreparation), std::runtime_error);
    EXPECT_THROW(orderit.changeStatus(ItemStatus::readyToDeliver), std::runtime_error);
    EXPECT_THROW(orderit.changeStatus(ItemStatus::delivered), std::runtime_error);

    OrderItem orderit2(pierogi, 1);
    orderit2.changeStatus(ItemStatus::ordered);
    orderit2.changeStatus(ItemStatus::canceled);
    ASSERT_EQ(orderit2.getStatus(), ItemStatus::canceled);

    OrderItem orderit3(pierogi, 1);
    orderit3.changeStatus(ItemStatus::inPreparation);
    orderit3.changeStatus(ItemStatus::canceled);
    ASSERT_EQ(orderit3.getStatus(), ItemStatus::canceled);

    OrderItem orderit4(pierogi, 1);
    orderit4.changeStatus(ItemStatus::readyToDeliver);
    orderit4.changeStatus(ItemStatus::canceled);
    ASSERT_EQ(orderit4.getStatus(), ItemStatus::canceled);

    OrderItem orderit5(pierogi, 1);
    orderit5.changeStatus(ItemStatus::delivered);
    EXPECT_THROW(orderit5.changeStatus(ItemStatus::canceled), std::runtime_error);
}

TEST(OrderItemTest, setDelivered_typical)
{
    Dish pierogi("Pierogi", "Ręcznnie lepione pierogi z mięsem, smaożone na maśle", MenuItem::CATEGORY::mainCourse, 1999, "mięso, mąka, woda, cebula, przyprawy", 300);
    OrderItem orderit(pierogi, 1);
    orderit.changeStatus(ItemStatus::readyToDeliver);
    orderit.setDelivered();
    ASSERT_EQ(orderit.getStatus(), ItemStatus::delivered);
}

TEST(OrderItemTest, setDelivered_while_not_ready)
{
    Dish pierogi("Pierogi", "Ręcznnie lepione pierogi z mięsem, smaożone na maśle", MenuItem::CATEGORY::mainCourse, 1999, "mięso, mąka, woda, cebula, przyprawy", 300);
    OrderItem orderit(pierogi, 1);
    ASSERT_EQ(orderit.getStatus(), ItemStatus::created);
    EXPECT_THROW(orderit.setDelivered(), std::runtime_error);
    orderit.changeStatus(ItemStatus::ordered);
    EXPECT_THROW(orderit.setDelivered(), std::runtime_error);
    orderit.changeStatus(ItemStatus::inPreparation);
    EXPECT_THROW(orderit.setDelivered(), std::runtime_error);
}

TEST(OrderItemTest, setCancelled_typical)
{
    Dish pierogi("Pierogi", "Ręcznnie lepione pierogi z mięsem, smaożone na maśle", MenuItem::CATEGORY::mainCourse, 1999, "mięso, mąka, woda, cebula, przyprawy", 300);
    OrderItem orderit1(pierogi, 1);

    ASSERT_EQ(orderit1.getStatus(), ItemStatus::created);
    orderit1.setCancelled();
    ASSERT_EQ(orderit1.getStatus(), ItemStatus::canceled);

    OrderItem orderit2(pierogi, 1);
    orderit2.changeStatus(ItemStatus::ordered);
    orderit2.setCancelled();
    ASSERT_EQ(orderit2.getStatus(), ItemStatus::canceled);
}

TEST(OrderItemTest, setCancelled_after_ordered)
{
    Dish pierogi("Pierogi", "Ręcznnie lepione pierogi z mięsem, smaożone na maśle", MenuItem::CATEGORY::mainCourse, 1999, "mięso, mąka, woda, cebula, przyprawy", 300);
    OrderItem orderit(pierogi, 1);

    orderit.changeStatus(ItemStatus::inPreparation);
    EXPECT_THROW(orderit.setCancelled(), std::runtime_error);

    orderit.changeStatus(ItemStatus::readyToDeliver);
    EXPECT_THROW(orderit.setCancelled(), std::runtime_error);

    orderit.changeStatus(ItemStatus::delivered);
    EXPECT_THROW(orderit.setCancelled(), std::runtime_error);
}

TEST(OrderItemTest, getPrice_typical)
{
    Dish pierogi("Pierogi", "Ręcznnie lepione pierogi z mięsem, smaożone na maśle", MenuItem::CATEGORY::mainCourse, 1999, "mięso, mąka, woda, cebula, przyprawy", 300);
    OrderItem orderit1(pierogi, 1);
    ASSERT_EQ(orderit1.getPrice(), 1999);
    OrderItem orderit2(pierogi, 3);
    ASSERT_EQ(orderit2.getPrice(), 3 * 1999);
}

TEST(OrderItemTest, getPrice_discount_typical)
{
    Dish pierogi("Pierogi", "Ręcznnie lepione pierogi z mięsem, smaożone na maśle", MenuItem::CATEGORY::mainCourse, 1999, "mięso, mąka, woda, cebula, przyprawy", 300);
    OrderItem orderit1(pierogi, 3);
    ASSERT_EQ(orderit1.getPrice(), 3 * 1999);
    orderit1.setDiscount(5);
    ASSERT_EQ(orderit1.getPrice(), 5697);
    orderit1.setDiscount(1);
    ASSERT_EQ(orderit1.getPrice(), 5937);
    orderit1.setDiscount(99);
    ASSERT_EQ(orderit1.getPrice(), 59);
    orderit1.setDiscount(100);
    ASSERT_EQ(orderit1.getPrice(), 0);
}

TEST(OrderItemTest, getPrice_discount_big_price)
{
    Dish pierogi("Pierogi", "Ręcznnie lepione pierogi z mięsem, smaożone na maśle", MenuItem::CATEGORY::mainCourse, 2345678, "mięso, mąka, woda, cebula, przyprawy", 300);
    OrderItem orderit1(pierogi, 7);
    ASSERT_EQ(orderit1.getPrice(), 7 * 2345678);
    orderit1.setDiscount(1);
    ASSERT_EQ(orderit1.getPrice(), 16255548);
    orderit1.setDiscount(13);
    ASSERT_EQ(orderit1.getPrice(), 14285179);
    orderit1.setDiscount(97);
    ASSERT_EQ(orderit1.getPrice(), 492592);
    orderit1.setDiscount(100);
    ASSERT_EQ(orderit1.getPrice(), 0);

    Dish pierogi2("Pierogi", "Ręcznnie lepione pierogi z mięsem, smaożone na maśle", MenuItem::CATEGORY::mainCourse, 42949600, "mięso, mąka, woda, cebula, przyprawy", 300);
    OrderItem orderit2(pierogi2, 1);
    ASSERT_EQ(orderit2.getPrice(), 42949600);
    orderit2.setDiscount(1);
    ASSERT_EQ(orderit2.getPrice(), 42520104);
    orderit2.setDiscount(17);
    ASSERT_EQ(orderit2.getPrice(), 35648168);
    orderit2.setDiscount(99);
    ASSERT_EQ(orderit2.getPrice(), 429496);
    orderit2.setDiscount(100);
    ASSERT_EQ(orderit2.getPrice(), 0);
}

TEST(OrderItemTest, getPrice_discount_too_big_price)
{
    Dish pierogi("Pierogi", "Ręcznnie lepione pierogi z mięsem, smaożone na maśle", MenuItem::CATEGORY::mainCourse, 42949601, "mięso, mąka, woda, cebula, przyprawy", 300);
    OrderItem orderit1(pierogi, 1);
    EXPECT_THROW(orderit1.getPrice(), std::runtime_error);
}

TEST(OrderItemTest, getWaitingTime_typical)
{
    Dish pierogi("Pierogi", "Ręcznnie lepione pierogi z mięsem, smaożone na maśle", MenuItem::CATEGORY::mainCourse, 1999, "mięso, mąka, woda, cebula, przyprawy", 300);
    OrderItem orderit1(pierogi, 1);
    sleep(3);
    ASSERT_EQ(orderit1.getWaitingTime(), 3);
    sleep(2);
    ASSERT_EQ(orderit1.getWaitingTime(), 5);
}

TEST(WaiterOrderItemTest, interface_methods)
{
    Dish pierogi("Pierogi", "Ręcznnie lepione pierogi z mięsem, smaożone na maśle", MenuItem::CATEGORY::mainCourse, 1999, "mięso, mąka, woda, cebula, przyprawy", 300);
    OrderItem orderit1(pierogi, 3);

    WaiterOrderItem &order = orderit1;

    ASSERT_EQ(order.getComment(), "");
    order.addComment("Bez posypki.");
    ASSERT_EQ(order.getComment(), "Bez posypki.");

    ASSERT_EQ(order.getPrice(), 5997);

    ASSERT_EQ(order.getDiscount(), 0);
    order.setDiscount(5);
    ASSERT_EQ(order.getDiscount(), 5);

    ASSERT_EQ(order.getPrice(), 5697);

    sleep(1);
    ASSERT_EQ(order.getWaitingTime(), 1);

    ASSERT_EQ(order.getStatus(), ItemStatus::created);
    order.setCancelled();
    ASSERT_EQ(order.getStatus(), ItemStatus::canceled);
}
