#include <iostream>
#include "GatorAVLTree.h"

// default constructor
GatorAVLTree::GatorAVLTree()
{
    root = nullptr;
}

#pragma region MEMORY MANAGEMENT
// destructor
GatorAVLTree::~GatorAVLTree()
{
    // should delete all dynamically allocated memory assuming that all created roots were properly assigned at some point as children of root and none were lost
    // PostorderTraversalDeletion(root);
}
#pragma endregion

#pragma region REQUIRED FUNCTIONS
// insert new node with input name and id
void GatorAVLTree::Insert(string name, unsigned int id)
{
    stack<StudentNode*> searchPath;

    // recursive insert helper function that updates current root and height values for each recursive iteration - also balances tree as necessary
    if(RecursiveInsert(root, 0, name, id, searchPath))
    {
        // ensure the tree is balanced
        while(!searchPath.empty())
        {
            // incrememnt current node
            StudentNode* currNode = searchPath.top();
            searchPath.pop();
            
            // balance section with currNode as root
            currNode = Balance(currNode);
            
            if(!searchPath.empty()) // ensures currNode is not the root
            {
                // assign balanced subtree as proper child of node above it
                if(currNode->id < searchPath.top()->id)
                    searchPath.top()->left = currNode;
                else
                    searchPath.top()->right = currNode;   
            }
            else
            {
                root = currNode; // ensures root is properly reassigned if this was the root, and it was rotated
            }

            // update height of each node in search path based on re-balanced arrangement of child nodes
            UpdateHeight(currNode);
        }

        // success!
        cout << "successful";
    }
    else
    {
        // failure!
        cout << "unsuccessful";
    }
}

// remove node with input id
void GatorAVLTree::Remove(unsigned int searchID)
{
    stack<StudentNode*> searchPath;

    // find node to remove (and also its parent node)
    StudentNode* nodeToRemove = nullptr;
    StudentNode* parent = nullptr;
    RecursiveFindNode(root, searchID, nodeToRemove, parent, searchPath);

    // failure if node with searchID is not found in tree
    if(nodeToRemove == nullptr)
    {
        cout << "unsuccessful";
    }
    else
    {   
        // determine if nodeToRemove is left or right of parent
        bool leftOfParent = false;
        if(parent != nullptr && (parent->left != nullptr && parent->left->id == nodeToRemove->id))
            leftOfParent = true;

        // removing with no children
        if(nodeToRemove->left == nullptr && nodeToRemove->right == nullptr)
        {
            // deallocate nodeToRemove
            delete nodeToRemove;

            // make sure parent no longer points to nodeToRemove
            if(parent != nullptr  && leftOfParent)
            {
                parent->left = nullptr;
            }
            else if(parent != nullptr && !leftOfParent)
            {
                parent->right = nullptr;
            }
            else if(parent == nullptr)
            {
                // set root to nullptr because this means that the root was deleted
                root = nullptr;
            }

            // update heights along search path
            while(!searchPath.empty())
            {
                UpdateHeight(searchPath.top());
                searchPath.pop();
            }
        }
        // removing with one child (right)
        else if(nodeToRemove->left == nullptr)
        {
            // make sure parent points to the one child
            if(parent != nullptr  && leftOfParent)
            {
                parent->left = nodeToRemove->right;
            }
            else if(parent != nullptr && !leftOfParent)
            {
                parent->right = nodeToRemove->right;
            }
            else if (parent == nullptr)
            {
                // set root to one child because the root was deleted
                root = nodeToRemove->right;
            }

            // deallocate nodeToRemove
            delete nodeToRemove;

            // update heights along search path
            while(!searchPath.empty())
            {
                UpdateHeight(searchPath.top());
                searchPath.pop();
            }
        }
        // removing with one child (left)
        else if(nodeToRemove->right == nullptr)
        {
            // make sure parent points to the one child
            if(parent != nullptr  && leftOfParent)
            {
                parent->left = nodeToRemove->left;
            }
            else if(parent != nullptr && !leftOfParent)
            {
                parent->right = nodeToRemove->left;
            }
            else if(parent == nullptr)
            {
                // set root to one child because the root was deleted
                root = nodeToRemove->left;
            }

            // deallocate nodeToRemove
            delete nodeToRemove;

            // update heights along search path
            while(!searchPath.empty())
            {
                UpdateHeight(searchPath.top());
                searchPath.pop();
            }
        }
        // removing with two children (hard case)
        else
        {
            // find inorder successor
            StudentNode* inorderSuccessor = nodeToRemove->right;
            StudentNode* inorderSuccessorParent = nullptr;
            GetInorderSuccessor(nodeToRemove->right, inorderSuccessor, inorderSuccessorParent);

            // make sure parent points to the inorder successor
            if(parent != nullptr  && leftOfParent)
            {
                parent->left = inorderSuccessor;
            }
            else if(parent != nullptr && !leftOfParent)
            {
                parent->right = inorderSuccessor;
            }
            else if(parent == nullptr)
            {
                // set root to inorder successor because the root was deleted
                root = inorderSuccessor;
            }

            // prevents floating nodes lost to the right of inorderSuccessor
            if(inorderSuccessorParent != nullptr) // if inorderSuccessorParent is nullptr then inorder successor was child of nodeToDelete and the parent is null - so no assignment should be done
                inorderSuccessorParent->left = inorderSuccessor->right;
            // connects inorderSuccessor to nodes left of original nodeToRemove
            inorderSuccessor->left = nodeToRemove->left;
            // connects inorderSuccessor to nodes right of original nodeToRemove
            if(nodeToRemove->right != inorderSuccessor) // prevents inorderSuccessor from pointing to itself
                inorderSuccessor->right = nodeToRemove->right;

            // deallocate nodeToRemove
            delete nodeToRemove;

            // update heights of everything starting at and below inorderSuccessor
            RecursiveUpdateHeight(inorderSuccessor);

            // update heights along search path (AFTER everything at inorder successor and below has been updated)
            while(!searchPath.empty())
            {
                UpdateHeight(searchPath.top());
                searchPath.pop();
            }
        }

        cout << "successful";
    }
}

// search for node with input id and print name
void GatorAVLTree::Search(unsigned int searchID)
{
    // conduct recursive search for name
    string name = RecursiveSearchID(root, searchID);

    // check for name not found case
    if(name == "-1")
        cout << "unsuccessful";
    else
        cout << name;
}

// search for node with input name and print id
void GatorAVLTree::Search(string searchName)
{
    // create print queue and recursively search for all matching names
    queue<int> printQueue;
    PreorderTraversalSearchName(root, searchName, printQueue);

    // no matching names found
    if(printQueue.empty())
        cout << "unsuccessful";

    // each ID from the print queue
    while(!printQueue.empty())
    {   
        // convert next print queue element to string for parsing
        string printStr = to_string(printQueue.front());

        // remove printed element from print queue
        printQueue.pop();

        // print a "0" for each leading zero the ID, then print the rest of the ID number
        for(int i = 8 - printStr.length(); i > 0 ; i--)
        {
            cout << "0";
        }
        cout << printStr;

        // do not print end line character on final ID in print queue
        if(!printQueue.empty())
            cout << endl;
        
        // no additional formatting required since each ID is printed on a new line
    }
}

// print comma separated inorder traversal of names
void GatorAVLTree::PrintInorder()
{
    // conduct revursive traversal (fill print queue)
    queue<string> printQueue;
    InorderTraversalRecursive(root, printQueue);

    // each name from the print queue
    while(!printQueue.empty())
    {
        // print element
        cout << printQueue.front();
        // remove printed element from queue
        printQueue.pop();
        // print separator ONLY if this was not the last element
        if(!printQueue.empty()) cout << ", ";
    }

    cout << ""; // prints empty string to ensure some output (may not matter)
}

// print comma separated preorder traversal of names
void GatorAVLTree::PrintPreorder()
{
    // conduct revursive traversal (fill print queue)
    queue<string> printQueue;
    PreorderTraversalRecursive(root, printQueue);

    // each name from the print queue
    while(!printQueue.empty())
    {
        // print element
        cout << printQueue.front();
        // remove printed element from queue
        printQueue.pop();
        // print separator ONLY if this was not the last element
        if(!printQueue.empty()) cout << ", ";
    }

    cout << ""; // prints empty string to ensure some output (may not matter)
}

// print comma separated postorder traversal of names
void GatorAVLTree::PrintPostorder()
{
    // conduct revursive traversal (fill print queue)
    queue<string> printQueue;
    PostorderTraversalRecursive(root, printQueue);

    // each name from the print queue
    while(!printQueue.empty())
    {
        // print element
        cout << printQueue.front();
        // remove printed element from queue
        printQueue.pop();
        // print separator ONLY if this was not the last element
        if(!printQueue.empty()) cout << ", ";
    }

    cout << ""; // prints empty string to ensure some output (may not matter)
}

// print number of levels in the tree
void GatorAVLTree::PrintLevelCount()
{
    if(root == nullptr)
        cout << "0";
    else
        cout << root->height;
}

// removes the Nth node from the inorder traversal
void GatorAVLTree::RemoveInorder(unsigned int n)
{
    // generate stack of elements with Nth element at the top
    stack<unsigned int> idStack;
    RecursiveGetNthID(root, n, idStack);

    // there is no Nth ID (size too small)
    if(n >= idStack.size())
    {
        cout << "unsuccessful";
    }
    else
    {
        // call remove function for the ID corresponding with the top of the stack (Nth node)
        Remove(idStack.top());
    }
}
#pragma endregion

#pragma region HELPER FUNCTIONS
// recursively searches for the location to insert - returns whether insertion was successful
bool GatorAVLTree::RecursiveInsert(StudentNode*& currentRoot, int currHeight, string name, unsigned int id, stack<StudentNode*>& searchPath)
{
    // base case where the inserted node is set to the location of the current nullptr node
    if(currentRoot == nullptr)
    {
        // create new node to insert at current node (leaf of tree)
        currentRoot = new StudentNode(id, name);
        return true; // indicates a successful insert
    }
    // add to search path (does not add leaf node)
    searchPath.push(currentRoot);
    // newNode less than current root
    if(id < currentRoot->id)
    {
        return RecursiveInsert(currentRoot->left, currHeight+1, name, id, searchPath);
    }
    // newNode greater than current root
    if(id > currentRoot->id)
    {
        return RecursiveInsert(currentRoot->right, currHeight+1, name, id, searchPath);
    }
    
    // newNode and currentNode have the same id (unsuccessful since input ID must be unique)
    return false; // indicates an unsucessful insert
}

// rotation to conduct in left left imbalance case
GatorAVLTree::StudentNode* GatorAVLTree::RightRotation(StudentNode*& root)
{
    // conduct rotation
    StudentNode* grandchild = root->left->right;
    StudentNode* newParent = root->left;
    newParent->right = root;
    root->left = grandchild;

    // update heights
    UpdateHeight(root);

    // returns parent for the sake of ensuring that the parent of the rotation can properly be reassigned within right left rotation function
    return newParent; 
}

// rotation to conduct in right right imbalance case
GatorAVLTree::StudentNode* GatorAVLTree::LeftRotation(StudentNode*& root)
{
    // conduct rotation
    StudentNode* grandchild = root->right->left;
    StudentNode* newParent = root->right;
    newParent->left = root;
    root->right = grandchild;

    // update heights
    UpdateHeight(root);
    // update height of new parent

    // returns parent for the sake of ensuring that the parent of the rotation can properly be reassigned within left right rotation function
    return newParent;
}

// rotation to conduct in left right imbalance case
GatorAVLTree::StudentNode* GatorAVLTree::LeftRightRotation(StudentNode*& root)
{
    root->left = LeftRotation(root->left);
    return RightRotation(root);
}

// rotation to conduct in right left imbalance case
GatorAVLTree::StudentNode* GatorAVLTree::RightLeftRotation(StudentNode*& root)
{
    root->right = RightRotation(root->right);
    return LeftRotation(root);
}

// checks balance state and calls appropriate rotation methods to balance on a single node
GatorAVLTree::StudentNode* GatorAVLTree::Balance(StudentNode*& node)
{   
    // check for imbalance (from bottom up) as the recursive calls return back towards the root
    if((node->left == nullptr ? 0 : node->left->height) - (node->right == nullptr ? 0 : node->right->height) == 2) // Left imbalance
    {
        if((node->left == nullptr || node->left->left == nullptr ? 0 : node->left->left->height) - (node->left == nullptr || node->left->right == nullptr ? 0 : node->left->right->height) == 1) // Left Left Imbalance
        {
            return RightRotation(node);
        }
        else // Left Right imbalance
        {
            return LeftRightRotation(node);
        }
    }
    else if((node->left == nullptr ? 0 : node->left->height) - (node->right == nullptr ? 0 : node->right->height) == -2) // Right imbalance
    {
        if((node->right == nullptr || node->right->left == nullptr ? 0 : node->right->left->height) - (node->right == nullptr || node->right->right == nullptr ? 0 : node->right->right->height) == -1) // Right Right imbalance
        {
            return LeftRotation(node);
        }
        else // Right Left imbalance
        {
            return RightLeftRotation(node);
        }
    }
    return node;
}

// updates a node's height based on the heights of its children
void GatorAVLTree::UpdateHeight(StudentNode* node)
{
    // get value for left height
    int leftHeight;
    node->left == nullptr ? leftHeight = 0 : leftHeight = node->left->height;
    // get value for right height
    int rightHeight;
    node->right == nullptr? rightHeight = 0 : rightHeight = node->right->height;

    // set value of current node's height
    node->height = 1 + max(leftHeight, rightHeight);
}

// conducts inorder traversal recursively
void GatorAVLTree::InorderTraversalRecursive(StudentNode* currNode, queue<string>& printQueue)
{
    if(currNode!=nullptr)
    {
        InorderTraversalRecursive(currNode->left, printQueue);
        printQueue.push(currNode->name);
        InorderTraversalRecursive(currNode->right, printQueue);
    }
}

// conducts preorder traversal recursively
void GatorAVLTree::PreorderTraversalRecursive(StudentNode* currNode, queue<string>& printQueue)
{
    if(currNode!=nullptr)
    {
        printQueue.push(currNode->name);
        PreorderTraversalRecursive(currNode->left, printQueue);
        PreorderTraversalRecursive(currNode->right, printQueue);
    }
}

// conducts postorder traversal recursively
void GatorAVLTree::PostorderTraversalRecursive(StudentNode* currNode, queue<string>& printQueue)
{
    if(currNode!=nullptr)
    {
        PostorderTraversalRecursive(currNode->left, printQueue);
        PostorderTraversalRecursive(currNode->right, printQueue);
        printQueue.push(currNode->name);
    }
}

// conducts postorder traversal recursively and deletes each element's dynamically allocated data
void GatorAVLTree::PostorderTraversalDeletion(StudentNode* currNode)
{
    if(currNode!=nullptr)
    {
        PostorderTraversalDeletion(currNode->left);
        PostorderTraversalDeletion(currNode->right);
        delete currNode;
    }
}

// recursively searches (binary search) by ID and returns the name of the match
string GatorAVLTree::RecursiveSearchID(StudentNode* currNode, unsigned int searchID)
{
    // reached leaf and id not found
    if(currNode == nullptr)
        return "-1"; // indicates failure to find node
    
    // reached node with searchID
    if(searchID == currNode->id)
        return currNode->name;

    // search ID must be left of current node
    if(searchID < currNode->id)
        return RecursiveSearchID(currNode->left, searchID);

    // search ID must be right of current node
    if(searchID > currNode->id)
        return RecursiveSearchID(currNode->right, searchID);

    // required to avoid no return warning, should never be called because above cases cover all cases (could remove one condition above and have this be default exit state)
    return "-1";
}

// conducts postorder traversal recursively to find nodes with searchName
void GatorAVLTree::PreorderTraversalSearchName(StudentNode* currNode, string searchName, queue<int>& printQueue)
{
    if(currNode!=nullptr)
    {
        if(searchName == currNode->name) printQueue.push(currNode->id); // add ID to print queue if it has matching name
        PreorderTraversalSearchName(currNode->left, searchName, printQueue);
        PreorderTraversalSearchName(currNode->right, searchName, printQueue);
    }
}

// recursively searches (binary search) by ID and returns node at match
void GatorAVLTree::RecursiveFindNode(StudentNode* currNode, unsigned int searchID, StudentNode*& nodeToRemove, StudentNode*& parent, stack<StudentNode*>& searchPath)
{
    // reached leaf and id not found
    if(currNode == nullptr)
        return; // indicates failure to find node

    // reached node with searchID
    if(searchID == currNode->id)
        nodeToRemove = currNode;

    // add current node to search path queue for height update after this recursive function ends - does not push nullptr OR found node (because found node will be deleted and thus not in search path)
    searchPath.push(currNode);

    // search ID must be left of current node
    if(searchID < currNode->id)
    {
        parent = currNode;
        RecursiveFindNode(currNode->left, searchID, nodeToRemove, parent, searchPath);
    }

    // search ID must be right of current node
    if(searchID > currNode->id)
    {
        parent = currNode;
        RecursiveFindNode(currNode->right, searchID, nodeToRemove, parent, searchPath);
    }
}

// finds inorder successor (of node with two children), starting at the node's right child (should be first input)
void GatorAVLTree::GetInorderSuccessor(StudentNode* currNode, StudentNode*& inorderSuccessor, StudentNode*& inorderSucessorParent)
{
    if(currNode != nullptr && currNode->left != nullptr)
    {
        inorderSucessorParent = currNode;
        inorderSuccessor = currNode->left;
        GetInorderSuccessor(currNode->left, inorderSuccessor, inorderSucessorParent);
    }
}

// conducts inorder traversal, stopping at the targetIndex and returning the ID
void GatorAVLTree::RecursiveGetNthID(StudentNode* currNode, unsigned int n, stack<unsigned int>& idStack)
{
    if(currNode!=nullptr)
    {
        RecursiveGetNthID(currNode->left, n, idStack);
        // only add to stack if the top of the stack is not the Nth node
        if(idStack.size() - 1 != n)
            idStack.push(currNode->id);
        RecursiveGetNthID(currNode->right, n, idStack);
    }
}

// recursively updates height of current node and everything below it (by postorder traversal)
void GatorAVLTree::RecursiveUpdateHeight(StudentNode* currNode)
{
    if(currNode!=nullptr)
    {
        RecursiveUpdateHeight(currNode->left);
        RecursiveUpdateHeight(currNode->right);
        UpdateHeight(currNode);
    }
}
#pragma endregion