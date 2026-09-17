#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <queue>
#include <limits>

using namespace std;

enum class ProjectType { RURAL_DEVELOPMENT = 1, HIGHWAY = 2, RAILWAY = 3, RENEWABLE_ENERGY = 4 };

// ======================================================
// DATA STRUCTURE & ADT (Base Class)
// ======================================================
class AcquisitionCase {
public:
    string parcelId, owner, projectName;
    double compensationINR;
    int affectedFamilies, daysPending;
    ProjectType type;

    AcquisitionCase(string id, string o, string proj, double comp, int fam, int days, ProjectType pt)
        : parcelId(id), owner(o), projectName(proj), compensationINR(comp), affectedFamilies(fam), daysPending(days), type(pt) {}

    double getPriorityScore() const {
        return (affectedFamilies * 5.0) + (daysPending * 1.5) + (static_cast<int>(type) * 10.0);
    }

    void showRow() const {
        cout << "| " << left << setw(8) << parcelId << setw(18) << owner 
             << setw(20) << projectName << setw(10) << compensationINR 
             << setw(10) << getPriorityScore() << "|\n";
    }
};

// ======================================================
// DOUBLY LINKED LIST (Undo System)
// ======================================================
struct DLLNode {
    string action;
    DLLNode* prev;
    DLLNode* next;
    DLLNode(string a) : action(a), prev(nullptr), next(nullptr) {}
};

class UndoSystemADT {
    DLLNode *head, *tail;
public:
    UndoSystemADT() : head(nullptr), tail(nullptr) {}
    void addAction(string action) {
        DLLNode* newNode = new DLLNode(action);
        if(!head) { head = tail = newNode; }
        else { tail->next = newNode; newNode->prev = tail; tail = newNode; }
    }
    void undoLast() {
        if(!tail) { cout << "[i] Nothing to undo.\n"; return; }
        cout << "\n[DOUBLY LINKED LIST] Undoing Action: " << tail->action << "\n";
        DLLNode* temp = tail;
        tail = tail->prev;
        if(tail) tail->next = nullptr;
        else head = nullptr;
        delete temp;
    }
};

// ======================================================
// STACK ADT (Implementation using Linked List)
// ======================================================
struct StackNode {
    string data;
    StackNode* next;
    StackNode(string val) : data(val), next(nullptr) {}
};

class StackADT { // Used for Recent Search History
    StackNode* topNode;
public:
    StackADT() : topNode(nullptr) {}
    void push(string val) {
        StackNode* newNode = new StackNode(val);
        newNode->next = topNode;
        topNode = newNode;
    }
    void displayAndPopAll() {
        if (!topNode) { cout << "[i] Stack is empty. No recent searches.\n"; return; }
        cout << "\n[STACK ADT via Linked List] Recent Searches (LIFO):\n";
        while (topNode) {
            cout << " -> " << topNode->data << "\n";
            StackNode* temp = topNode;
            topNode = topNode->next;
            delete temp;
        }
    }
};

// ======================================================
// CIRCULAR QUEUE ADT (Implementation using Array)
// ======================================================
#define MAX_Q 5
class CircularQueueADT {
    string arr[MAX_Q];
    int front, rear;
public:
    CircularQueueADT() : front(-1), rear(-1) {}
    void enqueue(string file) {
        if ((rear + 1) % MAX_Q == front) { cout << "[!] Queue is Full (Overflow)!\n"; return; }
        if (front == -1) front = 0;
        rear = (rear + 1) % MAX_Q;
        arr[rear] = file;
        cout << "[CIRCULAR QUEUE] Added Normal File: " << file << "\n";
    }
    void dequeue() {
        if (front == -1) { cout << "[!] Queue is Empty (Underflow)!\n"; return; }
        cout << "\n[CIRCULAR QUEUE] Processing File: " << arr[front] << "\n";
        if (front == rear) { front = -1; rear = -1; }
        else { front = (front + 1) % MAX_Q; }
    }
};

// Priority Queue Comparator (For VIP Cases)
struct PriorityComparator {
    bool operator()(const AcquisitionCase& a, const AcquisitionCase& b) const {
        return a.getPriorityScore() < b.getPriorityScore();
    }
};

// ======================================================
// SEARCHING & SORTING TECHNIQUES
// ======================================================
class AlgorithmEngine {
public:
    // 1. Linear Search (O(N))
    static void linearSearch(const vector<AcquisitionCase>& arr, string ownerName) {
        cout << "\n[LINEAR SEARCH] Scanning array sequentially...\n";
        for (int i = 0; i < arr.size(); i++) {
            if (arr[i].owner == ownerName) {
                cout << "[SUCCESS] Found Owner at Index " << i << ":\n";
                arr[i].showRow();
                return;
            }
        }
        cout << "[!] Owner not found.\n";
    }

    // 2. Bubble Sort (Stable Sort, O(N^2))
    static void bubbleSort(vector<AcquisitionCase>& arr) {
        cout << "\n[BUBBLE SORT] Sorting by Parcel ID (Stable Sort)...\n";
        int n = arr.size();
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (arr[j].parcelId > arr[j + 1].parcelId) {
                    swap(arr[j], arr[j + 1]);
                }
            }
        }
    }

    // 3. Binary Search (O(log N)) - Requires sorted array
    static void binarySearch(const vector<AcquisitionCase>& arr, string targetId, StackADT& stack) {
        cout << "\n[BINARY SEARCH] Searching sorted array by Divide & Conquer...\n";
        int left = 0, right = arr.size() - 1;
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (arr[mid].parcelId == targetId) {
                cout << "[SUCCESS] Found Parcel at Index " << mid << ":\n";
                arr[mid].showRow();
                stack.push(targetId); // Add to Stack history
                return;
            }
            if (arr[mid].parcelId < targetId) left = mid + 1;
            else right = mid - 1;
        }
        cout << "[!] Parcel ID not found.\n";
    }
};

// ======================================================
// SAFE INPUT FUNCTIONS
// ======================================================
int getInt(string prompt) { 
    int val; cout << prompt; cin >> val; 
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return val; 
}
string getText(string prompt) { 
    string text; cout << prompt; getline(cin, text); 
    return text; 
}

// ======================================================
// MAIN FUNCTION
// ======================================================
int main() {
    vector<AcquisitionCase> cases; // Array representation
    CircularQueueADT normalQueue;  // Circular Queue
    StackADT searchStack;          // Stack ADT
    UndoSystemADT undoSystem;      // Doubly Linked List
    priority_queue<AcquisitionCase, vector<AcquisitionCase>, PriorityComparator> pq;

    // Seeding Demo Data
    cases.push_back({"MH-03", "Anil Patil", "Pune Metro", 150.0, 5, 45, ProjectType::RAILWAY});
    cases.push_back({"UP-01", "Ankita Sharma", "Highway Ext", 300.0, 15, 120, ProjectType::HIGHWAY});
    cases.push_back({"TN-04", "Karthik Iyer", "Port Road", 400.0, 50, 300, ProjectType::HIGHWAY}); 
    cases.push_back({"GJ-02", "Ramesh Bhai", "Solar Park", 80.0, 0, 10, ProjectType::RENEWABLE_ENERGY});
    
    for(const auto& c : cases) pq.push(c);
    
    normalQueue.enqueue("RTI-101: Valuation Report");
    normalQueue.enqueue("RTI-102: Dispute Claim");

    cout << "\n============================================================\n";
    cout << "      NATIONAL LAND ACQUISITION SYSTEM (DSA DEMO)\n";
    cout << "============================================================\n";

    while (true) {
        cout << "\n====================== MAIN MENU ======================\n";
        cout << "1. View All Cases (Array)\n";
        cout << "2. Search by Owner Name (Linear Search)\n";
        cout << "3. Sort & Search by ID (Bubble Sort & Binary Search)\n";
        cout << "4. View Recent Searches (Stack ADT via Linked List)\n";
        cout << "5. Process Normal RTI File (Circular Queue ADT)\n";
        cout << "6. Escalate VIP Case (Priority Queue)\n";
        cout << "7. Undo Last Action (Doubly Linked List)\n";
        cout << "8. Exit\n";
        
        int choice = getInt("Select Option: ");

        if (choice == 1) { 
            cout << "\n+----------+------------------+--------------------+----------+----------+\n";
            for (const auto& c : cases) c.showRow();
            cout << "+----------+------------------+--------------------+----------+----------+\n";
        } 
        else if (choice == 2) { 
            string owner = getText("Enter Owner Name (e.g., Anil Patil): ");
            AlgorithmEngine::linearSearch(cases, owner);
        }
        else if (choice == 3) { 
            AlgorithmEngine::bubbleSort(cases); // Sort first
            cout << "[i] Array is now sorted using Bubble Sort.\n";
            string id = getText("Enter Parcel ID to Binary Search (e.g., MH-03): ");
            AlgorithmEngine::binarySearch(cases, id, searchStack);
        }
        else if (choice == 4) {
            searchStack.displayAndPopAll();
        }
        else if (choice == 5) {
            normalQueue.dequeue();
            undoSystem.addAction("Processed Normal RTI File");
        }
        else if (choice == 6) {
            if(pq.empty()) { cout << "\n[i] All priority cases resolved!\n"; continue; }
            cout << "\n[PRIORITY QUEUE] Escalating Case: " << pq.top().parcelId << "\n";
            undoSystem.addAction("Escalated VIP Case: " + pq.top().parcelId);
            pq.pop();
        }
        else if (choice == 7) {
            undoSystem.undoLast();
        }
        else if (choice == 8) {
            cout << "\nExiting System. Best of luck for the SIH Demo!\n";
            break;
        }
    }
    return 0;
}
