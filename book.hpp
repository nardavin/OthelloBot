#include <map>
#include <string>
#include <iostream>
#include <stdio.h>
#include "store.hpp"

class CBookData {
public:
    CBookData();
}

class CBook {
public:
    /* The reason there is an initializer and a FindBook function is for
     * generalisation. Theoretically this program could be modified to use more
     * than one book. Also, ntest did it this way so it's easier to copy. */
    CBook(const char* filename); //, std::ostream* os=&std::cerr);
    //FindBook();
    ~CBook();
    bool Load(const Board& mr, CHeightInfo hi, CValue alpha, CValue beta, CValue& value, int nEmpty) const;
    const CBookData* FindData(const Board& b) const;

    void Read();
    IsInBook(Board* CurrentBoard);
    GetBestMove(Board* CurrentBoard);
protected:
    std::map<Board, CBookData> entries[64-3];

private:
    std::auto_ptr<Store> m_store; /* Location of the read book */

CBook::CBook(const char* filename){ //, std::ostream* os) {
    m_store = std::auto_ptr<Store>(new File(filename));
    Read();
}

void CBook::Read() {
    return;
}

// CBook* CBook::FindBook() {
//     /* In order to add several books, make a list of books and iterate over it.
//      * If it wasn't found, then make new. Otherwise it's already built.
//      * That's more efficient but harder, isn't necessary atm. */
//     CBook* result;
//     result = new CBook();
//     return result;
// }
