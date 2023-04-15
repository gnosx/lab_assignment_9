#include <stdio.h>
#include <stdlib.h>

#define HASH_SIZE 11

// RecordType
struct RecordType
{
	int id;
	char name;
	int order;
	struct RecordType *nextNode;
};
// Fill out this structure
struct HashType
{
	struct RecordType *table;
};

// Compute the hash function
int hash(int x)
{
	return x % HASH_SIZE;
}

// parses input file to an integer array
int parseData(char *inputFileName, struct RecordType **ppData)
{
	FILE *inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType *)malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	printf("\nRecords:\n");
	for (int i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

void insertData(struct RecordType hMap[], struct RecordType pData[], int dataSz, int hashSz)
{

	// set all next nodes to null
	for (int i = 0; i < hashSz; i++)
	{
		hMap[i].id = 0;
		hMap[i].nextNode = NULL;
	}

	// loop through the pData and set to hash
	for (int i = 0; i < dataSz; ++i)
	{
		// get the key
		int key = hash(pData[i].id);

		// check if key has been used
		if (hMap[key].id == 0)
		{
			//	if not set key at hash = to data
			hMap[key] = pData[i];
		}
		// if key has been used
		else
		{
			// make a node to find the end
			struct RecordType *lastNode = &hMap[key];
			// push the linked list until end
			while (lastNode->nextNode != NULL)
			{
				lastNode = lastNode->nextNode;
			}
			// make a new node to store the data
			struct RecordType *node = malloc(sizeof(struct RecordType));
			// add all the data into the new node
			node->id = pData[i].id;
			node->name = pData[i].name;
			node->order = pData[i].order;
			node->nextNode = NULL;

			// set the last node to the new node
			lastNode->nextNode = node;
		}
	}
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct RecordType hMap[], int hashSz)
{

	for (int i = 0; i < hashSz; i++)
	{

		if(hMap[i].id != 0){
			struct RecordType *node = &hMap[i];
			printf("index: %d ", i);
			while(node != NULL){
				printf("-> %d, %c, %d ", node->id, node->name, node->order);
				node = node->nextNode;
			}
			printf("\n");
			// if index is occupied with any records, print all
		}
	}
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);

	// make the hash array
	struct RecordType hMap[HASH_SIZE];

	// implement the hash
	insertData(hMap, pRecords, recordSz, HASH_SIZE);
	// print the hash map
	displayRecordsInHash(hMap, HASH_SIZE);
}