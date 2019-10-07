/**
 * Name: Michael Gumm
 * Date: 10/7/2019
 * Course: CMPS 3013
 * Program: A03
 * Description:
 *
 *      This program reads data from a .json file, and uses it to create 
 *		an array of Animal pointers. It declares Buckets, an array of
 *		Heap pointers, and then uses a command line parameter to 
 *		determine the number of buckets to allocate. It then calls a
 *		functions that traverses the array of Animal pointers,
 *		calculates the correct bucket, and inserts the Animal pointer
 *		into that bucket. It then calls a function that extracts the
 *		top 5 values from each bucket, and prints the values to an
 *		output file.
 */

//https://stackoverflow.com/questions/55116344/how-to-setup-vs-code-for-c-14-c-17
//https://github.com/nlohmann/json

#include "json_helper.cpp"
#include <cstdlib>
#include <fstream>
#include <iostream>

#define _USE_MATH_DEFINES

#include <math.h>

using namespace std;
//Prototype for nameToNumber, so it can be used in the Animal
//constructor
int nameToNumber(string);
//Prototype for findPriority, so it can be used in the Animal
//constructor
float findPriority(string, double, double, float, bool);
const static double EarthRadiusKm = 6372.8;

inline double DegreeToRadian(double angle) {
    return M_PI * angle / 180.0;
}

class Coordinate {
public:
    Coordinate(double latitude, double longitude) : myLatitude(latitude), myLongitude(longitude) {}

    double Latitude() const {
        return myLatitude;
    }

    double Longitude() const {
        return myLongitude;
    }

private:
    double myLatitude;
    double myLongitude;
};

float HaversineDistance(const Coordinate &p1, const Coordinate &p2) {
    float latRad1 = DegreeToRadian(p1.Latitude());
    float latRad2 = DegreeToRadian(p2.Latitude());
    float lonRad1 = DegreeToRadian(p1.Longitude());
    float lonRad2 = DegreeToRadian(p2.Longitude());

    float diffLa = latRad2 - latRad1;
    float doffLo = lonRad2 - lonRad1;

    float computation = asin(sqrt(sin(diffLa / 2) * sin(diffLa / 2) + cos(latRad1) * cos(latRad2) * sin(doffLo / 2) * sin(doffLo / 2)));
    return 2 * EarthRadiusKm * computation;
}

struct Animal {
    string animal_name;
    long date;
    double latitude;
    double longitude;
    float adjuster;
    bool validated;
    string version;
    float priority;

    Animal() {
        animal_name = "";
        date = 0;
        latitude = 0.0;
        longitude = 0.0;
        adjuster = 0.0;
        validated = 0;
        version = "";
        priority = 0.0;
    }

    Animal(string name, long _date, double lat, double lon, float adj, bool val, string ver) {
        animal_name = name;
        date = _date;
        latitude = lat;
        longitude = lon;
        adjuster = adj;
        validated = val;
        version = ver;
        priority = 0.0;
    }

    Animal(json j) {
        animal_name = j["animal_name"];
        date = j["date"];
        latitude = j["latitude"];
        longitude = j["longitude"];
        adjuster = j["adjuster"];
        validated = j["validated"];
        version = j["version"];
        priority = findPriority(animal_name, latitude, longitude, adjuster, validated);
    }
};

class AnimalHelper {

private:
    Animal **Animals;

    JsonHelper *J;

    json obj;

    int size;

public:
    AnimalHelper(string filename) {

        J = new JsonHelper(filename);

        size = J->getSize();

        Animals = new Animal *[size];

        for (int i = 0; i < size; i++) {

            obj = J->getNext();

            Animals[i] = new Animal(obj);
        }
    }
};

/**

 * Class Heap:

 * 

 * Public Methods:

 *    Heap(int)

 *    void Insert(int)

 *    int Remove()

 *    void PrintHeap()

 *    int Size()

 *    bool Empty()

 *    void Heapify(int*,int)

 */

class Heap {

private:
    Animal **A; // Pointer to allocate dynamic array

    int Next; // Next available location

    int MaxSize; // Max size since were using array

    int HeapSize; // Actual number of items in the array.

    /**

     * Function IncreaseKey:

     *      Bubbles element up from given index.

     * 

     * Params:

     *     [int] index  - index of item to be increased

     * Returns

     *      void

     */

    void BubbleUp(int i) {

        int p = Parent(i);

        while (p > 0 && A[i]->priority > A[p]->priority) {

            Swap(i, p);

            i = p;

            p = Parent(i);
        }
    }

    /**

     * Function DecreaseKey:

     *      Bubbles element down from given index.

     * 

     * Params:

     *      [int] index - index of item to be decreased

     * Returns

     *      void

     */

    void BubbleDown(int i) {

        int c = PickChild(i);

        while (c > 0) {

            if (A[i]->priority < A[c]->priority) {

                Swap(i, c);

                i = c;

                c = PickChild(i);

            } else {

                c = -1;
            }
        }
    }

    /**

     * Function Swap:

     *      Swaps two elements in an array

     * 

     * Params:

     *      [int] index1 - index of item to swap with

     *      [int] index2 - same

     * Returns

     *      void

     */

    void Swap(int p, int i) {

        Animal *temp = A[p];

        A[p] = A[i];

        A[i] = temp;
    }

    /**

     * Function Parent:

     *      Returns parent index of a given index

     * 

     * Params:

     *      [int]index - index to calculate parent from

     * Returns

     *      index [int]

     */

    int Parent(int i) {

        return int(i / 2);
    }

    /**

     * Function LeftChild:

     *      Returns left index of a given index

     * 

     * Params:

     *      [int] index - index to calculate child from

     * Returns

     *      [int] index - left child index

     */

    int LeftChild(int i) {
        //    if (i == 0)
        //    return 1;
        return i * 2 + 1;
    }

    /**

     * Function RightChild:

     *      Returns right index of a given index

     * 

     * Params:

     *      [int] index - index to calculate child from

     * Returns

     *      [int] index - right child index

     */

    int RightChild(int i) {

        return i * 2 + 2;
    }

    /**

     * Function PickChild:

     *      Return index of child to swap with or -1 to not swap.

     * 

     * Params:

     *      [int] index - index of parent element

     * Returns

     *      [int] index - index to swap with or -1 to not swap

     */

    int PickChild(int i) {

        if (RightChild(i) >= Next) { //No right child

            if (LeftChild(i) >= Next) { //No left child

                return -1;

            } else { //you have a left no right

                return LeftChild(i);
            }

        } else {

            //right child exists

            if (A[RightChild(i)]->priority > A[LeftChild(i)]->priority) {

                return RightChild(i);

            } else {

                return LeftChild(i);
            }
        }
    }

public:
    /**

     * Function Heap:

     *      Constructor that allocates memory for array and

     *      inits vars.

     * 

     * Params:

     *      void

     * Returns

     *      void

     */

    Heap(int size) {

        A = new Animal *[size];

        Next = 1;

        MaxSize = size;

        HeapSize = 0;
    }

    //Default Constructor
    Heap() {
        A = new Animal *[1000];
        Next = 1;
        MaxSize = 0;
        HeapSize = 0;
    }
    /**

     * Function Insert:

     *      Insert value into heap.

     * 

     * Params:

     *      [Animal*] x - value to be inserted

     * Returns

     *      void

     */

    void Insert(Animal *x) {

        A[Next] = x;

        BubbleUp(Next);

        Next++;

        HeapSize++;
    }

    /**

     * Function Extract:

     *      Removes top element from heap (whether min or max).

     * 

     * Params:

     *      void

     * Returns

     *      [Animal] top_value - top value in the heap (min or max)

     */

    Animal Extract() {

        //   if (Empty()) {

        //      return NULL;	//PROBLEM
        //    }

        Animal retval = *A[1];

        A[1] = A[--Next];

        HeapSize--;

        if (HeapSize > 1) {

            BubbleDown(1);
        }

        return retval;
    }

    /**

     * Function PrintHeap:

     *      Prints the values currently in the heap array

     *      based on array location, not heap order

     * 

     * Params:

     *      void

     * Returns

     *      void

     */

    void PrintHeap() {

        for (int i = 0; i < Next; i++) {

            cout << i << " " << A[i]->animal_name << " ";
            cout << A[i]->priority << endl;
        }

        cout << endl;
    }

    /**

     * Function Size:

     *      Returns the number of items in the heap

     * 

     * Params:

     *      void

     * Returns

     *      [int] heapSize - size of heap

     */

    int Size() {

        return Next - 1;
    }

    /**

     * Function Empty:

     *      Returns boolean true of array is empty

     * 

     * Params:

     *      void

     * Returns

     *      [bool] empty - is array empty

     */

    bool Empty() {

        return Next == 1;
    }

    /**

     * Function Heapify:

     *      Creates a heap out of a given array of Animals in max heap order

     * 

     * Params:

     *      [Animal**] array - array of values to heapify

     *      [int] size - size of array

     * Returns

     *      void

     */

    void Heapify(Animal **&Animals, int size) {

        int i = size / 2;

        A = Animals;

        Next = size;

        HeapSize = size - 1;

        for (int j = i; j >= 0; j--) {

            BubbleDown(j);
        }
    }
};
/**

     * Function nameToNumber:

     *      Turns a string into an integer value by adding together 

     *		the ascii value of all of its letters.

     * Params:

     *      string name - name of the animal

     * Returns

     *      int total - the total value of the ascii values of each

	 *		letter in the string.

     */
int nameToNumber(string name) {
    int total = 0;
    for (int i = 0; i < name.length(); i++)
        total += (int)name[i];
    return total;
}

/**

     * Function findDistance:

     *      Accepts the coordinates of an animal, and returns

	 *		the distance between the animal's position, and 

	 *		33.9137, -98.4934

     *		

     * Params: 
	 
	 *		double lat, double long- the latitude

	 *		and longitude values of an animal

     *      

     * Returns

     *      float HaversineDistance()

     */
float findDistance(double lat, double log) {
    Coordinate Coord1(lat, log);
    Coordinate Coord2(33.9137, -98.4934);
    return HaversineDistance(Coord1, Coord2);
}
/**

     * Function findPriority:

     *      Calculates the proper priority value of an

	 *		animal.

     *		

     * Params:

     *		string name - animal's name, double lat - animal's

	 *		latitude, double log - animal's longitude, fload adj -

	 *		animal's adjuster value, bool v - animal's validated value

     * Returns

     *      float priority

     */
float findPriority(string name, double lat, double log, float adj, bool v) {
    float distance = findDistance(lat, log);
    int length = name.length();
    float priority;
    priority = (6372.8 - distance) * adj / length;
    if (v == true)
        priority = priority * -1;
    return priority;
}
/**

     * Function fillBuckets:

     *      Traverses the array of Animal pointers, calculates

	 *		the proper bucket to insert the animal pointer into,

	 *		and inserts the animal pointer.

     *		

     * Params:

     *      Heap **&Buckets - a reference to an array of heap pointers,

	 *		Animal **&Animals - a reference to an array of animal

	 *		pointers, int size - size of the array, int NumBuckets -

	 *		the number of buckets to be filled

     * Returns

     *      void

     */
void fillBuckets(Heap **&Buckets, Animal **&Animals, int size, int NumBuckets) {
    int Bucket;
    for (int i = 0; i < size; i++) {
        Bucket = abs(Animals[i]->date) % NumBuckets;
        Buckets[Bucket]->Insert(Animals[i]);
    }
}
/**

     * Function removeAnimals:

     *      Extracts the top 5 values from each bucket,

	 *		and prints their names and priority values to the 

	 *		output file.

     *		

     * Params:

     *      Heap **&Buckets - a reference to an array of heap pointers,

	 *		Animal **&Animals - a reference to an array of animal

	 *		pointers, int NumBuckets - the number of buckets to be

	 *		extracted from, ofstream &outfile - a reference to the output

	 *		file to be printed to.

     * Returns

     *      void

     */
void removeAnimals(Heap **&Buckets, Animal **&Animals, int NumBuckets, ofstream &outfile) {
    Animal temp;
    outfile << "Name: Michael Gumm" << endl;
    for (int i = 0; i < NumBuckets; i++) {
        outfile << "Heap " << i << endl;
        outfile << "============" << endl;
        for (int j = 0; j < 5; j++) {
            temp = Buckets[i]->Extract();
            outfile << temp.animal_name << " " << temp.priority << endl;
        }
        outfile << endl;
    }
}

int main(int argc, char **argv) {
    int size = 1000;
    ofstream outfile;
    outfile.open("output.txt");
    json obj;
    Animal **Animals;
    JsonHelper *J;
    int NumberOfBuckets = 0;
    if (argc > 1) {
        NumberOfBuckets = atoi(argv[1]);
    } else {
        cout << "ERROR: No buckets created" << endl;
    }
    int Bucket;
    J = new JsonHelper("animals.json");
    size = J->getSize();
    Animals = new Animal *[size];
    for (int i = 0; i < size; i++) {
        obj = J->getNext();

        Animals[i] = new Animal(obj);
    }
    Heap **Buckets = new Heap *[NumberOfBuckets];

    for (int i = 0; i < NumberOfBuckets; i++) {
        Buckets[i] = new Heap;
    }
    cout << "Inserting..." << endl;
    fillBuckets(Buckets, Animals, size, NumberOfBuckets);
    cout << "Extracting..." << endl;
    removeAnimals(Buckets, Animals, NumberOfBuckets, outfile);
    return 0;
}
