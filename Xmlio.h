/**
	* \file Xmlio.h
	* methods for XML tree input/output based on libxml2 (declarations)
	* \copyright (C) 2016-2020 MPSI Technologies GmbH
	* \author Alexander Wirthmüller
	* \date created: 19 Feb 2007
	*/

#ifndef SBECORE_XMLIO_H
#define SBECORE_XMLIO_H

#include <sys/stat.h>
#include <fcntl.h>
#ifdef _WIN32
	#include <io.h>
#else
	#include <unistd.h>
#endif
#include <string.h>

#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>

#include <json/json.h>

#include <libxml/encoding.h>
#include <libxml/xmlreader.h>
#include <libxml/xmlwriter.h>
#include <libxml/xpath.h>
#include <libxml/xpointer.h>

#include <sbecore/Types.h>

namespace Sbecore {
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
		std::set<uint> mask;

	public:
		bool has(const uint item);
		bool hasAll(const std::set<uint>& items);
		bool hasAny(const std::set<uint>& items);

		void add(const uint item);
		void clear();
	};

	/**
		* Feeditem
		*/
	class Feeditem {

	public:
		Feeditem(const bool Avail = true, const uint ix = 0, const ubigint ref = 0, const std::string& sref = "", const std::string& Title1 = "", const std::string& Title2 = "", const std::string& Title3 = "");

	public:
		bool Avail;
		uint ix;
		ubigint ref;
		std::string sref;
		std::string Title1;
		std::string Title2;
		std::string Title3;

	public:
		bool operator==(const Feeditem& comp);
		bool operator!=(const Feeditem& comp);

		void cap(const bool tit1 = true, const bool tit2 = false, const bool tit3 = false);

		bool readJSON(const Json::Value& me, const unsigned int ix, const bool shorttags);
		bool readXML(xmlXPathContext* docctx, std::string basexpath, const unsigned int num, const bool shorttags);
		void writeJSON(Json::Value& sup, const unsigned int num);
		void writeXML(xmlTextWriter* wr, const unsigned int num, std::string difftag = "");
	};

	/**
		* Feed
		*/
	class Feed {

	public:
		Feed(const std::string& tag = "");
		Feed(const Feed& src);
		~Feed();

	public:
		std::string tag;
		std::vector<Feeditem*> nodes;

	public:
		void clear();
		unsigned int size() const;

		void appendIxSrefTitles(const uint ix, const std::string& sref, const std::string& Title1 = "", const std::string& Title2 = "", const std::string& Title3 = "");
		void appendIxRefSrefTitles(const uint ix, const ubigint ref, const std::string& sref, const std::string& Title1 = "", const std::string& Title2 = "", const std::string& Title3 = "");
		void appendRefTitles(const ubigint ref, const std::string& Title1 = "", const std::string& Title2 = "", const std::string& Title3 = "");
		void appendRefSrefTitles(const ubigint ref, const std::string& sref, const std::string& Title1 = "", const std::string& Title2 = "", const std::string& Title3 = "");
		void appendTitles(const std::string& Title1, const std::string& Title2 = "", const std::string& Title3 = "");

		Feeditem* operator[](const uint ix);
		Feeditem* getByNum(const uint num);
		bool getAvailByNum(const uint num);
		bool getAvailByNums(const std::vector<Sbecore::uint>& nums);
		Sbecore::uint getNumByIx(const uint ix);
		Sbecore::uint getNumByRef(const ubigint ref);
		Sbecore::uint getNumBySref(const std::string& sref);
		Sbecore::uint getIxByNum(const uint num);
		Sbecore::ubigint getRefByNum(const uint num);
		std::string getSrefByNum(const uint num);

		Feed& operator=(const Feed& src);
		bool operator==(const Feed& comp);
		bool operator!=(const Feed& comp);

		void cap(const bool tit1 = true, const bool tit2 = false, const bool tit3 = false);

		bool readJSON(const Json::Value& sup, bool addbasetag = false);
		bool readXML(xmlXPathContext* docctx, std::string basexpath = "", bool addbasetag = false);
		void writeJSON(Json::Value& sup, std::string difftag = "");
		void writeXML(xmlTextWriter* wr, std::string difftag = "");
	};

	/**
		* Jsonio
		*/
	namespace Jsonio {
		bool extractBoolvec(const Json::Value& sup, const std::string& tag, std::vector<bool>& vec);
		bool extractUtinyintvec(const Json::Value& sup, const std::string& tag, std::vector<utinyint>& vec);
		bool extractUsmallintvec(const Json::Value& sup, const std::string& tag, std::vector<usmallint>& vec);
		bool extractIntvec(const Json::Value& sup, const std::string& tag, std::vector<int>& vec);
		bool extractUintvec(const Json::Value& sup, const std::string& tag, std::vector<uint>& vec);
		bool extractUbigintvec(const Json::Value& sup, const std::string& tag, std::vector<ubigint>& vec);
		bool extractFloatvec(const Json::Value& sup, const std::string& tag, std::vector<float>& vec);
		bool extractFloatmat(const Json::Value& sup, const std::string& tag, Floatmat& mat);
		bool extractDoublevec(const Json::Value& sup, const std::string& tag, std::vector<double>& vec);
		bool extractDoublemat(const Json::Value& sup, const std::string& tag, Doublemat& mat);
		bool extractStringvec(const Json::Value& sup, const std::string& tag, std::vector<std::string>& vec);

		void writeBoolvec(Json::Value& sup, const std::string& tag, const std::vector<bool>& vec);
		void writeUtinyintvec(Json::Value& sup, const std::string& tag, const std::vector<utinyint>& vec);
		void writeUsmallintvec(Json::Value& sup, const std::string& tag, const std::vector<usmallint>& vec);
		void writeIntvec(Json::Value& sup, const std::string& tag, const std::vector<int>& vec);
		void writeUintvec(Json::Value& sup, const std::string& tag, const std::vector<uint>& vec);
		void writeUbigintvec(Json::Value& sup, const std::string& tag, const std::vector<ubigint>& vec);
		void writeFloatvec(Json::Value& sup, const std::string& tag, const std::vector<float>& vec);
		void writeFloatmat(Json::Value& sup, const std::string& tag, const Floatmat& mat);
		void writeDoublevec(Json::Value& sup, const std::string& tag, const std::vector<double>& vec);
		void writeDoublemat(Json::Value& sup, const std::string& tag, const Doublemat& mat);
		void writeStringvec(Json::Value& sup, const std::string& tag, const std::vector<std::string>& vec);
	};

	/**
		* Xmlio: set of methods for XML structure input / output
		*/
	namespace Xmlio {
		bool has(std::set<uint>& items, const uint item);
		bool hasAll(std::set<uint>& items, const std::set<uint>& _items);
		bool hasAny(std::set<uint>& items, const std::set<uint>& _items);

		void add(std::set<uint>& items, const uint item);

		std::string fromUTF8(const std::string& s);
		std::string toUTF8(const std::string& s);

		void fromBase64(const char* inbuf, unsigned int inbuflen, unsigned char** outbuf, unsigned int& outbuflen);
		void toBase64(const unsigned char* inbuf, unsigned int inbuflen, char** outbuf, unsigned int& outbuflen);

		bool bigendian();
		void invertBuffer(unsigned char* buf, const unsigned int len, const unsigned int varlen);

		void base64ToUtinyintvec(const char* buf, const unsigned int buflen, std::vector<Sbecore::utinyint>& vec);
		template<class T> void base64ToVec(const char* buf, const unsigned int buflen, std::vector<T>& vec);

		std::string dataToBase64(const unsigned char* _buf, const unsigned int len, const unsigned int varlen);

																														//! parse XML file (doc, docctx are returned)
		void parseFile(const std::string& fullpath, xmlDoc** doc, xmlXPathContext** docctx);

																														//! parse text buffer containing an XML structure (doc, docctx are returned)
		void parseBuffer(char* buffer, size_t length, xmlDoc** doc, xmlXPathContext** docctx, const bool rmvns = true);

		void closeParsed(xmlDoc* doc, xmlXPathContext* docctx);	//!< free memory of parsed XML structure

																														//! write data members to XML file (opens file, wr is returned)
		void startwriteFile(const std::string& fullpath, xmlTextWriter** wr);

		void closewriteFile(xmlTextWriter* wr);									//!< finalize writing to XML file

																														//! dump XML structure to text buffer (wr, buf are returned)
		void startwriteBuffer(xmlTextWriter** wr, xmlBuffer** buf);

		void closewriteBuffer(xmlTextWriter* wr);								//!< finalize writing to text buffer

																														//! check if XML data block is present at given path
		bool checkXPath(xmlXPathContext* docctx, const std::string& xpath);
		bool checkXPath(xmlXPathContext* docctx, const std::string& xpath, unsigned int& lineno);
		bool checkAltXPaths(xmlXPathContext* docctx, std::string& goodxpath, const std::string& basexpath, const std::string& alt1 = "", const std::string& alt2 = "", const std::string& alt3 = "", const std::string& alt4 = "", const std::string& alt5 = "", const std::string& alt6 = "");
		bool checkUclcXPaths(xmlXPathContext* docctx, std::string& goodxpath, const std::string& basexpath, const std::string& regc);
		bool checkAttrUclcXpaths(xmlXPathContext* docctx, const std::string& basexpath, std::string& goodpath, const std::string& tag, const std::string& shorttag, const std::string& attr, const std::string& attrval);

		std::string extractRoot(xmlDoc* doc);

		void extractBool(xmlXPathContext* docctx, const std::string& xpath, bool& val);
		void extractTinyint(xmlXPathContext* docctx, const std::string& xpath, tinyint& val);
		void extractUtinyint(xmlXPathContext* docctx, const std::string& xpath, utinyint& val);
		void extractSmallint(xmlXPathContext* docctx, const std::string& xpath, smallint& val);
		void extractUsmallint(xmlXPathContext* docctx, const std::string& xpath, usmallint& val);
		void extractInt(xmlXPathContext* docctx, const std::string& xpath, int& val);
		void extractUint(xmlXPathContext* docctx, const std::string& xpath, uint& val);
		void extractBigint(xmlXPathContext* docctx, const std::string& xpath, bigint& val);
		void extractUbigint(xmlXPathContext* docctx, const std::string& xpath, ubigint& val);
		void extractFloat(xmlXPathContext* docctx, const std::string& xpath, float& val);
		void extractDouble(xmlXPathContext* docctx, const std::string& xpath, double& val);
		void extractString(xmlXPathContext* docctx, const std::string& xpath, std::string& val);
		void extractBoolvec(xmlXPathContext* docctx, const std::string& xpath, std::vector<bool>& vec);
		void extractUtinyintvec(xmlXPathContext* docctx, const std::string& xpath, std::vector<utinyint>& vec);
		void extractUsmallintvec(xmlXPathContext* docctx, const std::string& xpath, std::vector<usmallint>& vec);
		void extractIntvec(xmlXPathContext* docctx, const std::string& xpath, std::vector<int>& vec);
		void extractUintvec(xmlXPathContext* docctx, const std::string& xpath, std::vector<uint>& vec);
		void extractUbigintvec(xmlXPathContext* docctx, const std::string& xpath, std::vector<ubigint>& vec);
		void extractFloatvec(xmlXPathContext* docctx, const std::string& xpath, std::vector<float>& vec);
		void extractFloatmat(xmlXPathContext* docctx, const std::string& xpath, Floatmat& mat);
		void extractDoublevec(xmlXPathContext* docctx, const std::string& xpath, std::vector<double>& vec);
		void extractDoublemat(xmlXPathContext* docctx, const std::string& xpath, Doublemat& mat);
		void extractStringvec(xmlXPathContext* docctx, const std::string& xpath, std::vector<std::string>& vec);

		bool extractBoolUclc(xmlXPathContext* docctx, const std::string& basexpath, const std::string& tag, const std::string& shorttag, bool& val);
		bool extractTinyintUclc(xmlXPathContext* docctx, const std::string& basexpath, const std::string& tag, const std::string& shorttag, tinyint& val);
		bool extractUtinyintUclc(xmlXPathContext* docctx, const std::string& basexpath, const std::string& tag, const std::string& shorttag, utinyint& val);
		bool extractSmallintUclc(xmlXPathContext* docctx, const std::string& basexpath, const std::string& tag, const std::string& shorttag, smallint& val);
		bool extractUsmallintUclc(xmlXPathContext* docctx, const std::string& basexpath, const std::string& tag, const std::string& shorttag, usmallint& val);
		bool extractIntUclc(xmlXPathContext* docctx, const std::string& basexpath, const std::string& tag, const std::string& shorttag, int& val);
		bool extractUintUclc(xmlXPathContext* docctx, const std::string& basexpath, const std::string& tag, const std::string& shorttag, uint& val);
		bool extractBigintUclc(xmlXPathContext* docctx, const std::string& basexpath, const std::string& tag, const std::string& shorttag, bigint& val);
		bool extractUbigintUclc(xmlXPathContext* docctx, const std::string& basexpath, const std::string& tag, const std::string& shorttag, ubigint& val);
		bool extractFloatUclc(xmlXPathContext* docctx, const std::string& basexpath, const std::string& tag, const std::string& shorttag, float& val);
		bool extractDoubleUclc(xmlXPathContext* docctx, const std::string& basexpath, const std::string& tag, const std::string& shorttag, double& val);
		bool extractStringUclc(xmlXPathContext* docctx, const std::string& basexpath, const std::string& tag, const std::string& shorttag, std::string& val);
		bool extractBoolvecUclc(xmlXPathContext* docctx, const std::string& basexpath, const std::string& tag, const std::string& shorttag, std::vector<bool>& vec);
		bool extractUtinyintvecUclc(xmlXPathContext* docctx, const std::string& basexpath, const std::string& tag, const std::string& shorttag, std::vector<utinyint>& vec);
		bool extractUsmallintvecUclc(xmlXPathContext* docctx, const std::string& basexpath, const std::string& tag, const std::string& shorttag, std::vector<usmallint>& vec);
		bool extractIntvecUclc(xmlXPathContext* docctx, const std::string& basexpath, const std::string& tag, const std::string& shorttag, std::vector<int>& vec);
		bool extractUintvecUclc(xmlXPathContext* docctx, const std::string& basexpath, const std::string& tag, const std::string& shorttag, std::vector<uint>& vec);
		bool extractUbigintvecUclc(xmlXPathContext* docctx, const std::string& basexpath, const std::string& tag, const std::string& shorttag, std::vector<ubigint>& vec);
		bool extractFloatvecUclc(xmlXPathContext* docctx, const std::string& basexpath, const std::string& tag, const std::string& shorttag, std::vector<float>& vec);
		bool extractFloatmatUclc(xmlXPathContext* docctx, const std::string& basexpath, const std::string& tag, const std::string& shorttag, Floatmat& mat);
		bool extractDoublevecUclc(xmlXPathContext* docctx, const std::string& basexpath, const std::string& tag, const std::string& shorttag, std::vector<double>& vec);
		bool extractDoublematUclc(xmlXPathContext* docctx, const std::string& basexpath, const std::string& tag, const std::string& shorttag, Doublemat& mat);
		bool extractStringvecUclc(xmlXPathContext* docctx, const std::string& basexpath, const std::string& tag, const std::string& shorttag, std::vector<std::string>& vec);

		bool extractBoolAttrUclc(xmlXPathContext* docctx, const std::string& basexpath, const std::string& tag, const std::string& shorttag, const std::string& attr, const std::string& attrval, bool& val);
		bool extractTinyintAttrUclc(xmlXPathContext* docctx, const std::string& basexpath, const std::string& tag, const std::string& shorttag, const std::string& attr, const std::string& attrval, tinyint& val);
		bool extractUtinyintAttrUclc(xmlXPathContext* docctx, const std::string& basexpath, const std::string& tag, const std::string& shorttag, const std::string& attr, const std::string& attrval, utinyint& val);
		bool extractSmallintAttrUclc(xmlXPathContext* docctx, const std::string& basexpath, const std::string& tag, const std::string& shorttag, const std::string& attr, const std::string& attrval, smallint& val);
		bool extractUsmallintAttrUclc(xmlXPathContext* docctx, const std::string& basexpath, const std::string& tag, const std::string& shorttag, const std::string& attr, const std::string& attrval, usmallint& val);
		bool extractIntAttrUclc(xmlXPathContext* docctx, const std::string& basexpath, const std::string& tag, const std::string& shorttag, const std::string& attr, const std::string& attrval, int& val);
		bool extractUintAttrUclc(xmlXPathContext* docctx, const std::string& basexpath, const std::string& tag, const std::string& shorttag, const std::string& attr, const std::string& attrval, uint& val);
		bool extractBigintAttrUclc(xmlXPathContext* docctx, const std::string& basexpath, const std::string& tag, const std::string& shorttag, const std::string& attr, const std::string& attrval, bigint& val);
		bool extractUbigintAttrUclc(xmlXPathContext* docctx, const std::string& basexpath, const std::string& tag, const std::string& shorttag, const std::string& attr, const std::string& attrval, ubigint& val);
		bool extractFloatAttrUclc(xmlXPathContext* docctx, const std::string& basexpath, const std::string& tag, const std::string& shorttag, const std::string& attr, const std::string& attrval, float& val);
		bool extractDoubleAttrUclc(xmlXPathContext* docctx, const std::string& basexpath, const std::string& tag, const std::string& shorttag, const std::string& attr, const std::string& attrval, double& val);
		bool extractStringAttrUclc(xmlXPathContext* docctx, const std::string& basexpath, const std::string& tag, const std::string& shorttag, const std::string& attr, const std::string& attrval, std::string& val);
		bool extractBoolvecAttrUclc(xmlXPathContext* docctx, const std::string& basexpath, const std::string& tag, const std::string& shorttag, const std::string& attr, const std::string& attrval, std::vector<bool>& vec);
		bool extractUtinyintvecAttrUclc(xmlXPathContext* docctx, const std::string& basexpath, const std::string& tag, const std::string& shorttag, const std::string& attr, const std::string& attrval, std::vector<utinyint>& vec);
		bool extractUsmallintvecAttrUclc(xmlXPathContext* docctx, const std::string& basexpath, const std::string& tag, const std::string& shorttag, const std::string& attr, const std::string& attrval, std::vector<usmallint>& vec);
		bool extractIntvecAttrUclc(xmlXPathContext* docctx, const std::string& basexpath, const std::string& tag, const std::string& shorttag, const std::string& attr, const std::string& attrval, std::vector<int>& vec);
		bool extractUintvecAttrUclc(xmlXPathContext* docctx, const std::string& basexpath, const std::string& tag, const std::string& shorttag, const std::string& attr, const std::string& attrval, std::vector<uint>& vec);
		bool extractUbigintvecAttrUclc(xmlXPathContext* docctx, const std::string& basexpath, const std::string& tag, const std::string& shorttag, const std::string& attr, const std::string& attrval, std::vector<ubigint>& vec);
		bool extractFloatvecAttrUclc(xmlXPathContext* docctx, const std::string& basexpath, const std::string& tag, const std::string& shorttag, const std::string& attr, const std::string& attrval, std::vector<float>& vec);
		bool extractFloatmatAttrUclc(xmlXPathContext* docctx, const std::string& basexpath, const std::string& tag, const std::string& shorttag, const std::string& attr, const std::string& attrval, Floatmat& mat);
		bool extractDoublevecAttrUclc(xmlXPathContext* docctx, const std::string& basexpath, const std::string& tag, const std::string& shorttag, const std::string& attr, const std::string& attrval, std::vector<double>& vec);
		bool extractDoublematAttrUclc(xmlXPathContext* docctx, const std::string& basexpath, const std::string& tag, const std::string& shorttag, const std::string& attr, const std::string& attrval, Doublemat& mat);
		bool extractStringvecAttrUclc(xmlXPathContext* docctx, const std::string& basexpath, const std::string& tag, const std::string& shorttag, const std::string& attr, const std::string& attrval, std::vector<std::string>& vec);

		void extractList(xmlXPathContext* docctx, const std::string& basexpath, const std::string& tag, const std::string& shorttag, const std::string& attr, std::vector<unsigned int>& ics, std::vector<bool>& shorttags);

		void writeBase64(xmlTextWriter* wr, const char* buf, const unsigned int len, const unsigned int varlen);

		void writeBool(xmlTextWriter* wr, const std::string& tag, bool val);
		void writeTinyint(xmlTextWriter* wr, const std::string& tag, tinyint val);
		void writeUtinyint(xmlTextWriter* wr, const std::string& tag, utinyint val);
		void writeSmallint(xmlTextWriter* wr, const std::string& tag, smallint val);
		void writeUsmallint(xmlTextWriter* wr, const std::string& tag, usmallint val);
		void writeInt(xmlTextWriter* wr, const std::string& tag, int val);
		void writeUint(xmlTextWriter* wr, const std::string& tag, uint val);
		void writeBigint(xmlTextWriter* wr, const std::string& tag, bigint val);
		void writeUbigint(xmlTextWriter* wr, const std::string& tag, ubigint val);
		void writeFloat(xmlTextWriter* wr, const std::string& tag, float val);
		void writeDouble(xmlTextWriter* wr, const std::string& tag, double val);
		void writeString(xmlTextWriter* wr, const std::string& tag, const std::string& val);
		void writeBoolvec(xmlTextWriter* wr, const std::string& tag, const std::vector<bool>& vec);
		void writeUtinyintvec(xmlTextWriter* wr, const std::string& tag, const std::vector<utinyint>& vec);
		void writeUsmallintvec(xmlTextWriter* wr, const std::string& tag, const std::vector<usmallint>& vec);
		void writeIntvec(xmlTextWriter* wr, const std::string& tag, const std::vector<int>& vec);
		void writeUintvec(xmlTextWriter* wr, const std::string& tag, const std::vector<uint>& vec);
		void writeUbigintvec(xmlTextWriter* wr, const std::string& tag, const std::vector<ubigint>& vec);
		void writeFloatvec(xmlTextWriter* wr, const std::string& tag, const std::vector<float>& vec);
		void writeFloatmat(xmlTextWriter* wr, const std::string& tag, const Floatmat& mat);
		void writeDoublevec(xmlTextWriter* wr, const std::string& tag, const std::vector<double>& vec);
		void writeDoublemat(xmlTextWriter* wr, const std::string& tag, const Doublemat& mat);
		void writeStringvec(xmlTextWriter* wr, const std::string& tag, const std::vector<std::string>& vec);

		void writeBoolAttr(xmlTextWriter* wr, const std::string& tag, const std::string& attr, const std::string& attrval, bool val);
		void writeTinyintAttr(xmlTextWriter* wr, const std::string& tag, const std::string& attr, const std::string& attrval, tinyint val);
		void writeUtinyintAttr(xmlTextWriter* wr, const std::string& tag, const std::string& attr, const std::string& attrval, utinyint val);
		void writeSmallintAttr(xmlTextWriter* wr, const std::string& tag, const std::string& attr, const std::string& attrval, smallint val);
		void writeUsmallintAttr(xmlTextWriter* wr, const std::string& tag, const std::string& attr, const std::string& attrval, usmallint val);
		void writeIntAttr(xmlTextWriter* wr, const std::string& tag, const std::string& attr, const std::string& attrval, int val);
		void writeUintAttr(xmlTextWriter* wr, const std::string& tag, const std::string& attr, const std::string& attrval, uint val);
		void writeBigintAttr(xmlTextWriter* wr, const std::string& tag, const std::string& attr, const std::string& attrval, bigint val);
		void writeUbigintAttr(xmlTextWriter* wr, const std::string& tag, const std::string& attr, const std::string& attrval, ubigint val);
		void writeFloatAttr(xmlTextWriter* wr, const std::string& tag, const std::string& attr, const std::string& attrval, float val);
		void writeDoubleAttr(xmlTextWriter* wr, const std::string& tag, const std::string& attr, const std::string& attrval, double val);
		void writeStringAttr(xmlTextWriter* wr, const std::string& tag, const std::string& attr, const std::string& attrval, const std::string& val);
		void writeBoolvecAttr(xmlTextWriter* wr, const std::string& tag, const std::string& attr, const std::string& attrval, const std::vector<bool>& vec);
		void writeUtinyintvecAttr(xmlTextWriter* wr, const std::string& tag, const std::string& attr, const std::string& attrval, const std::vector<utinyint>& vec);
		void writeUsmallintvecAttr(xmlTextWriter* wr, const std::string& tag, const std::string& attr, const std::string& attrval, const std::vector<usmallint>& vec);
		void writeIntvecAttr(xmlTextWriter* wr, const std::string& tag, const std::string& attr, const std::string& attrval, const std::vector<int>& vec);
		void writeUintvecAttr(xmlTextWriter* wr, const std::string& tag, const std::string& attr, const std::string& attrval, const std::vector<uint>& vec);
		void writeUbigintvecAttr(xmlTextWriter* wr, const std::string& tag, const std::string& attr, const std::string& attrval, const std::vector<ubigint>& vec);
		void writeFloatvecAttr(xmlTextWriter* wr, const std::string& tag, const std::string& attr, const std::string& attrval, const std::vector<float>& vec);
		void writeFloatmatAttr(xmlTextWriter* wr, const std::string& tag, const std::string& attr, const std::string& attrval, const Floatmat& mat);
		void writeDoublevecAttr(xmlTextWriter* wr, const std::string& tag, const std::string& attr, const std::string& attrval, const std::vector<double>& vec);
		void writeDoublematAttr(xmlTextWriter* wr, const std::string& tag, const std::string& attr, const std::string& attrval, const Doublemat& mat);
		void writeStringvecAttr(xmlTextWriter* wr, const std::string& tag, const std::string& attr, const std::string& attrval, const std::vector<std::string>& vec);

		float compareFloat(const float a, const float b);
		double compareDouble(const double a, const double b);
		bool compareBoolvec(const std::vector<bool>& a, const std::vector<bool>& b);
		bool compareUtinyintvec(const std::vector<utinyint>& a, const std::vector<utinyint>& b);
		bool compareUsmallintvec(const std::vector<usmallint>& a, const std::vector<usmallint>& b);
		bool compareIntvec(const std::vector<int>& a, const std::vector<int>& b);
		bool compareUintvec(const std::vector<uint>& a, const std::vector<uint>& b);
		float compareFloatvec(const std::vector<float>& a, const std::vector<float>& b);
		float compareFloatmat(const Floatmat& a, const Floatmat& b);
		double compareDoublevec(const std::vector<double>& a, const std::vector<double>& b);
		double compareDoublemat(const Doublemat& a, const Doublemat& b);
		bool compareStringvec(const std::vector<std::string>& a, const std::vector<std::string>& b);

		bool find(const std::set<uint>& mask, const uint item);

		void insert(std::set<uint>& items, const uint item);
		void insert(std::set<uint>& items, const std::set<uint>& _items);

		void push_back(std::vector<uint>& ics, const uint ix);
		void push_back(std::vector<uint>& ics, const std::vector<uint>& _ics);
	};
};
#endif
