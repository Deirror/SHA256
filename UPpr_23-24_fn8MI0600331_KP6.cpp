/**
*
* Solution to course project # 6
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2023/2024
*
* @author Alexander Angelow
* @idnumber 8MI0600331
* @compiler VC
*
* <file with helper functions>
*
*/

#include <iostream>
#include <fstream>

//Hard-coded constants
const char NAME_OF_READFILE[] = "text.txt";
const char NAME_OF_WRITEFILE[] = "result.txt";
const int BYTES64 = 64;
const int BYTES32 = 32;
const int BYTES16 = 16;
const int BYTES8 = 8;
const int BITS8 = 8;
const int DIFFERENCE = 4;

const unsigned int CONST8_ARRAY[] = {
0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
};
const unsigned int CONST64_ARRAY[] = {
0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85
,0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070
,0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3
,0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};


int getMaxSizeOfMessageBlock(int size);

int getLengthOfMessage(int& lengthOfStr);

int getSizeOfArray(const unsigned char* arr);

bool readFile(char* messageBlock);

int powerNumber(int times);

void writeCurrentSizeInMessageBlock(char* messageBlock, int current_size, const int MAX_SIZE);

void appendUniqueElementsToArray(char* messageBlock, int current_size, const int MAX_SIZE);

unsigned int fillLinesInEachW(const char* messageBlock, int index);

void copyChunkIntoMessageSchedule(unsigned int messageSchedule[], const char* messageBlock, const int startingIndex, const int endingIndex);

unsigned int rightRotateNumber(unsigned int number, const int rightRotate);

unsigned int calculateSmallSigma(unsigned int number, const int firstRightRotate, const int secondRightRotate, const int rightShift);

unsigned int returnValueOfSmallSigma(bool choice, unsigned int number);

void fillMessageSchedule(unsigned int messageSchedule[]);

void updateValuesOfArrays(unsigned int valuesOfSmallLetterArray[], unsigned int updatedHashValuesArray[], const int size, bool choice);

unsigned int calculateBigSigma(unsigned int num, bool choice);

unsigned int calculateChoice(unsigned int e, unsigned int f, unsigned int g);

unsigned int calculateMajority(unsigned int a, unsigned int b, unsigned int c);

unsigned int calculateTemp1(unsigned int h, unsigned int Sigma1, unsigned int choice, unsigned int const64Number, unsigned int numberFromMsgSchedule);

unsigned int calculateTemp2(unsigned int Sigma0, unsigned int majority);

void updateSmallLetters(unsigned int valuesOfSmallLetterArray[], unsigned int temp1, unsigned int temp2, const int size);

void generateHashedMessage(unsigned int updatedHashValuesArray[], unsigned int messageSchedule[]);

void fillHashedMessageWithRecursion(unsigned int number, unsigned char hashedMessage[], int startingIndex, int times);

void convertToHex(unsigned int updatedHashValuesArray[], unsigned char hashedMessage[]);

void hashMessageWithSHA256(char* messageBlock, int current_size, const int MAX_SIZE, unsigned char hashedMessage[]);

void writeHashedMessageInFile(unsigned char hashedMessage[]);

void readHashedMessageFromFile(unsigned char readHash[]);

bool checkIfHashesAreEqual(const unsigned char* readHash, const unsigned char* hashedMessage);

int main()
{
	int sizeOfMessageInFile = 0;

	const int* const MAX_SIZE = new int(getLengthOfMessage(sizeOfMessageInFile));

	if (*MAX_SIZE == -1)
	{
		std::cout << "Cannot open file!";

		return -1;
	}

	char* messageBlock = new char[*(MAX_SIZE)] {};

	unsigned char hashedMessage[BYTES64 + 1]{};//Also including terminating zero

	if (!readFile(messageBlock))
	{
		std::cout << "Cannot open file!";

		return -1;
	}

	appendUniqueElementsToArray(messageBlock, sizeOfMessageInFile, *(MAX_SIZE));

	hashMessageWithSHA256(messageBlock, sizeOfMessageInFile, *(MAX_SIZE), hashedMessage);

	writeHashedMessageInFile(hashedMessage);

	unsigned char readHash[BYTES64 + 1]{};//That's the hashed message which will be read from the file

	readHashedMessageFromFile(readHash);

	std::cout << checkIfHashesAreEqual(readHash, hashedMessage);

	delete[] messageBlock;

	delete MAX_SIZE;

	return 0;
}

//Getting sizes
int getMaxSizeOfMessageBlock(int size)
{
	int multiplier = 1;

	while (size >= multiplier * BYTES64 - BYTES8)
	{
		multiplier++;
	}

	return multiplier * BYTES64;
}

int getLengthOfMessage(int& lengthOfStr)
{
	std::ifstream streamReader(NAME_OF_READFILE);

	if (!streamReader.is_open())
		return -1;

	char symbol = 0;

	int length = 0;

	while (!streamReader.eof())
	{
		streamReader >> std::noskipws >> symbol;

		if (symbol != '\n' && streamReader.good())
		{
			length++;
		}
	}

	lengthOfStr = length;

	streamReader.close();

	return getMaxSizeOfMessageBlock(length);
}

int getSizeOfArray(const unsigned char* arr)
{
	int index = 0;

	while (arr[index++]);

	return index--;
}

//Filling the message block
bool readFile(char* messageBlock)
{
	std::ifstream streamReader(NAME_OF_READFILE);

	if (!streamReader.is_open() || !messageBlock)
		return false;

	char symbol = 0;

	int index = 0;

	while (!streamReader.eof())
	{
		streamReader >> std::noskipws >> symbol;

		if (symbol != '\n' && streamReader.good())
		{
			messageBlock[index++] = symbol;
		}
	}

	streamReader.close();

	return true;
}

int powerNumber(int times)
{
	int result = 1;

	for (int i = 0; i < times; i++)
	{
		result *= 256;
	}

	return result;
}

void writeCurrentSizeInMessageBlock(char* messageBlock, int current_size, const int MAX_SIZE)
{
	int result = 0;

	int times = 0;

	int multiplier = 0;

	bool bit = false;

	int endingIndex = 1, startingIndex = 0;

	while (current_size * BITS8 >= (powerNumber(times) - 1))
	{
		startingIndex = BITS8 * multiplier;

		for (int i = startingIndex; i < BITS8 * (multiplier + 1); i++)
		{
			bit = ((current_size * BITS8) & (1 << i)) ? 1 : 0;

			result |= (bit << i);
		}

		result >>= startingIndex;

		messageBlock[MAX_SIZE - (endingIndex++)] = result;

		result = 0;

		multiplier++;

		times++;
	}
}

void appendUniqueElementsToArray(char* messageBlock, int current_size, const int MAX_SIZE)
{
	if (!messageBlock)
		return;

	//append a single one
	messageBlock[current_size] = unsigned(1 << (BYTES8 - 1));

	//last bytes = length of text
	writeCurrentSizeInMessageBlock(messageBlock, current_size, MAX_SIZE);
}

//Copying chunk into message schedule
unsigned int fillLinesInEachW(const char* messageBlock, int index)
{
	unsigned int W = 0;

	int multiplier = 0;

	unsigned char symb = 0;

	for (int j = DIFFERENCE - 1; j >= 0;j--)
	{
		symb = messageBlock[index + j];

		W |= (symb << multiplier * BYTES8);

		multiplier++;
	}

	return W;
}

void copyChunkIntoMessageSchedule(unsigned int messageSchedule[], const char* messageBlock, const int startingIndex, const int endingIndex)
{
	for (int i = startingIndex, j = 0; i < endingIndex && j < BYTES16; i += DIFFERENCE, j++)
	{
		messageSchedule[j] = fillLinesInEachW(messageBlock, i);
	}
}

//Filling message schedule
unsigned int rightRotateNumber(unsigned int number, const int rightRotate)
{
	unsigned int firstBinaryNumber = 0;

	for (int i = 0; i < rightRotate; i++)
	{
		firstBinaryNumber += (number & (1 << i));
	}

	return ((number >> rightRotate) | (firstBinaryNumber << (BYTES32 - rightRotate)));

}

unsigned int calculateSmallSigma(unsigned int number, const int firstRightRotate, const int secondRightRotate, const int rightShift)
{
	unsigned int firstRightRotatedNumber = rightRotateNumber(number, firstRightRotate);

	unsigned int secondRightRotatedNumber = rightRotateNumber(number, secondRightRotate);

	unsigned int rightShiftedNumber = (number >> rightShift);

	return firstRightRotatedNumber ^ secondRightRotatedNumber ^ rightShiftedNumber;
}

unsigned int returnValueOfSmallSigma(bool choice, unsigned int number)
{
	switch (choice)
	{
		case false:
		{
			return calculateSmallSigma(number, 7, 18, 3);
		}
		case true:
		{
			return calculateSmallSigma(number, 17, 19, 10);
		}
	}

	return -1;
}

void fillMessageSchedule(unsigned int messageSchedule[])
{
	unsigned int sigma0, sigma1;

	for (int i = BYTES16; i < BYTES64; i++)
	{
		sigma0 = returnValueOfSmallSigma(0, messageSchedule[i - 15]);

		sigma1 = returnValueOfSmallSigma(1, messageSchedule[i - 2]);

		messageSchedule[i] = messageSchedule[i - BYTES16] + sigma0 + messageSchedule[i - 7] + sigma1;
	}
}

//Generating hashed message
void updateValuesOfArrays(unsigned int valuesOfSmallLetterArray[], unsigned int updatedHashValuesArray[], const int size, bool choice)
{
	switch (choice)
	{
		case false:
		{
			for (int i = 0; i < size; i++)
			{
			valuesOfSmallLetterArray[i] = updatedHashValuesArray[i];
			}
			break;
		}
		case true:
		{
			for (int i = 0; i < size; i++)
			{
				updatedHashValuesArray[i] += valuesOfSmallLetterArray[i];
			}
			break;
		}
	}
}

unsigned int calculateBigSigma(unsigned int num, bool choice)
{
	switch (choice)
	{
		case false:
		{
			return rightRotateNumber(num, 2) ^ rightRotateNumber(num, 13) ^ rightRotateNumber(num, 22);
		}
		case true:
		{
			return rightRotateNumber(num, 6) ^ rightRotateNumber(num, 11) ^ rightRotateNumber(num, 25);
		}
	}

	return -1;
}

unsigned int calculateChoice(unsigned int e, unsigned int f, unsigned int g)
{
	return (e & f) ^ ((~e) & g);
}

unsigned int calculateMajority(unsigned int a, unsigned int b, unsigned int c)
{
	return (a & b) ^ (a & c) ^ (b & c);
}

unsigned int calculateTemp1(unsigned int h, unsigned int Sigma1, unsigned int choice, unsigned int const64Number, unsigned int numberFromMsgSchedule)
{
	return h + Sigma1 + choice + const64Number + numberFromMsgSchedule;
}

unsigned int calculateTemp2(unsigned int Sigma0, unsigned int majority)
{
	return Sigma0 + majority;
}

void updateSmallLetters(unsigned int valuesOfSmallLetterArray[], unsigned int temp1, unsigned int temp2, const int size)
{
	for (int i = size - 1; i > 0; i--)
	{
		if (i == 4)
		{
			valuesOfSmallLetterArray[i] = valuesOfSmallLetterArray[i - 1] + temp1;
		}
		else
		{
			valuesOfSmallLetterArray[i] = valuesOfSmallLetterArray[i - 1];
		}
	}

	valuesOfSmallLetterArray[0] = temp1 + temp2;
}

void generateHashedMessage(unsigned int updatedHashValuesArray[], unsigned int messageSchedule[])
{
	const int sizeOfSmallLetters = 8;

	unsigned int valuesOfSmallLetterArray[sizeOfSmallLetters];

	unsigned int temp1, temp2, Sigma0, Sigma1, choice, majority;

	updateValuesOfArrays(valuesOfSmallLetterArray, updatedHashValuesArray, sizeOfSmallLetters, 0);

	for (int i = 0; i < BYTES64; i++)
	{
		Sigma0 = calculateBigSigma(valuesOfSmallLetterArray[0], 0);

		Sigma1 = calculateBigSigma(valuesOfSmallLetterArray[4], 1);

		choice = calculateChoice(valuesOfSmallLetterArray[4], valuesOfSmallLetterArray[5], valuesOfSmallLetterArray[6]);

		majority = calculateMajority(valuesOfSmallLetterArray[0], valuesOfSmallLetterArray[1], valuesOfSmallLetterArray[2]);

		temp1 = calculateTemp1(valuesOfSmallLetterArray[7], Sigma1, choice, CONST64_ARRAY[i], messageSchedule[i]);

		temp2 = calculateTemp2(Sigma0, majority);

		updateSmallLetters(valuesOfSmallLetterArray, temp1, temp2, sizeOfSmallLetters);
	}

	updateValuesOfArrays(valuesOfSmallLetterArray, updatedHashValuesArray, sizeOfSmallLetters, 1);
}

void fillHashedMessageWithRecursion(unsigned int number, unsigned char hashedMessage[], int startingIndex, int times)
{
	int remainder = 0;

	if (number != 0 || times != BYTES8)
	{
		if (number == 0)
		{
			for (int i = times;i < BYTES8; i++)
			{
				hashedMessage[startingIndex--] = '0';//fill those with ascii code 0
			}
			return;
		}

		remainder = number % 16;

		if (remainder < 10)
		{
			hashedMessage[startingIndex--] = '0' + remainder;

			fillHashedMessageWithRecursion(number / 16, hashedMessage, startingIndex, ++times);
		}
		else
		{
			hashedMessage[startingIndex--] = 'a' + (remainder - 10);

			fillHashedMessageWithRecursion(number / 16, hashedMessage, startingIndex, ++times);

		}
	}
}

void convertToHex(unsigned int updatedHashValuesArray[], unsigned char hashedMessage[])
{
	int multiplier = 1;

	for (int i = 0; i < BYTES8; i++)
	{
		fillHashedMessageWithRecursion(updatedHashValuesArray[i], hashedMessage, multiplier * BYTES8 - 1, 0);
		multiplier++;
	}
}

void hashMessageWithSHA256(char* messageBlock, int current_size, const int MAX_SIZE, unsigned char hashedMessage[])
{
	if (!messageBlock)
		return;

	unsigned int messageSchedule[BYTES64]{};

	int multiplier = 1;

	unsigned int updatedHashValuesArray[BYTES8]{
		CONST8_ARRAY[0], CONST8_ARRAY[1], CONST8_ARRAY[2], CONST8_ARRAY[3],
		CONST8_ARRAY[4], CONST8_ARRAY[5], CONST8_ARRAY[6], CONST8_ARRAY[7]
	};

	while (multiplier * BYTES64 <= MAX_SIZE)
	{
		copyChunkIntoMessageSchedule(messageSchedule, messageBlock, (multiplier - 1) * BYTES64, multiplier * BYTES64);

		fillMessageSchedule(messageSchedule);

		generateHashedMessage(updatedHashValuesArray, messageSchedule);

		multiplier++;
	}

	convertToHex(updatedHashValuesArray, hashedMessage);
}

//Writing and reading hashed message
void writeHashedMessageInFile(unsigned char hashedMessage[])
{
	std::ofstream streamWriter(NAME_OF_WRITEFILE);

	if (!streamWriter.is_open())
		return;

	streamWriter << hashedMessage;

	streamWriter.close();
}

void readHashedMessageFromFile(unsigned char readHash[])
{
	std::ifstream streamReader(NAME_OF_WRITEFILE);

	if (!streamReader.is_open())
		return;

	streamReader >> readHash;

	streamReader.close();
}

bool checkIfHashesAreEqual(const unsigned char* readHash, const unsigned char* hashedMessage)
{
	if (!readHash || !hashedMessage)
		return false;

	int sizeOfReadHash = getSizeOfArray(readHash);

	int sizeOfHashedMessage = getSizeOfArray(readHash);

	if (sizeOfReadHash != sizeOfHashedMessage)
		return false;

	for (int i = 0; i < sizeOfReadHash; i++)
	{
		if (*(readHash++) != *(hashedMessage++))
		{
			return false;
		}
	}

	return true;
}