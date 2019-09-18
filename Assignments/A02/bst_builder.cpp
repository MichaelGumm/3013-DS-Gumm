/*
Name: Michael Gumm
Date: 9/17/2019
Class: 3013-alg
Description: This program reads in values from an input file, storing
them in a vector. It then sorts the vector in numerical ascending order.
Finally, it uses the vector to create a binary tree, reading in the 
values in such a way that the value of the left child is less than the
parent, and the value of the right child is greater than the parent
value. It does this by locating the median value of the vector, and
inserting it into the tree. It then continues this process until every
value in the vector has been inserted. After the binary tree has been
created, it prints a diagram of the tree to the screen. It also prints
data to an outfile, which can be used to create a graphic at 
http://www.webgraphviz.com/.

*/
#include <iostream>

#include <fstream>

#include <string>

#include <vector>



//http://www.webgraphviz.com/



using namespace std;



struct node

{

	int data;

	node* left;

	node* right;

	node()

	{

		data = -1;

		left = NULL;

		right = NULL;

	}

	node(int x)

	{

		data = x;

		left = NULL;

		right = NULL;

	}

};



class BSTree

{

private:

	node* root;



	int count(node* root)

	{

		if (!root)

		{

			return 0;

		}

		else

		{

			return 1 + count(root->left) + count(root->right);

		}

	}



	void insert(node*& root, node*& temp)

	{
		if (temp == NULL)
			return;
		else if (!root)

		{

			root = temp;

		}
	

		else

		{
			if (temp->data < root->data)

			{

				insert(root->left, temp);

			}

			else

			{

				insert(root->right, temp);

			}

		}

	}



	void print_node(node* n, string label = "")

	{

		if (label != "")

		{

			cout << "[" << label << "]";

		}

		cout << "[[" << n << "][" << n->data << "]]\n";

		if (n->left)

		{

			cout << "\t|-->[L][[" << n->left << "][" << n->left->data << "]]\n";

		}

		else

		{

			cout << "\t\\-->[L][null]\n";

		}

		if (n->right)

		{

			cout << "\t\\-->[R][[" << n->right << "][" << n->right->data << "]]\n";

		}

		else

		{

			cout << "\t\\-->[R][null]\n";

		}

	}



	/**

	 * type = ['predecessor','successor']

	 */

	node* minValueNode(node* root)

	{

		node* current = root;



		if (root->right)

		{

			current = root->right;

			while (current->left != NULL)

			{

				current = current->left;

			}

		}

		else if (root->left)

		{

			current = root->left;

			while (current->right != NULL)

			{

				current = current->right;

			}

		}



		return current;

	}



	int height(node* root)

	{

		if (!root)

		{

			return 0;

		}

		else

		{

			int left = height(root->left);

			int right = height(root->right);

			if (left > right)

			{

				return left + 1;

			}

			else

			{

				return right + 1;

			}

		}

	}



	/* Print nodes at a given level */

	void printGivenLevel(node* root, int level)

	{

		if (root == NULL)

			return;

		if (level == 1)

		{

			print_node(root);

		}

		else if (level > 1)

		{

			printGivenLevel(root->left, level - 1);

			printGivenLevel(root->right, level - 1);

		}

	}

	//************************************************************************

	// Method to help create GraphViz code so the expression tree can

	// be visualized. This method prints out all the unique node id's

	// by traversing the tree.

	// Recivies a node pointer to root and performs a simple recursive

	// tree traversal.

	//************************************************************************

	void GraphVizGetIds(node* nodePtr, ofstream& VizOut)

	{

		static int NullCount = 0;

		if (nodePtr)

		{

			GraphVizGetIds(nodePtr->left, VizOut);

			VizOut << "node" << nodePtr->data

				<< "[label=\"" << nodePtr->data << "\\n"

				//<<"Add:"<<nodePtr<<"\\n"

				//<<"Par:"<<nodePtr->parent<<"\\n"

				//<<"Rt:"<<nodePtr->right<<"\\n"

				//<<"Lt:"<<nodePtr->left<<"\\n"

				<< "\"]" << endl;

			if (!nodePtr->left)

			{

				NullCount++;

				VizOut << "nnode" << NullCount << "[label=\"X\",shape=point,width=.15]" << endl;

			}

			GraphVizGetIds(nodePtr->right, VizOut);

			if (!nodePtr->right)

			{

				NullCount++;

				VizOut << "nnode" << NullCount << "[label=\"X\",shape=point,width=.15]" << endl;

			}

		}

	}



	//************************************************************************

	// This method is partnered with the above method, but on this pass it

	// writes out the actual data from each node.

	// Don't worry about what this method and the above method do, just

	// use the output as your told:)

	//************************************************************************

	void GraphVizMakeConnections(node* nodePtr, ofstream& VizOut)

	{

		static int NullCount = 0;

		if (nodePtr)

		{

			GraphVizMakeConnections(nodePtr->left, VizOut);

			if (nodePtr->left)

				VizOut << "node" << nodePtr->data << "->"

				<< "node" << nodePtr->left->data << endl;

			else

			{

				NullCount++;

				VizOut << "node" << nodePtr->data << "->"

					<< "nnode" << NullCount << endl;

			}



			if (nodePtr->right)

				VizOut << "node" << nodePtr->data << "->"

				<< "node" << nodePtr->right->data << endl;

			else

			{

				NullCount++;

				VizOut << "node" << nodePtr->data << "->"

					<< "nnode" << NullCount << endl;

			}



			GraphVizMakeConnections(nodePtr->right, VizOut);

		}

	}



public:

	BSTree()

	{

		root = NULL;

	}

	~BSTree()

	{

	}



	int count()

	{

		return count(root);

	}



	void insert(int x)

	{

		node* temp = new node(x);

		insert(root, temp);

	}



	void minValue()

	{

		print_node(minValueNode(root), "minVal");

	}



	int height()

	{



		return height(root);

	}



	int top()

	{

		if (root)

			return root->data;

		else

			return 0;

	}



	/* Function to line by line print level order traversal a tree*/

	void printLevelOrder()

	{

		cout << "Begin Level Order===================\n";

		int h = height(root);

		int i;

		for (i = 1; i <= h; i++)

		{

			printGivenLevel(root, i);

			cout << "\n";

		}

		cout << "End Level Order===================\n";

	}



	//************************************************************************

	// Recieves a filename to place the GraphViz data into.

	// It then calls the above two graphviz methods to create a data file

	// that can be used to visualize your expression tree.

	//************************************************************************

	void GraphVizOut(string filename)

	{

		ofstream VizOut;

		VizOut.open(filename);

		VizOut << "Digraph G {\n";

		GraphVizGetIds(root, VizOut);

		GraphVizMakeConnections(root, VizOut);

		VizOut << "}\n";

		VizOut.close();

	}

};

//************************************************************************

//Method to fill a vector using values read in from a file. Recieves a

//reference to the vector to read to values into. Opens the input file,

//and reads in values, filling the vector until the file is empty.

//**********************************************************************
void fillVector(vector<int>& vecOfInts)
{
	ifstream infile;
	string filename;
	cout << "Please enter the name of the input file." << endl;
	cin >> filename;
	int value;
	infile.open(filename);
	while (infile >> value)
	{
		vecOfInts.push_back(value);
	}
}

//********************************************************************

//Method to sort the values in the vector in numerical ascending order.

//Recieves a reference to the vector as a parameter. It then uses a

//bubble sort algorithm to traverse the vector, swapping values as

//needed until the vector is in order.

//*********************************************************************
void sortVector(vector<int>& vecOfInts)
{
	bool swapp = true;
	int temp = 0;
	while (swapp) {
		swapp = false;
		for (int i = 0; i < vecOfInts.size() - 1; i++) {
			if (vecOfInts[i] > vecOfInts[i + 1]) {
				temp = vecOfInts[i];
				vecOfInts[i] = vecOfInts[i + 1];
				vecOfInts[i + 1] = temp;
				swapp = true;
			}
		}
	}
}

//*********************************************************************

//Method to fill a binary tree class with values from the vector, in 

//order to create a complete binary tree. It receives a reference to

//the binary tree declared in main(), a reference to the sorted vector,

//and the starting and ending values determined in main. It determines

//median value of the data set, and calls the insert() method to insert

//it into the tree. It then recursively calls itself twice to find and

//insert the median values of the left and right halves of the vector
 
//into the tree. It repeats the recursive call until all values have 

//been inserted.

int createTree(BSTree &B, vector<int> &vecOfInts, int start, int end)
{
	if (end < start) {
		return NULL;
	}
	int mid = (start + end) / 2;
	if ((start + end) % 2 != 0)
		mid++;
	
	cout << mid << endl;
	int n = vecOfInts[mid];
	cout << n << endl;

	B.insert(vecOfInts[mid]);
	createTree(B, vecOfInts, start, mid - 1);
	createTree(B, vecOfInts, mid + 1, end);
	return n;
}
int main()

{

	srand(2342);
	



	BSTree B;
	vector<int> vecOfInts;

	int start;
	int end;
	fillVector(vecOfInts);
	sortVector(vecOfInts);
	start = 0;
	int size = (vecOfInts.size() - 1);
	end = size;
	createTree(B, vecOfInts, start, end);

	B.printLevelOrder();

	B.GraphVizOut("before.txt");



	return 0;

}
