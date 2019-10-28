/**
 * Name: Michael Gumm
 * Date: 10/28/2019
 * Course: CMPS 3013
 * Program: A05
 * Description:
 *
 *      This program declares a heap that can hold 1000 City objects.
 *		It then reads data from a .json file, and uses it to create
 *		an array of City pointers. It then calls a function that
 *		prompts the user for the number of neighbor cities to print
 *		to the output. It then loops through the array of City
 *		objects, and for each city, finds the closest cities by
 *		calculating the distance between the first city and every
 *		other city in the array, and pushing them onto the heap. It
 *		then extracts a certain number of cities depending on user 
 *		input, and prints them to the output file, and clears the
 *		heap for the next iteration. 
 */

#include <iostream>
#include <fstream>
#include <string.h>
#include "Heap.hpp"
#include "JsonFacade.hpp"
#include "json.hpp"
#include "Geo.hpp"

using namespace std;

using json = nlohmann::json;

struct City{
    double Priority;
    string Name;
	double Longitude;
	double Latitude;
    City(){
        Priority = 0;
        Name = "";
		Longitude = 0;
		Latitude = 0;
    }
    City(json obj){
        Name = obj["city"];
        Priority = obj["population"];
		Latitude = obj["latitude"];
		Longitude =  obj["longitude"];
    }
};


/**

	 * Function findDistance:

	 *      Accepts the coordinates of two cities, and returns

	 *		the distance between them.

	 * Params:

	 *		double lat1, double log1, double lat2, double log2:
	 
	 *		the latitude and longitude values of the two cities

	 * Returns

	 *      float HaversineDistance()

	 */
float findDistance(double lat1, double log1, double lat2, double log2) {

	Coordinate Coord1(lat1, log1);

	Coordinate Coord2(lat1, log2);

	return HaversineDistance(Coord1, Coord2);

}


/**

	 * Function IO:

	 *      Accepts user input, and returns a value that determines

	 *		how many neighbor cities are printed to the output file.

	 * Params:

	 *		void

	 * Returns:

	 *      int numdisplay

	 */
int IO() {
	int numdisplay;
	cout << "How many cities do you wish to display?" << endl;
	cin >> numdisplay;
	return numdisplay;
}

int main(){
	int display;
	ofstream outfile;
	outfile.open("output.txt");
    json obj;
    City** Cities;
    City* temp;
    string filename = "cities.json";
    JsonFacade J(filename);

    Heap <City> H(1000,false);

    int size = J.getSize();
    Cities = new City *[size];
    // loads an array of cities with the json data
    for (int i = 0; i < size; i++) {
        obj = J.getNext();
        Cities[i] = new City(obj);
    }

	display = IO(); //finds the number of cities to print to output
	
	for (int i = 0; i < size; i++){
		temp = Cities[i];
		outfile << i+1 << " : " << temp->Name << endl;
		for (int j=0; j < size; j++){
			Cities[j]->Priority = findDistance(temp->Latitude, 
				temp->Longitude, Cities[j]->Latitude, 
				Cities[j]->Longitude);
			if (Cities[j]->Priority != 0)
				H.Insert(Cities[j]);
		}
		for (int c=0; c < display; c++){
			temp = H.Extract();//finds closest x cities
			outfile << "	" << c+1 << ") " << temp->Name << " " 
				<< temp->Priority << endl;	
		}
		while (!H.Empty()) {
			H.Extract();
		}
	}
}
