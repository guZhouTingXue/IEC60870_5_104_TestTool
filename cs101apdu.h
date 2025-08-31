#ifndef CS101APDU_H
#define CS101APDU_H

#include <QObject>
#include <qdatetime.h>
#include <qqmlintegration.h>

#include "iec60870_common.h"

class APCI {
public:
    enum Type {
        IFormat,
        SFormat,
        UFormat,
    };
    virtual ~APCI() { }
    virtual QString display() = 0;

    Type type() const { return m_type; }
protected:
    void copyFromBuffer(const uint8_t *b) {
        for(int i=0; i< sizeof(m_buffer); i++)
            m_buffer[i] = b[i];
    }
protected:
    Type m_type = IFormat;
    uint8_t m_buffer[6] = {0};
};



class APCI_I : public APCI {
public:
    APCI_I(const uint8_t *b) {
        m_type = IFormat;
        copyFromBuffer(b);
        m_recvSequenceNumber = ((m_buffer [5] * 0x100) + (m_buffer [4] & 0xfe)) / 2;
        m_sendSequenceNumber = ((m_buffer [3] * 0x100) + (m_buffer [2] & 0xfe)) / 2;
    }
    QString display() override {
        return QString("I(S=%0, R=%1)").arg(m_sendSequenceNumber).arg(m_recvSequenceNumber);
    }

private:
    size_t m_recvSequenceNumber = 0;
    size_t m_sendSequenceNumber = 0;
};

class APCI_S : public APCI {
public:
    QString display() override {
        return QString("S(%1)").arg(m_recvSequenceNumber);
    }
    APCI_S(const uint8_t *b) {
        m_type = SFormat;
        copyFromBuffer(b);
        m_recvSequenceNumber = (m_buffer[4] + m_buffer[5] * 0x100) / 2;
    }
private:
    size_t m_recvSequenceNumber = 0;
};

class APCI_U : public APCI {
public:
    enum U_Type {
        STARTDT_ACT,
        STARTDT_CON,
        STOPDT_ACT,
        STOPDT_CON,
        TESTDT_ACT,
        TESTDT_CON,
        UNKNOW,
    };
    QString display() override {
        return typeToString(u_type);
    }
    static QString typeToString(U_Type type) {
        QString s;
        switch(type)
        {
        case STARTDT_ACT:
            s = "STARTDT_ACT";
            break;
        case STARTDT_CON:
            s = "STARTDT_CON";
            break;
        case STOPDT_ACT:
            s = "STOPDT_ACT";
            break;
        case STOPDT_CON:
            s = "STOPDT_CON";
            break;
        case TESTDT_ACT:
            s = "TEST_ACT";
            break;
        case TESTDT_CON:
            s = "TESTDT_CON";
            break;
        default:
            s = "UNKNOW";
            break;
        }
        return s;
    }
    APCI_U(const uint8_t *b) {
        m_type = UFormat;
        copyFromBuffer(b);
        switch(b[2]) {
        case 0x43:
            u_type = TESTDT_ACT;
            break;
        case 0x83:
            u_type = TESTDT_CON;
            break;
        case 0x13:
            u_type = STOPDT_ACT;
            break;
        case 0x23:
            u_type = STOPDT_CON;
            break;
        case 0x07:
            u_type = STARTDT_ACT;
            break;
        case 0x0B:
            u_type = STARTDT_CON;
            break;
        default:
            u_type = UNKNOW;
            break;
        }
    }

private:
    U_Type u_type;
};

class InformationElement {
public:
    using IOA = size_t; using Value = float;
    InformationElement(IOA ioa, Value value)
        : m_ioa(ioa)
        , m_value(value)
    {
    }
    InformationElement() = default;
    IOA ioa() const { return m_ioa; }
    Value value() const { return m_value; }
    void setIOA(IOA ioa) { m_ioa = ioa; }
    void setValue(Value value) { m_value = value; }
private:
    IOA m_ioa = 0;
    Value m_value = 0;
};

class ASDU {
public:
    using CA = size_t;
    struct VSQ {
        using SQ = bool;
        using NumIx = size_t;
        VSQ(SQ sq, NumIx num) : sequence(sq), number(num) {}
        //VSQ() = default;
        SQ sequence = false;
        NumIx number = 0;
    };
    struct COT {
        using Test = bool; using Negative = bool; using OA = uchar;
        COT(Test t, Negative n, CS101_CauseOfTransmission c, OA o = 0) : test(t), negative(n), cot(c), oa(o) { }
        //COT() = default;
        Test test = false;
        Negative negative = false;
        CS101_CauseOfTransmission cot;
        OA oa = 0;
    };

    ASDU(IEC60870_5_TypeID type, VSQ vsq, COT cot)
        : m_typeId(type)
        , m_vsq(vsq)
        , m_cot(cot)
    {

    }
    ~ASDU() {
        qDeleteAll(m_elements);
    }

    void addInformationElement(InformationElement *ie) {
        m_elements.append(ie);
    }
    InformationElement* informationElement(size_t index) {
        if(index >= 0 && index < m_elements.size())
            return m_elements.at(index);
        else
            return nullptr;
    }

    QList<InformationElement*> elements() const { return m_elements; }
    IEC60870_5_TypeID typeId() const { return m_typeId; }
    VSQ vsq() const { return m_vsq; }
    COT cot() const { return m_cot; }
    CA ca() const { return m_ca; }

    void setCA(const CA& ca) { m_ca = ca; }
private:
    IEC60870_5_TypeID m_typeId;
    VSQ m_vsq;
    COT m_cot;
    CA  m_ca = 0;
    QList<InformationElement*> m_elements;
};


class APDU : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit APDU(APCI *apci, ASDU *asdu = nullptr,
                       const QDateTime &time = QDateTime::currentDateTime(), QObject *parent = nullptr);
    ~APDU() {
        qDebug() << __FUNCTION__ ;
        if(m_apci)
        {
            delete m_apci;
            m_apci = nullptr;
        }
        if(m_asdu)
        {
            delete m_asdu;
            m_asdu = nullptr;
        }
    }
    const QDateTime& time() const { return m_time; }
    APCI* apci() const { return m_apci; }
    ASDU* asdu() const { return m_asdu; }
    bool sent() const { return m_sent; }

    //set
    void setSent(bool sent) { m_sent = sent; }
private:
    QDateTime m_time;
    APCI *m_apci = nullptr;
    ASDU *m_asdu = nullptr;
    bool m_sent = false;
};

#endif // CS101APDU_H
