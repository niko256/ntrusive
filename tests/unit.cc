#include <gtest/gtest.h>
#include <ntrusive/intrusive.hpp>
#include <vector>

struct DefaultTag {};

struct QueueATag {};

struct QueueBTag {};

struct Item : AutoUnlinkHook<DefaultTag> {
    int value;

    explicit Item(int v) : value(v) {}
};

struct MultiItem : AutoUnlinkHook<QueueATag>,
                   AutoUnlinkHook<QueueBTag> {
    int value;

    explicit MultiItem(int v) : value(v) {}
};

struct NItem : NoLinkHook<DefaultTag> {
    int value;

    explicit NItem(int v) : value(v) {}
};

using ItemList = IntrusiveList<Item, DefaultTag>;

class IntrusiveListTest : public testing::Test {
  protected:
    ItemList list;
    Item a{1}, b{2}, c{3}, d{4};
};

TEST_F(IntrusiveListTest, DFCEmpty) {
    EXPECT_TRUE(list.empty());
}

TEST_F(IntrusiveListTest, PushBackSingle) {
    list.push_back(a);
    EXPECT_FALSE(list.empty());
    EXPECT_EQ(&list.front(), &a);
}

TEST_F(IntrusiveListTest, PushBackOrder) {
    list.push_back(a);
    list.push_back(b);
    list.push_back(c);

    std::vector<int> values;
    for (const auto& item : list) {
        values.push_back(item.value);
    }
    EXPECT_EQ(values, (std::vector{1, 2, 3}));
}

TEST_F(IntrusiveListTest, UnlinkMiddle) {
    list.push_back(a);
    list.push_back(b);
    list.push_back(c);

    b.unlink();
    EXPECT_FALSE(b.is_linked());

    std::vector<int> values;
    for (const auto& item : list) {
        values.push_back(item.value);
    }
    EXPECT_EQ(values, (std::vector{1, 3}));
}

TEST_F(IntrusiveListTest, AutoUnlinkOnDestruction) {
    {
        Item temp{99};
        list.push_back(a);
        list.push_back(temp);
        list.push_back(b);
    }

    std::vector<int> values;
    for (const auto& item : list) {
        values.push_back(item.value);
    }
    EXPECT_EQ(values, (std::vector{1, 2}));
}
