#ifndef MEMBERTYPE_H_INCLUDED
#define MEMBERTYPE_H_INCLUDED

enum ElementType {
	ET_None = 0x0,
	ET_Word = 0x28,
	ET_DWord = 0x29,
	ET_Float = 0x2A,
	ET_Char = 0x2C,
	ET_UChar = 0x2D,
	ET_String = 0x33,
	ET_Array = 0x34,
	ET_Template = 0x1F,
	ET_TemplateArray = 0x3F
};

union TemplateGuid {
	char rawData[16];
	struct {
		unsigned int   Data1;
		unsigned short Data2;
		unsigned short Data3;
		unsigned char  Data4[8];
	};

	TemplateGuid() {
		int i;
		for(i=0; i<16; i++)
			rawData[i] = 0;
	}
};

#endif // MEMBERTYPE_H_INCLUDED
