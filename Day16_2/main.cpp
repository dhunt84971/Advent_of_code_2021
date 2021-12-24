#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <chrono>
#include <thread>
#include <math.h>
#include <assert.h>

struct packetType {
	int version;
	int type;
	long int literal;
	std::vector <packetType> packets;
};

std::string getDataFromFile(std::string fileName){
	std::string data;
	std::cout << "Retrieved data from " << fileName << std::endl;
	std::ifstream infile(fileName);
	std::getline(infile, data);
	return data;
}

std::string convertHexToBinary(char hexChar){
	std::string bits;
	// Get the ASCII value
	int asc = (int)hexChar;
	int decValue {0};
	if (asc >= 48 && asc <= 57){ // It's a number 0-9
		decValue = asc - 48;
	}
	else { // It's a letter A-F
		decValue = asc - 55;
	}
	for (int i {3}; i>=0; i--){
		int test = static_cast<int>(pow (2, i));
		if (decValue >= test){
			bits.push_back('1');
			decValue -= test;
		}
		else{
			bits.push_back('0');
		}
	}
	return bits;
}

long int convertBinaryToInt(std::string binary){
	//assert(binary.size() <= 32);
	long int decValue {0};
	for (size_t i{0}; i<binary.size(); i++){
		int exp = binary.size() - 1 - i;
		if (binary[i] == '1')	decValue += static_cast<long int> (pow(2, exp));
	}
	return decValue;
}

std::string convertDataToBinary(std::string data){
	std::string binaryData {};
	for (size_t i {0}; i<data.size(); i++){
		binaryData += convertHexToBinary(data[i]);
	}
	return binaryData;
}

long int getIntFromBits(std::string bitData, int &pointer, int length){
	std::string binary = bitData.substr(pointer, length);
	pointer += length;
	return convertBinaryToInt(binary);
}

std::string getBitsFromBits(std::string bitData, int &pointer, int length){
	std::string binary = bitData.substr(pointer, length);
	pointer += length;
	return binary;
}

packetType getPacket(std::string bitData, int &pointer){
	packetType packet;
	packet.version = getIntFromBits(bitData, pointer, 3);
	packet.type = getIntFromBits(bitData, pointer, 3);
	if (packet.type == 4){  // Literal Packet
		bool last = false;
		std::string digits {};
		do {
			last = (getIntFromBits(bitData,pointer, 1) == 0);
			digits += getBitsFromBits(bitData, pointer, 4);
		} while (!last);
		packet.literal = convertBinaryToInt(digits);
		return packet;
	}
	else{ // Operator Packet
		if (getIntFromBits(bitData, pointer, 1) == 0){  // packet length specified.
			int endOfPacket = getIntFromBits(bitData, pointer, 15) + pointer;
			while (pointer < endOfPacket){
				packetType subPacket = getPacket(bitData, pointer);
				packet.packets.push_back(subPacket);
			}
			return packet;
		}
		else{ // number of subpackets specified.
			int numOfPackets = getIntFromBits(bitData, pointer, 11);
			for (size_t i {0}; i<numOfPackets; i++){
				packetType subPacket = getPacket(bitData, pointer);
				packet.packets.push_back(subPacket);
			}
			return packet;
		}
	}
}

int sumVersions (packetType packet){
	int sum {0};
	sum += packet.version;
	for (size_t i {0}; i<packet.packets.size(); i++){
		sum += sumVersions(packet.packets[i]);
	}
	return sum;
}

long int evaluatePacket(packetType packet){
	// Literal Value
	if (packet.type == 4) return packet.literal;
	// Sum Operator
	if (packet.type == 0){
		long int sum {0};
		for (size_t i {0}; i<packet.packets.size(); i++){
			sum += evaluatePacket(packet.packets[i]);
		}
		return sum;
	}
	// Product Operator
	if (packet.type == 1){
		long int product {1};
		for (size_t i {0}; i<packet.packets.size(); i++){
			product *= evaluatePacket(packet.packets[i]);
		}
		return product;
	}
	// Minimum Operator
	if (packet.type == 2){
		long int minimum {evaluatePacket(packet.packets[0])};
		for (size_t i {1}; i<packet.packets.size(); i++){
			long int value = evaluatePacket(packet.packets[i]);
			minimum = (minimum > value) ? value : minimum;
		}
		return minimum;
	}
	// Maximum Operator
	if (packet.type == 3){
		long int maximum {evaluatePacket(packet.packets[0])};
		for (size_t i {1}; i<packet.packets.size(); i++){
			long int value = evaluatePacket(packet.packets[i]);
			maximum = (maximum < value) ? value : maximum;
		}
		return maximum;
	}
	// Greater Than Operator
	if (packet.type == 5){
		long int first {evaluatePacket(packet.packets[0])};
		long int second {evaluatePacket(packet.packets[1])};
		long int greater = (first > second) ? 1 : 0;
		return greater;
	}
	// Less Than Operator
	if (packet.type == 6){
		long int first {evaluatePacket(packet.packets[0])};
		long int second {evaluatePacket(packet.packets[1])};
		long int less = (first < second) ? 1 : 0;
		return less;
	}
	// Equal To Operator
	if (packet.type == 7){
		long int first {evaluatePacket(packet.packets[0])};
		long int second {evaluatePacket(packet.packets[1])};
		long int equal = (first == second) ? 1 : 0;
		return equal;
	}
}

int main(){
	std::string data = getDataFromFile("../data.dat");
	std::string bitData = convertDataToBinary(data);
	int pointer {0};
	packetType packets = getPacket(bitData, pointer);
	std::cout << "Packet evaluation = " << evaluatePacket(packets) << std::endl;
	return 0;
}