/**
  * \file Xmlio.h
  * methods for XML tree input/output based on libxml2 (declarations)
  * \author Alexander Wirthmüller
  * \date created: 19 Feb 2007
  * \date modified: 27 May 2018
  */

#ifndef SBECORE_XMLIO_H
#define SBECORE_XMLIO_H

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

#include <libxml/encoding.h>
#include <libxml/xmlreader.h>
#include <libxml/xmlwriter.h>
#include <libxml/xpath.h>
#include <libxml/xpointer.h>

#include <sbecore/Types.h>
#include <sbecore/Strmod.h>

/**
	* Xmlio: set of methods to for XML structure input / output
	*/
namespace Xmlio {
	bool has(set<uint>& items, const uint item);
	bool hasAll(set<uint>& items, const set<uint>& _items);
	bool hasAny(set<uint>& items, const set<uint>& _items);

	void add(set<uint>& items, const uint item);

	string fromUTF8(const string& s);
	string toUTF8(const string& s);

	void fromBase64(const char* inbuf, unsigned int inbuflen, unsigned char** outbuf, unsigned int& outbuflen);
	void toBase64(const unsigned char* inbuf, unsigned int inbuflen, char** outbuf, unsigned int& outbuflen);

	bool bigendian();
	void invertBuffer(unsigned char* buf, const unsigned int len, const unsigned int varlen);

																													//! parse XML file (doc, docctx are returned)
	void parseFile(const string& fullpath, xmlDoc** doc, xmlXPathContext** docctx);

																													//! parse text buffer containing an XML structure (doc, docctx are returned)
	void parseBuffer(char* buffer, int length, xmlDoc** doc, xmlXPathContext** docctx, const bool rmvns = true);

	void closeParsed(xmlDoc* doc, xmlXPathContext* docctx);	//!< free memory of parsed XML structure

																													//! write data members to XML file (opens file, wr is returned)
	void startwriteFile(const string& fullpath, xmlTextWriter** wr);

	void closewriteFile(xmlTextWriter* wr);									//!< finalize writing to XML file

																													//! dump XML structure to text buffer (wr, buf are returned)
	void startwriteBuffer(xmlTextWriter** wr, xmlBuffer** buf);

	void closewriteBuffer(xmlTextWriter* wr);								//!< finalize writing to text buffer

																													//! check if XML data block is present at given path
	bool checkXPath(xmlXPathContext* docctx, const string& xpath);
	bool checkAltXPaths(xmlXPathContext* docctx, string& goodxpath, const string& basexpath, const string& alt1 = "", const string& alt2 = "", const string& alt3 = "", const string& alt4 = "", const string& alt5 = "", const string& alt6 = "");
	bool checkUclcXPaths(xmlXPathContext* docctx, string& goodxpath, const string& basexpath, const string& regc);
	bool checkAttrUclcXpaths(xmlXPathContext* docctx, const string& basexpath, string& goodpath, const string& tag, const string& shorttag, const string& attr, const string& attrval);

	string extractRoot(xmlDoc* doc);

	void extractBool(xmlXPathContext* docctx, const string& xpath, bool& val);
	void extractTinyint(xmlXPathContext* docctx, const string& xpath, tinyint& val);
	void extractUtinyint(xmlXPathContext* docctx, const string& xpath, utinyint& val);
	void extractSmallint(xmlXPathContext* docctx, const string& xpath, smallint& val);
	void extractUsmallint(xmlXPathContext* docctx, const string& xpath, usmallint& val);
	void extractInt(xmlXPathContext* docctx, const string& xpath, int& val);
	void extractUint(xmlXPathContext* docctx, const string& xpath, uint& val);
	void extractBigint(xmlXPathContext* docctx, const string& xpath, bigint& val);
	void extractUbigint(xmlXPathContext* docctx, const string& xpath, ubigint& val);
	void extractFloat(xmlXPathContext* docctx, const string& xpath, float& val);
	void extractDouble(xmlXPathContext* docctx, const string& xpath, double& val);
	void extractString(xmlXPathContext* docctx, const string& xpath, string& val);
	void extractUtinyintvec(xmlXPathContext* docctx, const string& xpath, vector<utinyint>& vec);
	void extractUsmallintvec(xmlXPathContext* docctx, const string& xpath, vector<usmallint>& vec);
	void extractIntvec(xmlXPathContext* docctx, const string& xpath, vector<int>& vec);
	void extractUintvec(xmlXPathContext* docctx, const string& xpath, vector<uint>& vec);
	void extractUbigintvec(xmlXPathContext* docctx, const string& xpath, vector<ubigint>& vec);
	void extractFloatvec(xmlXPathContext* docctx, const string& xpath, vector<float>& vec);
	void extractFloatmat(xmlXPathContext* docctx, const string& xpath, Floatmat& mat);
	void extractDoublevec(xmlXPathContext* docctx, const string& xpath, vector<double>& vec);
	void extractDoublemat(xmlXPathContext* docctx, const string& xpath, Doublemat& mat);
	void extractStringvec(xmlXPathContext* docctx, const string& xpath, vector<string>& vec);

	bool extractBoolUclc(xmlXPathContext* docctx, const string& basexpath, const string& tag, const string& shorttag, bool& val);
	bool extractTinyintUclc(xmlXPathContext* docctx, const string& basexpath, const string& tag, const string& shorttag, tinyint& val);
	bool extractUtinyintUclc(xmlXPathContext* docctx, const string& basexpath, const string& tag, const string& shorttag, utinyint& val);
	bool extractSmallintUclc(xmlXPathContext* docctx, const string& basexpath, const string& tag, const string& shorttag, smallint& val);
	bool extractUsmallintUclc(xmlXPathContext* docctx, const string& basexpath, const string& tag, const string& shorttag, usmallint& val);
	bool extractIntUclc(xmlXPathContext* docctx, const string& basexpath, const string& tag, const string& shorttag, int& val);
	bool extractUintUclc(xmlXPathContext* docctx, const string& basexpath, const string& tag, const string& shorttag, uint& val);
	bool extractBigintUclc(xmlXPathContext* docctx, const string& basexpath, const string& tag, const string& shorttag, bigint& val);
	bool extractUbigintUclc(xmlXPathContext* docctx, const string& basexpath, const string& tag, const string& shorttag, ubigint& val);
	bool extractFloatUclc(xmlXPathContext* docctx, const string& basexpath, const string& tag, const string& shorttag, float& val);
	bool extractDoubleUclc(xmlXPathContext* docctx, const string& basexpath, const string& tag, const string& shorttag, double& val);
	bool extractStringUclc(xmlXPathContext* docctx, const string& basexpath, const string& tag, const string& shorttag, string& val);
	bool extractUtinyintvecUclc(xmlXPathContext* docctx, const string& basexpath, const string& tag, const string& shorttag, vector<utinyint>& vec);
	bool extractUsmallintvecUclc(xmlXPathContext* docctx, const string& basexpath, const string& tag, const string& shorttag, vector<usmallint>& vec);
	bool extractIntvecUclc(xmlXPathContext* docctx, const string& basexpath, const string& tag, const string& shorttag, vector<int>& vec);
	bool extractUintvecUclc(xmlXPathContext* docctx, const string& basexpath, const string& tag, const string& shorttag, vector<uint>& vec);
	bool extractUbigintvecUclc(xmlXPathContext* docctx, const string& basexpath, const string& tag, const string& shorttag, vector<ubigint>& vec);
	bool extractFloatvecUclc(xmlXPathContext* docctx, const string& basexpath, const string& tag, const string& shorttag, vector<float>& vec);
	bool extractFloatmatUclc(xmlXPathContext* docctx, const string& basexpath, const string& tag, const string& shorttag, Floatmat& mat);
	bool extractDoublevecUclc(xmlXPathContext* docctx, const string& basexpath, const string& tag, const string& shorttag, vector<double>& vec);
	bool extractDoublematUclc(xmlXPathContext* docctx, const string& basexpath, const string& tag, const string& shorttag, Doublemat& mat);
	bool extractStringvecUclc(xmlXPathContext* docctx, const string& basexpath, const string& tag, const string& shorttag, vector<string>& vec);

	bool extractBoolAttrUclc(xmlXPathContext* docctx, const string& basexpath, const string& tag, const string& shorttag, const string& attr, const string& attrval, bool& val);
	bool extractTinyintAttrUclc(xmlXPathContext* docctx, const string& basexpath, const string& tag, const string& shorttag, const string& attr, const string& attrval, tinyint& val);
	bool extractUtinyintAttrUclc(xmlXPathContext* docctx, const string& basexpath, const string& tag, const string& shorttag, const string& attr, const string& attrval, utinyint& val);
	bool extractSmallintAttrUclc(xmlXPathContext* docctx, const string& basexpath, const string& tag, const string& shorttag, const string& attr, const string& attrval, smallint& val);
	bool extractUsmallintAttrUclc(xmlXPathContext* docctx, const string& basexpath, const string& tag, const string& shorttag, const string& attr, const string& attrval, usmallint& val);
	bool extractIntAttrUclc(xmlXPathContext* docctx, const string& basexpath, const string& tag, const string& shorttag, const string& attr, const string& attrval, int& val);
	bool extractUintAttrUclc(xmlXPathContext* docctx, const string& basexpath, const string& tag, const string& shorttag, const string& attr, const string& attrval, uint& val);
	bool extractBigintAttrUclc(xmlXPathContext* docctx, const string& basexpath, const string& tag, const string& shorttag, const string& attr, const string& attrval, bigint& val);
	bool extractUbigintAttrUclc(xmlXPathContext* docctx, const string& basexpath, const string& tag, const string& shorttag, const string& attr, const string& attrval, ubigint& val);
	bool extractFloatAttrUclc(xmlXPathContext* docctx, const string& basexpath, const string& tag, const string& shorttag, const string& attr, const string& attrval, float& val);
	bool extractDoubleAttrUclc(xmlXPathContext* docctx, const string& basexpath, const string& tag, const string& shorttag, const string& attr, const string& attrval, double& val);
	bool extractStringAttrUclc(xmlXPathContext* docctx, const string& basexpath, const string& tag, const string& shorttag, const string& attr, const string& attrval, string& val);
	bool extractUtinyintvecAttrUclc(xmlXPathContext* docctx, const string& basexpath, const string& tag, const string& shorttag, const string& attr, const string& attrval, vector<utinyint>& vec);
	bool extractUsmallintvecAttrUclc(xmlXPathContext* docctx, const string& basexpath, const string& tag, const string& shorttag, const string& attr, const string& attrval, vector<usmallint>& vec);
	bool extractIntvecAttrUclc(xmlXPathContext* docctx, const string& basexpath, const string& tag, const string& shorttag, const string& attr, const string& attrval, vector<int>& vec);
	bool extractUintvecAttrUclc(xmlXPathContext* docctx, const string& basexpath, const string& tag, const string& shorttag, const string& attr, const string& attrval, vector<uint>& vec);
	bool extractUbigintvecAttrUclc(xmlXPathContext* docctx, const string& basexpath, const string& tag, const string& shorttag, const string& attr, const string& attrval, vector<ubigint>& vec);
	bool extractFloatvecAttrUclc(xmlXPathContext* docctx, const string& basexpath, const string& tag, const string& shorttag, const string& attr, const string& attrval, vector<float>& vec);
	bool extractFloatmatAttrUclc(xmlXPathContext* docctx, const string& basexpath, const string& tag, const string& shorttag, const string& attr, const string& attrval, Floatmat& mat);
	bool extractDoublevecAttrUclc(xmlXPathContext* docctx, const string& basexpath, const string& tag, const string& shorttag, const string& attr, const string& attrval, vector<double>& vec);
	bool extractDoublematAttrUclc(xmlXPathContext* docctx, const string& basexpath, const string& tag, const string& shorttag, const string& attr, const string& attrval, Doublemat& mat);
	bool extractStringvecAttrUclc(xmlXPathContext* docctx, const string& basexpath, const string& tag, const string& shorttag, const string& attr, const string& attrval, vector<string>& vec);

	void extractList(xmlXPathContext* docctx, const string& basexpath, const string& tag, const string& shorttag, const string& attr, vector<unsigned int>& ics, vector<bool>& shorttags);

	void writeBase64(xmlTextWriter* wr, const char* buf, const unsigned int len, const unsigned int varlen);

	void writeBool(xmlTextWriter* wr, const string& tag, bool val);
	void writeTinyint(xmlTextWriter* wr, const string& tag, tinyint val);
	void writeUtinyint(xmlTextWriter* wr, const string& tag, utinyint val);
	void writeSmallint(xmlTextWriter* wr, const string& tag, smallint val);
	void writeUsmallint(xmlTextWriter* wr, const string& tag, usmallint val);
	void writeInt(xmlTextWriter* wr, const string& tag, int val);
	void writeUint(xmlTextWriter* wr, const string& tag, uint val);
	void writeBigint(xmlTextWriter* wr, const string& tag, bigint val);
	void writeUbigint(xmlTextWriter* wr, const string& tag, ubigint val);
	void writeFloat(xmlTextWriter* wr, const string& tag, float val);
	void writeDouble(xmlTextWriter* wr, const string& tag, double val);
	void writeString(xmlTextWriter* wr, const string& tag, const string& val);
	void writeUtinyintvec(xmlTextWriter* wr, const string& tag, const vector<utinyint>& vec);
	void writeUsmallintvec(xmlTextWriter* wr, const string& tag, const vector<usmallint>& vec);
	void writeIntvec(xmlTextWriter* wr, const string& tag, const vector<int>& vec);
	void writeUintvec(xmlTextWriter* wr, const string& tag, const vector<uint>& vec);
	void writeUbigintvec(xmlTextWriter* wr, const string& tag, const vector<ubigint>& vec);
	void writeFloatvec(xmlTextWriter* wr, const string& tag, const vector<float>& vec);
	void writeFloatmat(xmlTextWriter* wr, const string& tag, const Floatmat& mat);
	void writeDoublevec(xmlTextWriter* wr, const string& tag, const vector<double>& vec);
	void writeDoublemat(xmlTextWriter* wr, const string& tag, const Doublemat& mat);
	void writeStringvec(xmlTextWriter* wr, const string& tag, const vector<string>& vec);

	void writeBoolAttr(xmlTextWriter* wr, const string& tag, const string& attr, const string& attrval, bool val);
	void writeTinyintAttr(xmlTextWriter* wr, const string& tag, const string& attr, const string& attrval, tinyint val);
	void writeUtinyintAttr(xmlTextWriter* wr, const string& tag, const string& attr, const string& attrval, utinyint val);
	void writeSmallintAttr(xmlTextWriter* wr, const string& tag, const string& attr, const string& attrval, smallint val);
	void writeUsmallintAttr(xmlTextWriter* wr, const string& tag, const string& attr, const string& attrval, usmallint val);
	void writeIntAttr(xmlTextWriter* wr, const string& tag, const string& attr, const string& attrval, int val);
	void writeUintAttr(xmlTextWriter* wr, const string& tag, const string& attr, const string& attrval, uint val);
	void writeBigintAttr(xmlTextWriter* wr, const string& tag, const string& attr, const string& attrval, bigint val);
	void writeUbigintAttr(xmlTextWriter* wr, const string& tag, const string& attr, const string& attrval, ubigint val);
	void writeFloatAttr(xmlTextWriter* wr, const string& tag, const string& attr, const string& attrval, float val);
	void writeDoubleAttr(xmlTextWriter* wr, const string& tag, const string& attr, const string& attrval, double val);
	void writeStringAttr(xmlTextWriter* wr, const string& tag, const string& attr, const string& attrval, const string& val);
	void writeUtinyintvecAttr(xmlTextWriter* wr, const string& tag, const string& attr, const string& attrval, const vector<utinyint>& vec);
	void writeUsmallintvecAttr(xmlTextWriter* wr, const string& tag, const string& attr, const string& attrval, const vector<usmallint>& vec);
	void writeIntvecAttr(xmlTextWriter* wr, const string& tag, const string& attr, const string& attrval, const vector<int>& vec);
	void writeUintvecAttr(xmlTextWriter* wr, const string& tag, const string& attr, const string& attrval, const vector<uint>& vec);
	void writeUbigintvecAttr(xmlTextWriter* wr, const string& tag, const string& attr, const string& attrval, const vector<ubigint>& vec);
	void writeFloatvecAttr(xmlTextWriter* wr, const string& tag, const string& attr, const string& attrval, const vector<float>& vec);
	void writeFloatmatAttr(xmlTextWriter* wr, const string& tag, const string& attr, const string& attrval, const Floatmat& mat);
	void writeDoublevecAttr(xmlTextWriter* wr, const string& tag, const string& attr, const string& attrval, const vector<double>& vec);
	void writeDoublematAttr(xmlTextWriter* wr, const string& tag, const string& attr, const string& attrval, const Doublemat& mat);
	void writeStringvecAttr(xmlTextWriter* wr, const string& tag, const string& attr, const string& attrval, const vector<string>& vec);

	float compareFloat(const float a, const float b);
	double compareDouble(const double a, const double b);
	bool compareUtinyintvec(const vector<utinyint>& a, const vector<utinyint>& b);
	bool compareUsmallintvec(const vector<usmallint>& a, const vector<usmallint>& b);
	bool compareIntvec(const vector<int>& a, const vector<int>& b);
	bool compareUintvec(const vector<uint>& a, const vector<uint>& b);
	float compareFloatvec(const vector<float>& a, const vector<float>& b);
	float compareFloatmat(const Floatmat& a, const Floatmat& b);
	double compareDoublevec(const vector<double>& a, const vector<double>& b);
	double compareDoublemat(const Doublemat& a, const Doublemat& b);
	bool compareStringvec(const vector<string>& a, const vector<string>& b);

	bool find(const set<uint>& mask, const uint item);

	void insert(set<uint>& items, const uint item);
	void insert(set<uint>& items, const set<uint>& _items);

	void push_back(vector<uint>& ics, const uint ix);
	void push_back(vector<uint>& ics, const vector<uint>& _ics);

	/**
		* Block
		*/
	class Block {

	public:
		static const uint NONE = 0;

	public:
		Block();
		virtual ~Block();
	
	public:
		set<uint> mask;

	public:
		bool has(const uint item);
		bool hasAll(const set<uint>& items);
		bool hasAny(const set<uint>& items);

		void add(const uint item);
		void clear();
	};

	/**
		* Feeditem
		*/
	class Feeditem {

	public:
		Feeditem(const bool Avail = true, const uint ix = 0, const ubigint ref = 0, const string& sref = "", const string& Title1 = "", const string& Title2 = "", const string& Title3 = "");

	public:
		bool Avail;
		uint ix;
		ubigint ref;
		string sref;
		string Title1;
		string Title2;
		string Title3;

	public:
		bool operator==(const Feeditem& comp);
		bool operator!=(const Feeditem& comp);

		void cap(const bool tit1 = true, const bool tit2 = false, const bool tit3 = false);

		bool readXML(xmlXPathContext* docctx, string basexpath = "", bool addbasetag = false);
		void writeXML(xmlTextWriter* wr, unsigned int num, string difftag = "");
	};

	/**
		* Feed
		*/
	class Feed {

	public:
		Feed(const string& tag = "");
		Feed(const Feed& src);
		~Feed();

	public:
		string tag;
		vector<Feeditem*> nodes;

	public:
		void clear();
		unsigned int size() const;

		void appendIxSrefTitles(const uint ix, const string& sref, const string& Title1 = "", const string& Title2 = "", const string& Title3 = "");
		void appendIxRefSrefTitles(const uint ix, const ubigint ref, const string& sref, const string& Title1 = "", const string& Title2 = "", const string& Title3 = "");
		void appendRefTitles(const ubigint ref, const string& Title1 = "", const string& Title2 = "", const string& Title3 = "");
		void appendRefSrefTitles(const ubigint ref, const string& sref, const string& Title1 = "", const string& Title2 = "", const string& Title3 = "");
		void appendTitles(const string& Title1, const string& Title2 = "", const string& Title3 = "");

		Feeditem* operator[](const uint ix);
		Feeditem* getByNum(const uint num);
		uint getNumByIx(const uint ix);
		uint getNumByRef(const ubigint ref);
		uint getNumBySref(const string& sref);
		uint getIxByNum(const uint num);
		ubigint getRefByNum(const uint num);
		string getSrefByNum(const uint num);

		Feed& operator=(const Feed& src);
		bool operator==(const Feed& comp);
		bool operator!=(const Feed& comp);

		void cap(const bool tit1 = true, const bool tit2 = false, const bool tit3 = false);

		bool readXML(xmlXPathContext* docctx, string basexpath = "", bool addbasetag = false);
		void writeXML(xmlTextWriter* wr, string difftag = "");
	};
};
#endif

