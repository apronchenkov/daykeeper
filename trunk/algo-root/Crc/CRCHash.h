#ifndef CRCHashH
#define CRCHashH

//******************************************************************************
// Template class 'CRCHash' implements mirror-algorithm of CRC calculation.

template <typename T, const T POLYNOM> struct CRCTable {
	T Data[256];

	CRCTable() {
		for (int i = 0, t = 0; i < 256; t = 8, i++) {
			Data[i] = i;
			while (t--) Data[i] = (Data[i] >> 1) ^ (Data[i] & 1 ? POLYNOM : 0);
		}
	}
};


template <typename T, const T POLYNOM, const T INITIAL, const T FINAL> class CRCHash {
	static const CRCTable<T, POLYNOM> Table;

	T CRC;

public:
	CRCHash() : CRC(INITIAL) { }

	CRCHash(const CRCHash & iCRCHash) : CRC(iCRCHash.CRC) { }

	void Reset() {
		CRC = INITIAL;
	}

	inline void Update(unsigned char Value) {
		CRC = (CRC >> 8) ^ Table.Data[Value ^ CRC & 0xFFU];
	}

	void Update(const void * Buffer, size_t Length) {
		const unsigned char * Block = static_cast<const unsigned char *>(Buffer);
		while (Length--) Update(*Block++);
	}

	T Evaluate() const {
		return CRC ^ FINAL;
	}

	static T Evaluate(const void * Buffer, size_t Length) {
		CRCHash Instance;
		Instance.Update(Buffer, Length);
		return Instance.Evaluate();
	}
};

template <typename T, const T POLYNOM, const T INITIAL, const T FINAL> const CRCTable<T, POLYNOM> CRCHash<T, POLYNOM, INITIAL, FINAL>::Table;

//******************************************************************************

typedef unsigned char  CRC08;
typedef unsigned short CRC16;
typedef unsigned long  CRC32;
typedef unsigned long long CRC64;

class CRC08Hash : public CRCHash<CRC08, 0x8CU, 0xFFU, 0xFFU> {};
class CRC16Hash : public CRCHash<CRC16, 0xA001U, 0x0000U, 0x0000U> {};
class CRC32Hash : public CRCHash<CRC32, 0xEDB88320UL, 0xFFFFFFFFUL, 0xFFFFFFFFUL> {};
class CRC64Hash : public CRCHash<CRC64, 0xD800000000000000ULL, 0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL> {};

//******************************************************************************

#endif
