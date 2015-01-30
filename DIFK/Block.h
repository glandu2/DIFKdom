#ifndef DIFK_BLOCK_H
#define DIFK_BLOCK_H

#include <stdio.h>
#include "MemberType.h"
#include <assert.h>
#include <stdlib.h>

namespace DIFK {

class Block
{
	public:
		Block() : elementType(ET_None), numElement(0), data(0), associatedString(0) {};

		void load(FILE* file);

		void* getData() { return data; }
		ElementType getType() { return elementType; }

		void dumpToStdout();

		template<typename T> T getData(int index);
		template<typename T> T getDataPtr();
		Block* getBlock(int index);

	protected:
		static ElementType getTypeFromByte(char data);

	private:
		ElementType elementType;
		int numElement;
		void* data;
		TemplateGuid templateGuid;
		char* associatedString;
};


template<typename T> T Block::getData(int index) {
	if(index >= numElement)
		::exit(-2);
	return static_cast<T*>(data)[index];
}

template<typename T> T Block::getDataPtr() {
	return static_cast<T>(data);
}

} // namespace DIFK

#endif // DIFK_BLOCK_H
