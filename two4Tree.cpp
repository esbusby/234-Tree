#include <iostream>
using namespace std;

template<typename valuetype>
class CircularDynamicArray {
public:
    int size;
    int cap;
    int front;
    int back;
    valuetype error;
    valuetype* array;

    CircularDynamicArray() {
        array = new valuetype[cap = 2];
        size = 0;
        front = 0;
        back = 0;
    }
    valuetype& operator[](int i) {
        if (i < 0 || i >= size) {
            //cout << "Error: Index is out of bounds." << endl;
            return error;
        }
        return array[(front + i) % cap];
    }
    void addEnd(valuetype v) {
        if (size == cap) {
            int newCap = cap * 2;
            valuetype* newArray = new valuetype[newCap];
            for (int i = 0; i < size; i++) {
                newArray[i] = array[(front + i) % cap];
            }

            front = 0;
            back = size - 1;

            delete[] array;

            array = newArray;
            cap = newCap;
        }
        array[(front + size) % cap] = v;
        size++;
        back = (size - 1) % cap;
    }
    void addFront(valuetype v) {
        if (size == cap) {
            int newCap = cap * 2;
            valuetype* newArray = new valuetype[newCap];

            for (int i = 0; i < size; i++) {
                newArray[i + 1] = array[(front + i) % cap];
            }

            front = 1;
            back = size - 1;

            delete[] array;

            array = newArray;
            cap = newCap;
        }
        front = (front - 1 + cap) % cap;
        array[front] = v;
        size++;
    }
    void delEnd() {
        size--;
        back = (back - 1 + cap) % cap;

        if (size <= (cap / 4) && cap > 2) {
            int newCap = cap / 2;
            valuetype* newArray = new valuetype[newCap];

            for (int i = 0; i < size; i++) {
                newArray[i] = array[(front + i) % cap];
            }

            front = 0;
            back = size - 1;

            delete[] array;

            array = newArray;
            cap = newCap;
        }

    }
    void delFront() {
        front = (front + 1) % cap;
        size--;

        if (size == (cap / 4) && cap > 2) {
            int newCap = cap / 2;
            valuetype* newArray = new valuetype[newCap];

            for (int i = 0; i < size; i++) {
                newArray[i] = array[(front + i) % cap];
            }

            front = 0;
            back = size - 1;

            delete[] array;

            array = newArray;
            cap = newCap;
        }
    }
    int length() {
        return size;
    }
    void clear(){
        delete[] array;
        array = new valuetype[cap = 2];
        size = 0;
        front = 0;
        back = 0;
     }
    ~CircularDynamicArray() {
        delete[] array;
    }
    CircularDynamicArray(const CircularDynamicArray& old) {
        size = old.size;
        cap = old.cap;
        front = old.front;
        back = old.back;
        array = new valuetype[cap];
        for (int i = 0; i < cap; i++) {
            array[i] = old.array[i];
        }

    }
    CircularDynamicArray& operator=(const CircularDynamicArray& rhs) {
        delete[] array;
        size = rhs.size;
        cap = rhs.cap;
        front = rhs.front;
        back = rhs.back;
        array = new valuetype[cap];
        for (int i = 0; i < rhs.cap; i++) {
            array[i] = rhs.array[i];
        }

        return *this;
    }
};


const int t = 2; // Minimum degree
template<typename keytype, typename valuetype>
class two4Tree{

    public:

    struct Node {
        int numKeys;
        keytype keys[3];
        CircularDynamicArray<valuetype> CDAs[3];
        Node* children[4];
        bool leaf = true;
        int leftTreeSize[3] = {0,0,0};


        Node(bool leaf) {
            this->numKeys = 0;
            this->leaf = leaf;
            for (int i = 0; i < 4; ++i){
                this->children[i] = nullptr;
            }
            
        }
    };

    void splitchildren(Node* x, int i, keytype k) {
        Node* y = x->children[i];
        Node* z = new Node(y->leaf);
        z->numKeys = 1;
        
        int temp = x->leftTreeSize[i];

        for (int j = 0; j < 1; j++){
            z->keys[j] = y->keys[j + t];
            z->CDAs[j] = y->CDAs[j+t];
            z->leftTreeSize[j] = y->leftTreeSize[j+t];
        }
        //z->leftTreeSize[0] = y->leftTreeSize[2];


        if (y->children[0] != nullptr) {
            for (int j = 0; j < t; j++)
                z->children[j] = y->children[j + t];
        }

        y->numKeys = 1;

        for (int j = x->numKeys; j >= i + 1; --j){
            x->children[j + 1] = x->children[j];
        }

        x->children[i + 1] = z;

        for (int j = x->numKeys - 1; j >= i; --j){
            x->keys[j + 1] = x->keys[j];
            x->CDAs[j + 1] = x->CDAs[j];
            x->leftTreeSize[j+1] = x->leftTreeSize[j];
        }
        //x->leftTreeSize

        // for (int j = x->numKeys; j >= i; --j){
        //     x->leftTreeSize[j+1] = x->leftTreeSize[j];
        // }

        x->keys[i] = y->keys[1];
        x->CDAs[i] = y->CDAs[1];
        x->leftTreeSize[i] = y->leftTreeSize[0] + y->leftTreeSize[1] + y->CDAs[0].length();

        int tempLeft = y->CDAs[0].length() + y->leftTreeSize[0] + y->CDAs[1].length() + y->leftTreeSize[1] + y->CDAs[2].length() + y->leftTreeSize[2];
        if(i != x->numKeys){
            int rightTree = temp - tempLeft;
            x->leftTreeSize[i+1] = y->leftTreeSize[2] + z->CDAs[0].length() + rightTree;
        }
        //x->leftTreeSize[i+1] = y->leftTreeSize[2] + z->CDAs[0].length() - 1;
        //x->leftTreeSize[i]

        x->numKeys++;
        // for(int a = 0; a < x->numKeys; a++){
        //     x->leftTreeSize[i] += y->leftTreeSize[a];
        // }
        // if(x->keys[i] != k){
        //     x->leftTreeSize[i]++;
        // }
        //x->leftTreeSize[x->numKeys-1] = y->leftTreeSize[0] + y->leftTreeSize[1] + y->CDAs[0].length();
    }

    void insertNonFull(Node* x, keytype k, valuetype v) {
        int i = x->numKeys - 1;

        if (x->children[0] == nullptr) {
            while (i >= 0 && k < x->keys[i]) {
                x->keys[i + 1] = x->keys[i];
                x->CDAs[i + 1] = x->CDAs[i];
                x->leftTreeSize[i+1] = x->leftTreeSize[i];
                i--;
            }

            if(i >= 0 && x->keys[i] == k){
                x->CDAs[i].addEnd(v);
                return;
            }
            else{
                x->keys[i + 1] = k;
                x->CDAs[i+1].clear();
                x->CDAs[i+1].addEnd(v);
                x->leftTreeSize[i+1] = 0;
                x->numKeys++;
                //x->numKeys[i+1]++;
                //cout << "First time " << x->keys[i+1] <<  " has been seen, numKeys " << x->numKeys[i+1] << endl;
            }
        } 
        else {
            while (i >= 0 && k < x->keys[i]){
                i--;
            }
            i++;

            if (x->children[i]->numKeys == 3) {
                splitchildren(x, i, k);
                if (k > x->keys[i])
                    i++;
            }
            if(x != nullptr){
                for(int a = 0; a < x->numKeys; a++){
                    if(x->keys[a] == k){
                        x->CDAs[a].addEnd(v);
                        return;
                    }
                }
            }
            
            insertNonFull(x->children[i], k,v);
        }
    }

    void insertHelper(Node*& root, keytype k, valuetype v) {
        if (!root) {
            root = new Node(true);
            root->keys[0] = k;
            root->CDAs[0].addEnd(v);
            root->leftTreeSize[0] = 0;
            root->numKeys = 1;
        } else {
            if (root->numKeys == 3) {
                Node* s = new Node(false);
                s->children[0] = root;
                splitchildren(s, 0, k);
                root = s;
            }
            insertNonFull(root, k,v);
        }
    }

    //public:
    Node* root;
    int treeSize = 0;
    int rankVal;

    two4Tree(){
        //Node* x = new Node(true);
        root = nullptr;
        treeSize = 0;
    }
   
   two4Tree(keytype K[], valuetype V[], float s){
        root = nullptr;
        treeSize = 0;
        for(int i = 0; i < s; i++){
            insert(K[i],V[i]);
        }        
    }

    valuetype *search(keytype k){
        Node* curr = root;

        while (curr != nullptr) {
            // Search for the keys in the curr node's keys
            int i = 0;
            while (i < curr->numKeys && k > curr->keys[i]) {
                i++;
            }

            // If key is found, return
            if (i < curr->numKeys && k == curr->keys[i]) {
                //cout << curr->keys[i] << " " << "i = " << i << "    ";
                return &curr->CDAs[i][0];
            }

            // Move to the children if not found
            curr = curr->children[i];
        }

        // If the loop terminates without finding the keys and without hitting a leaf node, return nullptr
        return nullptr;
    }

    void insert(keytype k, valuetype v){
        treeSize++;  
        insertHelper(root,k,v);
        leftTreeUpdateInsert(k);
    }
    
     keytype selectHelper(Node* curr, int pos) {
        //Recursively incrementing through node x to find corresponding keyvalue
        int i = 0;
        while(i < curr->numKeys) {
            if (pos > curr->leftTreeSize[i] + curr->CDAs[i].length()) {
                pos -= curr->leftTreeSize[i] + curr->CDAs[i].length();
                //if(curr->CDAs[i].length() == 0) pos -= 1;
                i++;
            }
            else if (pos == curr->leftTreeSize[i] + curr->CDAs[i].length()) {
                return curr->keys[i];
            }
            else {
                //Check for dups
                if (curr->CDAs[i].length() > 1) {
                    if (pos < curr->leftTreeSize[i] + 1) {
                        return selectHelper(curr->children[i], pos);
                    }
                    else {
                        return curr->keys[i];
                    }
                }
                else {
                    return selectHelper(curr->children[i], pos);
                }
            }
        }

        return selectHelper(curr->children[i], pos);
    }

    keytype select(int pos){
        return selectHelper(root, pos);
    }
 
    int rank(keytype k) {

        rankVal = 0;
        Node* curr = root;
        while (curr != nullptr) {
            int i = 0;

            while (i < curr->numKeys && k > curr->keys[i]) {
                if(curr->CDAs[i].length() == 0) rankVal+=1;
                rankVal += curr->leftTreeSize[i] + curr->CDAs[i].length();
                i++;
            }

            // If key is found, return to stop transversing
            if (i < curr->numKeys && k == curr->keys[i]) {
                //cout << root->leftTreeSize[0] << endl << endl;
                if(curr->children[i] != nullptr){
                    //cout << curr->keys[i] << "'s rankVal before + 1 = " << rankVal + curr->leftTreeSize[i] << endl;
                    return rankVal + curr->leftTreeSize[i] + 1;
                }
                //cout << curr->keys[i] << "'s rankVal before + 1 = " << rankVal << endl;

                return rankVal + 1;
            }

            // Move to the children if not found
            curr = curr->children[i];
        }

       return 0;
    }
   
    int duplicates(keytype k){
         Node* curr = root;

        while(curr != nullptr){
            // Search for the keys in the curr node's keys
            int i = 0;
            while (i < curr->numKeys && k > curr->keys[i]) {
                i++;
            }

            // If key is found, return
            if (i < curr->numKeys && k == curr->keys[i]) {
                // for(int j = 0; j < 4; j++){
                //     cout << curr->keyArr[i].CDAs[j]<< endl;
                // }
                return curr->CDAs[i].length(); 
            }

            curr = curr->children[i];
        }
        return 0;
    }

    int size(){
        return treeSize;
    }

    void preorder(Node* curr){
        if(curr == nullptr) return;

        cout << curr->keys[0] << " ";
        if (curr->numKeys >= 2){
            cout << curr->keys[1] << " "; 
        }
        if (curr->numKeys == 3){
            cout << curr->keys[2] << " ";
        } 
        cout << endl;

        if(curr->numKeys == 1){
        preorder(curr->children[0]);
        preorder(curr->children[1]);
        }
        else if(curr->numKeys == 2){
            preorder(curr->children[0]);
            preorder(curr->children[1]);
            preorder(curr->children[2]);
        }
        else if(curr->numKeys == 3){
            preorder(curr->children[0]);
            preorder(curr->children[1]);
            preorder(curr->children[2]);
            preorder(curr->children[3]);
        }
    }
    void preorder(){
        preorder(root);
    }

    void inorder(){
        inorder(root);
        cout << endl;
    }
    void inorder(Node* curr){

        if(curr != nullptr){
            for(int i = 0; i < curr->numKeys; i++){
                inorder(curr->children[i]);
                // for(int j = 0; j < curr->CDAs[i].length(); j++){
                //     cout << curr->keys[j] << " ";
                // }
                cout << curr->keys[i] << " ";
                if(curr->CDAs[i].length() > 1){
                    for(int a = 1; a < curr->CDAs[i].length(); a++){
                        cout << curr->keys[i] << " ";
                    }
                }
            }
            inorder(curr->children[curr->numKeys]);
        }

    }

    void postorder(){
        postorderHelp(root);
    }
    void postorder(Node *curr){
        if(curr == nullptr) return;

        
        if(curr->numKeys == 1){
            postorderHelp(curr->children[0]);
            postorderHelp(curr->children[1]);
        }else if(curr->numKeys == 2){
            postorderHelp(curr->children[0]);
            postorderHelp(curr->children[1]);
            postorderHelp(curr->children[2]);
        }else if(curr->numKeys == 3){
            postorderHelp(curr->children[0]);
            postorderHelp(curr->children[1]);
            postorderHelp(curr->children[2]);
            postorderHelp(curr->children[3]);
        }
        for(int i = 0; i < curr->numKeys; i++){
            cout << curr->keys[i] << " ";
            //cout << curr->CDAs[i][0] << " ";
        }
        cout << endl;
    }

    int remove(keytype k){
        int check = removeHelper(root, k);
        if(check != 0){
        leftTreeUpdateRemove(k);
        }
        return check;
    }

    int removeHelper(Node* x, keytype k){
        if(x == root && x->children[0] == nullptr && x->numKeys == 1 && k == x->keys[0]){
            treeSize--;
            x->CDAs[0].clear();
            //x->numKeys = 0;
            root = nullptr;
            x = nullptr;
            return 1;
        }
        
        int i = 0;
        while (i < x->numKeys && k > x->keys[i]) {
            i++;
        }

        if(i == x->numKeys && i != 1){
            i--;
        }

        if(x == root && x->numKeys == 1 && x->keys[0] != k){
            if(x->children[0]->numKeys != 1 && x->children[1]->numKeys == 1 && k > x->keys[0]){
                x = rotateRight(x, x->children[0], x->children[1], i);
                return removeHelper(x, k);
            }
            else if(x->children[0]->numKeys == 1 && x->children[1]->numKeys != 1 && k < x->keys[0]){
                //cout << root->keys[0] << "'s call when root is 2 node" << endl << endl;
                x = rotateLeft(x, x->children[0], x->children[1], i);
                return removeHelper(x, k);
            }
            else if(x->children[0]->numKeys == 1 && x->children[1]->numKeys == 1){
                x = merge(x, x->children[0], x->children[1], i);
                return removeHelper(x, k);
            }
            else if(k > x->keys[0]){
                return removeHelper(x->children[1], k);
            }
            else if(k < x->keys[0]){
                return removeHelper(x->children[0], k);
            }

        }
        //not at farthest right child
        if(x->keys[i] != k && x->children[0] != nullptr && i != x->numKeys){
                if(x->children[i]->numKeys == 1 && x->children[i+1]->numKeys == 1){
                    x = merge(x, x->children[i], x->children[i+1], i);
                    return removeHelper(x, k);
                }
                if(x->children[i]->numKeys == 1 && x->children[i+1]->numKeys != 1){
                    if(k < x->keys[i]){
                        x = rotateLeft(x, x->children[i], x->children[i+1], i);
                        return removeHelper(x, k);
                    }
                    else if(k > x->keys[i]){
                        return removeHelper(x->children[i+1], k);
                    }
                }

                if(x->children[i]->numKeys != 1 && x->children[i+1]->numKeys == 1){
                    if(k > x->keys[i]){
                        x= rotateRight(x, x->children[i], x->children[i+1], i);
                        return removeHelper(x, k);
                    }
                    else if(k < x->keys[i]){
                        return removeHelper(x->children[i], k);
                    }
                }
                //if neither are 2 nodes
                if(x->children[i]->numKeys != 1 && x->children[i+1]->numKeys != 1){
                    if(k < x->keys[i]){
                        return removeHelper(x->children[i], k);
                    }
                    else if(k > x->keys[i]){
                        return removeHelper(x->children[i+1], k);
                    }
                }
        }
        //at far right child
        if(x->keys[i] != k && x->children[0] != nullptr && i == x->numKeys){
                if(x->children[i]->numKeys == 1 && x->children[i-1]->numKeys == 1){
                    x = merge(x, x->children[i], x->children[i-1], i);
                    return removeHelper(x, k);
                }
                if(x->children[i]->numKeys == 1 && x->children[i-1]->numKeys != 1){
                    if(k < x->keys[i]){
                        x = rotateLeft(x, x->children[i], x->children[i-1], i);
                        return removeHelper(x, k);
                    }
                    else if(k > x->keys[i]){
                        return removeHelper(x->children[i-1], k);
                    }
                }

                if(x->children[i]->numKeys != 1 && x->children[i-1]->numKeys == 1){
                    if(k > x->keys[i]){
                        x= rotateRight(x, x->children[i], x->children[i-1], i);
                        return removeHelper(x, k);
                    }
                    else if(k < x->keys[i]){
                        return removeHelper(x->children[i], k);
                    }
                }
                //if neither are 2 nodes
                if(x->children[i]->numKeys != 1 && x->children[i-1]->numKeys != 1){
                    if(k < x->keys[i]){
                        return removeHelper(x->children[i], k);
                    }
                    else if(k > x->keys[i]){
                        return removeHelper(x->children[i-1], k);
                    }
                }
        }
        //check for duplicates
        if(x->CDAs[i].length() > 1 && x->keys[i] == k){
            x->CDAs[i].delFront();
            treeSize--;
            return 1;
        }
        if(x->keys[i] == k){
            if(x->children[0] != nullptr){
                keytype pred = predecessor(x, x->children[i], i);
                //cout << pred << endl << endl;
                changeTreeFindingPred(root, pred);
                Node* predNode = searchNode(root, pred);
                int predIndex = 0;
                Node* keyNode = searchNode(root, k);
                int keyIndex = 0;
                for(int a = 0; a < predNode->numKeys; a++){
                    if(pred == predNode->keys[a]){
                        predIndex = a;
                        break;
                    }
                }
                for(int a = 0; a < keyNode->numKeys; a++){
                    if(k == keyNode->keys[a]){
                        keyIndex = a;
                        break;
                    }
                }
                // Node *temp = keyNode;
                // int tempIndex = keyIndex;
                keytype tempk = keyNode->keys[keyIndex];
                CircularDynamicArray<valuetype> tempCDA = keyNode->CDAs[keyIndex];
                int tempLeft = keyNode->leftTreeSize[keyIndex];

                keyNode->keys[keyIndex] = predNode->keys[predIndex];
                keyNode->CDAs[keyIndex] = predNode->CDAs[predIndex];
                keyNode->leftTreeSize[keyIndex] = predNode->leftTreeSize[predIndex];

                predNode->keys[predIndex] = tempk;
                predNode->CDAs[predIndex] = tempCDA;
                predNode->leftTreeSize[predIndex] = tempLeft;
                
                deleteKey(predNode, k, predIndex);
                return 1;
            }
            else if(x->children[0] == nullptr){
                deleteKey(x, k, i);
                return 1;
            }
        }

        return 0;
    }

    void changeTreeFindingPred(Node* x, keytype k){
        int i = 0;
        while (i < x->numKeys && k > x->keys[i]) {
            i++;
        }

        if(i == x->numKeys && i != 1){
            i--;
        }

        if(x == root && x->numKeys == 1 && x->keys[0] != k){
            if(x->children[0]->numKeys != 1 && x->children[1]->numKeys == 1 && k > x->keys[0]){
                x = rotateRight(x, x->children[0], x->children[1], i);
                return changeTreeFindingPred(x, k);
            }
            else if(x->children[0]->numKeys == 1 && x->children[1]->numKeys != 1 && k < x->keys[0]){
                //cout << root->keys[0] << "'s call when root is 2 node" << endl << endl;
                x = rotateLeft(x, x->children[0], x->children[1], i);
                return changeTreeFindingPred(x, k);
            }
            else if(x->children[0]->numKeys == 1 && x->children[1]->numKeys == 1){
                x = merge(x, x->children[0], x->children[1], i);
                return changeTreeFindingPred(x, k);
            }
            else if(k > x->keys[0]){
                return changeTreeFindingPred(x->children[1], k);
            }
            else if(k < x->keys[0]){
                return changeTreeFindingPred(x->children[0], k);
            }

        }
        //not at farthest right child
        if(x->keys[i] != k && x->children[0] != nullptr && i != x->numKeys){
                if(x->children[i]->numKeys == 1 && x->children[i+1]->numKeys == 1){
                    x = merge(x, x->children[i], x->children[i+1], i);
                    return changeTreeFindingPred(x, k);
                }
                if(x->children[i]->numKeys == 1 && x->children[i+1]->numKeys != 1){
                    if(k < x->keys[i]){
                        x = rotateLeft(x, x->children[i], x->children[i+1], i);
                        return changeTreeFindingPred(x, k);
                    }
                    else if(k > x->keys[i]){
                        return changeTreeFindingPred(x->children[i+1], k);
                    }
                }

                if(x->children[i]->numKeys != 1 && x->children[i+1]->numKeys == 1){
                    if(k > x->keys[i]){
                        x= rotateRight(x, x->children[i], x->children[i+1], i);
                        return changeTreeFindingPred(x, k);
                    }
                    else if(k < x->keys[i]){
                        return changeTreeFindingPred(x->children[i], k);
                    }
                }
                //if neither are 2 nodes
                if(x->children[i]->numKeys != 1 && x->children[i+1]->numKeys != 1){
                    if(k < x->keys[i]){
                        return changeTreeFindingPred(x->children[i], k);
                    }
                    else if(k > x->keys[i]){
                        return changeTreeFindingPred(x->children[i+1], k);
                    }
                }
        }
        //at far right child
        if(x->keys[i] != k && x->children[0] != nullptr && i == x->numKeys){
                if(x->children[i]->numKeys == 1 && x->children[i-1]->numKeys == 1){
                    x = merge(x, x->children[i], x->children[i-1], i);
                    return changeTreeFindingPred(x, k);
                }
                if(x->children[i]->numKeys == 1 && x->children[i-1]->numKeys != 1){
                    if(k < x->keys[i]){
                        x = rotateLeft(x, x->children[i], x->children[i-1], i);
                        return changeTreeFindingPred(x, k);
                    }
                    else if(k > x->keys[i]){
                        return changeTreeFindingPred(x->children[i-1], k);
                    }
                }

                if(x->children[i]->numKeys != 1 && x->children[i-1]->numKeys == 1){
                    if(k > x->keys[i]){
                        x= rotateRight(x, x->children[i], x->children[i-1], i);
                        return changeTreeFindingPred(x, k);
                    }
                    else if(k < x->keys[i]){
                        return changeTreeFindingPred(x->children[i], k);
                    }
                }
                //if neither are 2 nodes
                if(x->children[i]->numKeys != 1 && x->children[i-1]->numKeys != 1){
                    if(k < x->keys[i]){
                        return changeTreeFindingPred(x->children[i], k);
                    }
                    else if(k > x->keys[i]){
                        return changeTreeFindingPred(x->children[i-1], k);
                    }
                }
        }
        // //check for duplicates
        // if(x->CDAs[i].length() > 1){
        //     x->CDAs[i].delFront();
        //     return;
        // }
        if(x->keys[i] == k){
            return;
        }

        return;
    }

    Node *searchNode(Node* curr, keytype k){
        curr = root;

        while (curr != nullptr) {
            // Search for the keys in the curr node's keys
            int i = 0;
            while (i < curr->numKeys && k > curr->keys[i]) {
                i++;
            }

            // If key is found, return
            if (i < curr->numKeys && k == curr->keys[i]) {
                //cout << curr->keys[i] << " " << "i = " << i << "    ";
                return curr;
            }

            // Move to the children if not found
            curr = curr->children[i];
        }

        // If the loop terminates without finding the keys and without hitting a leaf node, return nullptr
        return nullptr;
    }

    void deleteKey(Node* x, keytype k, int i){
        // if(x->CDAs[i].length() > 1){
        //     x->CDAs[i].delFront();
        //     return;
        // }
        // if(x->numKeys == 1 && x == root && x->children[0] == nullptr){
        //     cout << "------------------";
        //     x->numKeys--;
        //     x = nullptr;
        //     return;
        // }

        if(x->keys[x->numKeys - 1] == k){
            x->numKeys--;
            x->CDAs[x->numKeys-1].clear();
        }
        else if(x->keys[1] == k && x->numKeys == 3){
            x->keys[1] = x->keys[2];
            x->CDAs[1] = x->CDAs[2];
            x->leftTreeSize[1] = x->leftTreeSize[2];
            x->numKeys--;
            x->CDAs[2].clear();
        }
        else if(x->keys[0] == k){
            for(int a = 0; a < x->numKeys - 1; a++){
                x->keys[a] = x->keys[a+1];
                x->CDAs[a] = x->CDAs[a+1];
                x->leftTreeSize[a] = x->leftTreeSize[a+1];
            }
            x->numKeys--;
            //x->CDAs[x->numKeys-1].clear();
        }
        treeSize--;
    }

    keytype predecessor(Node* x, Node* y, int i){
        while(y->children[y->numKeys] != nullptr){
            y = y->children[y->numKeys];
        }


        return y->keys[y->numKeys-1];
    }

    Node* merge(Node* x, Node* y, Node* z, int i){
        Node* m = new Node(false); //this will be the current node back in removeHelper
        m->keys[0] = y->keys[0];
        m->CDAs[0] = y->CDAs[0];
        m->leftTreeSize[0] = y->leftTreeSize[0];

        m->keys[1] = x->keys[i];
        m->CDAs[1] = x->CDAs[i];
        m->leftTreeSize[1] = x->leftTreeSize[i] - y->leftTreeSize[0] - y->CDAs[0].length(); 
        //not sure if you need to do -1. do test cases

        m->keys[2] = z->keys[0];
        m->CDAs[2] = z->CDAs[0];
        m->leftTreeSize[2] = z->leftTreeSize[0];

        //move all children from y and z to new node
        m->children[0] = y->children[0];
        m->children[1] = y->children[1];
        m->children[2] = z->children[0];
        m->children[3] = z->children[1];
        m->numKeys = 3;
        if(x == root && x->numKeys == 1){
            root = m;
            return m;
        }
        
        //shift keys and children from node that merged down
        //x->numKeys--;
        for(int a = i; a < x->numKeys-1; a++){
            x->keys[a] = x->keys[a+1];
            x->CDAs[a] = x->CDAs[a+1];
            x->leftTreeSize[a] = x->leftTreeSize[a+1];
        }
        x->leftTreeSize[i] = x->leftTreeSize[i] + m->leftTreeSize[0] + m->CDAs[0].length() + m->leftTreeSize[1] + m->CDAs[1].length(); 
        x->CDAs[x->numKeys-1].clear();

        //set new node to be child at index
        x->children[i] = m;
        for(int a = i + 1; a < x->numKeys; a++){
            x->children[a] = x->children[a+1];
        }
        // x->keys[x->numKeys-1] = x->keys[x->numKeys];
        // x->CDAs[x->numKeys-1] = x->CDAs[x->numKeys];
        // x->children[x->numKeys] = x->children[x->numKeys+1];
        // x->children[x->numKeys-1] = m;

        x->numKeys--;
        //cout << "D's left tree size: " << x->leftTreeSize[0] << endl << endl;

        return m; 
    }

    Node* rotateLeft(Node* x, Node* y, Node* z, int i){
        //move x's first key down to y node
        y->keys[1] = x->keys[i];
        y->CDAs[1] = x->CDAs[i];
        y->leftTreeSize[1] = x->leftTreeSize[i] - y->leftTreeSize[0] - y->CDAs[0].length();
        //cout << "This is D's left sub tree size: " << y->leftTreeSize[1] << endl << endl;

        //shift x's keys over
        // for(int a = 0; a < x->numKeys-1; a++){
        //     x->keys[a] = x->keys[a+1];
        //     x->CDAs[a] = x->CDAs[a+1];
        // }
        //shift left key of z of to last key of x
        x->keys[i] = z->keys[0];
        x->CDAs[i] = z->CDAs[0];
        x->leftTreeSize[i] = y->leftTreeSize[0] + y->CDAs[0].length() + y->leftTreeSize[1] + y->CDAs[1].length() + z->leftTreeSize[0];
        x->leftTreeSize[i+1] -= z->leftTreeSize[0] + z->CDAs[0].length();
        //cout << "F's subtree size " << x->leftTreeSize[x->numKeys-1] << endl << endl;


        //shift z's values over
        for(int a = 0; a < z->numKeys-1; a++){
            z->keys[a] = z->keys[a+1];
            z->CDAs[a] = z->CDAs[a+1];
            z->leftTreeSize[a] = z->leftTreeSize[a+1];
        }
        y->children[2] = z->children[0];
        for(int a = 0; a < z->numKeys; a++){
            z->children[a] = z->children[a+1];
        }

        z->numKeys--;
        y->numKeys++;
        return x;
    }

    Node* rotateRight(Node* x, Node* y, Node* z, int i){
        //move z's values over to make room for rotated value
        //cout << "------------------" << i << endl;
        if(i == 1 && x->numKeys == 1) i = i-1;
        z->keys[1] = z->keys[0];
        z->CDAs[1] = z->CDAs[0];
        z->leftTreeSize[1] = z->leftTreeSize[0];
        z->keys[0] = x->keys[i];
        z->CDAs[0] = x->CDAs[i];
        int temp = 0;
        for(int a = 0; a < y->numKeys; a++){
            temp += y->leftTreeSize[a] + y->CDAs[a].length();
        }
        z->leftTreeSize[0] = x->leftTreeSize[i] - temp;
        // for(int a = 0; a < y->numKeys; a++){

        // }

        //move z's children over to make room for z's first child to be the last child of y
        z->children[2] = z->children[1];
        z->children[1] = z->children[0];
        z->children[0] = y->children[y->numKeys];
        //shift x's values over
        for(int a = x->numKeys-1; a > 0; a--){
            x->keys[a] = x->keys[a-1];
            x->CDAs[a] = x->CDAs[a-1];
            x->leftTreeSize[a] = x->leftTreeSize[a-1];
        }
        //shift far right key of y up to first key of x
        x->keys[i] = y->keys[y->numKeys - 1];
        x->CDAs[i] = y->CDAs[y->numKeys - 1];
        x->leftTreeSize[i] = temp - y->CDAs[y->numKeys-1].length();

        //z->leftTreeSize[0] = x->leftTreeSize[i] - temp;
        // for(int a = 0; a < y->numKeys; a++){
            
        // }

        z->numKeys++;
        y->CDAs[y->numKeys-1].clear();
        y->numKeys--;



        //cout << "W's left tree size: " <<temp << endl << endl;

        return x;
    }

    void leftTreeUpdateInsert(keytype k){
        Node* curr = root;

        while (curr != nullptr) {
            int i = 0;
            while (i < curr->numKeys && k > curr->keys[i]) {
                i++;
            }
            
            //if not key, increase left tree size by 1
            if(i < curr->numKeys && k != curr->keys[i]){
                curr->leftTreeSize[i] += 1;
                //cout << "------------";
            }

            // If key is found, return to stop transversing
            if (i < curr->numKeys && k == curr->keys[i]) {
                break;
            }

            curr = curr->children[i];
        }
    }

    void leftTreeUpdateRemove(keytype k){
         Node * curr = root;

        while (curr != nullptr) {
            int i = 0;
            while (i < curr->numKeys && k > curr->keys[i]) {
                i++;
            }
            
            //if not key, increase left tree size by 1
            if(i < curr->numKeys && k != curr->keys[i]){
                curr->leftTreeSize[i] -= 1;
                if(curr->leftTreeSize[i] < 0) curr->leftTreeSize[i] = 0;
                //cout << "------------";
            }

            // If key is found, return to stop transversing
            if (i < curr->numKeys && k == curr->keys[i]) {
                break;
            }

            curr = curr->children[i];
        }
    }

    //copy constructor
    two4Tree(const two4Tree &old) {          
        root = nullptr;             
         
        treeSize = 0;
        rankVal = old.rankVal;
        copyTree(old.root, root);        
    }

    two4Tree &operator=(const two4Tree &other) {
        if (this != &other) {
            destroyTree(root);          
            root = nullptr;             
            copyTree(other.root, root); 
            treeSize = other.treeSize;
            rankVal = other.rankVal;
        }
        return *this;
    }
    void copyTree(const Node *src, Node *&dest) {
        if (src == nullptr)
            return;

        dest = new Node(src->leaf);
        dest->numKeys = src->numKeys;
        dest->leaf = src->leaf;
        for (int i = 0; i < src->numKeys; ++i) {
            dest->keys[i] = src->keys[i];
            //dest->keyCount[i] = src->keyCount[i];
            dest->CDAs[i] = src->CDAs[i];
            dest->leftTreeSize[i] = src->leftTreeSize[i];
        }
        dest->leftTreeSize[src->n] = src->leftTreeSize[src->n];

        for (int i = 0; i < src->n + 1; ++i){
            copyTree(src->children[i], dest->children[i]);
        }
    }

    ~two4Tree() {
        destroyTree(root);
    }

    void destroyTree(Node* x){
        if(x != nullptr){
            for(int i = 0; i < x->numKeys + 1; ++i){
                destroyTree(x->children[i]);
            }
            delete x;
        }
    }

};
