#include "Item.h"
#include <iostream>


// Definition of toLowerCase function
string toLowerCase(const string& str) {
    string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

// Constructor
Item::Item(const string& itemName,
    const string& itemDescription,
    int itemAmount,
    const string& itemImagePath)
    : name(toLowerCase(itemName)),
    description(itemDescription),
    amount(itemAmount),
    imagePath(itemImagePath),
    category("[None]") {
}

// Setter for the name (stored in lowercase)
void Item::setName(const string& itemName) {
    name = toLowerCase(itemName);
}

// Getter for the name
string Item::getName() const {
    return name;
}

// Setter for the description
void Item::setDescription(const string& itemDescription) {
    description = itemDescription;
}

// Getter for the description
string Item::getDescription() const {
    return description;
}

// Setter for the image path
void Item::setImage(const string& itemImagePath) {
    imagePath = itemImagePath;
}

// Getter for the image path
string Item::getImage() const {
    return imagePath;
}

// Setter for the category
void Item::setCategory(const string& itemCategory) {
    category = itemCategory;
}

// Getter for the category
string Item::getCategory() const {
    return category;
}

// Setter for the amount
void Item::setAmount(int itemAmount) {
    amount = itemAmount;
}

// Getter for the amount
int Item::getAmount() const {
    return amount;
}

// Increase item quantity
void Item::increment() {
    ++amount;
}

// Decrease item quantity
void Item::decrement() {
    --amount;
}

// Equality operator
bool Item::operator==(const Item& other) const {
    return name == other.name &&
        description == other.description &&
        amount == other.amount &&
        imagePath == other.imagePath &&
        category == other.category;
}