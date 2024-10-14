#include <string>
#include <stack>
#include <queue>
using namespace std;

class GatorAVLTree
{
    public:

    struct StudentNode
    {
        unsigned int id;
        string name;
        StudentNode* left;
        StudentNode* right;
        int height;

        StudentNode(unsigned int id, string name)
        {
            this->id = id;
            this->name = name;
            left = nullptr;
            right = nullptr;
            height = 1;
        }
    };

    // default constructor
    GatorAVLTree();

    // MEMORY MANAGEMENT
    // destructor
    ~GatorAVLTree();

    // REQUIRED FUNCTIONS
    // insert new node with input name and id
    void Insert(string name, unsigned int id);
    // remove node with input id
    void Remove(unsigned int searchID);
    // search for node with input id and print name
    void Search(unsigned int searchID);
    // search for node with input name and print id
    void Search(string searchName);
    // print comma separated inorder traversal of names
    void PrintInorder();
    // print comma separated preorder traversal of names
    void PrintPreorder();
    // print comma separated postorder traversal of names
    void PrintPostorder();
    // print number of levels in the tree
    void PrintLevelCount();
    // removes the Nth node from the inorder traversal
    void RemoveInorder(unsigned int n);

    private:

    // root node
    StudentNode* root;

    // HELPER FUNCTIONS
    // recursively searches for the location to insert - returns whether insertion was successful
    bool RecursiveInsert(StudentNode*& currentRoot, int currHeight, string name, unsigned int id, stack<StudentNode*>& searchPath);
    // rotation to conduct in left left imbalance case
    StudentNode* RightRotation(StudentNode*& root);
    // rotation to conduct in right right imbalance case
    StudentNode* LeftRotation(StudentNode*& root);
    // rotation to conduct in left right imbalance case
    StudentNode* LeftRightRotation(StudentNode*& root);
    // rotation to conduct in right left imbalance case
    StudentNode* RightLeftRotation(StudentNode*& root);
    // checks balance state and calls appropriate rotation methods to balance on a single node
    StudentNode* Balance(StudentNode*& node);
    // updates a node's height based on the heights of its children
    void UpdateHeight(StudentNode* node);
    // conducts inorder traversal recursively
    void InorderTraversalRecursive(StudentNode* currNode, queue<string>& printQueue);
    // conducts preorder traversal recursively
    void PreorderTraversalRecursive(StudentNode* currNode, queue<string>& printQueue);
    // conducts postorder traversal recursively
    void PostorderTraversalRecursive(StudentNode* currNode, queue<string>& printQueue);
    // conducts postorder traversal recursively and deletes each element's dynamically allocated data
    void PostorderTraversalDeletion(StudentNode* currNode);
    // recursively searches (binary search) by ID and returns the name of the match
    string RecursiveSearchID(StudentNode* currNode, unsigned int searchID);
    // conducts postorder traversal recursively to find nodes with searchName
    void PreorderTraversalSearchName(StudentNode* currNode, string searchName, queue<int>& printQueue);
    // recursively searches (binary search) by ID and returns node at match
    void RecursiveFindNode(StudentNode* currNode, unsigned int searchID, StudentNode*& nodeToRemove, StudentNode*& parent, stack<StudentNode*>& searchPath);
    // finds inorder successor (of node with two children), starting at the node's right child (should be first input)
    void GetInorderSuccessor(StudentNode* currNode, StudentNode*& inorderSuccessor, StudentNode*& inorderSucessorParent);
    // conducts inorder traversal, stopping at the targetIndex and returning the ID
    void RecursiveGetNthID(StudentNode* currNode, unsigned int n, stack<unsigned int>& idStack);
    // recursively updates height of current node and everything below it (by postorder traversal)
    void RecursiveUpdateHeight(StudentNode* currNode);
};