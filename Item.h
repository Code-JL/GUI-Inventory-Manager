#pragma once
#include <string>
#include <vector>
#include <algorithm>

using namespace std;


// Declaration of the toLowerCase function
string toLowerCase(const string& str);

class Item
{
private:
    // Member variables
    string name;                // Item name (stored in lowercase)
    string description;         // Item description
    string imagePath;           // Path to a photo
    string category = "[None]";  // Default category
    int amount;                 // Item quantity

public:
    // Constructor
    Item(const string& itemName, const string& itemDescription, int itemAmount, const string& itemImagePath);

    // Setter and getter for name
    void setName(const string& itemName);
    string getName() const;

    // Setter and getter for description
    void setDescription(const string& itemDescription);
    string getDescription() const;

    // Setter and getter for image path
    void setImage(const string& itemImagePath);
    string getImage() const;

	// Setter and getter for category
    void setCategory(const string& itemCategory);
    string getCategory() const;

    // Setter and getter for amount
    void setAmount(int itemAmount);
    int getAmount() const;


    // Increment and decrement item quantity
    void increment();
    void decrement();

    // Equality operator
    bool operator==(const Item& other) const;
};

