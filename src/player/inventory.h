#ifndef BLOCKGAME_INVENTORY_H
#define BLOCKGAME_INVENTORY_H


class Inventory {
public:
    Inventory();

    static constexpr int MAX_HOTBAR_SLOTS = 10;

    int getSelected() const;

    void setSelected(int index);

private:
    int selected = 0;
};


#endif //BLOCKGAME_INVENTORY_H
