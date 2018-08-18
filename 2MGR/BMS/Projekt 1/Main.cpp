/*
* Autor: Bc. Lukas Pelanek (xpelan03)
* Datum: 6.11.2017
* Soubor: Main.cpp
* Komentar: Konzolovy program slouzici pro zjisteni priblizne polohy pomoci okolnich BTS
*/

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <iterator>
#include <tuple>
#include <math.h>

#define INPUT_BTS_FILE "bts.csv"
#define OUTPUT_FILE "out.txt"
#define FREQUENCY 900
#define MS_HEIGHT 1.2
#define PI 3.14159265

#define degreesToRadians(degrees) (degrees * PI / 180.0)
#define radiansToDegrees(radians) (radians * 180.0 / PI)

// bts record structure
typedef struct {
	int cid;
	int lac;
	int bch;
	struct {
		double latitude;
		double longitude;
	}gps;
} BTSRecord;

// input record structure
typedef struct {
	int cid;
	int lac;
	int rssi;
	int signal;
	int antH;
	int power;
} InputRecord;

// found couples
typedef struct {
	BTSRecord bts;
	InputRecord input;
} BTSCouple;


// struct to store BTS and its distance
typedef struct {
	double distance;
	struct {
		double latitude;
		double longitude;
	}gps;
} BTSDistance;

/*
* Watts to dB
*/
double wattsToDB(double watts) {
	return 10 * log10(watts) + 30;
}

/*
* Subtract vectors of same size
*/
std::vector<double> subtractVectors(std::vector<double> vec1, std::vector<double> vec2) {
	std::vector<double> result;

	for (unsigned int x = 0; x < vec1.size(); x++) {
		double tmp = vec1[x] - vec2[x];
		result.push_back(tmp);
	}
	return result;
}

/*
* Normalize vector
*/
double normalizeVector(std::vector<double> vec) {
	double result = 0;

	for (auto &num : vec) {
		result += pow(num, 2);

	}

	return sqrt(result);
}

/*
* Dot product of 2 vacter of same size
*/
double dotProduct(std::vector<double> vec1, std::vector<double> vec2) {
	double result = 0;

	for (unsigned int x = 0; x < vec1.size(); x++) {
		result += vec1[x] * vec2[x];
	}

	return result;
}

/*
* Cross product of 2 vectors of size 3
*/
std::vector<double> crossProduct(std::vector<double> vec1, std::vector<double> vec2) {
	std::vector<double> result;

	result.push_back(vec1[1] * vec2[2] - vec1[2] * vec2[1]);
	result.push_back(vec1[2] * vec2[0] - vec1[0] * vec2[2]);
	result.push_back(vec1[0] * vec2[1] - vec1[1] * vec2[0]);

	return result;
}

/*
* Split string into vector of strings by delimiter
*/
std::vector<std::string> stringIntoVector(std::string str, char delimiter) {
	std::vector<std::string> result;
	std::string cell;
	std::stringstream lineStream(str);

	while (std::getline(lineStream, cell, delimiter)) {
		result.push_back(cell);
	}

	return result;
}

/*
* Create BTS Record from vector of strings (the size of vector has to be 5)
*/
BTSRecord stringVectorIntoBTSRecord(std::vector<std::string> vec) {
	BTSRecord record;
	std::string tmp;
	bool isWest = false;
	bool isSouth = false;
	size_t latitudePosition;
	size_t longitudePosition;

	record.cid = std::atoi(vec[0].c_str());
	record.lac = std::atoi(vec[1].c_str());
	record.bch = std::atoi(vec[2].c_str());

	std::string gps = vec[4];

	if ((latitudePosition = gps.find('N')) == std::string::npos) {
		latitudePosition = gps.find('S');
		isSouth = true;
	}

	if ((longitudePosition = gps.find('E')) == std::string::npos) {
		longitudePosition = gps.find('W');
		isWest = true;
	}

	std::string latitude = (longitudePosition > latitudePosition) ? gps.substr(0, gps.find(',')) : gps.substr(gps.find(',') + 1);
	std::string longitude = (longitudePosition > latitudePosition) ? gps.substr(gps.find(',') + 1) : gps.substr(0, gps.find(','));

	// latitude
	tmp = latitude.substr(0, latitude.find('°'));
	double degrees = std::stod(tmp);

	tmp = latitude.substr(latitude.find('°') + 1, latitude.find('\''));
	double minutes = std::stod(tmp);

	tmp = latitude.substr(latitude.find('\'') + 1, latitude.find("''"));
	double seconds = std::stod(tmp);

	record.gps.latitude = degrees + (minutes / 60) + (seconds / 3600);
	record.gps.latitude = isSouth ? record.gps.latitude * (-1) : record.gps.latitude;

	// longitude
	tmp = longitude.substr(0, longitude.find('°'));
	degrees = std::stod(tmp);

	tmp = longitude.substr(longitude.find('°') + 1, longitude.find('\''));
	minutes = std::stod(tmp);

	tmp = longitude.substr(longitude.find('\'') + 1, longitude.find("''"));
	seconds = std::stod(tmp);

	record.gps.longitude = degrees + (minutes / 60) + (seconds / 3600);
	record.gps.longitude = isWest ? record.gps.longitude * (-1) : record.gps.longitude;

	return record;
}

/*
* Get BTS records from input file
*/
std::vector<BTSRecord> getBTSRecords() {
	std::vector<BTSRecord> result;
	std::vector<std::string> lineItems;
	std::ifstream data(INPUT_BTS_FILE);
	std::string line;

	BTSRecord record;

	if (!data.is_open()) {
		std::cerr << "Nepodarilo se otevrit vstupni soubor \"bts.cvs\"" << std::endl;
		exit(-1);
	}

	// skip first line
	std::getline(data, line);

	while (std::getline(data, line)) {
		lineItems = stringIntoVector(line, ';');

		if (lineItems.size() != 5) {
			std::cerr << "Spatny format vstupniho souboru!" << std::endl;
			exit(-1);
		}

		record = stringVectorIntoBTSRecord(lineItems);

		result.push_back(record);
	}
	data.close();

	return result;
}

/*
* Get input records
*/
std::vector<InputRecord> getInputRecords(std::string path) {
	std::vector<InputRecord> result;
	std::ifstream data(path);
	std::vector<std::string> lineItems;
	std::string line;

	InputRecord record;

	if (!data.is_open()) {
		std::cerr << "Nepodarilo se otevrit vstupni soubor \"" + path + "\"" << std::endl;
		exit(-1);
	}

	// skip first line
	std::getline(data, line);

	while (std::getline(data, line)) {
		lineItems = stringIntoVector(line, ';');

		if (lineItems.size() != 6) {
			std::cerr << "Spatny format vstupniho souboru!" << std::endl;
			exit(-1);
		}

		record.lac = std::atoi(lineItems[0].c_str());
		record.cid = std::atoi(lineItems[1].c_str());
		record.rssi = std::atoi(lineItems[2].c_str());
		record.signal = std::atoi(lineItems[3].c_str());
		record.antH = std::atoi(lineItems[4].c_str());
		record.power = std::atoi(lineItems[5].c_str());

		result.push_back(record);
	}
	data.close();

	// sort items by RSSI
	std::sort(result.begin(), result.end(),
		[](const InputRecord &a, const InputRecord &b) -> bool {
		return abs(a.rssi) < abs(b.rssi);
	});

	return result;
}

/*
* Get couples with same cid and lac
*/
std::vector<BTSCouple> getCurrentBTSRecords(std::vector<BTSRecord> btsRecords, std::vector<InputRecord> inputRecords) {
	std::vector<BTSCouple> couples;
	BTSCouple couple;

	for (auto &input : inputRecords) {
		for (auto &bts : btsRecords) {
			if (input.cid == bts.cid && input.lac == bts.lac) {
				couple.bts = bts;
				couple.input = input;
				couples.push_back(couple);
			}
		}
	}

	// check if found couples match size of input size
	if (couples.size() != inputRecords.size()) {
		std::cerr << "Nepodarilo se nalezt nektere BTS!" << std::endl;
	}

	return couples;
}

/*
* Get BTS records with calculated distances
*/
std::vector<BTSDistance> getBTSWithDistances(std::vector<BTSCouple> couples) {
	std::vector<BTSDistance> finalBTS;
	std::vector<BTSDistance> tmpBTS;
	BTSDistance btsDistance;
	bool found = false;

	// calculate distance with hata model (https://en.wikipedia.org/wiki/Hata_model)

	// Antenna height correction factor for small or medium-sized city
	double ch = 0.8 + (1.1 * log10(FREQUENCY) - 0.7) * MS_HEIGHT - 1.56 * log10(FREQUENCY);

	// calculate distances and save BTS
	for (auto &couple : couples) {
		double lu = wattsToDB(couple.input.power) - couple.input.signal;
		double distance = (lu - 69.55 - 26.16 * log10(FREQUENCY) + 13.82 * log10(couple.input.antH) + ch) / (44.9 - 6.55 * log10(couple.input.antH));
		distance = pow(10, distance);

		btsDistance.distance = distance;
		btsDistance.gps.longitude = couple.bts.gps.longitude;
		btsDistance.gps.latitude = couple.bts.gps.latitude;

		tmpBTS.push_back(btsDistance);
	}

	// get different bts records
	for (auto &bts : tmpBTS) {
		found = false;
		for (auto &tmp : finalBTS) {
			if (bts.gps.latitude == tmp.gps.latitude && bts.gps.longitude == tmp.gps.longitude) {
				found = true;
				tmp.distance = (tmp.distance + bts.distance) / 2;
			}
		}
		if (!found) {
			finalBTS.push_back(bts);
		}
	}

	if (finalBTS.size() < 3) {
		std::cerr << "Nepodarilo se najit dostatecny pocet BTS pro ziskani polohy!" << std::endl;
		exit(-1);
	}

	return finalBTS;
}

/*
* Get coords from BTS with distances
*/
std::tuple<double, double> getCoords(std::vector<BTSDistance> finalBTS) {
	int earthR = 6371;
	double latA = finalBTS[0].gps.latitude;
	double lonA = finalBTS[0].gps.longitude;
	double disA = finalBTS[0].distance;
	double latB = finalBTS[1].gps.latitude;
	double lonB = finalBTS[1].gps.longitude;
	double disB = finalBTS[1].distance;
	double latC = finalBTS[2].gps.latitude;
	double lonC = finalBTS[2].gps.longitude;
	double disC = finalBTS[2].distance;

	double xA = earthR * (cos(degreesToRadians(latA)) * cos(degreesToRadians(lonA)));
	double yA = earthR * (cos(degreesToRadians(latA)) * sin(degreesToRadians(lonA)));
	double zA = earthR * (sin(degreesToRadians(latA)));

	double xB = earthR * (cos(degreesToRadians(latB)) * cos(degreesToRadians(lonB)));
	double yB = earthR * (cos(degreesToRadians(latB)) * sin(degreesToRadians(lonB)));
	double zB = earthR * (sin(degreesToRadians(latB)));

	double xC = earthR * (cos(degreesToRadians(latC)) * cos(degreesToRadians(lonC)));
	double yC = earthR * (cos(degreesToRadians(latC)) * sin(degreesToRadians(lonC)));
	double zC = earthR * (sin(degreesToRadians(latC)));

	std::vector<double> P1 = { xA, yA, zA };
	std::vector<double> P2 = { xB, yB, zB };
	std::vector<double> P3 = { xC, yC, zC };

	std::vector<double> ex = subtractVectors(P2, P1);
	double norm = normalizeVector(ex);

	for (auto &val : ex) {
		val = val / norm;
	}
	double i = dotProduct(ex, subtractVectors(P3, P1));

	std::vector<double> iMulEx = ex;
	for (auto &val : iMulEx) {
		val = val * i;
	}

	std::vector<double> ey = subtractVectors(subtractVectors(P3, P1), iMulEx);
	norm = normalizeVector(ey);
	for (auto &val : ey) {
		val = val / norm;
	}

	std::vector<double> ez = crossProduct(ex, ey);
	double d = normalizeVector(subtractVectors(P2, P1));
	double j = dotProduct(ey, subtractVectors(P3, P1));

	double x = (pow(disA, 2) - pow(disB, 2) + pow(d, 2)) / (2 * d);
	double y = ((pow(disA, 2) - pow(disC, 2) + pow(i, 2) + pow(j, 2)) / (2 * j)) - ((i / j) * x);
	double z = sqrt(pow(disA, 2) - pow(x, 2) - pow(y, 2));

	std::vector<double> xMulEx = ex;
	for (auto &val : xMulEx) {
		val = val * x;
	}

	std::vector<double> yMulEy = ey;
	for (auto &val : yMulEy) {
		val = val * y;
	}

	std::vector<double> zMulEz = ez;
	for (auto &val : zMulEz) {
		val = val * z;
	}

	std::vector<double> triPt = P1;

	for (unsigned int i = 0; i < triPt.size(); i++) {
		triPt[i] += xMulEx[i] + yMulEy[i] + zMulEz[i];
	}

	double lat = radiansToDegrees(asin(triPt[2] / earthR));
	double lon = radiansToDegrees(atan2(triPt[1], triPt[0]));

	return std::tuple<double, double>(lat, lon);
}

int main(int argc, char **argv) {
	std::vector<BTSRecord> btsRecords;
	std::vector<InputRecord> inputRecords;
	std::vector<BTSCouple> couples;
	std::vector<BTSDistance> finalBTS;
	std::tuple<double, double> latLon;
	std::ofstream outputFile;

	if (argc != 2) {
		std::cerr << "Nespravny pocet parametru. Zadejte jako parametr jmenno vstupniho souboru!" << std::endl;
		return -1;
	}

	//get BTS records 
	btsRecords = getBTSRecords();

	// get input records
	inputRecords = getInputRecords(argv[1]);

	// get couples of input records and BTS 
	couples = getCurrentBTSRecords(btsRecords, inputRecords);

	// get BTS with distances
	finalBTS = getBTSWithDistances(couples);

	// get coords
	latLon = getCoords(finalBTS);

	// print coords into file
	outputFile.open(OUTPUT_FILE);
	outputFile << "https://www.google.com/maps?q=" << std::get<0>(latLon) << "," << std::get<1>(latLon) << std::endl;
	outputFile.close();

	return 0;
}