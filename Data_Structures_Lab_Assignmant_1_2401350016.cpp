#include <iostream>
#include <string>
using namespace std;

// Class for items in store (like product card)
class InventoryItem {
public:
    int itemId;       // product id
    string itemName;  // product name
    int quantity;     // how many items
    float price;      // price per item

    // constructor to make new product
    InventoryItem(int id = 0, string name = "", int qty = 0, float cost = 0.0) {
        itemId = id;
        itemName = name;
        quantity = qty;
        price = cost;
    }

    // show details of item
    void displayItem() {
        int rupees = static_cast<int>(price);
        int paise = static_cast<int>((price - rupees) * 100 + 0.5);
        cout << "ID: " << itemId << " | Name: " << itemName
             << " | Stock: " << quantity << " | Price: ₹"
             << rupees << '.';
        if (paise < 10) cout << '0';
        cout << paise << endl;
    }
};

// Main system for inventory
class InventoryManagementSystem {
private:
    static const int MAX_ITEMS = 100;        // max products
    static const int PRICE_CATEGORIES = 5;   // price ranges
    static const int QTY_CATEGORIES = 4;     // quantity ranges
    
    InventoryItem itemArray[MAX_ITEMS];      // array of products
    int currentItemCount;                    // current count
    
    float priceQuantityTable[PRICE_CATEGORIES][QTY_CATEGORIES]; // table
    
    // For "rarely restocked" items, keep array instead of sparse matrix
    int rareItems[MAX_ITEMS];
    int rareFreq[MAX_ITEMS];
    int rareCount;

    // find item index using id
    int findItemIndex(int targetId) {
        for (int i = 0; i < currentItemCount; i++) {
            if (itemArray[i].itemId == targetId) {
                return i;
            }
        }
        return -1;
    }

public:
    // constructor
    InventoryManagementSystem() {
        currentItemCount = 0;
        rareCount = 0;
        initializePriceQuantityTable();
        
        cout << "Welcome to Singh & Sons Grocery Store Inventory System!" << endl;
    }
    
    // initialize table with 0
    void initializePriceQuantityTable() {
        for (int priceRange = 0; priceRange < PRICE_CATEGORIES; priceRange++) {
            for (int qtyRange = 0; qtyRange < QTY_CATEGORIES; qtyRange++) {
                priceQuantityTable[priceRange][qtyRange] = 0.0;
            }
        }
    }
    
    // insert new item
    bool insertItem(int id, string name, int qty, float cost) {
        if (currentItemCount >= MAX_ITEMS) {
            cout << "Store is full!" << endl;
            return false;
        }
        if (findItemIndex(id) != -1) {
            cout << "Item already exists!" << endl;
            return false;
        }
        itemArray[currentItemCount] = InventoryItem(id, name, qty, cost);
        currentItemCount++;
        cout << "Added " << name << " to inventory!" << endl;
        return true;
    }
    
    // delete item
    bool deleteItem(int targetId) {
        int itemIndex = findItemIndex(targetId);
        if (itemIndex == -1) {
            cout << "Item not found!" << endl;
            return false;
        }
        string removedItemName = itemArray[itemIndex].itemName;
        for (int i = itemIndex; i < currentItemCount - 1; i++) {
            itemArray[i] = itemArray[i + 1];
        }
        currentItemCount--;
        cout << "Removed " << removedItemName << "!" << endl;
        return true;
    }
    
    // search item by id
    InventoryItem* searchItemById(int searchId) {
        int index = findItemIndex(searchId);
        if (index != -1) {
            cout << "Found item:" << endl;
            return &itemArray[index];
        }
        cout << "Item with ID " << searchId << " not found!" << endl;
        return nullptr;
    }
    
    // search item by name
    InventoryItem* searchItemByName(string searchName) {
        for (int i = 0; i < currentItemCount; i++) {
            if (itemArray[i].itemName == searchName) {
                cout << "Found " << searchName << ":" << endl;
                return &itemArray[i];
            }
        }
        cout << searchName << " not found!" << endl;
        return nullptr;
    }
    
    // show all items
    void displayAllItems() {
        if (currentItemCount == 0) {
            cout << "Store is empty!" << endl;
            return;
        }
        cout << "\nCurrent Inventory (Total: " << currentItemCount << "):" << endl;
        cout << string(70, '-') << endl;
        for (int i = 0; i < currentItemCount; i++) {
            itemArray[i].displayItem();
        }
        cout << string(70, '-') << endl;
    }
    
    // row major print of table
    void displayPriceQuantityRowMajor() {
        cout << "\nPrice-Quantity Table (Row-Major):" << endl;
        for (int row = 0; row < PRICE_CATEGORIES; row++) {
            cout << "Price Range " << (row + 1) << ": ";
            for (int col = 0; col < QTY_CATEGORIES; col++) {
                char buffer[16];
                snprintf(buffer, sizeof(buffer), "%8.2f", priceQuantityTable[row][col]);
                cout << buffer << " ";
            }
            cout << endl;
        }
    }
    
    // column major print of table
    void displayPriceQuantityColumnMajor() {
        cout << "\nPrice-Quantity Table (Column-Major):" << endl;
        for (int col = 0; col < QTY_CATEGORIES; col++) {
            cout << "Quantity Range " << (col + 1) << ": ";
            for (int row = 0; row < PRICE_CATEGORIES; row++) {
                char buffer[16];
                snprintf(buffer, sizeof(buffer), "%8.2f", priceQuantityTable[row][col]);
                cout << buffer << " ";
            }
            cout << endl;
        }
    }
    
    // add to "rarely restocked" array
    void addRareItem(int itemId, int restockFreq) {
        if (rareCount < MAX_ITEMS) {
            rareItems[rareCount] = itemId;
            rareFreq[rareCount] = restockFreq;
            rareCount++;
            cout << "Added item " << itemId << " to rare storage (freq: " << restockFreq << ")" << endl;
        } else {
            cout << "Rare storage full!" << endl;
        }
    }
    
    // show "rarely restocked" items
    void displayRareItems() {
        if (rareCount == 0) {
            cout << "No rare items!" << endl;
            return;
        }
        cout << "\nRarely Restocked Items:" << endl;
        cout << string(50, '-') << endl;
        for (int i = 0; i < rareCount; i++) {
            cout << "Item ID: " << rareItems[i]
                 << " | Restock Freq: " << rareFreq[i] << endl;
        }
        cout << string(50, '-') << endl;
    }
    
    // update value in table
    void updatePriceQuantityTable(int priceRange, int qtyRange, float value) {
        if (priceRange >= 0 && priceRange < PRICE_CATEGORIES && qtyRange >= 0 && qtyRange < QTY_CATEGORIES) {
            priceQuantityTable[priceRange][qtyRange] = value;
            cout << "Updated table at (" << priceRange << "," << qtyRange << ") with " << value << endl;
        } else {
            cout << "Invalid index!" << endl;
        }
    }
    
    // check for low stock
    void checkLowStock(int threshold = 10) {
        cout << "\nLow Stock Alert (threshold: " << threshold << "):" << endl;
        cout << string(60, '-') << endl;
        bool foundLowStock = false;
        for (int i = 0; i < currentItemCount; i++) {
            if (itemArray[i].quantity <= threshold) {
                cout << "LOW: ";
                itemArray[i].displayItem();
                foundLowStock = true;
            }
        }
        if (!foundLowStock) {
            cout << "All good, no low stock!" << endl;
        }
        cout << string(60, '-') << endl;
    }
    
    // calculate total inventory value
    float calculateTotalInventoryValue() {
        float totalValue = 0.0;
        for (int i = 0; i < currentItemCount; i++) {
            totalValue += (itemArray[i].quantity * itemArray[i].price);
        }

        int rupees = static_cast<int>(totalValue);
        int paise = static_cast<int>((totalValue - rupees) * 100 + 0.5);

        cout << "\nTotal Inventory Value: ₹" << rupees << '.';
        if (paise < 10) cout << '0';
        cout << paise << endl;

        return totalValue;
    }
};

// function to demo the system
void demonstrateSystem() {
    InventoryManagementSystem grocery;
    
    cout << "\n=== Adding Items ===" << endl;
    grocery.insertItem(101, "Basmati Rice", 50, 75.50);
    grocery.insertItem(102, "Toor Dal", 30, 120.00);
    grocery.insertItem(103, "Cooking Oil", 25, 180.75);
    grocery.insertItem(104, "Wheat Flour", 40, 45.25);
    grocery.insertItem(105, "Sugar", 20, 42.00);
    grocery.insertItem(106, "Tea Leaves", 15, 250.00);
    
    cout << "\n=== Inventory ===" << endl;
    grocery.displayAllItems();
    
    cout << "\n=== Search ===" << endl;
    InventoryItem* foundItem = grocery.searchItemById(103);
    if (foundItem) foundItem->displayItem();
    foundItem = grocery.searchItemByName("Sugar");
    if (foundItem) foundItem->displayItem();
    
    cout << "\n=== Price-Quantity Table ===" << endl;
    grocery.updatePriceQuantityTable(0, 0, 100.5);
    grocery.updatePriceQuantityTable(1, 2, 250.75);
    grocery.updatePriceQuantityTable(2, 1, 180.25);
    grocery.displayPriceQuantityRowMajor();
    grocery.displayPriceQuantityColumnMajor();
    
    cout << "\n=== Rare Items ===" << endl;
    grocery.addRareItem(107, 2);
    grocery.addRareItem(108, 1);
    grocery.addRareItem(109, 3);
    grocery.displayRareItems();
    
    cout << "\n=== Stock Check ===" << endl;
    grocery.checkLowStock(25);
    grocery.calculateTotalInventoryValue();
    
    cout << "\n=== Delete Item ===" << endl;
    grocery.deleteItem(106);
    grocery.displayAllItems();
}

// main function
int main() {
    cout << "Welcome to Grocery Store Inventory System!" << endl;
    cout << string(80, '=') << endl;
    
    demonstrateSystem();
    
    cout << "\n" << string(80, '=') << endl;
    cout << "Demo finished!" << endl;
    return 0;
}
