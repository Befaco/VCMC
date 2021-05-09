
#ifndef __NoteEvent_h
#define __NoteEvent_h

#include "../ArduinoTools.h"
#include "BitField128.h"
#include "Intrusive.h"
//#include "Voice.h"

const uint8_t MAXNOTES = 20;

class Voice;

class NoteEvent : public ItemHook
{
public:
    uint8_t chann = 1;
    uint8_t pitch = 0;
    uint8_t velocity = 0;
    uint32_t timestamp = 0;
    //Voice *pVoice = nullptr;

    virtual void reset(){
        chann = pitch = velocity = timestamp = 0;
        //pVoice = nullptr;
        ItemHook::reset();
    }
    virtual void print(const char *text = nullptr){
        //ItemHook::print(text);
        if( text)
            debPrint(text);
        else
            debPrint("  ");
        debPrintf("id:%d c: %d p:%d v:%d t:%" PRIu32 "\n", getIdx(), chann, pitch, velocity, timestamp);
        }
    void fill(uint8_t ch, uint8_t pit, uint8_t vel, uint32_t time){
        chann = ch;
        pitch = pit;
        velocity = vel;
        timestamp = time;
    }
    void fill(NoteEvent *pEv){
        chann = pEv->chann;
        pitch = pEv->pitch;
        velocity = pEv->velocity;
        timestamp = pEv->timestamp;
    }
    virtual void free(void) {
        reset();
        }
};

/*
class NoteEventList
{
protected:
    HashTable<uint8_t, NoteEvent, MAXNOTES> _evTable;
    BitField128 noteField_; // TODO use only hash table or also this for more efficient check
public:
    virtual bool isPlaying(uint8_t pitch){
        return noteField_.isbitset128((size_t)pitch);
        }

    virtual NoteEvent *setPlaying(uint8_t pitch, uint8_t velocity, Voice *pV = nullptr, uint32_t time = 0){
        NoteEvent *pEv = nullptr;
        if (velocity){ // Note On
            if (!noteField_.isbitset128(pitch)){
                noteField_.setbit128(pitch);
                pEv = _evTable.insert(pitch);
                if(!pEv){ // Not possible to insert
                    DP("Note Not inserted");
                    return nullptr;}
                pEv->fill(pitch, velocity, time, pV);
            }
            else
                return nullptr; // Pitch already active
        }
        else
        { // Note off
            if (noteField_.isbitset128(pitch)){
                noteField_.unsetbit128(pitch);
                // Remove note from the list
                _evTable.remove(pitch);
                return nullptr;
            }
        }

        return pEv;
    }

    NoteEvent *find(uint8_t pitch){
        return _evTable.find(pitch);
    }

    uint8_t size(){
        return _evTable.size();
    }

    void clear(){
        _evTable.clear();
        noteField_.clearbitset128();
    }

    virtual void print(const char *text = nullptr)
    {
        if (text)
            debPrint(text);
        debPrintf("Items in list: %d\n", size());
        for (NoteEvent *pEv = _evTable.front(); pEv; pEv = (NoteEvent *)pEv->next()){
            //Serial.print("  ");
            pEv->print("  ");
        }
    }

};
//byte NoteEventList::findpitch=0;
*/

#endif
