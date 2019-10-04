/**
 * Name: Michael Gumm
 * Date: 10/4/2019
 * Course: CMPS 3013
 * Program: A03
 * Description:
 *
 *      This program creates an instance of class AnimalHelper, which 
 *		reads data from a .json file, and uses it to create an Animal
 *		object, which is then stored in the AnimalHelper instance. It
 *		then orders the array of objects in MaxHeap order, with the
 *		root node being larger than its children nodes. Finally, the
 *		contents of the heap, 1,000 Animal objects, are printed.
 */

//https://stackoverflow.com/questions/55116344/how-to-setup-vs-code-for-c-14-c-17
//https://github.com/nlohmann/json

#include "json_helper.cpp"
#include <fstream>
#include <iostream>

#define _USE_MATH_DEFINES

#include <math.h>

using namespace std;
//Prototype for nameToNumber, so it can be used in the Animal 
//constructor.
int nameToNumber(string);
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

double HaversineDistance(const Coordinate &p1, const Coordinate &p2) {
    double latRad1 = DegreeToRadian(p1.Latitude());
    double latRad2 = DegreeToRadian(p2.Latitude());
    double lonRad1 = DegreeToRadian(p1.Longitude());
    double lonRad2 = DegreeToRadian(p2.Longitude());

    double diffLa = latRad2 - latRad1;
    double doffLo = lonRad2 - lonRad1;

    double computation = asin(sqrt(sin(diffLa / 2) * sin(diffLa / 2) + cos(latRad1) * cos(latRad2) * sin(doffLo / 2) * sin(doffLo / 2)));
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
        priority = nameToNumber(animal_name);
    }
};

class AnimalHelper {
private:
    Animal **Animals;
    JsonHelper *J;
    json obj;
    int Next; // Next available location

    int MaxSize; // Max size since were using array

    int size; // Actual number of items in the array.

public:
    AnimalHelper(string filename) {
        Next = 1;
        size = 0;
        J = new JsonHelper(filename);
        MaxSize = J->getSize();
        Animals = new Animal *[MaxSize];
        for (int i = 0; i < MaxSize; i++) {
            obj = J->getNext();
            Animals[i] = new Animal(obj);
        }
    }

    /**

     * Function PrintAnimals:

     *      Prints the contents of the heap

     * 

     * Params:

     *      None

     * Returns

     *      void

     */
    void PrintAnimals(ofstream& outfile) {
        outfile << "Name: Michael Gumm" << endl;
        for (int i = 0; i < MaxSize; i++) {

            outfile << Animals[i]->animal_name << " "
                 << Animals[i]->latitude << " "
                 << Animals[i]->longitude << " "
                 << Animals[i]->date << " "
                 << Animals[i]->adjuster << " "
                 << Animals[i]->validated << " "
                 << Animals[i]->version << " "
                 << Animals[i]->priority << endl;
        }
    }

	//Currently not in use
    /*
    void BubbleUp(int i) {

        int p = Parent(i);

        while (p > 0 && Animals[i]->priority > Animals[p]->priority) {

            Swap(i, p);

            i = p;

            p = Parent(i);
        }
    }
	
    void BubbleDown(int i) {

        int c = PickChild(i);

        while (c > 0) {

            if (Animals[i]->priority < Animals[c]->priority) {

                Swap(i, c);

                i = c;

                c = PickChild(i);

            } else {

                c = -1;
            }
        }
    }
	*/

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

        Animal *temp = Animals[p];

        Animals[p] = Animals[i];

        Animals[i] = temp;
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
        if (i == 0)
            return i + 1;
        else
            return i * 2;
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
        if (i == 0)
            return i + 2;
        else
            return (i * 2) + 1;
    }

	//Not currently in use
	/**

     * Function PickChild:

     *      Return index of child to swap with or -1 to not swap.

     * 

     * Params:

     *      [int] index - index of parent element

     * Returns

     *      [int] index - index to swap with or -1 to not swap

     *
    int PickChild(int i) {

        if (RightChild(i) >= Next) { //No right child

            if (LeftChild(i) >= Next) { //No left child

                return -1;

            } else { //you have a left not right

                return LeftChild(i);
            }

        } else {

            //right child exists

            if (Animals[RightChild(i)]->priority > Animals[LeftChild(i)]->priority) {

                return RightChild(i);

            } else {

                return LeftChild(i);
            }
        }
    }
	*/

    //Not currently in use
    /**

     * Function Insert:

     *      Insert value into heap.

     * 

     * Params:

     *      [int] x - value to be inserted

     * Returns

     *      void

     */

    /*
    void Insert(Animal *x) {

        Animals[Next] = x;

        BubbleUp(Next);	

        Next++;

        size++;
    }
	*/
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

     *      Compares the priority value of the root node with its left

	 *		and right children, swapping the root with the largest

	 *		child, and recursively calling itself to compare again

     * 

     * Params:

     *      None

     * Returns

     *      void

     */
    void Heapify(int i) {
        int largest;
        int left = LeftChild(i);
        int right = RightChild(i);
        if (left < MaxSize && Animals[left]->priority > Animals[i]->priority)
            largest = left;
        else
            largest = i;
        if (right < MaxSize && Animals[right]->priority > Animals[largest]->priority)
            largest = right;
        if (largest != i) {
            Swap(i, largest);
            Heapify(largest);
        }
    }

	/**

     * Function HeapSort:

     *      Uses Heapify() to sort the array in MaxHeap order, so

	 *		the priority value of each parent object will be greater

	 *		than the priority values of their child objects

     * Params:

     *      void

     * Returns

     *      void

     */
    void HeapSort() {
       for (int i = MaxSize / 2 - 1; i >= 0; i--) {
          Heapify(i);
       }
       for (int i = MaxSize - 1; i >= 0; i--) {
          Swap(0, i);
          Heapify(0);
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

int main(int argc, char **argv) {
    ofstream outfile;
    outfile.open("output.txt");
    AnimalHelper AH("animals.json");
    int i = 0;
    AH.HeapSort();
    AH.PrintAnimals(outfile);
    return 0;
}
