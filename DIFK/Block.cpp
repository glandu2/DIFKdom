#include "Block.h"
#include <stdlib.h>
#include <iostream>
#include <iomanip>

namespace DIFK {

void Block::load(FILE* file) {
	char magic;

	fread(&magic, 1, 1, file);
	elementType = getTypeFromByte(magic);
	if(elementType == ET_None) {
		std::cerr << "Unknown block type: " << (int)magic << " at offset " << ftell(file) << '\n';
		exit(-1);
	}

	if(elementType == ET_Array) {
		int stringSize;

		fread(&magic, 1, 1, file);
		elementType = getTypeFromByte(magic);
		if(elementType == ET_None) {
			std::cerr << "Unknown block type: " << (int)magic << " at offset " << ftell(file) << '\n';
			exit(-1);
		} else if(elementType == ET_Array) {
			std::cerr << "Array of array not supported" << " at offset " << ftell(file) << '\n';
			exit(-1);
		} else if(elementType == ET_Template) {
			elementType = ET_TemplateArray;
			fread(&templateGuid.rawData[0], 16, 1, file);
		}

		fread(&numElement, 4, 1, file);

		fread(&stringSize, 4, 1, file);
		if(stringSize > 0) {
			associatedString = new(std::nothrow) char[stringSize+1];
			fread(associatedString, 1, stringSize, file);
			associatedString[stringSize] = 0;
		}
	} else if(elementType == ET_Template) {
		fread(&templateGuid.rawData[0], 16, 1, file);
		fread(&numElement, 4, 1, file);
	} else numElement = 1;

	int i;
	switch(elementType) {
		case ET_Char:
		case ET_UChar:
			data = new(std::nothrow) char[numElement];
			for(i=0; i<numElement; i++)
				fread(static_cast<char*>(data) + i, 1, 1, file);
			break;

		case ET_Word:
			data = new(std::nothrow) short[numElement];
			for(i=0; i<numElement; i++)
				fread(static_cast<short*>(data) + i, 2, 1, file);
			break;

		case ET_DWord:
			data = new(std::nothrow) int[numElement];
			for(i=0; i<numElement; i++)
				fread(static_cast<int*>(data) + i, 4, 1, file);
			break;

		case ET_Float:
			data = new(std::nothrow) float[numElement];
			for(i=0; i<numElement; i++)
				fread(static_cast<float*>(data) + i, 4, 1, file);
			break;

		case ET_String:
			data = new(std::nothrow) char*[numElement];
			for(i=0; i<numElement; i++) {
				int stringSize;
				fread(&stringSize, 4, 1, file);
				if(stringSize > 0) {
					static_cast<char**>(data)[i] = new(std::nothrow) char[stringSize+1];
					fread(static_cast<char**>(data)[i], 1, stringSize, file);
					(static_cast<char**>(data)[i])[stringSize] = 0;
				} else static_cast<char**>(data)[i] = 0;
			}
			break;

		case ET_Template:
		case ET_TemplateArray:
			data = new(std::nothrow) Block[numElement];
			for(i=0; i<numElement; i++)
				static_cast<Block*>(data)[i].load(file);
			break;

		case ET_Array:
		case ET_None:
			std::cerr << "Internal error, magic is None or Array\n";
			exit(-1);
	}
}

ElementType Block::getTypeFromByte(char data) {
	switch(data) {
		case 31: return ET_Template;
		case 40: return ET_Word;
		case 41: return ET_DWord;
		case 42: return ET_Float;
		case 44: return ET_Char;
		case 45: return ET_UChar;
		case 51: return ET_String;
		case 52: return ET_Array;
	}

	return ET_None;
}

Block* Block::getBlock(int index) {
	if(index >= numElement)
		exit(-2);
	return static_cast<Block*>(data) + index;
}

void Block::dumpToStdout() {
	int i;

	std::cout << std::resetiosflags((std::ios_base::fmtflags)-1);
	std::cout << "Block with " << numElement << " elements: ";

	switch(elementType) {
		case ET_Char:
			std::cout << "Char, value = ";
			for(i=0; i<numElement; i++)
				std::cout << (int)static_cast<char*>(data)[i] << ", ";
			break;

		case ET_UChar:
			std::cout << "UChar, value = ";
			for(i=0; i<numElement; i++)
				std::cout << (unsigned int)static_cast<unsigned char*>(data)[i] << ", ";
			break;

		case ET_Word:
			std::cout << "Word, value = ";
			for(i=0; i<numElement; i++)
				std::cout << static_cast<short*>(data)[i] << ", ";
			break;

		case ET_DWord:
			std::cout << "DWord, value = ";
			for(i=0; i<numElement; i++)
				std::cout << static_cast<int*>(data)[i] << ", ";
			break;

		case ET_Float:
			std::cout << "Float, value = " << std::setprecision(5);
			for(i=0; i<numElement; i++)
				std::cout << static_cast<float*>(data)[i] << ", ";
			break;

		case ET_String:
			std::cout << "String\n{\n";
			for(i=0; i<numElement; i++)
				std::cout << static_cast<char**>(data)[i] << '\n';
			std::cout << "}";
			break;

		case ET_Template:
			std::cout << "Template GUID = " << std::hex << std::uppercase <<
						(unsigned int)templateGuid.Data1 << '-' << (unsigned int)templateGuid.Data2 << '-' << (unsigned int)templateGuid.Data3 << '-' <<
                        (unsigned int)templateGuid.Data4[0] << (unsigned int)templateGuid.Data4[1] << '-' << (unsigned int)templateGuid.Data4[2] <<
                        (unsigned int)templateGuid.Data4[3] << (unsigned int)templateGuid.Data4[4] << (unsigned int)templateGuid.Data4[5] <<
                        (unsigned int)templateGuid.Data4[6] << (unsigned int)templateGuid.Data4[7] << "\n{\n";
			for(i=0; i<numElement; i++)
				static_cast<Block*>(data)[i].dumpToStdout();
			std::cout << "}";
			break;

		case ET_TemplateArray:
			std::cout << "Template array\n{\n";
			for(i=0; i<numElement; i++)
				static_cast<Block*>(data)[i].dumpToStdout();
			std::cout << "}";
			break;

		case ET_Array:
		case ET_None:
			std::cerr << "Internal error, magic is None or Array\n";
			exit(-1);
	}
	std::cout << std::resetiosflags((std::ios_base::fmtflags)-1);

	std::cout << '\n';
}

} // namespace DIFK
