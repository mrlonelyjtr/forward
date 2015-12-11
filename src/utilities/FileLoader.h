//--------------------------------------------------------------------------------
// FileLoader
//
// The FileLoader class is a helper class for loading simple files.  The contents
// of the desired file are loaded with the Open() method, and an internally memory
// is allocated to hold the entire contents of the file.  This memory can then be
// accessed by the user, and it is freed when the class instance is destroyed or
// the Close() method is called.
//
// Because of this, it is convenient to declare an instance of this class on the 
// stack.  Then once it goes out of scope, the memory is automatically freed, and
// you don't have to worry about releasing it.
//
// This class should also be compatible with Win7 classic, Win8 desktop, and
// Win8 Metro style applications.
//--------------------------------------------------------------------------------
#ifndef FileLoader_h
#define FileLoader_h
//--------------------------------------------------------------------------------
#include "PCH.h"
//--------------------------------------------------------------------------------
namespace forward
{
	class FileLoader 
	{
	public:
		FileLoader();
		~FileLoader();

		bool Open( const std::wstring& filename );
		bool Close( );

		char* GetDataPtr();
		u32 GetDataSize();

	protected:
		char*			m_pData;
		u32	m_uiSize;

	};
};
//--------------------------------------------------------------------------------
#endif // FileLoader_h
//--------------------------------------------------------------------------------