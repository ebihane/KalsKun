#pragma once

class SequencerBase
{
public :

    typedef enum
    {
        E_SEQ_IDLE = 0,
        E_SEQ_KUSAKARI,
        E_SEQ_YAKEI,
        E_SEQ_SETTING,
        E_SEQ_ERROR,
    } SequenceTypeEnum;

    SequencerBase(const SequenceTypeEnum type) : MY_SEQUENCE_TYPE(type) {};
    virtual ~SequencerBase() {};

    virtual void Initialize() = 0;
    virtual void Destroy() = 0;
    virtual SequenceTypeEnum Process() = 0;

    inline SequenceTypeEnum GetSequence()
    {
        return MY_SEQUENCE_TYPE;
    }

protected :

    const SequenceTypeEnum MY_SEQUENCE_TYPE;

private :

};
