
//#warning "this is deprecated"


#pragma message("")  

#pragma message( "Compiling " __FILE__ )   
#pragma message( "Last modified on " __TIMESTAMP__ )  

#pragma message("")  

// with line number  
#define STRING2(x) #x  
#define STRING(x) STRING2(x)  

#define BUG001
#define BUG002
#define BUG003

#if !defined(BUG001)
#pragma message (__FILE__ "[" STRING(__LINE__) "]: sometimes read files from FS is greater than the total")  
#endif
#if !defined(BUG002)
#pragma message (__FILE__ "[" STRING(__LINE__) "]: combos for maxYears and maxImdbRating need sorting")  
#endif

#if !defined(BUG003)
#pragma message (__FILE__ "[" STRING(__LINE__) "]: {1117} normal block at 0x007B82A8, 14 bytes long.")  
#pragma message (__FILE__ "[" STRING(__LINE__) "]: Data: <Rating  > 52 00 61 00 74 00 69 00 6E 00 67 00 00 00")  
#pragma message (__FILE__ "[" STRING(__LINE__) "]: {1116} normal block at 0x007B7F28, 14 bytes long.")  
#pragma message (__FILE__ "[" STRING(__LINE__) "]: Data : <ImdbID  > 49 00 6D 00 64 00 62 00 49 00 44 00 00 00")  
#pragma message (__FILE__ "[" STRING(__LINE__) "]: {1115} normal block at 0x007B7DE8, 14 bytes long.")  
#pragma message (__FILE__ "[" STRING(__LINE__) "]: Data : <isAnim  > 69 00 73 00 41 00 6E 00 69 00 6D 00 00 00")  
#pragma message (__FILE__ "[" STRING(__LINE__) "]: {1114} normal block at 0x007B7728, 16 bytes long.")  
#pragma message (__FILE__ "[" STRING(__LINE__) "]: Data : <isSerie  > 69 00 73 00 53 00 65 00 72 00 69 00 65 00 00 00")  
#pragma message (__FILE__ "[" STRING(__LINE__) "]: {1113} normal block at 0x007CB8D8, 10 bytes long.")  
#pragma message (__FILE__ "[" STRING(__LINE__) "]: Data : <Size  > 53 00 69 00 7A 00 65 00 00 00")  
#pragma message (__FILE__ "[" STRING(__LINE__) "]: {1112} normal block at 0x007CB8A0, 12 bytes long.")  
#pragma message (__FILE__ "[" STRING(__LINE__) "]: Data : <Added  > 41 00 64 00 64 00 65 00 64 00 00 00")  
#pragma message (__FILE__ "[" STRING(__LINE__) "]: {1111} normal block at 0x007CB718, 12 bytes long.")  
#pragma message (__FILE__ "[" STRING(__LINE__) "]: Data : <isDel  > 69 00 73 00 44 00 65 00 6C 00 00 00")  
#pragma message (__FILE__ "[" STRING(__LINE__) "]: {1110} normal block at 0x007CB5C8, 10 bytes long.")  
#pragma message (__FILE__ "[" STRING(__LINE__) "]: Data : <Name  > 4E 00 61 00 6D 00 65 00 00 00")  
#pragma message (__FILE__ "[" STRING(__LINE__) "]: {1109} normal block at 0x007CB868, 10 bytes long.")  
#pragma message (__FILE__ "[" STRING(__LINE__) "]: Data : <Path  > 50 00 61 00 74 00 68 00 00 00")  
#pragma message (__FILE__ "[" STRING(__LINE__) "]: {1108} normal block at 0x007CB590, 10 bytes long.")  
#pragma message (__FILE__ "[" STRING(__LINE__) "]: Data : <FsId  > 46 00 73 00 49 00 64 00 00 00")  
#endif

#if !defined(BUG004)
#pragma message (__FILE__ "[" STRING(__LINE__) "]: curl::curl_easy perform() fails if no connection")  
#endif
#if !defined(BUG005)
#pragma message (__FILE__ "[" STRING(__LINE__) "]: deleting extention while editing filename")  
#endif


#pragma message("")