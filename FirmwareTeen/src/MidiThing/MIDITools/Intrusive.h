

#ifndef __Intrusive_h
#define __Intrusive_h
#include "../ArduinoTools.h"


#ifdef DEBUG_NOTEEVENT
#define DEB(x) x		/// Shorthand for Debug Print
#define DEBP(x) Serial.println(x)		/// Shorthand for Deug serial print
#else
#define DEB(x)
#define DEBP(x)
#endif


const size_t MAX_ITEMS = 50;
const uint8_t INVALIDIDX = 0xff;

//< Base class to use for items on Intrusive container
class ItemHook
{
public:
    // Define friend classes
    template <class uIt, size_t umaxItems>
    friend class ItemFixCollection;
    template <class uIt, size_t umaxItems>
    friend class FixList;

protected:
    bool inUse = false;
    uint8_t idx = INVALIDIDX;
    ItemHook *pNext = nullptr;
    ItemHook *pPrev = nullptr;
    void *pParent = nullptr;
    //void *pInfo = nullptr;

    void use(uint8_t index)
    {
        inUse = true;
        idx = index;
    }

public:
    bool isParent(void *par) { return (pParent == par); }
    ItemHook *next(void) { return pNext; }
    ItemHook *prev(void) { return pPrev; }
    uint8_t getIdx(void) { return idx; }
    bool isFree(void) { return !inUse; }
    virtual void free(void) { reset(); }
    virtual void reset(){
        inUse = false;
        pNext = pPrev = nullptr;
        pParent = /* pInfo = */ nullptr;
        idx = INVALIDIDX;
        };
    virtual void print(const char *text = nullptr)
    {
        if (text)
            debPrint(text);
        /////////////
        debPrintf("%d,%X,%X:%X\n",getIdx(),pParent,pPrev,pNext);
    }
};

template <class T>
class BasicItem : public ItemHook
{
public:
    T data;
    BasicItem *next(void) { return (BasicItem *)pNext; }
    BasicItem *prev(void) { return (BasicItem *)pPrev; }
};


/**
 * \brief Fix size collection. Circular pos assignment
 * 
 * \tparam Item Object to include in the collection (must be derived from ItemHook)
 * \tparam maxIt Max number of elements in collection
 */
template <class Item, size_t maxIt = MAX_ITEMS>
class ItemFixCollection
{
protected:
    uint8_t _pushIndex = 0;
    Item mIt[maxIt];
    //Item errorItem;

public:
    //< Set all items available (free all)
    void resetItems(void)
    {
        for (size_t i = 0; i < maxIt; i++)
        {
            mIt[i].reset();
        }
    }
    //< Get reference to a new item (like new)
    // If all items are in use will return null
    Item *getItem(void)
    {
        uint8_t saveIndex = _pushIndex;

        do
        {
            _pushIndex++;
            if (_pushIndex == maxIt)
                _pushIndex = 0;
            if (_pushIndex == saveIndex)
            {
                DEBP("Event queue full!");
                return nullptr;
            }
        } while (!mIt[_pushIndex].isFree());

        mIt[_pushIndex].use(_pushIndex);
        mIt[_pushIndex].pParent = this;
        return &mIt[_pushIndex];
    }
    //< Release item (like free)
    bool releaseItem(Item *it)
    {
        // Check validity of object: 
        //  object not null, item not in use, this collection is his container
        if (!it || it->isFree() || !it->isParent(this)){
            DEBP("Not possible to release");
            return false;}
        uint8_t index = it->getIdx();
        if(index==INVALIDIDX){
            DEBP("Id not valid");
            return false;
        }
        mIt[index].free();
        if (index != _pushIndex)
            return true;
        // Item to release was at head of queue, update push position
        if (_pushIndex)
            _pushIndex--;
        else
            _pushIndex = maxIt-1;

        return true;
    }
};

/**
 * \brief Double linked list. Fix allocation.
 * Interface like STL, no iterators.
 * 
 * \tparam It Object type to include in the collection
 * \tparam maxItems Max number of elements in collection
 */
template <class It, size_t maxItems = MAX_ITEMS>
class FixList : public ItemFixCollection<It, maxItems>
{
protected:
    It *_first = nullptr;
    It *_last = nullptr;
    size_t _count = 0;

public:
    //< Number of Elements active in table
    size_t size(void) { return this->_count; }
    //< Returns first element
    It *front(void) { return _first; }
    //< Returns last element
    It *back(void) { return _last; }
    void clear(void)
    {
        this->resetItems();
        _count = 0;
        _first = _last = nullptr;
    }
    //< Activates new element and place it as first element
    It *push_front(void)
    {
        It *pnew = this->getItem();
        if (!pnew) // list full
        {
            DEBP("List full");
            return nullptr;
            }
        pnew->pPrev = nullptr;
        pnew->pNext = _first;
        if (_first) // check if first element exist
            _first->pPrev = pnew;
        else // new element is the first in the list
            _last = pnew;
        _first = pnew;
        _count++;
        return pnew;
    }
    //< Activates new element and place it as last element
    It *push_back(void)
    {
        It *pnew = this->getItem();
        if (!pnew) // list full
        {
            DEBP("List full");
            return nullptr;
            }
        pnew->pPrev = _last;
        pnew->pNext = nullptr;
        if (_last) // check if _last element exist
            _last->pNext = pnew;
        else // new element is the first in the list
            _first = pnew;
        _last = pnew;
        _count++;
        return pnew;
    }
    //< Insert before passed element
    It *insert_before(It *pAct)
    {
        if( !pAct || pAct->pParent) // Not a valid element
            return nullptr;
        It *pnew = this->getItem();
        if (!pnew) // list full
            return nullptr;
        pnew->pPrev = pAct->pPrev;
        if(!pAct->pPrev) // It was first element in the list
            _first = pnew;
        pnew->pNext = pAct;
        pAct->pPrev = pnew;
        _count++;
        return pnew;
    }
    void pop_front(void)
    {
        if (!_first) // Empty list
            return;
        It *next = _first->pNext;
        this->releaseItem(_first); // Free item
        if (!next)                // Removing Last element in list
            _last = nullptr;
        _first = next;
        _count--;
    }
    void pop_back(void)
    {
        if (!_last)
            return;
        It *prev = _last->pPrev;
        this->releaseItem(_last); // Free item
        if (!prev)               // Removing Last element in list
            _first = nullptr;
        _last = prev;
        _count--;
    }
    bool remove(It *rem)
    {
        if (_count == 0 ||               // table is empty
            !rem ||                      // nullptr received
            !rem->isParent(this) ||      // element not in this list
            rem->getIdx() == INVALIDIDX) // currently not in any collection`
            {
                DEBP("Error removing from FixList");
                return false;
            }
        It *prev = (It*)rem->pPrev;
        It *next = (It*)rem->pNext;
        if (prev)
            prev->pNext = next;
        else // Element was _first in list
            _first = next;
        if (next)
            next->pPrev = prev;
        else // Element was _last in list
            _last = prev;
        this->releaseItem(rem);
        if(!_count){
            DEBP("          Error removing item");
            return false;
        }
        _count--;
        if(!_count){
            DEBP("          List empty");
        }

        return true;
    }
    void print(const char *text = nullptr)
    {
        if (text)
            debPrintf(text);
        debPrintf("Items in list: %d\n", this->size());
        for (It *pEv = this->front(); //, *pEnd = this->back();
             pEv; //pEv != pEnd;
             pEv = (It *)pEv->next())
        {
            debPrintf("  ");
            pEv->print();
        }
    }
};

/**
 * \brief Unsorted intrusive hash table
 * 
 * \tparam hash Type for has key
 * \tparam It Object type to include in the collection
 * \tparam maxItems Max number of elements in collection
 */
template <class hash, class It, size_t maxItems = MAX_ITEMS>
class HashTable : public FixList<It, maxItems>
{
protected:
    hash _keys[maxItems] = {0};
    uint8_t findKey(hash fkey)
    {
        // TODO traverse elements in list instead of this approach?
        for (size_t i = 0; i < maxItems; i++)
            if (_keys[i] == fkey)
                return i;
        return INVALIDIDX;
    }

public:
    //< Clear elements and keys
    void clear(void)
    {
        FixList<It, maxItems>::clear();
        for (size_t i = 0; i < maxItems; i++)
        {
            _keys[i] = 0;
        }
    }
    //< find element by hash. Return nullptr if not in table
    It *find(hash fkey)
    {
        if (!this->_count){ // table is empty
            //DEBP("Table is empty, zero count");
            return nullptr;}
        uint8_t fpos = findKey(fkey);
        if (fpos == INVALIDIDX){ // currently not in table
            DEBP("Hash Key Not found in table");
            return nullptr;}
        else
            return &this->mIt[fpos];
    }
    //< insert element by hash or return existing if hash already in table
    It *insert(hash newkey)
    {
        if (this->_count == maxItems) // table full
            return nullptr;
        uint8_t fpos = findKey(newkey);
        if (fpos == INVALIDIDX)
        {                                  // currently not in table
            It *pIt = this->push_back();  // Reserve item in collection
            _keys[pIt->getIdx()] = newkey; // store key
            //DEB(pIt->print("Insert hash key "));
            return pIt;
        }
        else
            return &this->mIt[fpos];
    }
    //< Remove element by hashkey. Return false if invalid hash.
    bool remove(hash oldkey)
    {
        if (this->_count == 0){ // table is empty
            DEBP("Table is empty");
            return false;}
        uint8_t fpos = findKey(oldkey);
        if (fpos == INVALIDIDX){ // currently not in table
            DEBP("Not removed. Key Not in table");
            return false;}
        else
        {
            DEB(this->mIt[fpos].print("Remove hash key "));
            _keys[fpos] = 0;
            if(FixList<It, maxItems>::remove(&this->mIt[fpos]))
                DEBP("Removed hash key");
            else
                DEBP("Hash key Not removed");            
            //FixList<It, maxItems>::remove(&this->mIt[fpos]);
            //DEB(this->mIt[fpos].print("Removed hash key "));
        }
        return true;
    }
    //< Remove element by element address. Return false if invalid.
    bool remove(It* pIt)
    {
        if (this->_count == 0 || // table is empty
            !pIt->isParent(this)){ // element doe not belong in this collection
            DEBP("Error, element not removed");
            return false;}

        uint8_t fpos = pIt->getIdx();
        if (fpos == INVALIDIDX){ // currently not in table
            DEBP("Object Key Not in table");
            return false;}
        else
        {
            //DEB(this->mIt[fpos].print("Remove hash key "));
            //DEBP(fpos);
            _keys[fpos] = 0;
            if(FixList<It, maxItems>::remove(&this->mIt[fpos]))
                DEBP("");//"Removed hash key ");
            else
                DEBP("Hash key Not removed");
        }
        return true;
    }

};

#undef DEB
#undef DEBP


#endif
