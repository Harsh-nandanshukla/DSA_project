#include <bits/stdc++.h>
using namespace std;

#define ll  long long 
#define elif else if

ll Max(ll a, ll b, ll c) {
    if(a > b) {
        if(a > c)   return a;
        return c;
    } else {
        if(b > c)   return b;
        return c;
    }
}

int n, cnt = 0;

// IntervalTree using Red-Black Tree
class IntervalTree {
private:
    class node {
    public:
        ll LeftEndPoint, RightEndPoint, MaxRight;
        int Priority; 
        bool isRed;
        int id;
        node *Parent, *Left, *Right;

        node(ll L, ll R, int preference, int j) 
            : LeftEndPoint(L), RightEndPoint(R), Priority(preference), MaxRight(R), 
              Parent(nullptr), Left(nullptr), Right(nullptr), isRed(true), id(j) {}
    };

    node* Root;

    node* CreateNode(ll L, ll R, int preference, int j) {
        return new node(L, R, preference, j);
    }

    void Insert(ll L, ll R, int preference, int j) {
        node* x = Root;
        node* y = nullptr;
        node* z = CreateNode(L, R, preference, j);

        while(x) {
            y = x;
            if(L < x->LeftEndPoint) x = x->Left;
            else    x = x->Right;
            if(R > y->MaxRight)    y->MaxRight = R;
        }

        if(!y) {  
            Root = z;
            Root->isRed = false;
            return;
        }    
        else    (L < y->LeftEndPoint) ? y->Left = z : y->Right = z;

        z->Parent = y;
        UpdateMaxheight(z);
        Rebalance(z);
    }

    void LeftRotate(node* x) {
        node* y = x->Right;
        x->Right = y->Left;

        if(y->Left) y->Left->Parent = x;
        y->Parent = x->Parent;

        if(!x->Parent)  Root = y;
        elif(x == x->Parent->Left)  x->Parent->Left = y;
        else    x->Parent->Right = y;

        y->Left = x;
        x->Parent = y;

        y->MaxRight = x->MaxRight;
        UpdateMaxheight(x);
    }

    void RightRotate(node* x) {
        node* y = x->Left;
        x->Left = y->Right;

        if(y->Right) y->Right->Parent = x;
        y->Parent = x->Parent;

        if(!x->Parent)  Root = y;
        elif(x == x->Parent->Right)  x->Parent->Right = y;
        else    x->Parent->Left = y;

        y->Right = x;
        x->Parent = y;

        y->MaxRight = x->MaxRight;
        UpdateMaxheight(x);
    }

    void Rebalance(node* n) {
        while(n->Parent && n->Parent->isRed) {
            if(n->Parent == n->Parent->Parent->Left) {
                node* y = n->Parent->Parent->Right;
                if (y && y->isRed) {
                    n->Parent->isRed = false;
                    y->isRed = false;
                    n->Parent->Parent->isRed = true;
                    n = n->Parent->Parent;
                } else {
                    if(n == n->Parent->Right) {
                        n = n->Parent;
                        LeftRotate(n);
                    }
                    n->Parent->isRed = false;
                    n->Parent->Parent->isRed = true;
                    RightRotate(n->Parent->Parent);
                }
            } else {
                node* y = n->Parent->Parent->Left;
                if (y && y->isRed) {
                    n->Parent->isRed = false;
                    y->isRed = false;
                    n->Parent->Parent->isRed = true;
                    n = n->Parent->Parent;
                } else {
                    if(n == n->Parent->Left) {
                        n = n->Parent;
                        RightRotate(n);
                    }
                    n->Parent->isRed = false;
                    n->Parent->Parent->isRed = true;
                    LeftRotate(n->Parent->Parent);
                }
            }
        }
        Root->isRed = false;
    }

    void UpdateMaxheight(node* n) {
        ll l, r;
        (n->Left == nullptr) ? l = 0 : l = n->Left->MaxRight;
        (n->Right == nullptr) ? r = 0 : r = n->Right->MaxRight;
        n->MaxRight = Max(n->RightEndPoint, l, r);  
    }

    node* FindNode(ll L) {
        node* curr = Root;
        while(curr != nullptr) {
            if(L < curr->LeftEndPoint)
                curr = curr->Left;
            elif(L > curr->LeftEndPoint)
                curr = curr->Right;
            else
                return curr;
        }
        return nullptr;
    }

    void TransplantNode(node* target, node* replacement) {
        if(target->Parent == nullptr)
            Root = replacement;
        elif(target == target->Parent->Left)
            target->Parent->Left = replacement;
        else
            target->Parent->Right = replacement;
        if(replacement != nullptr)
            replacement->Parent = target->Parent;
    }

    node* MinimumNode(node* curr) {
        while(curr->Left != nullptr)
            curr = curr->Left;
        return curr;
    }

    void UpdateAncestorsMaxRight(node* ancestor) {
        while(ancestor != nullptr) {
            UpdateMaxheight(ancestor);
            ancestor = ancestor->Parent;
        }
    }

    void DeleteFix(node* target) {
        while(target != Root && (!target || !target->isRed)) {
            if(target->Parent == nullptr)
                break;
            if(target == target->Parent->Left) {
                node* sibling = target->Parent->Right;
                if(sibling && sibling->isRed) {
                    sibling->isRed = false;
                    target->Parent->isRed = true;
                    LeftRotate(target->Parent);
                    sibling = target->Parent->Right;
                }
                if(sibling && (!sibling->Left || !sibling->Left->isRed) && (!sibling->Right || !sibling->Right->isRed)) {
                    sibling->isRed = true;
                    target = target->Parent;
                } else {
                    if(sibling && (!sibling->Right || !sibling->Right->isRed)) {
                        if(sibling->Left)
                            sibling->Left->isRed = false;
                        sibling->isRed = true;
                        RightRotate(sibling);
                        sibling = target->Parent->Right;
                    }
                    if(sibling) {
                        sibling->isRed = target->Parent->isRed;
                        if(sibling->Right)
                            sibling->Right->isRed = false;
                    }
                    target->Parent->isRed = false;
                    LeftRotate(target->Parent);
                    target = Root;
                }
            } else {
                node* sibling = target->Parent->Left;
                if(sibling && sibling->isRed) {
                    sibling->isRed = false;
                    target->Parent->isRed = true;
                    RightRotate(target->Parent);
                    sibling = target->Parent->Left;
                }
                if(sibling && (!sibling->Left || !sibling->Left->isRed) && (!sibling->Right || !sibling->Right->isRed)) {
                    sibling->isRed = true;
                    target = target->Parent;
                } else {
                    if(sibling && (!sibling->Left || !sibling->Left->isRed)) {
                        if(sibling->Right)
                            sibling->Right->isRed = false;
                        sibling->isRed = true;
                        LeftRotate(sibling);
                        sibling = target->Parent->Left;
                    }
                    if(sibling) {
                        sibling->isRed = target->Parent->isRed;
                        if(sibling->Left)
                            sibling->Left->isRed = false;
                    }
                    target->Parent->isRed = false;
                    RightRotate(target->Parent);
                    target = Root;
                }
            }
        }
        if(target)
            target->isRed = false;
    }

    void Delete(ll L, ll R) {
        node* target = FindNode(L);
        if(target == nullptr || target->LeftEndPoint != L || target->RightEndPoint != R)
            return;

        node* replacement = target, *child = nullptr;
        bool replacement_original_colour = replacement->isRed;
        if (target->Left == nullptr) {
            child = target->Right;
            TransplantNode(target, target->Right);
            UpdateAncestorsMaxRight(target->Parent);
        } else if (target->Right == nullptr) {
            child = target->Left;
            TransplantNode(target, target->Left);
            UpdateAncestorsMaxRight(target->Parent);
        } else {
            replacement = MinimumNode(target->Right);
            replacement_original_colour = replacement->isRed;
            child = replacement->Right;
            if (replacement->Parent == target) {
                if (child != nullptr) 
                    child->Parent = replacement;
            } else {
                TransplantNode(replacement, replacement->Right);
                replacement->Right = target->Right;
                if (replacement->Right != nullptr)
                    replacement->Right->Parent = replacement;
                UpdateAncestorsMaxRight(replacement->Parent);
            }        
            TransplantNode(target, replacement);
            replacement->Left = target->Left;
            if (replacement->Left != nullptr) 
                replacement->Left->Parent = replacement;
            replacement->isRed = target->isRed;
            UpdateAncestorsMaxRight(replacement);
        }
        if (!replacement_original_colour)
            DeleteFix(child);
        delete target;    
    }

    void  Inordertraversal(node* r) const {
        if(!r)  return;
        Inordertraversal(r->Left);
        cout << "ID: " << r->id << " | Interval: [" << r->LeftEndPoint << ", " << r->RightEndPoint << "] | Priority: " << r->Priority << " | MaxRight: " << r->MaxRight << " | Color: " << (r->isRed ? "Red" : "Black") << endl;
        Inordertraversal(r->Right);
    }

public:
    IntervalTree() { Root = nullptr; }
    ~IntervalTree() {}

    void insert(ll L, ll R, int preference) {
        cnt++;
        cnt > n ? usingCheckAndModify(L, R, preference, n) : Insert(L, R, preference, cnt);
    }

    void remove(ll L, ll R) {
        Delete(L, R);
    }

    void Inordertraversal() {
        Inordertraversal(Root);
    }

    // --- Advanced features ---
    bool doOverlap(ll L1, ll R1, ll L2, ll R2) {
        return L1 <= R2 && L2 <= R1;
    }

    void checkAndModifyInterval(node* curr, ll L, ll R, vector<bool>& overlapArray, vector<node*>& overlappingnodes, int priority, int n) {
        if(!curr) return;
        if (doOverlap(L, R, curr->LeftEndPoint, curr->RightEndPoint)) {
            node* overlapNode = curr;  
            overlapArray[overlapNode->id - 1] = true;
            if(overlapNode->Priority < priority) {   
                overlappingnodes.push_back(overlapNode);
            }
        }
        if (curr->Left && curr->Left->MaxRight >= L) {
            checkAndModifyInterval(curr->Left, L, R, overlapArray, overlappingnodes, priority, n); 
        }
        checkAndModifyInterval(curr->Right, L, R, overlapArray, overlappingnodes, priority, n);
        return;
    }

    void usingCheckAndModify(ll L, ll R, int priority, int n) {
        vector<bool> overlapArray(n, false);
        vector<node*> overlappingnodes;
        checkAndModifyInterval(this->Root, L, R, overlapArray, overlappingnodes, priority, n);
        for (int i = 0; i < n; i++) {
            if (!overlapArray[i]) {
                Insert(L, R, priority, i + 1);
                cout << "Inserting new interval: [" << L << ", " << R << "] with priority " << priority << " for worker " << i + 1 << endl;
                return;
            }
        }
        if(!overlappingnodes.empty()) {
            sort(overlappingnodes.begin(), overlappingnodes.end(), [](node* a, node* b){return a->Priority < b->Priority;});
            auto overlapNode = overlappingnodes[0];
            cout << "Deleting lower-priority node: [" << overlapNode->LeftEndPoint << ", " << overlapNode->RightEndPoint << "] for worker " << overlapNode->id << endl;
            Insert(L, R, priority, overlapNode->id);
            Delete(overlapNode->LeftEndPoint, overlapNode->RightEndPoint);
            return;
        } else {
            cout << "This interval cannot be inserted" << endl;
            return;
        }
    }
};

int main() {
    cout << "Enter number of employees (n): ";
    cin >> n;

    IntervalTree T;

    int choice;
    while (true) {
        cout << "\nMenu:\n";
        cout << "1. Insert interval\n";
        cout << "2. Delete interval\n";
        cout << "3. Display intervals (inorder)\n";
        cout << "4. Check if two intervals overlap\n";
       // cout << "5. Insert interval with advanced logic (usingCheckAndModify)\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            ll L, R;
            int priority;
            cout << "Enter LeftEndPoint, RightEndPoint, Priority: ";
            cin >> L >> R >> priority;
            T.usingCheckAndModify(L, R, priority, n);
        } else if (choice == 2) {
            ll L, R;
            cout << "Enter LeftEndPoint and RightEndPoint to delete: ";
            cin >> L >> R;
            T.remove(L, R);
        } else if (choice == 3) {
            cout << "Intervals in the tree:\n";
            T.Inordertraversal();
        } else if (choice == 4) {
            ll L1, R1, L2, R2;
            cout << "Enter first interval (L1 R1): ";
            cin >> L1 >> R1;
            cout << "Enter second interval (L2 R2): ";
            cin >> L2 >> R2;
            if (T.doOverlap(L1, R1, L2, R2))
                cout << "Intervals overlap.\n";
            else
                cout << "Intervals do NOT overlap.\n";
        } else if (choice == 5) {
            ll L, R;
            int priority;
            cout << "Enter LeftEndPoint, RightEndPoint, Priority: ";
            cin >> L >> R >> priority;
            T.usingCheckAndModify(L, R, priority, n);
        } else if (choice == 6) {
            break;
        } else {
            cout << "Invalid choice. Try again.\n";
        }
    }
    return 0;
}
