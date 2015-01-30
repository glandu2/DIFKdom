#ifndef DIFK_DIFKFILE_H
#define DIFK_DIFKFILE_H

#include <stdio.h>
#include "MemberType.h"

namespace DIFK {

class Block;

class DIFKFile
{
	public:
		DIFKFile() : version(0), numBlocks(0), rootBlocks(0) {}

		void load(FILE* file);
		void dumpToStdout();

		int getNumBlocks() { return numBlocks; }
		Block* getRootBlock(int i);

	private:
		int version;
		int numBlocks;
		Block* rootBlocks;


		struct GlobalHeader {
			char difk[4];
			char nibk[4];
			int master_version;
			int sub_version;
			int nblocks;
			char reserved[108];
		};

		struct BlockHeader {
			char klbk[4];
			int blocksize;
			char reserved[24];
		};
};

} // namespace DIFK

#endif // DIFK_DIFKFILE_H
