#include "DIFKFile.h"
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include "Block.h"

namespace DIFK {

void DIFKFile::load(FILE* file) {
	GlobalHeader gHeader;
	BlockHeader bHeader;
	int i;

	fread(&gHeader, sizeof(gHeader), 1, file);
	if(strncmp(gHeader.difk, "DIFK", 4) || strncmp(gHeader.nibk, "NIBK", 4) || gHeader.nblocks < 0) {
		std::cerr << "Invalid file\n";
		exit(-3);
	}

	version = gHeader.master_version;
	numBlocks = gHeader.nblocks;
	rootBlocks = new Block[numBlocks];

	for(i=0; i<numBlocks; i++) {
		fread(&bHeader, sizeof(bHeader), 1, file);
		if(strncmp(bHeader.klbk, "KLBK", 4)) {
			std::cerr << "Invalid block\n";
			exit(-3);
		}
		rootBlocks[i].load(file);
	}
}

void DIFKFile::dumpToStdout() {
	std::cout << "KFID KBIN File\n" << numBlocks << " root blocks\n";
	for(int i = 0; i<numBlocks; i++) {
		std::cout << "Block no" << i << "\n{\n";
		rootBlocks[i].dumpToStdout();
		std::cout << "}\n";
	}
}

Block* DIFKFile::getRootBlock(int i) {
	if(i < numBlocks)
		return rootBlocks + i;
	else return 0;
}

} // namespace DIFK
