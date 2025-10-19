/*
Name: Darya Pylypenko
FSUID: dp24s
Due Date: Jul 29 2025
The program in this file is the individual work of Darya Pylypenko
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REFERENCES 100
#define ADDRESS_SIZE 32
#define MAX_ADDRESS ((1 << 30) - 1)
typedef struct {
    int valid, tag, dirty, lruCounter;
} CacheLine;
typedef struct {
    int blockSize, numSets, associativity;
    CacheLine **sets;
    int globalLruCounter;
} Cache;
int log2Int(int n) {
    int result = 0;
    while (n > 1) {
        n /= 2;
        result++;
    }
    return result;
}
Cache* createCache(int blockSize, int numSets, int associativity) {
    Cache *cache = malloc(sizeof(Cache));
    cache->blockSize = blockSize;
    cache->numSets = numSets;
    cache->associativity = associativity;
    cache->globalLruCounter = 0;
    cache->sets = malloc(numSets * sizeof(CacheLine*));
    for (int i = 0; i < numSets; i++) {
        cache->sets[i] = malloc(associativity * sizeof(CacheLine));
        for (int j = 0; j < associativity; j++) {
            cache->sets[i][j].valid = 0;
            cache->sets[i][j].tag = 0;
            cache->sets[i][j].dirty = 0;
            cache->sets[i][j].lruCounter = 0;
        }
    }
    return cache;
}
void freeCache(Cache *cache) {
    for (int i = 0; i < cache->numSets; i++) {
        free(cache->sets[i]);
    }
    free(cache->sets);
    free(cache);
}
void resetCache(Cache *cache) {
    cache->globalLruCounter = 0;
    for (int i = 0; i < cache->numSets; i++) {
        for (int j = 0; j < cache->associativity; j++) {
            cache->sets[i][j].valid = 0;
            cache->sets[i][j].tag = 0;
            cache->sets[i][j].dirty = 0;
            cache->sets[i][j].lruCounter = 0;
        }
    }
}
int findLRU(Cache *cache, int setIndex) {
    int lruWay = 0, minLru = cache->sets[setIndex][0].lruCounter;
    for (int i = 1; i < cache->associativity; i++) {
        if (cache->sets[setIndex][i].lruCounter < minLru) {
            minLru = cache->sets[setIndex][i].lruCounter;
            lruWay = i;
        }
    }
    return lruWay;
}
int findEmptyWay(Cache *cache, int setIndex) {
    for (int i = 0; i < cache->associativity; i++) {
        if (cache->sets[setIndex][i].valid == 0) return i;
    }
    return -1;
}
int checkHit(Cache *cache, int address) {
    int offsetBits = log2Int(cache->blockSize);
    int indexBits = log2Int(cache->numSets);
    int tag = address >> (offsetBits + indexBits);
    int setIndex = (address >> offsetBits) & ((1 << indexBits) - 1);
    for (int i = 0; i < cache->associativity; i++) {
        if (cache->sets[setIndex][i].valid == 1 && cache->sets[setIndex][i].tag == tag) {
            cache->globalLruCounter++;
            cache->sets[setIndex][i].lruCounter = cache->globalLruCounter;
            return i;
        }
    }
    return -1;
}
void insertAddress(Cache *cache, int address, int dirty) {
    int offsetBits = log2Int(cache->blockSize);
    int indexBits = log2Int(cache->numSets);
    int tag = address >> (offsetBits + indexBits);
    int setIndex = (address >> offsetBits) & ((1 << indexBits) - 1);
    int way = findEmptyWay(cache, setIndex);

    if (way == -1) way = findLRU(cache, setIndex);
    cache->globalLruCounter++;
    cache->sets[setIndex][way].valid = 1;
    cache->sets[setIndex][way].tag = tag;
    cache->sets[setIndex][way].dirty = dirty;
    cache->sets[setIndex][way].lruCounter = cache->globalLruCounter;
}
void simulateCache(Cache *cache, char *operations, int *addresses, int numReferences, 
                  int writeBack, int writeAllocate, const char *title) {
    int hits = 0, misses = 0, memoryReferences = 0;
    for (int i = 0; i < numReferences; i++) {
        char operation = operations[i];
        int address = addresses[i];
        int way = checkHit(cache, address);
        
        if (way != -1) {
            hits++;
            if (operation == 'W' && writeBack == 1) {
                int offsetBits = log2Int(cache->blockSize);
                int indexBits = log2Int(cache->numSets);
                int setIndex = (address >> offsetBits) & ((1 << indexBits) - 1);
                cache->sets[setIndex][way].dirty = 1;
            } else if (operation == 'W') {
                memoryReferences++;
            }
        } else {
            misses++;
            int offsetBits = log2Int(cache->blockSize);
            int indexBits = log2Int(cache->numSets);
            int setIndex = (address >> offsetBits) & ((1 << indexBits) - 1);
            int replaceWay = findEmptyWay(cache, setIndex);
            if (replaceWay == -1) {
                replaceWay = findLRU(cache, setIndex);
                if (cache->sets[setIndex][replaceWay].dirty == 1) {
                    memoryReferences++;
                }
            } 
            if (operation == 'R') {
                memoryReferences++;
                insertAddress(cache, address, 0);
            } else if (writeAllocate == 1) {
                memoryReferences++;
                insertAddress(cache, address, 1);
            } else {
                memoryReferences++;
            }
        }
    }
    printf("****************************************\n");
    printf("%s\n", title);
    printf("****************************************\n");
    printf("Total number of references: %d\n", numReferences);
    printf("Hits: %d\n", hits);
    printf("Misses: %d\n", misses);
    printf("Memory References: %d\n", memoryReferences);
}
void printConfig(int blockSize, int numSets, int associativity) {
    int offsetBits = log2Int(blockSize);
    int indexBits = log2Int(numSets);
    int tagBits = ADDRESS_SIZE - offsetBits - indexBits;
    
    printf("Block size: %d\n", blockSize);
    printf("Number of sets: %d\n", numSets);
    printf("Associativity: %d\n", associativity);
    printf("Number of offset bits: %d\n", offsetBits);
    printf("Number of index bits: %d\n", indexBits);
    printf("Number of tag bits: %d\n", tagBits);
}
int readInput(int *blockSize, int *numSets, int *associativity, 
              char **operations, int **addresses, char **types) {
    char line[256];
    char operation, type;
    int address, count = 0;
    
    *operations = malloc(MAX_REFERENCES * sizeof(char));
    *addresses = malloc(MAX_REFERENCES * sizeof(int));
    *types = malloc(MAX_REFERENCES * sizeof(char));
    
    scanf("%d %d %d", blockSize, numSets, associativity);
    
    while (fgets(line, sizeof(line), stdin)) {
        char *newline = strchr(line, '\n');
        if (newline) *newline = '\0';
        if (strlen(line) == 0) continue;
        
        int parsed = sscanf(line, "%c %d %c", &operation, &address, &type);
        
        if (parsed >= 2) {
            if (operation != 'R' && operation != 'W') continue;
            if (address < 0 || address > MAX_ADDRESS) continue;
            (*operations)[count] = operation;
            (*addresses)[count] = address;
            if (parsed == 3 && (type == 'I' || type == 'D')) {
                (*types)[count] = type;
            } else {
                (*types)[count] = 'X';
            }
            count++;
            if (count >= MAX_REFERENCES) break;
        }
    }
    return count;
}
int main() {
    int blockSize, numSets, associativity;
    char *operations, *types;
    int *addresses;
    int numReferences, hasSplit = 0;
    int splitSets, instructionCount = 0, dataCount = 0;
    char *instructionOperations, *dataOperations;
    int *instructionAddresses, *dataAddresses;
    Cache *cache, *unified, *instructionCache, *dataCache;
    numReferences = readInput(&blockSize, &numSets, &associativity, 
                              &operations, &addresses, &types);
    for (int i = 0; i < numReferences; i++) {
        if (types[i] == 'I' || types[i] == 'D') {
            hasSplit = 1;
            break;
        }
    }
    if (hasSplit == 1) {
        printf("================ Unified ================\n");
        printConfig(blockSize, numSets, associativity);
        unified = createCache(blockSize, numSets, associativity);
        simulateCache(unified, operations, addresses, numReferences, 0, 0, "Write-through with No Write Allocate");
        resetCache(unified);
        simulateCache(unified, operations, addresses, numReferences, 1, 1, "Write-back with Write Allocate");
        freeCache(unified);
        printf("============= Split I vs. D =============\n");
        splitSets = numSets / 2;
        printConfig(blockSize, splitSets, associativity);
        instructionOperations = malloc(MAX_REFERENCES * sizeof(char));
        dataOperations = malloc(MAX_REFERENCES * sizeof(char));
        instructionAddresses = malloc(MAX_REFERENCES * sizeof(int));
        dataAddresses = malloc(MAX_REFERENCES * sizeof(int));
        for (int i = 0; i < numReferences; i++) {
            if (types[i] == 'I') {
                instructionOperations[instructionCount] = operations[i];
                instructionAddresses[instructionCount] = addresses[i];
                instructionCount++;
            } else if (types[i] == 'D') {
                dataOperations[dataCount] = operations[i];
                dataAddresses[dataCount] = addresses[i];
                dataCount++;
            }
        }
        instructionCache = createCache(blockSize, splitSets, associativity);
        simulateCache(instructionCache, instructionOperations, instructionAddresses, instructionCount, 0, 0, "Instructions:");
        freeCache(instructionCache);
        dataCache = createCache(blockSize, splitSets, associativity);
        simulateCache(dataCache, dataOperations, dataAddresses, dataCount, 0, 0, "Data: Write-through with No Write Allocate");
        resetCache(dataCache);
        simulateCache(dataCache, dataOperations, dataAddresses, dataCount, 1, 1, "Data: Write-back with Write Allocate");
        freeCache(dataCache);
        free(instructionOperations);
        free(dataOperations);
        free(instructionAddresses);
        free(dataAddresses);
    } else {
        printConfig(blockSize, numSets, associativity);
        cache = createCache(blockSize, numSets, associativity);
        simulateCache(cache, operations, addresses, numReferences, 0, 0, "Write-through with No Write Allocate");
        resetCache(cache);
        simulateCache(cache, operations, addresses, numReferences, 1, 1, "Write-back with Write Allocate");
        freeCache(cache);
    }
    free(operations);
    free(addresses);
    free(types);
    return 0;
} 
