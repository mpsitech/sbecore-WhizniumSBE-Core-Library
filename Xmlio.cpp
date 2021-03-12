/**
	* \file Xmlio.cpp
	* methods for XML tree input/output based on libxml2 (implementation)
	* \copyright (C) 2016-2020 MPSI Technologies GmbH
	* \author Alexander Wirthmüller
	* \date created: 19 Feb 2007
	*/

#include "Xmlio.h"

using namespace std;

/******************************************************************************
 class Block
 ******************************************************************************/

Sbecore::Block::Block() {
};

Sbecore::Block::~Block() {
};

bool Sbecore::Block::has(
			const uint item
		) {
	return(mask.find(item) != mask.end());
};

bool Sbecore::Block::hasAll(
			const set<uint>& items
		) {
	for (set<uint>::iterator it=items.begin();it!=items.end();it++) if (!has(*it)) return false;
	return true;
};

bool Sbecore::Block::hasAny(
			const set<uint>& items
		) {
	for (set<uint>::iterator it=items.begin();it!=items.end();it++) if (has(*it)) return true;
	return false;
};

void Sbecore::Block::add(
			const uint item
		) {
	mask.insert(item);
};

void Sbecore::Block::clear() {
	mask.clear();
};

/******************************************************************************
 class Feeditem
 ******************************************************************************/

Sbecore::Feeditem::Feeditem(
			const bool Avail
			, const uint ix
			, const ubigint ref
			, const string& sref
			, const string& Title1
			, const string& Title2
			, const string& Title3
		) {
	this->Avail = Avail;
	this->ix = ix;
	this->ref = ref;
	this->sref = sref;
	this->Title1 = Title1;
	this->Title2 = Title2;
	this->Title3 = Title3;
};

bool Sbecore::Feeditem::operator==(
			const Feeditem& comp
		) {
	return ( (Avail == comp.Avail) && (ix == comp.ix) && (ref == comp.ref) && (sref == comp.sref) && (Title1 == comp.Title1) && (Title2 == comp.Title2) && (Title3 == comp.Title3) );
};

bool Sbecore::Feeditem::operator!=(
			const Feeditem& comp
		) {
	return(!operator==(comp));
};

void Sbecore::Feeditem::cap(
			const bool tit1
			, const bool tit2
			, const bool tit3
		) {
	if (tit1) Title1 = StrMod::cap(Title1);
	if (tit2) Title2 = StrMod::cap(Title2);
	if (tit3) Title3 = StrMod::cap(Title3);
};

bool Sbecore::Feeditem::readJSON(
			Json::Value& sup
			, const unsigned int ix
			, const bool shorttags
		) {
	Json::Value& me = sup[ix];

	bool found = me.isMember("num");

	if (found) {
		//Avail = me["Avail"].asBool();
		//ix = me["ix"].asInt();
		//ref = me["ref"].asInt64();
		if (shorttags) {
			sref = me["sref"].asString();
			Title1 = me["tit1"].asString();
			Title2 = me["tit2"].asString();
			Title3 = me["tit3"].asString();
		} else {
			sref = me["sref"].asString();
			Title1 = me["Title1"].asString();
			Title2 = me["Title2"].asString();
			Title3 = me["Title3"].asString();
		};
	};

	return found;
};

bool Sbecore::Feeditem::readXML(
			xmlXPathContext* docctx
			, string basexpath
			, const unsigned int num
			, const bool shorttags
		) {
	bool found;
	
	if (shorttags) basexpath += "/Fi[@num='" + to_string(num) + "']";
	else basexpath += "/Feeditem[@num='" + to_string(num) + "']";

	found = Xmlio::checkXPath(docctx, basexpath);

	if (found) {
		//Xmlio::extractBoolUclc(docctx, basexpath, "Avail", "Avail", Avail);
		//Xmlio::extractUintUclc(docctx, basexpath, "ix", "ix", ix);
		//Xmlio::extractUbigintUclc(docctx, basexpath, "ref", "ref", ref);
		Xmlio::extractStringUclc(docctx, basexpath, "sref", "sref", sref);
		Xmlio::extractStringUclc(docctx, basexpath, "Title1", "tit1", Title1);
		Xmlio::extractStringUclc(docctx, basexpath, "Title2", "tit2", Title2);
		Xmlio::extractStringUclc(docctx, basexpath, "Title3", "tit3", Title3);
	};

	return found;
};

void Sbecore::Feeditem::writeJSON(
			Json::Value& sup
			, unsigned int num
		) {
	Json::Value& me = sup.append(Json::Value(Json::objectValue));

	me["num"] = num;
	me["sref"] = sref;
	me["tit1"] = Title1;
	me["tit2"] = Title2;
	me["tit3"] = Title3;
};

void Sbecore::Feeditem::writeXML(
			xmlTextWriter* wr
			, unsigned int num
			, string difftag
		) {
	ostringstream str;

	if (difftag.length() == 0) difftag = "Fi";

	xmlTextWriterStartElement(wr, BAD_CAST difftag.c_str());

	str.str(""); str << num;
	xmlTextWriterWriteAttribute(wr, BAD_CAST "num", BAD_CAST str.str().c_str());

	//Xmlio::writeBool(wr, "Avail", Avail);
	//Xmlio::writeUint(wr, "ix", ix);
	//Xmlio::writeUbigint(wr, "ref", ref);
	Xmlio::writeString(wr, "sref", sref);
	Xmlio::writeString(wr, "tit1", Title1);
	Xmlio::writeString(wr, "tit2", Title2);
	Xmlio::writeString(wr, "tit3", Title3);

	xmlTextWriterEndElement(wr);
};

/******************************************************************************
 class Feed
 ******************************************************************************/

Sbecore::Feed::Feed(
			const string& tag
		) {
	this->tag = tag;
};

Sbecore::Feed::Feed(
			const Feed& src
		) {
	Feeditem* item;

	tag = src.tag;

	clear();
	for (unsigned int i = 0; i < src.size(); i++) {
		item = new Feeditem(*(src.nodes[i]));
		nodes.push_back(item);
	};
};

Sbecore::Feed::~Feed() {
	clear();
};

void Sbecore::Feed::clear() {
	for (unsigned int i = 0; i < size(); i++) delete nodes[i];
	nodes.resize(0);
};

unsigned int Sbecore::Feed::size() const {
	return(nodes.size());
};

void Sbecore::Feed::appendIxSrefTitles(
			const uint ix
			, const string& sref
			, const string& Title1
			, const string& Title2
			, const string& Title3
		) {
	Feeditem* item = new Feeditem(true, ix, 0, sref, Title1, Title2, Title3);

	nodes.push_back(item);
};

void Sbecore::Feed::appendIxRefSrefTitles(
			const uint ix
			, const ubigint ref
			, const string& sref
			, const string& Title1
			, const string& Title2
			, const string& Title3
		) {
	Feeditem* item = new Feeditem(true, ix, ref, sref, Title1, Title2, Title3);

	nodes.push_back(item);
};

void Sbecore::Feed::appendRefTitles(
			const ubigint ref
			, const string& Title1
			, const string& Title2
			, const string& Title3
		) {
	Feeditem* item = new Feeditem(true, 0, ref, "", Title1, Title2, Title3);

	nodes.push_back(item);
};

void Sbecore::Feed::appendRefSrefTitles(
			const ubigint ref
			, const string& sref
			, const string& Title1
			, const string& Title2
			, const string& Title3
		) {
	Feeditem* item = new Feeditem(true, 0, ref, sref, Title1, Title2, Title3);

	nodes.push_back(item);
};

void Sbecore::Feed::appendTitles(
			const string& Title1
			, const string& Title2
			, const string& Title3
		) {
	Feeditem* item = new Feeditem(true, 0, 0, "", Title1, Title2, Title3);

	nodes.push_back(item);
};

Sbecore::Feeditem* Sbecore::Feed::operator[](
			const uint ix
		) {
	Feeditem* retval = NULL;

	for (unsigned int i = 0; i < size(); i++) {
		if (nodes[i]->ix == ix) {
			retval = nodes[i];
			break;
		};
	};

	return retval;
};

Sbecore::Feeditem* Sbecore::Feed::getByNum(
			const uint num
		) {
	if ( (num <= size()) && (num > 0) ) {
		return(nodes[num-1]);
	} else {
		return NULL;
	};
};

Sbecore::uint Sbecore::Feed::getNumByIx(
			const uint ix
		) {
	uint retval = 0;

	for (unsigned int i = 0; i < size(); i++) {
		if (nodes[i]->ix == ix) {
			retval = i + 1;
			break;
		};
	};

	return retval;
};

Sbecore::uint Sbecore::Feed::getNumByRef(
			const ubigint ref
		) {
	uint retval = 0;

	for (unsigned int i = 0; i < size(); i++) {
		if (nodes[i]->ref == ref) {
			retval = i + 1;
			break;
		};
	};

	return retval;
};

Sbecore::uint Sbecore::Feed::getNumBySref(
			const string& sref
		) {
	uint retval = 0;

	for (unsigned int i = 0; i < size(); i++) {
		if (nodes[i]->sref == sref) {
			retval = i + 1;
			break;
		};
	};

	return retval;
};

Sbecore::uint Sbecore::Feed::getIxByNum(
			const uint num
		) {
	Feeditem* item = getByNum(num);
	if (item) return item->ix; else return 0;
};

Sbecore::ubigint Sbecore::Feed::getRefByNum(
			const uint num
		) {
	Feeditem* item = getByNum(num);
	if (item) return item->ref; else return 0;
};

string Sbecore::Feed::getSrefByNum(
			const uint num
		) {
	Feeditem* item = getByNum(num);
	if (item) return item->sref; else return "";
};

Sbecore::Feed& Sbecore::Feed::operator=(
			const Feed& src
		) {
	Feeditem* item;

	if (&src != this) {
		tag = src.tag;

		clear();
		for (unsigned int i = 0; i < src.size(); i++) {
			item = new Feeditem(*(src.nodes[i]));
			nodes.push_back(item);
		};
	};

	return(*this);
};

bool Sbecore::Feed::operator==(
			const Feed& comp
		) {
	bool retval;

	retval = (size() == comp.size());

	if (retval) {
		for (unsigned int i = 0; i < size(); i++) {
			retval = ( *(nodes[i]) == *(comp.nodes[i]) );

			if (!retval) break;
		};
	};

	return retval;
};

bool Sbecore::Feed::operator!=(
			const Feed& comp
		) {
	return(!operator==(comp));
};

void Sbecore::Feed::cap(
			const bool tit1
			, const bool tit2
			, const bool tit3
		) {
	for (unsigned int i = 0; i < size(); i++) nodes[i]->cap(tit1, tit2, tit3);
};

bool Sbecore::Feed::readJSON(
			Json::Value& sup
			, bool addbasetag
		) {
	bool basefound;

	vector<unsigned int> ics;
	unsigned int ix;
	vector<unsigned int> nums;
	unsigned int num;
	vector<bool> shorttags;
	bool shorttag;

	Feeditem* item = NULL;

	Json::Value& me = sup;
	if (addbasetag) me = sup[tag];

	basefound = (me != Json::nullValue);

	clear();

	if (me.isArray()) {
		// extractList() JSON-style
		ics.clear();
		nums.clear();
		shorttags.clear();

		for (unsigned int i = 0; i < me.size(); i++) {
			if (me[i].isMember("num")) {
				ics.push_back(i);
				nums.push_back(me[i]["num"].asInt());
				shorttags.push_back(me[i].isMember("tit1") || me[i].isMember("tit2") || me[i].isMember("tit3"));
			};
		};

		for (unsigned int i = 0; i < ics.size(); i++) {
			for (unsigned int j = i + 1; j < ics.size(); j++) {
				if (nums[j] < nums[i]) {
					ix = ics[i];
					num = nums[i];
					shorttag = shorttags[i];

					ics[i] = ics[j];
					nums[i] = nums[j];
					shorttags[i] = shorttags[j];
					
					ics[j] = ix;
					nums[j] = num;
					shorttags[j] = shorttag;
				};
			};
		};
		//

		for (unsigned int i = 0; i < ics.size(); i++) {
			item = new Feeditem();

			if (item->readJSON(me, ics[i], shorttags[i])) nodes.push_back(item);
			else delete item;
		};
	};

	return basefound;
};

bool Sbecore::Feed::readXML(
			xmlXPathContext* docctx
			, string basexpath
			, bool addbasetag
		) {
	bool basefound;

	vector<unsigned int> nums;
	vector<bool> shorttags;

	Feeditem* item = NULL;

	if (addbasetag)
		basefound = Xmlio::checkUclcXPaths(docctx, basexpath, basexpath, tag);
	else
		basefound = Xmlio::checkXPath(docctx, basexpath);

	Xmlio::extractList(docctx, basexpath, "Feeditem", "Fi", "num", nums, shorttags);

	clear();

	for (unsigned int i = 0; i < nums.size(); i++) {
		item = new Feeditem();

		if (item->readXML(docctx, basexpath, nums[i], shorttags[i])) nodes.push_back(item);
		else delete item;
	};

	return basefound;
};

void Sbecore::Feed::writeJSON(
			Json::Value& sup
			, std::string difftag
		) {
	if (difftag == "") difftag = tag;
	if (difftag == "") return;

	Json::Value& me = sup[difftag] = Json::Value(Json::arrayValue);
	for (unsigned int i = 0; i < nodes.size(); i++) if (nodes[i]->Avail) nodes[i]->writeJSON(me, i + 1);
};

void Sbecore::Feed::writeXML(
			xmlTextWriter* wr
			, string difftag
		) {
	if (difftag == "") difftag = tag;
	if (difftag == "") return;

	xmlTextWriterStartElement(wr, BAD_CAST difftag.c_str());
		for (unsigned int i = 0; i < nodes.size(); i++) if (nodes[i]->Avail) nodes[i]->writeXML(wr, i + 1);
	xmlTextWriterEndElement(wr);
};

/******************************************************************************
 namespace Jsonio
 ******************************************************************************/

bool Sbecore::Jsonio::extractBoolvec(
			Json::Value& sup
			, const string& tag
			, vector<bool>& vec
		) {
	string str;

	vec.resize(0);

	Json::Value& me = sup[tag];

	if (me != Json::nullValue) {
		str = me.asString();

		vec.resize(str.length());
		for (unsigned int i = 0; i < vec.size(); i++) vec[i] = (str[i] == '1');

		return true;
	};

	return false;
};

bool Sbecore::Jsonio::extractUtinyintvec(
			Json::Value& sup
			, const string& tag
			, vector<utinyint>& vec
		) {
	vec.resize(0);

	Json::Value& me = sup[tag];

	if (me != Json::nullValue) {
		Xmlio::base64ToUtinyintvec(me.asString().c_str(), me.asString().length(), vec);
		return true;
	};

	return false;
};

bool Sbecore::Jsonio::extractUsmallintvec(
			Json::Value& sup
			, const string& tag
			, vector<usmallint>& vec
		) {
	vec.resize(0);

	Json::Value& me = sup[tag];

	if (me != Json::nullValue) {
		Xmlio::base64ToVec<usmallint>(me.asString().c_str(), me.asString().length(), vec);
		return true;
	};

	return false;
};

bool Sbecore::Jsonio::extractIntvec(
			Json::Value& sup
			, const string& tag
			, vector<int>& vec
		) {
	vec.resize(0);

	Json::Value& me = sup[tag];

	if (me != Json::nullValue) {
		Xmlio::base64ToVec<int>(me.asString().c_str(), me.asString().length(), vec);
		return true;
	};

	return false;
};

bool Sbecore::Jsonio::extractUintvec(
			Json::Value& sup
			, const string& tag
			, vector<uint>& vec
		) {
	vec.resize(0);

	Json::Value& me = sup[tag];

	if (me != Json::nullValue) {
		Xmlio::base64ToVec<uint>(me.asString().c_str(), me.asString().length(), vec);
		return true;
	};

	return false;
};

bool Sbecore::Jsonio::extractUbigintvec(
			Json::Value& sup
			, const string& tag
			, vector<ubigint>& vec
		) {
	vec.resize(0);

	Json::Value& me = sup[tag];

	if (me != Json::nullValue) {
		Xmlio::base64ToVec<ubigint>(me.asString().c_str(), me.asString().length(), vec);
		return true;
	};

	return false;
};

bool Sbecore::Jsonio::extractFloatvec(
			Json::Value& sup
			, const string& tag
			, vector<float>& vec
		) {
	vec.resize(0);

	Json::Value& me = sup[tag];

	if (me != Json::nullValue) {
		Xmlio::base64ToVec<float>(me.asString().c_str(), me.asString().length(), vec);
		return true;
	};

	return false;
};

bool Sbecore::Jsonio::extractFloatmat(
			Json::Value& sup
			, const string& tag
			, Floatmat& mat
		) {
	bool valid;

	Json::Value& me = sup[tag];

	valid = (me != Json::nullValue);

	if (valid) valid = extractFloatvec(me, "vec", mat.vec);

	if (valid) valid = me.isMember("M");
	if (valid) mat.M = me["M"].asInt();

	if (valid) valid = me.isMember("N");
	if (valid) mat.N = me["N"].asInt();

	if (valid) valid = ((mat.M * mat.N) == mat.vec.size());

	if (!valid) mat = Floatmat();

	return valid;
};

bool Sbecore::Jsonio::extractDoublevec(
			Json::Value& sup
			, const string& tag
			, vector<double>& vec
		) {
	vec.resize(0);

	Json::Value& me = sup[tag];

	if (me != Json::nullValue) {
		Xmlio::base64ToVec<double>(me.asString().c_str(), me.asString().length(), vec);
		return true;
	};

	return false;
};

bool Sbecore::Jsonio::extractDoublemat(
			Json::Value& sup
			, const string& tag
			, Doublemat& mat
		) {
	bool valid;

	Json::Value& me = sup[tag];

	valid = (me != Json::nullValue);

	if (valid) valid = extractDoublevec(me, "vec", mat.vec);

	if (valid) valid = me.isMember("M");
	if (valid) mat.M = me["M"].asInt();

	if (valid) valid = me.isMember("N");
	if (valid) mat.N = me["N"].asInt();

	if (valid) valid = ((mat.M * mat.N) == mat.vec.size());

	if (!valid) mat = Doublemat();

	return valid;
};

bool Sbecore::Jsonio::extractStringvec(
			Json::Value& sup
			, const string& tag
			, vector<string>& vec
		) {
	string s;

	vec.resize(0);

	Json::Value& me = sup[tag];

	if (me != Json::nullValue) {
		s = Xmlio::fromUTF8(me.asString());
		StrMod::stringToVector(s, vec);

		return true;
	};

	return false;
};

void Sbecore::Jsonio::writeBoolvec(
			Json::Value& sup
			, const string& tag
			, const vector<bool>& vec
		) {
	string str(vec.size(), '0');

	for (unsigned int i = 0; i < vec.size(); i++) if (vec[i]) str[i] = '1';

	sup[tag] = str;
};

void Sbecore::Jsonio::writeUtinyintvec(
			Json::Value& sup
			, const string& tag
			, const vector<utinyint>& vec
		) {
	sup[tag] = Xmlio::dataToBase64((const unsigned char*) vec.data(), vec.size(), sizeof(utinyint));
};

void Sbecore::Jsonio::writeUsmallintvec(
			Json::Value& sup
			, const string& tag
			, const vector<usmallint>& vec
		) {
	sup[tag] = Xmlio::dataToBase64((const unsigned char*) vec.data(), vec.size(), sizeof(usmallint));
};

void Sbecore::Jsonio::writeIntvec(
			Json::Value& sup
			, const string& tag
			, const vector<int>& vec
		) {
	sup[tag] = Xmlio::dataToBase64((const unsigned char*) vec.data(), vec.size(), sizeof(int));
};

void Sbecore::Jsonio::writeUintvec(
			Json::Value& sup
			, const string& tag
			, const vector<uint>& vec
		) {
	sup[tag] = Xmlio::dataToBase64((const unsigned char*) vec.data(), vec.size(), sizeof(uint));
};

void Sbecore::Jsonio::writeUbigintvec(
			Json::Value& sup
			, const string& tag
			, const vector<ubigint>& vec
		) {
	sup[tag] = Xmlio::dataToBase64((const unsigned char*) vec.data(), vec.size(), sizeof(ubigint));
};

void Sbecore::Jsonio::writeFloatvec(
			Json::Value& sup
			, const string& tag
			, const vector<float>& vec
		) {
	sup[tag] = Xmlio::dataToBase64((const unsigned char*) vec.data(), vec.size(), sizeof(float));
};

void Sbecore::Jsonio::writeFloatmat(
			Json::Value& sup
			, const string& tag
			, const Floatmat& mat
		) {
	Json::Value& me = sup[tag] = Json::Value(Json::objectValue);

	me["M"] = mat.M;
	me["N"] = mat.N;
	me["vec"] = Xmlio::dataToBase64((const unsigned char*) mat.vec.data(), mat.vec.size(), sizeof(float));
};

void Sbecore::Jsonio::writeDoublevec(
			Json::Value& sup
			, const string& tag
			, const vector<double>& vec
		) {
	sup[tag] = Xmlio::dataToBase64((const unsigned char*) vec.data(), vec.size(), sizeof(double));
};

void Sbecore::Jsonio::writeDoublemat(
			Json::Value& sup
			, const string& tag
			, const Doublemat& mat
		) {
	Json::Value& me = sup[tag] = Json::Value(Json::objectValue);

	me["M"] = mat.M;
	me["N"] = mat.N;
	me["vec"] = Xmlio::dataToBase64((const unsigned char*) mat.vec.data(), mat.vec.size(), sizeof(double));
};

void Sbecore::Jsonio::writeStringvec(
			Json::Value& sup
			, const string& tag
			, const vector<string>& vec
		) {
	string s;

	for (unsigned int i = 0; i < vec.size(); i++) {
		if (i != 0) s = s + ";";
		s = s + vec[i];
	};

	sup[tag] = s;
};

/******************************************************************************
 namespace Xmlio
 ******************************************************************************/

bool Sbecore::Xmlio::has(
			set<uint>& items
			, const uint item
		) {
	return(items.find(item) != items.end());
};

bool Sbecore::Xmlio::hasAll(
			set<uint>& items
			, const set<uint>& _items
		) {
	for (set<uint>::iterator it=_items.begin();it!=_items.end();it++) if (!has(items, *it)) return false;
	return true;
};

bool Sbecore::Xmlio::hasAny(
			set<uint>& items
			, const set<uint>& _items
		) {
	for (set<uint>::iterator it=_items.begin();it!=_items.end();it++) if (has(items, *it)) return true;
	return false;
};

void Sbecore::Xmlio::add(
			set<uint>& items
			, const uint item
		) {
	items.insert(item);
};

string Sbecore::Xmlio::fromUTF8(
			const string& s
		) {
	// convert UTF-8 to ISO-8859-1 (ISO Latin 1)
	string retval = s;

	unsigned char* buf = NULL;
	int buflen, inlen, outlen;

	inlen = s.length();
	outlen = 2*inlen;
	buf = new unsigned char[outlen];

	buflen = UTF8Toisolat1(buf, &outlen, (const unsigned char*) s.c_str(), &inlen);

	if ((buflen > 0) && (buflen < (2*inlen))) {
		buf[buflen] = '\0';
		retval = (char*) buf;
	};

	delete[] buf;

	return(retval);
};

string Sbecore::Xmlio::toUTF8(
			const string& s
		) {
	// convert ISO-8859-1 (ISO Latin 1) to UTF-8
	string retval = s;

	unsigned char* buf = NULL;
	int buflen, inlen, outlen;

	inlen = s.length();
	outlen = 2*inlen;
	buf = new unsigned char[outlen];

	buflen = isolat1ToUTF8(buf, &outlen, (const unsigned char*) s.c_str(), &inlen);

	if ((buflen > 0) && (buflen < (2*inlen))) {
		buf[buflen] = '\0';
		retval = (char*) buf;
	};

	delete[] buf;

	return(retval);
};

void Sbecore::Xmlio::fromBase64(
			const char* inbuf
			, unsigned int inbuflen
			, unsigned char** outbuf
			, unsigned int& outbuflen
		) {
	char quad[4];
	unsigned char trip[3];

	unsigned int ix, cnt;

	*outbuf = NULL;
	outbuflen = 0;

	for (unsigned int i = inbuflen; i > 0; i--) {
		if ((inbuf[i-1] == '\n') || (inbuf[i-1] == '\r') || (inbuf[i-1] == '\t') || (inbuf[i-1] == ' ')) {
		} else if (inbuf[i-1] == '=') outbuflen--;
		else break;
	};

	ix = 0;
	for (unsigned int i = 0; i < inbuflen; i++) if ((inbuf[i] == '\n') || (inbuf[i] == '\r') || (inbuf[i] == '\t') || (inbuf[i] == ' ')) ix++;
	inbuflen -= ix;

	cnt = inbuflen/4;

	if ((inbuflen >= 4) && ((inbuflen%4) == 0)) {
		outbuflen += 3*cnt;
		*outbuf = new unsigned char[outbuflen];

		ix = 0;

		for (unsigned int i = 0; i < cnt; i++) {
			while ((inbuf[ix] == '\n') || (inbuf[ix] == '\r') || (inbuf[ix] == '\t') || (inbuf[ix] == ' ')) ix++;
			quad[0] = inbuf[ix++];
			while ((inbuf[ix] == '\n') || (inbuf[ix] == '\r') || (inbuf[ix] == '\t') || (inbuf[ix] == ' ')) ix++;
			quad[1] = inbuf[ix++];
			while ((inbuf[ix] == '\n') || (inbuf[ix] == '\r') || (inbuf[ix] == '\t') || (inbuf[ix] == ' ')) ix++;
			quad[2] = inbuf[ix++];
			while ((inbuf[ix] == '\n') || (inbuf[ix] == '\r') || (inbuf[ix] == '\t') || (inbuf[ix] == ' ')) ix++;
			quad[3] = inbuf[ix++];

			for (unsigned int j = 0; j < 4; j++) {
				if ((quad[j] >= 'A') && (quad[j] <= 'Z')) quad[j] = quad[j] - 'A';
				else if ((quad[j] >= 'a') && (quad[j] <= 'z')) quad[j] = quad[j] - 'a' + 26;
				else if ((quad[j] >= '0') && (quad[j] <= '9')) quad[j] = quad[j] - '0' + 52;
				else if (quad[j] == '+') quad[j] = 62;
				else if (quad[j] == ' ') quad[j] = 62; // quick fix for HTTP encoding of space as plus
				else if (quad[j] == '/') quad[j] = 63;
				else if (quad[j] == '=') quad[j] = -128;
				else quad[j] = 0;
			};

			trip[0] = (quad[0] << 2) + ((quad[1] & 0xf0) >> 4);
			trip[1] = ((quad[1] & 0x0F) << 4) + ((quad[2] & 0xFC) >> 2);
			trip[2] = ((quad[2] & 0x03) << 6) + quad[3];
/*
			trip[0] = 4*quad[0] + (quad[1] & 0xf0)/16;
			trip[1] = 16*(quad[1] & 0x0f) + (quad[2] & 0xfc)/4;
			trip[2] = 64*(quad[2] & 0x03) + quad[3];
*/

			(*outbuf)[3*i] = trip[0];

			if (i == (cnt-1)) {
				if (quad[2] != -128) {
					(*outbuf)[3*i+1] = trip[1];
					if (quad[3] != -128) (*outbuf)[3*i+2] = trip[2];
				};
			} else {
				(*outbuf)[3*i+1] = trip[1];
				(*outbuf)[3*i+2] = trip[2];
			};
		};

	} else {
		outbuflen = 0;
	};
};

void Sbecore::Xmlio::toBase64(
			const unsigned char* inbuf
			, unsigned int inbuflen
			, char** outbuf
			, unsigned int& outbuflen
		) {
	unsigned char trip[3];
	char quad[4];

	unsigned int ix, cnt;

	int rem;

	cnt = inbuflen/3;
	rem = (inbuflen%3);
	if (rem != 0) cnt++;

	outbuflen = 4*cnt;
	*outbuf = new char[outbuflen];

	ix = 0;

	for (unsigned int i = 0; i < cnt; i++) {
		trip[0] = inbuf[ix++];
		if ((rem == 0) || ((i+1) < cnt)) {
			trip[1] = inbuf[ix++];
			trip[2] = inbuf[ix++];
		} else {
			trip[1] = 0;
			trip[2] = 0;

			if (ix < inbuflen) trip[1] = inbuf[ix++];
			if (ix < inbuflen) trip[2] = inbuf[ix++];
		};

		quad[0] = (trip[0] >> 2);
		quad[1] = ((trip[0] & 0x03) << 4) + (trip[1] >> 4);
		quad[2] = ((trip[1] & 0x0F) << 2) + (trip[2] >> 6);
		quad[3] = (trip[2] & 0x3F);
/*
		quad[0] = trip[0]/4;
		quad[1] = 16*(trip[0] & 0x03) + trip[1]/16;
		quad[2] = 4*(trip[1] & 0x0f) + trip[2]/64;
		quad[3] = (trip[2] & 0x3f);
*/

		for (unsigned int j = 0; j < 4; j++) {
			if ((quad[j] >= 0) && (quad[j] <= 25)) quad[j] = quad[j] + 'A';
			else if ((quad[j] >= 26) && (quad[j] <= 51)) quad[j] = quad[j] - 26 + 'a';
			else if ((quad[j] >= 52) && (quad[j] <= 61)) quad[j] = quad[j] - 52 + '0';
			else if (quad[j] == 62) quad[j] = '+';
			else if (quad[j] == 63) quad[j] = '/';
			else quad[j] = 'A';

			(*outbuf)[4*i+j] = quad[j];
		};
	};

	if (rem == 1) (*outbuf)[outbuflen-2] = '=';
	if ((rem == 1) || (rem == 2)) (*outbuf)[outbuflen-1] = '=';
};

bool Sbecore::Xmlio::bigendian() {
	unsigned short int var = 255;
	char* buf = ((char*) &var);
	
	return(buf[0] == 0);
};

void Sbecore::Xmlio::invertBuffer(
			unsigned char* buf
			, const unsigned int len
			, const unsigned int varlen
		) {
	char c;

	for (unsigned int i = 0; i < len; i++) {
		for (unsigned int j = 0; j < varlen/2; j++) {
			c = buf[i*varlen+j];
			buf[i*varlen+j] = buf[i*varlen+(varlen-j-1)];
			buf[i*varlen+(varlen-j-1)] = c;
		};
	};
};

void Sbecore::Xmlio::base64ToUtinyintvec(
			const char* _buf
			, const unsigned int buflen
			, vector<utinyint>& vec
		) {
	unsigned char* buf = NULL;
	unsigned int len;

	vec.resize(0);

	fromBase64(_buf, buflen, &buf, len);

	if (buf) {
		vec.resize(len, 0);
		memcpy((void*) vec.data(), (void*) buf, len);

		delete[] buf;
	};
};

template<class T> void Sbecore::Xmlio::base64ToVec(
			const char* _buf
			, const unsigned int buflen
			, vector<T>& vec
		) {
	const unsigned int varlen = sizeof(T);

	unsigned char* buf = NULL;
	unsigned int len;

	vec.resize(0);

	fromBase64(_buf, buflen, &buf, len);

	if (buf) {
		if ((len%varlen) == 0) {
			if (!bigendian()) invertBuffer(buf, len/varlen, varlen);

			vec.resize(len/varlen, 0);
			memcpy((void*) vec.data(), (void*) buf, len);
		};

		delete[] buf;
	};
};

string Sbecore::Xmlio::dataToBase64(
			const unsigned char* _buf
			, const unsigned int len
			, const unsigned int varlen
		) {
	string s;

	char* outbuf = NULL;
	unsigned int outbuflen;

	if (!bigendian()) {
		unsigned char* buf = new unsigned char[len*varlen];

		memcpy((void*) buf, (void*) _buf, len*varlen);
		invertBuffer(buf, len, varlen);
		toBase64(buf, len*varlen, &outbuf, outbuflen);

		delete[] buf;

	} else {
		toBase64(_buf, len*varlen, &outbuf, outbuflen);
	};

	if (outbuf) {
		s.assign(outbuf, outbuflen);

		delete[] outbuf;
	};

	return s;
};

void Sbecore::Xmlio::parseFile(
			const string& fullpath
			, xmlDoc** doc
			, xmlXPathContext** docctx
		) {
#ifdef _WIN32
	if (_access(fullpath.c_str(), 0)) throw SbeException(SbeException::PATHNF, {{"path",fullpath}});
#else
	if (access(fullpath.c_str(), R_OK)) throw SbeException(SbeException::PATHNF, {{"path",fullpath}});
#endif

	*doc = xmlParseFile(fullpath.c_str());
	if (!*doc) throw SbeException(SbeException::XMLIO_FILEPARSE, {});

	*docctx = xmlXPathNewContext(*doc);
};

void Sbecore::Xmlio::parseBuffer(
			char* buf
			, size_t length
			, xmlDoc** doc
			, xmlXPathContext** docctx
			, const bool rmvns
		) {
	const unsigned int XMLNSL = 0;
	const unsigned int HDRR1 = 1;
	const unsigned int HDRL = 2;
	const unsigned int HDRR2 = 3;
	const unsigned int TAGR1 = 4;
	const unsigned int XMLNSR = 5;
	const unsigned int TAGL = 6;
	const unsigned int TAGR2 = 7;

	vector<string> keys = {" xmlns=\"", "?>\n", "<?", "?>", "/>", "\"", "<", ">"};

	vector<size_t> ptrs;
	size_t ptr;

	vector<unsigned int> tkns;
	unsigned int tkn;

	vector<size_t> ptrsSkipstart;
	size_t ptrSkipstart;
	vector<size_t> ptrsSkipstop;
	size_t ptrSkipstop;

	size_t shift;

	bool match;

	//cout << string(buf, length) << endl;

	if (rmvns) {
		// - remove header and xmlns attributes in all nodes:
		// from '<?' to '?>\n' or '?>' (whichever comes first) ; HDRL to HDRR1/HDRR2
		// from '<' to '/>' or '>' (whichever comes first) ; TAGL to TAGR1/TAGR2
		// from ' xmlns="' to '"' ; XMLNSL to XMLNSR

		// tokenize
		ptr = 0;

		while (ptr < length) {
			for (unsigned int i = 0; i < keys.size(); i++) {
				match = false;

				if ((ptr+keys[i].length()) < length) {
					match = true;
					for (unsigned int j = 0; j < keys[i].length(); j++) {
						if (buf[ptr+j] != keys[i][j]) {
							match = false;
							break;
						};
					};
				};

				if (match) {
					ptrs.push_back(ptr);
					tkns.push_back(i);
					ptr += keys[i].length()-1;
					break;
				};
			};

			ptr++;
		};

		// identify sections to skip
		for (unsigned int i = 0; i < ptrs.size(); i++) {
			ptr = ptrs[i];
			tkn = tkns[i];

			if (tkn == HDRL) {
				if ((i+1) < ptrs.size()) {
					if ((tkns[i+1] == HDRR1) || (tkns[i+1] == HDRR2)) {
						ptrsSkipstart.push_back(ptr);
						ptrsSkipstop.push_back(ptrs[i+1]+keys[tkns[i+1]].length());
						i += 1;
					};
				};
			} else if (tkn == TAGL) {
				if ((i+3) < ptrs.size()) {
					if ((tkns[i+1] == XMLNSL) && (tkns[i+2] == XMLNSR)) {
						if ((tkns[i+3] == TAGR1) || (tkns[i+3] == TAGR2)) {
							ptrsSkipstart.push_back(ptrs[i+1]);
							ptrsSkipstop.push_back(ptrs[i+2]+keys[tkns[i+2]].length());
							i += 3;
						};
					};
				};
			};
		};

		//cout << "identified " << ptrsSkipstart.size() << " sections to remove" << endl;

		// stuff buffer
		shift = 0;

		for (unsigned int i = 0; i < ptrsSkipstart.size(); i++) {
			ptrSkipstart = ptrsSkipstart[i];
			ptrSkipstop = ptrsSkipstop[i];

			shift += ptrSkipstop-ptrSkipstart;

			if ((i+1) < ptrsSkipstart.size()) memmove(&(buf[ptrSkipstop-shift]), &(buf[ptrSkipstop]), ptrsSkipstart[i+1]-ptrSkipstop);
			else memmove(&(buf[ptrSkipstop-shift]), &(buf[ptrSkipstop]), length-ptrSkipstop);
		};

		length = length - shift;
		//cout << "reduction in length: " << shift << endl;
	};

	*doc = xmlParseMemory(buf, length);
	if (!*doc) throw SbeException(SbeException::XMLIO_BUFPARSE, {});
	
	*docctx = xmlXPathNewContext(*doc);
};

void Sbecore::Xmlio::closeParsed(
			xmlDoc* doc
			, xmlXPathContext* docctx
		) {
	xmlXPathFreeContext(docctx);
	xmlFreeDoc(doc);
};

void Sbecore::Xmlio::startwriteFile(
			const string& fullpath
			, xmlTextWriter** wr
		) {
	*wr = xmlNewTextWriterFilename(fullpath.c_str(), 0);

	xmlTextWriterStartDocument(*wr, NULL, "UTF-8", NULL);
};

void Sbecore::Xmlio::closewriteFile(
			xmlTextWriter* wr
		) {
	xmlTextWriterEndDocument(wr);
	xmlFreeTextWriter(wr);
};

void Sbecore::Xmlio::startwriteBuffer(
			xmlTextWriter** wr
			, xmlBuffer** buf
		) {
	*buf = xmlBufferCreate();
	(*buf)->alloc = XML_BUFFER_ALLOC_EXACT;

	*wr = xmlNewTextWriterMemory(*buf, 0);

	xmlTextWriterStartDocument(*wr, NULL, "UTF-8", NULL);
};

void Sbecore::Xmlio::closewriteBuffer(
			xmlTextWriter* wr
		) {
	xmlTextWriterEndDocument(wr);
	xmlFreeTextWriter(wr);
};

bool Sbecore::Xmlio::checkXPath(
			xmlXPathContext* docctx															//! documant XPath context
			, const string& xpath																//! XPath location (absolute)
		) {
	bool retval;
	xmlXPathObject* obj;

	obj = xmlXPathEvalExpression(BAD_CAST xpath.c_str(), docctx);

	retval = (obj->nodesetval->nodeNr > 0);

	xmlXPathFreeObject(obj);
	return(retval);
};

bool Sbecore::Xmlio::checkXPath(
			xmlXPathContext* docctx
			, const string& xpath
			, unsigned int& lineno
		) {
	bool retval = false;
	xmlXPathObject* obj;

	obj = xmlXPathEvalExpression(BAD_CAST xpath.c_str(), docctx);

	if (obj->nodesetval->nodeNr > 0) {
		retval = true;
		lineno = obj->nodesetval->nodeTab[0]->line;
	};

	xmlXPathFreeObject(obj);
	return(retval);
};

bool Sbecore::Xmlio::checkAltXPaths(
			xmlXPathContext* docctx
			, string& goodxpath
			, const string& basexpath
			, const string& alt1
			, const string& alt2
			, const string& alt3
			, const string& alt4
			, const string& alt5
			, const string& alt6
		) {
	if (alt1.length() > 0) {
		if (checkXPath(docctx, basexpath + "/" + alt1)) {
			goodxpath = basexpath + "/" + alt1;
			return true;
		};
	};
	if (alt2.length() > 0) {
		if (checkXPath(docctx, basexpath + "/" + alt2)) {
			goodxpath = basexpath + "/" + alt2;
			return true;
		};
	};
	if (alt3.length() > 0) {
		if (checkXPath(docctx, basexpath + "/" + alt3)) {
			goodxpath = basexpath + "/" + alt3;
			return true;
		};
	};
	if (alt4.length() > 0) {
		if (checkXPath(docctx, basexpath + "/" + alt4)) {
			goodxpath = basexpath + "/" + alt4;
			return true;
		};
	};
	if (alt5.length() > 0) {
		if (checkXPath(docctx, basexpath + "/" + alt5)) {
			goodxpath = basexpath + "/" + alt5;
			return true;
		};
	};
	if (alt6.length() > 0) {
		if (checkXPath(docctx, basexpath + "/" + alt6)) {
			goodxpath = basexpath + "/" + alt6;
			return true;
		};
	};

	return false;
};

bool Sbecore::Xmlio::checkUclcXPaths(
			xmlXPathContext* docctx
			, string& goodxpath
			, const string& basexpath
			, const string& regc
		) {
	if (regc.length() > 0) {
		if (checkXPath(docctx, basexpath + "/" + regc)) {
			goodxpath = basexpath + "/" + regc;
			return true;
		};

		// upper case
		if (checkXPath(docctx, basexpath + "/" + StrMod::uc(regc))) {
			goodxpath = basexpath + "/" + StrMod::uc(regc);
			return true;
		};

		// lower case
		if (checkXPath(docctx, basexpath + "/" + StrMod::lc(regc))) {
			goodxpath = basexpath + "/" + StrMod::lc(regc);
			return true;
		};
	};

	return false;
};

bool Sbecore::Xmlio::checkAttrUclcXpaths(
			xmlXPathContext* docctx
			, const string& basexpath
			, string& goodpath
			, const string& tag
			, const string& shorttag
			, const string& attr
			, const string& attrval
		) {
	// assemble XPath alternatives
	string alt1 = tag + "[@" + attr + "='" + attrval + "']"; // ex. "StgBrlyLocMap[@sref='height']"
	string alt2 = shorttag + "[@" + attr + "='" + attrval + "']"; // ex. "Si[@sref='height']"
	string alt3 = StrMod::uc(tag) + "[@" + StrMod::uc(attr) + "='" + attrval + "']"; // ex. "STGBRLYLOCMAP[@SREF='height']"
	string alt4 = StrMod::uc(shorttag) + "[@" + StrMod::uc(attr) + "='" + attrval + "']"; // ex. "SI[@SREF='height']"
	string alt5 = StrMod::lc(tag) + "[@" + StrMod::lc(attr) + "='" + attrval + "']"; // ex. "stgbrlylocmap[@sref='height']"
	string alt6 = StrMod::lc(shorttag) + "[@" + StrMod::lc(attr) + "='" + attrval + "']"; // ex. "si[@sref='height']"

	return(checkAltXPaths(docctx, goodpath, basexpath, alt1, alt2, alt3, alt4, alt5, alt6));
};

string Sbecore::Xmlio::extractRoot(
			xmlDoc* doc
		) {
	string retval;
	xmlNode* node = NULL;

	node = xmlDocGetRootElement(doc);
	if (node) retval = (const char*) node->name;

	return retval;
};

// NOTE: the following methods 'bool Sbecore::Xmlio::extractXxx()' should only be called in case xpath is proven to be valid

void Sbecore::Xmlio::extractBool(
			xmlXPathContext* docctx
			, const string& xpath
			, bool& val
		) {
	xmlXPathObject* obj;
	xmlChar* nodebuf;

	obj = xmlXPathEvalExpression(BAD_CAST xpath.c_str(), docctx);
	nodebuf = xmlNodeGetContent(obj->nodesetval->nodeTab[0]);

	if (nodebuf) {
		val = !(strcmp((char*) nodebuf, "true"));
		xmlFree(nodebuf);

	} else val = false;;

	xmlXPathFreeObject(obj);
};

void Sbecore::Xmlio::extractTinyint(
			xmlXPathContext* docctx
			, const string& xpath
			, tinyint& val
		) {
	xmlXPathObject* obj;
	xmlChar* nodebuf;

	int _val = 0;

	obj = xmlXPathEvalExpression(BAD_CAST xpath.c_str(), docctx);
	nodebuf = xmlNodeGetContent(obj->nodesetval->nodeTab[0]);

	if (nodebuf) {
		_val = atoi((char*) nodebuf);
		xmlFree(nodebuf);
	};

	if ((_val < -127) || (_val > 127)) val = 0; else val = _val;

	xmlXPathFreeObject(obj);
};

void Sbecore::Xmlio::extractUtinyint(
			xmlXPathContext* docctx
			, const string& xpath
			, utinyint& val
		) {
	xmlXPathObject* obj;
	xmlChar* nodebuf;

	int _val = 0;

	obj = xmlXPathEvalExpression(BAD_CAST xpath.c_str(), docctx);
	nodebuf = xmlNodeGetContent(obj->nodesetval->nodeTab[0]);

	if (nodebuf) {
		_val = atoi((char*) nodebuf);
		xmlFree(nodebuf);
	};

	if ((_val < 0) || (_val > 255)) val = 0; else val = _val;

	xmlXPathFreeObject(obj);
};

void Sbecore::Xmlio::extractSmallint(
			xmlXPathContext* docctx
			, const string& xpath
			, smallint& val
		) {
	xmlXPathObject* obj;
	xmlChar* nodebuf;

	int _val = 0;

	obj = xmlXPathEvalExpression(BAD_CAST xpath.c_str(), docctx);
	nodebuf = xmlNodeGetContent(obj->nodesetval->nodeTab[0]);

	if (nodebuf) {
		_val = atoi((char*) nodebuf);
		xmlFree(nodebuf);
	};

	if ((_val < -16383) || (_val > 16383)) val = 0; else val = _val;

	xmlXPathFreeObject(obj);
};

void Sbecore::Xmlio::extractUsmallint(
			xmlXPathContext* docctx
			, const string& xpath
			, usmallint& val
		) {
	xmlXPathObject* obj;
	xmlChar* nodebuf;

	int _val = 0;

	obj = xmlXPathEvalExpression(BAD_CAST xpath.c_str(), docctx);
	nodebuf = xmlNodeGetContent(obj->nodesetval->nodeTab[0]);

	if (nodebuf) {
		_val = atoi((char*) nodebuf);
		xmlFree(nodebuf);
	};

	if ((_val < 0) || (_val > 32767)) val = 0; else val = _val;

	xmlXPathFreeObject(obj);
};

void Sbecore::Xmlio::extractInt(
			xmlXPathContext* docctx
			, const string& xpath
			, int& val
		) {
	xmlXPathObject* obj;
	xmlChar* nodebuf;

	obj = xmlXPathEvalExpression(BAD_CAST xpath.c_str(), docctx);
	nodebuf = xmlNodeGetContent(obj->nodesetval->nodeTab[0]);

	if (nodebuf) {
		val = atoi((char*) nodebuf);
		xmlFree(nodebuf);

	} else val = 0;

	xmlXPathFreeObject(obj);
};

void Sbecore::Xmlio::extractUint(
			xmlXPathContext* docctx
			, const string& xpath
			, uint& val
		) {
	xmlXPathObject* obj;
	xmlChar* nodebuf;

	int _val = 0;

	obj = xmlXPathEvalExpression(BAD_CAST xpath.c_str(), docctx);
	nodebuf = xmlNodeGetContent(obj->nodesetval->nodeTab[0]);

	if (nodebuf) {
		_val = atoi((char*) nodebuf);
		xmlFree(nodebuf);
	};

	if (_val < 0) val = 0; else val = _val;

	xmlXPathFreeObject(obj);
};

void Sbecore::Xmlio::extractBigint(
			xmlXPathContext* docctx
			, const string& xpath
			, bigint& val
		) {
	xmlXPathObject* obj;
	xmlChar* nodebuf;

	obj = xmlXPathEvalExpression(BAD_CAST xpath.c_str(), docctx);
	nodebuf = xmlNodeGetContent(obj->nodesetval->nodeTab[0]);

	if (nodebuf) {
		val = atoll((char*) nodebuf);
		xmlFree(nodebuf);

	} else val = 0;

	xmlXPathFreeObject(obj);
};

void Sbecore::Xmlio::extractUbigint(
			xmlXPathContext* docctx
			, const string& xpath
			, ubigint& val
		) {
	xmlXPathObject* obj;
	xmlChar* nodebuf;

	long long int _val = 0;

	obj = xmlXPathEvalExpression(BAD_CAST xpath.c_str(), docctx);
	nodebuf = xmlNodeGetContent(obj->nodesetval->nodeTab[0]);

	if (nodebuf) {
		_val = atoll((char*) nodebuf);
		xmlFree(nodebuf);
	};

	if (_val < 0) val = 0; else val = _val;

	xmlXPathFreeObject(obj);
};

void Sbecore::Xmlio::extractFloat(
			xmlXPathContext* docctx
			, const string& xpath
			, float& val
		) {
	xmlXPathObject* obj;
	xmlChar* nodebuf;

	obj = xmlXPathEvalExpression(BAD_CAST xpath.c_str(), docctx);
	nodebuf = xmlNodeGetContent(obj->nodesetval->nodeTab[0]);

	if (nodebuf) {
		val = (float) atof((char*) nodebuf);
		xmlFree(nodebuf);

	} else val = 0.0;

	xmlXPathFreeObject(obj);
};

void Sbecore::Xmlio::extractDouble(
			xmlXPathContext* docctx
			, const string& xpath
			, double& val
		) {
	xmlXPathObject* obj;
	xmlChar* nodebuf;

	obj = xmlXPathEvalExpression(BAD_CAST xpath.c_str(), docctx);
	nodebuf = xmlNodeGetContent(obj->nodesetval->nodeTab[0]);

	if (nodebuf) {
		val = atof((char*) nodebuf);
		xmlFree(nodebuf);

	} else val = 0.0;

	xmlXPathFreeObject(obj);
};

void Sbecore::Xmlio::extractString(
			xmlXPathContext* docctx
			, const string& xpath
			, string& val
		) {
	xmlXPathObject* obj;
	xmlChar* nodebuf;

	obj = xmlXPathEvalExpression(BAD_CAST xpath.c_str(), docctx);
	nodebuf = xmlNodeGetContent(obj->nodesetval->nodeTab[0]);

	if (nodebuf) {
		val.assign((char*) nodebuf);
		xmlFree(nodebuf);

	} else val = "";

	xmlXPathFreeObject(obj);
};

void Sbecore::Xmlio::extractBoolvec(
			xmlXPathContext* docctx
			, const string& xpath
			, vector<bool>& vec
		) {
	xmlXPathObject* obj;
	xmlChar* nodebuf;

	string str;

	vec.resize(0);

	obj = xmlXPathEvalExpression(BAD_CAST xpath.c_str(), docctx);
	nodebuf = xmlNodeGetContent(obj->nodesetval->nodeTab[0]);

	if (nodebuf) {
		str.assign((char*) nodebuf);
		xmlFree(nodebuf);

		vec.resize(str.length());
		for (unsigned int i = 0; i < vec.size(); i++) vec[i] = (str[i] == '1');
	};

	xmlXPathFreeObject(obj);
};

void Sbecore::Xmlio::extractUtinyintvec(
			xmlXPathContext* docctx
			, const string& xpath
			, vector<utinyint>& vec
		) {
	xmlXPathObject* obj;
	xmlChar* nodebuf;

	vec.resize(0);

	obj = xmlXPathEvalExpression(BAD_CAST xpath.c_str(), docctx);
	nodebuf = xmlNodeGetContent(obj->nodesetval->nodeTab[0]);

	if (nodebuf) {
		base64ToUtinyintvec((const char*) nodebuf, strlen((const char*) nodebuf), vec);
		xmlFree(nodebuf);
	};

	xmlXPathFreeObject(obj);
};

void Sbecore::Xmlio::extractUsmallintvec(
			xmlXPathContext* docctx
			, const string& xpath
			, vector<usmallint>& vec
		) {
	xmlXPathObject* obj;
	xmlChar* nodebuf;

	vec.resize(0);

	obj = xmlXPathEvalExpression(BAD_CAST xpath.c_str(), docctx);
	nodebuf = xmlNodeGetContent(obj->nodesetval->nodeTab[0]);

	if (nodebuf) {
		base64ToVec<usmallint>((const char*) nodebuf, strlen((const char*) nodebuf), vec);
		xmlFree(nodebuf);
	};

	xmlXPathFreeObject(obj);
};

void Sbecore::Xmlio::extractIntvec(
			xmlXPathContext* docctx
			, const string& xpath
			, vector<int>& vec
		) {
	xmlXPathObject* obj;
	xmlChar* nodebuf;

	vec.resize(0);

	obj = xmlXPathEvalExpression(BAD_CAST xpath.c_str(), docctx);
	nodebuf = xmlNodeGetContent(obj->nodesetval->nodeTab[0]);

	if (nodebuf) {
		base64ToVec<int>((const char*) nodebuf, strlen((const char*) nodebuf), vec);
		xmlFree(nodebuf);
	};

	xmlXPathFreeObject(obj);
};

void Sbecore::Xmlio::extractUintvec(
			xmlXPathContext* docctx
			, const string& xpath
			, vector<uint>& vec
		) {
	xmlXPathObject* obj;
	xmlChar* nodebuf;

	vec.resize(0);

	obj = xmlXPathEvalExpression(BAD_CAST xpath.c_str(), docctx);
	nodebuf = xmlNodeGetContent(obj->nodesetval->nodeTab[0]);

	if (nodebuf) {
		base64ToVec<uint>((const char*) nodebuf, strlen((const char*) nodebuf), vec);
		xmlFree(nodebuf);
	};

	xmlXPathFreeObject(obj);
};

void Sbecore::Xmlio::extractUbigintvec(
			xmlXPathContext* docctx
			, const string& xpath
			, vector<ubigint>& vec
		) {
	xmlXPathObject* obj;
	xmlChar* nodebuf;

	vec.resize(0);

	obj = xmlXPathEvalExpression(BAD_CAST xpath.c_str(), docctx);
	nodebuf = xmlNodeGetContent(obj->nodesetval->nodeTab[0]);

	if (nodebuf) {
		base64ToVec<ubigint>((const char*) nodebuf, strlen((const char*) nodebuf), vec);
		xmlFree(nodebuf);
	};

	xmlXPathFreeObject(obj);
};

void Sbecore::Xmlio::extractFloatvec(
			xmlXPathContext* docctx
			, const string& xpath
			, vector<float>& vec
		) {
	xmlXPathObject* obj;
	xmlChar* nodebuf;

	vec.resize(0);

	obj = xmlXPathEvalExpression(BAD_CAST xpath.c_str(), docctx);
	nodebuf = xmlNodeGetContent(obj->nodesetval->nodeTab[0]);

	if (nodebuf) {
		base64ToVec<float>((const char*) nodebuf, strlen((const char*) nodebuf), vec);
		xmlFree(nodebuf);
	};

	xmlXPathFreeObject(obj);
};

void Sbecore::Xmlio::extractFloatmat(
			xmlXPathContext* docctx
			, const string& xpath
			, Floatmat& mat
		) {
	extractFloatvecUclc(docctx, xpath, "vec", "vec", mat.vec);
	extractUintUclc(docctx, xpath, "M", "M", mat.M);
	extractUintUclc(docctx, xpath, "N", "N", mat.N);

	if ((mat.M * mat.N) != mat.vec.size()) mat = Floatmat();
};

void Sbecore::Xmlio::extractDoublevec(
			xmlXPathContext* docctx
			, const string& xpath
			, vector<double>& vec
		) {
	xmlXPathObject* obj;
	xmlChar* nodebuf;

	vec.resize(0);

	obj = xmlXPathEvalExpression(BAD_CAST xpath.c_str(), docctx);
	nodebuf = xmlNodeGetContent(obj->nodesetval->nodeTab[0]);

	if (nodebuf) {
		base64ToVec<double>((const char*) nodebuf, strlen((const char*) nodebuf), vec);
		xmlFree(nodebuf);
	};

	xmlXPathFreeObject(obj);
};

void Sbecore::Xmlio::extractDoublemat(
			xmlXPathContext* docctx
			, const string& xpath
			, Doublemat& mat
		) {
	extractDoublevecUclc(docctx, xpath, "vec", "vec", mat.vec);
	extractUintUclc(docctx, xpath, "M", "M", mat.M);
	extractUintUclc(docctx, xpath, "N", "N", mat.N);

	if ((mat.M * mat.N) != mat.vec.size()) mat = Doublemat();
};

void Sbecore::Xmlio::extractStringvec(
			xmlXPathContext* docctx
			, const string& xpath
			, vector<string>& vec
		) {
	xmlXPathObject* obj;
	xmlChar* nodebuf;

	string s;

	obj = xmlXPathEvalExpression(BAD_CAST xpath.c_str(), docctx);
	nodebuf = xmlNodeGetContent(obj->nodesetval->nodeTab[0]);

	if (nodebuf) {
		s.assign((char*) nodebuf);
		xmlFree(nodebuf);
	};

	s = fromUTF8(s);
	StrMod::stringToVector(s, vec);

	xmlXPathFreeObject(obj);
};

bool Sbecore::Xmlio::extractBoolUclc(
			xmlXPathContext* docctx
			, const string& basexpath
			, const string& tag
			, const string& shorttag
			, bool& val
		) {
	string goodpath;

	if (checkAltXPaths(docctx, goodpath, basexpath, tag, shorttag, StrMod::uc(tag), StrMod::uc(shorttag), StrMod::lc(tag), StrMod::lc(shorttag))) {
		extractBool(docctx, goodpath, val);
		return true;
	};

	return false;
};

bool Sbecore::Xmlio::extractTinyintUclc(
			xmlXPathContext* docctx
			, const string& basexpath
			, const string& tag
			, const string& shorttag
			, tinyint& val
		) {
	string goodpath;

	if (checkAltXPaths(docctx, goodpath, basexpath, tag, shorttag, StrMod::uc(tag), StrMod::uc(shorttag), StrMod::lc(tag), StrMod::lc(shorttag))) {
		extractTinyint(docctx, goodpath, val);
		return true;
	};

	return false;
};

bool Sbecore::Xmlio::extractUtinyintUclc(
			xmlXPathContext* docctx
			, const string& basexpath
			, const string& tag
			, const string& shorttag
			, utinyint& val
		) {
	string goodpath;

	if (checkAltXPaths(docctx, goodpath, basexpath, tag, shorttag, StrMod::uc(tag), StrMod::uc(shorttag), StrMod::lc(tag), StrMod::lc(shorttag))) {
		extractUtinyint(docctx, goodpath, val);
		return true;
	};

	return false;
};

bool Sbecore::Xmlio::extractSmallintUclc(
			xmlXPathContext* docctx
			, const string& basexpath
			, const string& tag
			, const string& shorttag
			, smallint& val
		) {
	string goodpath;

	if (checkAltXPaths(docctx, goodpath, basexpath, tag, shorttag, StrMod::uc(tag), StrMod::uc(shorttag), StrMod::lc(tag), StrMod::lc(shorttag))) {
		extractSmallint(docctx, goodpath, val);
		return true;
	};

	return false;
};

bool Sbecore::Xmlio::extractUsmallintUclc(
			xmlXPathContext* docctx
			, const string& basexpath
			, const string& tag
			, const string& shorttag
			, usmallint& val
		) {
	string goodpath;

	if (checkAltXPaths(docctx, goodpath, basexpath, tag, shorttag, StrMod::uc(tag), StrMod::uc(shorttag), StrMod::lc(tag), StrMod::lc(shorttag))) {
		extractUsmallint(docctx, goodpath, val);
		return true;
	};

	return false;
};

bool Sbecore::Xmlio::extractIntUclc(
			xmlXPathContext* docctx
			, const string& basexpath
			, const string& tag
			, const string& shorttag
			, int& val
		) {
	string goodpath;

	if (checkAltXPaths(docctx, goodpath, basexpath, tag, shorttag, StrMod::uc(tag), StrMod::uc(shorttag), StrMod::lc(tag), StrMod::lc(shorttag))) {
		extractInt(docctx, goodpath, val);
		return true;
	};

	return false;
};

bool Sbecore::Xmlio::extractUintUclc(
			xmlXPathContext* docctx
			, const string& basexpath
			, const string& tag
			, const string& shorttag
			, uint& val
		) {
	string goodpath;

	if (checkAltXPaths(docctx, goodpath, basexpath, tag, shorttag, StrMod::uc(tag), StrMod::uc(shorttag), StrMod::lc(tag), StrMod::lc(shorttag))) {
		extractUint(docctx, goodpath, val);
		return true;
	};

	return false;
};

bool Sbecore::Xmlio::extractBigintUclc(
			xmlXPathContext* docctx
			, const string& basexpath
			, const string& tag
			, const string& shorttag
			, bigint& val
		) {
	string goodpath;

	if (checkAltXPaths(docctx, goodpath, basexpath, tag, shorttag, StrMod::uc(tag), StrMod::uc(shorttag), StrMod::lc(tag), StrMod::lc(shorttag))) {
		extractBigint(docctx, goodpath, val);
		return true;
	};

	return false;
};

bool Sbecore::Xmlio::extractUbigintUclc(
			xmlXPathContext* docctx
			, const string& basexpath
			, const string& tag
			, const string& shorttag
			, ubigint& val
		) {
	string goodpath;

	if (checkAltXPaths(docctx, goodpath, basexpath, tag, shorttag, StrMod::uc(tag), StrMod::uc(shorttag), StrMod::lc(tag), StrMod::lc(shorttag))) {
		extractUbigint(docctx, goodpath, val);
		return true;
	};

	return false;
};

bool Sbecore::Xmlio::extractFloatUclc(
			xmlXPathContext* docctx
			, const string& basexpath
			, const string& tag
			, const string& shorttag
			, float& val
		) {
	string goodpath;

	if (checkAltXPaths(docctx, goodpath, basexpath, tag, shorttag, StrMod::uc(tag), StrMod::uc(shorttag), StrMod::lc(tag), StrMod::lc(shorttag))) {
		extractFloat(docctx, goodpath, val);
		return true;
	};

	return false;
};

bool Sbecore::Xmlio::extractDoubleUclc(
			xmlXPathContext* docctx
			, const string& basexpath
			, const string& tag
			, const string& shorttag
			, double& val
		) {
	string goodpath;

	if (checkAltXPaths(docctx, goodpath, basexpath, tag, shorttag, StrMod::uc(tag), StrMod::uc(shorttag), StrMod::lc(tag), StrMod::lc(shorttag))) {
		extractDouble(docctx, goodpath, val);
		return true;
	};

	return false;
};

bool Sbecore::Xmlio::extractStringUclc(
			xmlXPathContext* docctx
			, const string& basexpath
			, const string& tag
			, const string& shorttag
			, string& val
		) {
	string goodpath;

	if (checkAltXPaths(docctx, goodpath, basexpath, tag, shorttag, StrMod::uc(tag), StrMod::uc(shorttag), StrMod::lc(tag), StrMod::lc(shorttag))) {
		extractString(docctx, goodpath, val);
		return true;
	};

	return false;
};

bool Sbecore::Xmlio::extractBoolvecUclc(
			xmlXPathContext* docctx
			, const string& basexpath
			, const string& tag
			, const string& shorttag
			, vector<bool>& vec
		) {
	string goodpath;

	if (checkAltXPaths(docctx, goodpath, basexpath, tag, shorttag, StrMod::uc(tag), StrMod::uc(shorttag), StrMod::lc(tag), StrMod::lc(shorttag))) {
		extractBoolvec(docctx, goodpath, vec);
		return true;
	};

	return false;
};

bool Sbecore::Xmlio::extractUtinyintvecUclc(
			xmlXPathContext* docctx
			, const string& basexpath
			, const string& tag
			, const string& shorttag
			, vector<utinyint>& vec
		) {
	string goodpath;

	if (checkAltXPaths(docctx, goodpath, basexpath, tag, shorttag, StrMod::uc(tag), StrMod::uc(shorttag), StrMod::lc(tag), StrMod::lc(shorttag))) {
		extractUtinyintvec(docctx, goodpath, vec);
		return true;
	};

	return false;
};

bool Sbecore::Xmlio::extractUsmallintvecUclc(
			xmlXPathContext* docctx
			, const string& basexpath
			, const string& tag
			, const string& shorttag
			, vector<usmallint>& vec
		) {
	string goodpath;

	if (checkAltXPaths(docctx, goodpath, basexpath, tag, shorttag, StrMod::uc(tag), StrMod::uc(shorttag), StrMod::lc(tag), StrMod::lc(shorttag))) {
		extractUsmallintvec(docctx, goodpath, vec);
		return true;
	};

	return false;
};

bool Sbecore::Xmlio::extractIntvecUclc(
			xmlXPathContext* docctx
			, const string& basexpath
			, const string& tag
			, const string& shorttag
			, vector<int>& vec
		) {
	string goodpath;

	if (checkAltXPaths(docctx, goodpath, basexpath, tag, shorttag, StrMod::uc(tag), StrMod::uc(shorttag), StrMod::lc(tag), StrMod::lc(shorttag))) {
		extractIntvec(docctx, goodpath, vec);
		return true;
	};

	return false;
};

bool Sbecore::Xmlio::extractUintvecUclc(
			xmlXPathContext* docctx
			, const string& basexpath
			, const string& tag
			, const string& shorttag
			, vector<uint>& vec
		) {
	string goodpath;

	if (checkAltXPaths(docctx, goodpath, basexpath, tag, shorttag, StrMod::uc(tag), StrMod::uc(shorttag), StrMod::lc(tag), StrMod::lc(shorttag))) {
		extractUintvec(docctx, goodpath, vec);
		return true;
	};

	return false;
};

bool Sbecore::Xmlio::extractUbigintvecUclc(
			xmlXPathContext* docctx
			, const string& basexpath
			, const string& tag
			, const string& shorttag
			, vector<ubigint>& vec
		) {
	string goodpath;

	if (checkAltXPaths(docctx, goodpath, basexpath, tag, shorttag, StrMod::uc(tag), StrMod::uc(shorttag), StrMod::lc(tag), StrMod::lc(shorttag))) {
		extractUbigintvec(docctx, goodpath, vec);
		return true;
	};

	return false;
};

bool Sbecore::Xmlio::extractFloatvecUclc(
			xmlXPathContext* docctx
			, const string& basexpath
			, const string& tag
			, const string& shorttag
			, vector<float>& vec
		) {
	string goodpath;

	if (checkAltXPaths(docctx, goodpath, basexpath, tag, shorttag, StrMod::uc(tag), StrMod::uc(shorttag), StrMod::lc(tag), StrMod::lc(shorttag))) {
		extractFloatvec(docctx, goodpath, vec);
		return true;
	};

	return false;
};

bool Sbecore::Xmlio::extractFloatmatUclc(
			xmlXPathContext* docctx
			, const string& basexpath
			, const string& tag
			, const string& shorttag
			, Floatmat& mat
		) {
	string goodpath;

	if (checkAltXPaths(docctx, goodpath, basexpath, tag, shorttag, StrMod::uc(tag), StrMod::uc(shorttag), StrMod::lc(tag), StrMod::lc(shorttag))) {
		extractFloatmat(docctx, goodpath, mat);
		return true;
	};

	return false;
};

bool Sbecore::Xmlio::extractDoublevecUclc(
			xmlXPathContext* docctx
			, const string& basexpath
			, const string& tag
			, const string& shorttag
			, vector<double>& vec
		) {
	string goodpath;

	if (checkAltXPaths(docctx, goodpath, basexpath, tag, shorttag, StrMod::uc(tag), StrMod::uc(shorttag), StrMod::lc(tag), StrMod::lc(shorttag))) {
		extractDoublevec(docctx, goodpath, vec);
		return true;
	};

	return false;
};

bool Sbecore::Xmlio::extractDoublematUclc(
			xmlXPathContext* docctx
			, const string& basexpath
			, const string& tag
			, const string& shorttag
			, Doublemat& mat
		) {
	string goodpath;

	if (checkAltXPaths(docctx, goodpath, basexpath, tag, shorttag, StrMod::uc(tag), StrMod::uc(shorttag), StrMod::lc(tag), StrMod::lc(shorttag))) {
		extractDoublemat(docctx, goodpath, mat);
		return true;
	};

	return false;
};

bool Sbecore::Xmlio::extractStringvecUclc(
			xmlXPathContext* docctx
			, const string& basexpath
			, const string& tag
			, const string& shorttag
			, vector<string>& vec
		) {
	string goodpath;

	if (checkAltXPaths(docctx, goodpath, basexpath, tag, shorttag, StrMod::uc(tag), StrMod::uc(shorttag), StrMod::lc(tag), StrMod::lc(shorttag))) {
		extractStringvec(docctx, goodpath, vec);
		return true;
	};

	return false;
};

bool Sbecore::Xmlio::extractBoolAttrUclc(
			xmlXPathContext* docctx
			, const string& basexpath
			, const string& tag
			, const string& shorttag
			, const string& attr
			, const string& attrval
			, bool& val
		) {
	string goodpath;

	if (checkAttrUclcXpaths(docctx, basexpath, goodpath, tag, shorttag, attr, attrval)) {
		extractBool(docctx, goodpath, val);
		return true;
	};

	return false;
};

bool Sbecore::Xmlio::extractTinyintAttrUclc(
			xmlXPathContext* docctx
			, const string& basexpath
			, const string& tag
			, const string& shorttag
			, const string& attr
			, const string& attrval
			, tinyint& val
		) {
	string goodpath;

	if (checkAttrUclcXpaths(docctx, basexpath, goodpath, tag, shorttag, attr, attrval)) {
		extractTinyint(docctx, goodpath, val);
		return true;
	};

	return false;
};

bool Sbecore::Xmlio::extractUtinyintAttrUclc(
			xmlXPathContext* docctx
			, const string& basexpath
			, const string& tag
			, const string& shorttag
			, const string& attr
			, const string& attrval
			, utinyint& val
		) {
	string goodpath;

	if (checkAttrUclcXpaths(docctx, basexpath, goodpath, tag, shorttag, attr, attrval)) {
		extractUtinyint(docctx, goodpath, val);
		return true;
	};

	return false;
};

bool Sbecore::Xmlio::extractSmallintAttrUclc(
			xmlXPathContext* docctx
			, const string& basexpath
			, const string& tag
			, const string& shorttag
			, const string& attr
			, const string& attrval
			, smallint& val
		) {
	string goodpath;

	if (checkAttrUclcXpaths(docctx, basexpath, goodpath, tag, shorttag, attr, attrval)) {
		extractSmallint(docctx, goodpath, val);
		return true;
	};

	return false;
};

bool Sbecore::Xmlio::extractUsmallintAttrUclc(
			xmlXPathContext* docctx
			, const string& basexpath
			, const string& tag
			, const string& shorttag
			, const string& attr
			, const string& attrval
			, usmallint& val
		) {
	string goodpath;

	if (checkAttrUclcXpaths(docctx, basexpath, goodpath, tag, shorttag, attr, attrval)) {
		extractUsmallint(docctx, goodpath, val);
		return true;
	};

	return false;
};

bool Sbecore::Xmlio::extractIntAttrUclc(
			xmlXPathContext* docctx
			, const string& basexpath
			, const string& tag
			, const string& shorttag
			, const string& attr
			, const string& attrval
			, int& val
		) {
	string goodpath;

	if (checkAttrUclcXpaths(docctx, basexpath, goodpath, tag, shorttag, attr, attrval)) {
		extractInt(docctx, goodpath, val);
		return true;
	};

	return false;
};

bool Sbecore::Xmlio::extractUintAttrUclc(
			xmlXPathContext* docctx
			, const string& basexpath
			, const string& tag
			, const string& shorttag
			, const string& attr
			, const string& attrval
			, uint& val
		) {
	string goodpath;

	if (checkAttrUclcXpaths(docctx, basexpath, goodpath, tag, shorttag, attr, attrval)) {
		extractUint(docctx, goodpath, val);
		return true;
	};

	return false;
};

bool Sbecore::Xmlio::extractBigintAttrUclc(
			xmlXPathContext* docctx
			, const string& basexpath
			, const string& tag
			, const string& shorttag
			, const string& attr
			, const string& attrval
			, bigint& val
		) {
	string goodpath;

	if (checkAttrUclcXpaths(docctx, basexpath, goodpath, tag, shorttag, attr, attrval)) {
		extractBigint(docctx, goodpath, val);
		return true;
	};

	return false;
};

bool Sbecore::Xmlio::extractUbigintAttrUclc(
			xmlXPathContext* docctx
			, const string& basexpath
			, const string& tag
			, const string& shorttag
			, const string& attr
			, const string& attrval
			, ubigint& val
		) {
	string goodpath;

	if (checkAttrUclcXpaths(docctx, basexpath, goodpath, tag, shorttag, attr, attrval)) {
		extractUbigint(docctx, goodpath, val);
		return true;
	};

	return false;
};

bool Sbecore::Xmlio::extractFloatAttrUclc(
			xmlXPathContext* docctx
			, const string& basexpath
			, const string& tag
			, const string& shorttag
			, const string& attr
			, const string& attrval
			, float& val
		) {
	string goodpath;

	if (checkAttrUclcXpaths(docctx, basexpath, goodpath, tag, shorttag, attr, attrval)) {
		extractFloat(docctx, goodpath, val);
		return true;
	};

	return false;
};

bool Sbecore::Xmlio::extractDoubleAttrUclc(
			xmlXPathContext* docctx
			, const string& basexpath
			, const string& tag
			, const string& shorttag
			, const string& attr
			, const string& attrval
			, double& val
		) {
	string goodpath;

	if (checkAttrUclcXpaths(docctx, basexpath, goodpath, tag, shorttag, attr, attrval)) {
		extractDouble(docctx, goodpath, val);
		return true;
	};

	return false;
};

bool Sbecore::Xmlio::extractStringAttrUclc(
			xmlXPathContext* docctx
			, const string& basexpath
			, const string& tag
			, const string& shorttag
			, const string& attr
			, const string& attrval
			, string& val
		) {
	string goodpath;

	if (checkAttrUclcXpaths(docctx, basexpath, goodpath, tag, shorttag, attr, attrval)) {
		extractString(docctx, goodpath, val);
		return true;
	};

	return false;
};

bool Sbecore::Xmlio::extractBoolvecAttrUclc(
			xmlXPathContext* docctx
			, const string& basexpath
			, const string& tag
			, const string& shorttag
			, const string& attr
			, const string& attrval
			, vector<bool>& vec
		) {
	string goodpath;

	if (checkAttrUclcXpaths(docctx, basexpath, goodpath, tag, shorttag, attr, attrval)) {
		extractBoolvec(docctx, goodpath, vec);
		return true;
	};

	return false;
};

bool Sbecore::Xmlio::extractUtinyintvecAttrUclc(
			xmlXPathContext* docctx
			, const string& basexpath
			, const string& tag
			, const string& shorttag
			, const string& attr
			, const string& attrval
			, vector<utinyint>& vec
		) {
	string goodpath;

	if (checkAttrUclcXpaths(docctx, basexpath, goodpath, tag, shorttag, attr, attrval)) {
		extractUtinyintvec(docctx, goodpath, vec);
		return true;
	};

	return false;
};

bool Sbecore::Xmlio::extractUsmallintvecAttrUclc(
			xmlXPathContext* docctx
			, const string& basexpath
			, const string& tag
			, const string& shorttag
			, const string& attr
			, const string& attrval
			, vector<usmallint>& vec
		) {
	string goodpath;

	if (checkAttrUclcXpaths(docctx, basexpath, goodpath, tag, shorttag, attr, attrval)) {
		extractUsmallintvec(docctx, goodpath, vec);
		return true;
	};

	return false;
};

bool Sbecore::Xmlio::extractIntvecAttrUclc(
			xmlXPathContext* docctx
			, const string& basexpath
			, const string& tag
			, const string& shorttag
			, const string& attr
			, const string& attrval
			, vector<int>& vec
		) {
	string goodpath;

	if (checkAttrUclcXpaths(docctx, basexpath, goodpath, tag, shorttag, attr, attrval)) {
		extractIntvec(docctx, goodpath, vec);
		return true;
	};

	return false;
};

bool Sbecore::Xmlio::extractUintvecAttrUclc(
			xmlXPathContext* docctx
			, const string& basexpath
			, const string& tag
			, const string& shorttag
			, const string& attr
			, const string& attrval
			, vector<uint>& vec
		) {
	string goodpath;

	if (checkAttrUclcXpaths(docctx, basexpath, goodpath, tag, shorttag, attr, attrval)) {
		extractUintvec(docctx, goodpath, vec);
		return true;
	};

	return false;
};

bool Sbecore::Xmlio::extractUbigintvecAttrUclc(
			xmlXPathContext* docctx
			, const string& basexpath
			, const string& tag
			, const string& shorttag
			, const string& attr
			, const string& attrval
			, vector<ubigint>& vec
		) {
	string goodpath;

	if (checkAttrUclcXpaths(docctx, basexpath, goodpath, tag, shorttag, attr, attrval)) {
		extractUbigintvec(docctx, goodpath, vec);
		return true;
	};

	return false;
};

bool Sbecore::Xmlio::extractFloatvecAttrUclc(
			xmlXPathContext* docctx
			, const string& basexpath
			, const string& tag
			, const string& shorttag
			, const string& attr
			, const string& attrval
			, vector<float>& vec
		) {
	string goodpath;

	if (checkAttrUclcXpaths(docctx, basexpath, goodpath, tag, shorttag, attr, attrval)) {
		extractFloatvec(docctx, goodpath, vec);
		return true;
	};

	return false;
};

bool Sbecore::Xmlio::extractFloatmatAttrUclc(
			xmlXPathContext* docctx
			, const string& basexpath
			, const string& tag
			, const string& shorttag
			, const string& attr
			, const string& attrval
			, Floatmat& mat
		) {
	string goodpath;

	if (checkAttrUclcXpaths(docctx, basexpath, goodpath, tag, shorttag, attr, attrval)) {
		extractFloatmat(docctx, goodpath, mat);
		return true;
	};

	return false;
};

bool Sbecore::Xmlio::extractDoublevecAttrUclc(
			xmlXPathContext* docctx
			, const string& basexpath
			, const string& tag
			, const string& shorttag
			, const string& attr
			, const string& attrval
			, vector<double>& vec
		) {
	string goodpath;

	if (checkAttrUclcXpaths(docctx, basexpath, goodpath, tag, shorttag, attr, attrval)) {
		extractDoublevec(docctx, goodpath, vec);
		return true;
	};

	return false;
};

bool Sbecore::Xmlio::extractDoublematAttrUclc(
			xmlXPathContext* docctx
			, const string& basexpath
			, const string& tag
			, const string& shorttag
			, const string& attr
			, const string& attrval
			, Doublemat& mat
		) {
	string goodpath;

	if (checkAttrUclcXpaths(docctx, basexpath, goodpath, tag, shorttag, attr, attrval)) {
		extractDoublemat(docctx, goodpath, mat);
		return true;
	};

	return false;
};

bool Sbecore::Xmlio::extractStringvecAttrUclc(
			xmlXPathContext* docctx
			, const string& basexpath
			, const string& tag
			, const string& shorttag
			, const string& attr
			, const string& attrval
			, vector<string>& vec
		) {
	string goodpath;

	if (checkAttrUclcXpaths(docctx, basexpath, goodpath, tag, shorttag, attr, attrval)) {
		extractStringvec(docctx, goodpath, vec);
		return true;
	};

	return false;
};

void Sbecore::Xmlio::extractList(
			xmlXPathContext* docctx
			, const string& basexpath
			, const string& tag
			, const string& shorttag
			, const string& attr
			, vector<unsigned int>& ics
			, vector<bool>& shorttags
		) {
	xmlXPathObject* obj;
	xmlNode* node;
	xmlChar* nodebuf;

	unsigned int ix;

	string s;

	ics.resize(0);
	shorttags.resize(0);

	for (unsigned int i = 0; i < 2; i++) {
		if (i == 0) s = basexpath + "/" + tag + "/@" + attr;
		else s = basexpath + "/" + shorttag + "/@" + attr;

		obj = xmlXPathEvalExpression(BAD_CAST s.c_str(), docctx);

		if (obj) {
			if (obj->nodesetval) {
				// collect indices
				for (int j = 0; j < obj->nodesetval->nodeNr; j++) {
					node = obj->nodesetval->nodeTab[j];

					if (node->type == XML_ATTRIBUTE_NODE) {
						nodebuf = xmlNodeGetContent(node);

						if (nodebuf) {
							ics.push_back(atol((char*) nodebuf));
							shorttags.push_back(i == 1);

							xmlFree(nodebuf);
						};
					};
				};
			};

			xmlXPathFreeObject(obj);
		};
	};

	// order by indices ascending
	for (unsigned int i = 0; i < ics.size(); i++) {
		for (unsigned int j = i + 1; j < ics.size(); j++) {
			if (ics[j] < ics[i]) {
				ix = ics[i];
				ics[i] = ics[j];
				ics[j] = ix;

				if (shorttags[i] != shorttags[j]) {
					shorttags[i] = !shorttags[i];
					shorttags[j] = !shorttags[j];
				};
			};
		};
	};
};

void Sbecore::Xmlio::writeBase64(
			xmlTextWriter* wr
			, const char* _buf
			, const unsigned int len
			, const unsigned int varlen
		) {
	if (!bigendian()) {
		unsigned char* buf = new unsigned char[len*varlen];

		memcpy((void*) buf, (void*) _buf, len*varlen);
		invertBuffer(buf, len, varlen);
		xmlTextWriterWriteBase64(wr, (char*) buf, 0, len*varlen);

		delete[] buf;

	} else {
		xmlTextWriterWriteBase64(wr, (char*) _buf, 0, len*varlen);
	};
};

void Sbecore::Xmlio::writeBool(
			xmlTextWriter* wr
			, const string& tag
			, bool val
		) {
	xmlTextWriterWriteElement(wr, BAD_CAST tag.c_str(), BAD_CAST StrMod::boolToString(val).c_str());
};

void Sbecore::Xmlio::writeTinyint(
			xmlTextWriter* wr
			, const string& tag
			, tinyint val
		) {
	xmlTextWriterWriteElement(wr, BAD_CAST tag.c_str(), BAD_CAST to_string(val).c_str());
};

void Sbecore::Xmlio::writeUtinyint(
			xmlTextWriter* wr
			, const string& tag
			, utinyint val
		) {
	xmlTextWriterWriteElement(wr, BAD_CAST tag.c_str(), BAD_CAST to_string(val).c_str());
};

void Sbecore::Xmlio::writeSmallint(
			xmlTextWriter* wr
			, const string& tag
			, smallint val
		) {
	xmlTextWriterWriteElement(wr, BAD_CAST tag.c_str(), BAD_CAST to_string(val).c_str());
};

void Sbecore::Xmlio::writeUsmallint(
			xmlTextWriter* wr
			, const string& tag
			, usmallint val
		) {
	xmlTextWriterWriteElement(wr, BAD_CAST tag.c_str(), BAD_CAST to_string(val).c_str());
};

void Sbecore::Xmlio::writeInt(
			xmlTextWriter* wr
			, const string& tag
			, int val
		) {
	xmlTextWriterWriteElement(wr, BAD_CAST tag.c_str(), BAD_CAST to_string(val).c_str());
};

void Sbecore::Xmlio::writeUint(
			xmlTextWriter* wr
			, const string& tag
			, uint val
		) {
	xmlTextWriterWriteElement(wr, BAD_CAST tag.c_str(), BAD_CAST to_string(val).c_str());
};

void Sbecore::Xmlio::writeBigint(
			xmlTextWriter* wr
			, const string& tag
			, bigint val
		) {
	xmlTextWriterWriteElement(wr, BAD_CAST tag.c_str(), BAD_CAST to_string(val).c_str());
};

void Sbecore::Xmlio::writeUbigint(
			xmlTextWriter* wr
			, const string& tag
			, ubigint val
		) {
	xmlTextWriterWriteElement(wr, BAD_CAST tag.c_str(), BAD_CAST to_string(val).c_str());
};

void Sbecore::Xmlio::writeFloat(
			xmlTextWriter* wr
			, const string& tag
			, float val
		) {
	xmlTextWriterWriteElement(wr, BAD_CAST tag.c_str(), BAD_CAST to_string(val).c_str());
};

void Sbecore::Xmlio::writeDouble(
			xmlTextWriter* wr
			, const string& tag
			, double val
		) {
	xmlTextWriterWriteElement(wr, BAD_CAST tag.c_str(), BAD_CAST to_string(val).c_str());
};

void Sbecore::Xmlio::writeString(
			xmlTextWriter* wr
			, const string& tag
			, const string& val
		) {
	xmlTextWriterWriteElement(wr, BAD_CAST tag.c_str(), BAD_CAST toUTF8(val).c_str());
};

void Sbecore::Xmlio::writeBoolvec(
			xmlTextWriter* wr
			, const string& tag
			, const vector<bool>& vec
		) {
	string str(vec.size(), '0');

	for (unsigned int i = 0; i < vec.size(); i++) if (vec[i]) str[i] = '1';

	xmlTextWriterWriteElement(wr, BAD_CAST tag.c_str(), BAD_CAST toUTF8(str).c_str());
};

void Sbecore::Xmlio::writeUtinyintvec(
			xmlTextWriter* wr
			, const string& tag
			, const vector<utinyint>& vec
		) {
	xmlTextWriterStartElement(wr, BAD_CAST tag.c_str());
		writeBase64(wr, (const char*) vec.data(), vec.size(), 1);
	xmlTextWriterEndElement(wr);
};

void Sbecore::Xmlio::writeUsmallintvec(
			xmlTextWriter* wr
			, const string& tag
			, const vector<usmallint>& vec
		) {
	const unsigned int varlen = sizeof(usmallint); // should be 2

	xmlTextWriterStartElement(wr, BAD_CAST tag.c_str());
		writeBase64(wr, (const char*) vec.data(), vec.size(), varlen);
	xmlTextWriterEndElement(wr);
};

void Sbecore::Xmlio::writeIntvec(
			xmlTextWriter* wr
			, const string& tag
			, const vector<int>& vec
		) {
	const unsigned int varlen = sizeof(int); // should be 4

	xmlTextWriterStartElement(wr, BAD_CAST tag.c_str());
		writeBase64(wr, (const char*) vec.data(), vec.size(), varlen);
	xmlTextWriterEndElement(wr);
};

void Sbecore::Xmlio::writeUintvec(
			xmlTextWriter* wr
			, const string& tag
			, const vector<uint>& vec
		) {
	const unsigned int varlen = sizeof(uint); // should be 4

	xmlTextWriterStartElement(wr, BAD_CAST tag.c_str());
		writeBase64(wr, (const char*) vec.data(), vec.size(), varlen);
	xmlTextWriterEndElement(wr);
};

void Sbecore::Xmlio::writeUbigintvec(
			xmlTextWriter* wr
			, const string& tag
			, const vector<ubigint>& vec
		) {
	const unsigned int varlen = sizeof(ubigint); // should be 8

	xmlTextWriterStartElement(wr, BAD_CAST tag.c_str());
		writeBase64(wr, (const char*) vec.data(), vec.size(), varlen);
	xmlTextWriterEndElement(wr);
};

void Sbecore::Xmlio::writeFloatvec(
			xmlTextWriter* wr
			, const string& tag
			, const vector<float>& vec
		) {
	const unsigned int varlen = sizeof(float); // should be 4

	xmlTextWriterStartElement(wr, BAD_CAST tag.c_str());
		writeBase64(wr, (const char*) vec.data(), vec.size(), varlen);
	xmlTextWriterEndElement(wr);
};

void Sbecore::Xmlio::writeFloatmat(
			xmlTextWriter* wr
			, const string& tag
			, const Floatmat& mat
		) {
	/* example:
	<test>
		<M>3</M>
		<N>5</N>
		<vec>MTIzNA==</vec>
	</test> */

	const unsigned int varlen = sizeof(float); // should be 4

	xmlTextWriterStartElement(wr, BAD_CAST tag.c_str());
		xmlTextWriterWriteElement(wr, BAD_CAST "M", BAD_CAST to_string(mat.M).c_str());
		xmlTextWriterWriteElement(wr, BAD_CAST "N", BAD_CAST to_string(mat.N).c_str());

		xmlTextWriterStartElement(wr, BAD_CAST "vec");
			writeBase64(wr, (const char*) mat.vec.data(), mat.vec.size(), varlen);
		xmlTextWriterEndElement(wr);
	xmlTextWriterEndElement(wr);
};

void Sbecore::Xmlio::writeDoublevec(
			xmlTextWriter* wr
			, const string& tag
			, const vector<double>& vec
		) {
	const unsigned int varlen = sizeof(double); // should be 8

	xmlTextWriterStartElement(wr, BAD_CAST tag.c_str());
		writeBase64(wr, (const char*) vec.data(), vec.size(), varlen);
	xmlTextWriterEndElement(wr);
};

void Sbecore::Xmlio::writeDoublemat(
			xmlTextWriter* wr
			, const string& tag
			, const Doublemat& mat
		) {
	/* example:
	<test>
		<M>3</M>
		<N>5</N>
		<vec>MTIzNA==</vec>
	</test> */

	const unsigned int varlen = sizeof(double); // should be 8

	xmlTextWriterStartElement(wr, BAD_CAST tag.c_str());
		xmlTextWriterWriteElement(wr, BAD_CAST "M", BAD_CAST to_string(mat.M).c_str());
		xmlTextWriterWriteElement(wr, BAD_CAST "N", BAD_CAST to_string(mat.N).c_str());

		xmlTextWriterStartElement(wr, BAD_CAST "vec");
			writeBase64(wr, (const char*) mat.vec.data(), mat.vec.size(), varlen);
		xmlTextWriterEndElement(wr);
	xmlTextWriterEndElement(wr);
};

void Sbecore::Xmlio::writeStringvec(
			xmlTextWriter* wr
			, const string& tag
			, const vector<string>& vec
		) {
	string s;

	for (unsigned int i = 0; i < vec.size(); i++) {
		if (i != 0) s = s + ";";
		s = s + vec[i];
	};

	xmlTextWriterWriteElement(wr, BAD_CAST tag.c_str(), BAD_CAST toUTF8(s).c_str());
};

void Sbecore::Xmlio::writeBoolAttr(
			xmlTextWriter* wr
			, const string& tag
			, const string& attr
			, const string& attrval
			, bool val
		) {
	xmlTextWriterStartElement(wr, BAD_CAST tag.c_str());
		xmlTextWriterWriteAttribute(wr, BAD_CAST attr.c_str(), BAD_CAST attrval.c_str());
		xmlTextWriterWriteString(wr, BAD_CAST StrMod::boolToString(val).c_str());
	xmlTextWriterEndElement(wr);
};

void Sbecore::Xmlio::writeTinyintAttr(
			xmlTextWriter* wr
			, const string& tag
			, const string& attr
			, const string& attrval
			, tinyint val
		) {
	xmlTextWriterStartElement(wr, BAD_CAST tag.c_str());
		xmlTextWriterWriteAttribute(wr, BAD_CAST attr.c_str(), BAD_CAST attrval.c_str());
		xmlTextWriterWriteString(wr, BAD_CAST to_string(val).c_str());
	xmlTextWriterEndElement(wr);
};

void Sbecore::Xmlio::writeUtinyintAttr(
			xmlTextWriter* wr
			, const string& tag
			, const string& attr
			, const string& attrval
			, utinyint val
		) {
	xmlTextWriterStartElement(wr, BAD_CAST tag.c_str());
		xmlTextWriterWriteAttribute(wr, BAD_CAST attr.c_str(), BAD_CAST attrval.c_str());
		xmlTextWriterWriteString(wr, BAD_CAST to_string(val).c_str());
	xmlTextWriterEndElement(wr);
};

void Sbecore::Xmlio::writeSmallintAttr(
			xmlTextWriter* wr
			, const string& tag
			, const string& attr
			, const string& attrval
			, smallint val
		) {
	xmlTextWriterStartElement(wr, BAD_CAST tag.c_str());
		xmlTextWriterWriteAttribute(wr, BAD_CAST attr.c_str(), BAD_CAST attrval.c_str());
		xmlTextWriterWriteString(wr, BAD_CAST to_string(val).c_str());
	xmlTextWriterEndElement(wr);
};

void Sbecore::Xmlio::writeUsmallintAttr(
			xmlTextWriter* wr
			, const string& tag
			, const string& attr
			, const string& attrval
			, usmallint val
		) {
	xmlTextWriterStartElement(wr, BAD_CAST tag.c_str());
		xmlTextWriterWriteAttribute(wr, BAD_CAST attr.c_str(), BAD_CAST attrval.c_str());
		xmlTextWriterWriteString(wr, BAD_CAST to_string(val).c_str());
	xmlTextWriterEndElement(wr);
};

void Sbecore::Xmlio::writeIntAttr(
			xmlTextWriter* wr
			, const string& tag
			, const string& attr
			, const string& attrval
			, int val
		) {
	xmlTextWriterStartElement(wr, BAD_CAST tag.c_str());
		xmlTextWriterWriteAttribute(wr, BAD_CAST attr.c_str(), BAD_CAST attrval.c_str());
		xmlTextWriterWriteString(wr, BAD_CAST to_string(val).c_str());
	xmlTextWriterEndElement(wr);
};

void Sbecore::Xmlio::writeUintAttr(
			xmlTextWriter* wr
			, const string& tag
			, const string& attr
			, const string& attrval
			, uint val
		) {
	xmlTextWriterStartElement(wr, BAD_CAST tag.c_str());
		xmlTextWriterWriteAttribute(wr, BAD_CAST attr.c_str(), BAD_CAST attrval.c_str());
		xmlTextWriterWriteString(wr, BAD_CAST to_string(val).c_str());
	xmlTextWriterEndElement(wr);
};

void Sbecore::Xmlio::writeBigintAttr(
			xmlTextWriter* wr
			, const string& tag
			, const string& attr
			, const string& attrval
			, bigint val
		) {
	xmlTextWriterStartElement(wr, BAD_CAST tag.c_str());
		xmlTextWriterWriteAttribute(wr, BAD_CAST attr.c_str(), BAD_CAST attrval.c_str());
		xmlTextWriterWriteString(wr, BAD_CAST to_string(val).c_str());
	xmlTextWriterEndElement(wr);
};

void Sbecore::Xmlio::writeUbigintAttr(
			xmlTextWriter* wr
			, const string& tag
			, const string& attr
			, const string& attrval
			, ubigint val
		) {
	xmlTextWriterStartElement(wr, BAD_CAST tag.c_str());
		xmlTextWriterWriteAttribute(wr, BAD_CAST attr.c_str(), BAD_CAST attrval.c_str());
		xmlTextWriterWriteString(wr, BAD_CAST to_string(val).c_str());
	xmlTextWriterEndElement(wr);
};

void Sbecore::Xmlio::writeFloatAttr(
			xmlTextWriter* wr
			, const string& tag
			, const string& attr
			, const string& attrval
			, float val
		) {
	xmlTextWriterStartElement(wr, BAD_CAST tag.c_str());
		xmlTextWriterWriteAttribute(wr, BAD_CAST attr.c_str(), BAD_CAST attrval.c_str());
		xmlTextWriterWriteString(wr, BAD_CAST to_string(val).c_str());
	xmlTextWriterEndElement(wr);
};

void Sbecore::Xmlio::writeDoubleAttr(
			xmlTextWriter* wr
			, const string& tag
			, const string& attr
			, const string& attrval
			, double val
		) {
	xmlTextWriterStartElement(wr, BAD_CAST tag.c_str());
		xmlTextWriterWriteAttribute(wr, BAD_CAST attr.c_str(), BAD_CAST attrval.c_str());
		xmlTextWriterWriteString(wr, BAD_CAST to_string(val).c_str());
	xmlTextWriterEndElement(wr);
};

void Sbecore::Xmlio::writeStringAttr(
			xmlTextWriter* wr
			, const string& tag
			, const string& attr
			, const string& attrval
			, const string& val
		) {
	xmlTextWriterStartElement(wr, BAD_CAST tag.c_str());
		xmlTextWriterWriteAttribute(wr, BAD_CAST attr.c_str(), BAD_CAST attrval.c_str());
		xmlTextWriterWriteString(wr, BAD_CAST toUTF8(val).c_str());
	xmlTextWriterEndElement(wr);
};

void Sbecore::Xmlio::writeBoolvecAttr(
			xmlTextWriter* wr
			, const string& tag
			, const string& attr
			, const string& attrval
			, const vector<bool>& vec
		) {
	string str(vec.size(), '0');

	for (unsigned int i = 0; i < vec.size(); i++) if (vec[i]) str[i] = '1';

	xmlTextWriterStartElement(wr, BAD_CAST tag.c_str());
		xmlTextWriterWriteAttribute(wr, BAD_CAST attr.c_str(), BAD_CAST attrval.c_str());
		xmlTextWriterWriteString(wr, BAD_CAST toUTF8(str).c_str());
	xmlTextWriterEndElement(wr);
};

void Sbecore::Xmlio::writeUtinyintvecAttr(
			xmlTextWriter* wr
			, const string& tag
			, const string& attr
			, const string& attrval
			, const vector<utinyint>& vec
		) {
	xmlTextWriterStartElement(wr, BAD_CAST tag.c_str());
		xmlTextWriterWriteAttribute(wr, BAD_CAST attr.c_str(), BAD_CAST attrval.c_str());
		writeBase64(wr, (const char*) vec.data(), vec.size(), 1);
	xmlTextWriterEndElement(wr);
};

void Sbecore::Xmlio::writeUsmallintvecAttr(
			xmlTextWriter* wr
			, const string& tag
			, const string& attr
			, const string& attrval
			, const vector<usmallint>& vec
		) {
	const unsigned int varlen = sizeof(usmallint); // should be 2

	xmlTextWriterStartElement(wr, BAD_CAST tag.c_str());
		xmlTextWriterWriteAttribute(wr, BAD_CAST attr.c_str(), BAD_CAST attrval.c_str());
		writeBase64(wr, (const char*) vec.data(), vec.size(), varlen);
	xmlTextWriterEndElement(wr);
};

void Sbecore::Xmlio::writeIntvecAttr(
			xmlTextWriter* wr
			, const string& tag
			, const string& attr
			, const string& attrval
			, const vector<int>& vec
		) {
	const unsigned int varlen = sizeof(int); // should be 4

	xmlTextWriterStartElement(wr, BAD_CAST tag.c_str());
		xmlTextWriterWriteAttribute(wr, BAD_CAST attr.c_str(), BAD_CAST attrval.c_str());
		writeBase64(wr, (const char*) vec.data(), vec.size(), varlen);
	xmlTextWriterEndElement(wr);
};

void Sbecore::Xmlio::writeUintvecAttr(
			xmlTextWriter* wr
			, const string& tag
			, const string& attr
			, const string& attrval
			, const vector<uint>& vec
		) {
	const unsigned int varlen = sizeof(uint); // should be 4

	xmlTextWriterStartElement(wr, BAD_CAST tag.c_str());
		xmlTextWriterWriteAttribute(wr, BAD_CAST attr.c_str(), BAD_CAST attrval.c_str());
		writeBase64(wr, (const char*) vec.data(), vec.size(), varlen);
	xmlTextWriterEndElement(wr);
};

void Sbecore::Xmlio::writeUbigintvecAttr(
			xmlTextWriter* wr
			, const string& tag
			, const string& attr
			, const string& attrval
			, const vector<ubigint>& vec
		) {
	const unsigned int varlen = sizeof(ubigint); // should be 8

	xmlTextWriterStartElement(wr, BAD_CAST tag.c_str());
		xmlTextWriterWriteAttribute(wr, BAD_CAST attr.c_str(), BAD_CAST attrval.c_str());
		writeBase64(wr, (const char*) vec.data(), vec.size(), varlen);
	xmlTextWriterEndElement(wr);
};

void Sbecore::Xmlio::writeFloatvecAttr(
			xmlTextWriter* wr
			, const string& tag
			, const string& attr
			, const string& attrval
			, const vector<float>& vec
		) {
	const unsigned int varlen = sizeof(float); // should be 4

	xmlTextWriterStartElement(wr, BAD_CAST tag.c_str());
		xmlTextWriterWriteAttribute(wr, BAD_CAST attr.c_str(), BAD_CAST attrval.c_str());
		writeBase64(wr, (const char*) vec.data(), vec.size(), varlen);
	xmlTextWriterEndElement(wr);
};

void Sbecore::Xmlio::writeFloatmatAttr(
			xmlTextWriter* wr
			, const string& tag
			, const string& attr
			, const string& attrval
			, const Floatmat& mat
		) {
	const unsigned int varlen = sizeof(float); // should be 4

	xmlTextWriterStartElement(wr, BAD_CAST tag.c_str());
		xmlTextWriterWriteAttribute(wr, BAD_CAST attr.c_str(), BAD_CAST attrval.c_str());

		xmlTextWriterWriteElement(wr, BAD_CAST "M", BAD_CAST to_string(mat.M).c_str());
		xmlTextWriterWriteElement(wr, BAD_CAST "N", BAD_CAST to_string(mat.N).c_str());

		xmlTextWriterStartElement(wr, BAD_CAST "vec");
			writeBase64(wr, (const char*) mat.vec.data(), mat.vec.size(), varlen);
		xmlTextWriterEndElement(wr);
	xmlTextWriterEndElement(wr);
};

void Sbecore::Xmlio::writeDoublevecAttr(
			xmlTextWriter* wr
			, const string& tag
			, const string& attr
			, const string& attrval
			, const vector<double>& vec
		) {
	const unsigned int varlen = sizeof(double); // should be 8

	xmlTextWriterStartElement(wr, BAD_CAST tag.c_str());
		xmlTextWriterWriteAttribute(wr, BAD_CAST attr.c_str(), BAD_CAST attrval.c_str());
		writeBase64(wr, (const char*) vec.data(), vec.size(), varlen);
	xmlTextWriterEndElement(wr);
};

void Sbecore::Xmlio::writeDoublematAttr(
			xmlTextWriter* wr
			, const string& tag
			, const string& attr
			, const string& attrval
			, const Doublemat& mat
		) {
	const unsigned int varlen = sizeof(double); // should be 8

	xmlTextWriterStartElement(wr, BAD_CAST tag.c_str());
		xmlTextWriterWriteAttribute(wr, BAD_CAST attr.c_str(), BAD_CAST attrval.c_str());

		xmlTextWriterWriteElement(wr, BAD_CAST "M", BAD_CAST to_string(mat.M).c_str());
		xmlTextWriterWriteElement(wr, BAD_CAST "N", BAD_CAST to_string(mat.N).c_str());

		xmlTextWriterStartElement(wr, BAD_CAST "vec");
			writeBase64(wr, (const char*) mat.vec.data(), mat.vec.size(), varlen);
		xmlTextWriterEndElement(wr);
	xmlTextWriterEndElement(wr);
};

void Sbecore::Xmlio::writeStringvecAttr(
			xmlTextWriter* wr
			, const string& tag
			, const string& attr
			, const string& attrval
			, const vector<string>& vec
		) {
	string s;

	for (unsigned int i = 0; i < vec.size(); i++) {
		if (i != 0) s = s + ";";
		s = s + vec[i];
	};

	xmlTextWriterStartElement(wr, BAD_CAST tag.c_str());
		xmlTextWriterWriteAttribute(wr, BAD_CAST attr.c_str(), BAD_CAST attrval.c_str());
		xmlTextWriterWriteString(wr, BAD_CAST toUTF8(s).c_str());
	xmlTextWriterEndElement(wr);
};

float Sbecore::Xmlio::compareFloat(
			const float a
			, const float b
		) {
	if (a == 0.0) {
		if (b == 0.0) {
			return(0.0);
		} else {
			return(fabsf(b));
		};

	} else {
		return (fabsf((b-a)/a));
	};
};

double Sbecore::Xmlio::compareDouble(
			const double a
			, const double b
		) {
	if (a == 0.0) {
		if (b == 0.0) {
			return(0.0);
		} else {
			return(fabs(b));
		};

	} else {
		return (fabs((b-a)/a));
	};
};

bool Sbecore::Xmlio::compareBoolvec(
			const vector<bool>& a
			, const vector<bool>& b
		) {
	bool retval = false;

	if (a.size() == b.size()) {
		retval = true;

		for (unsigned int i = 0; i < a.size(); i++) {
			if (a[i] != b[i]) {
				retval = false;
				break;
			};
		};
	};

	return retval;
};

bool Sbecore::Xmlio::compareUtinyintvec(
			const vector<utinyint>& a
			, const vector<utinyint>& b
		) {
	bool retval = false;

	if (a.size() == b.size()) {
		retval = true;

		for (unsigned int i = 0; i < a.size(); i++) {
			if (a[i] != b[i]) {
				retval = false;
				break;
			};
		};
	};

	return retval;
};

bool Sbecore::Xmlio::compareUsmallintvec(
			const vector<usmallint>& a
			, const vector<usmallint>& b
		) {
	bool retval = false;

	if (a.size() == b.size()) {
		retval = true;

		for (unsigned int i = 0; i < a.size(); i++) {
			if (a[i] != b[i]) {
				retval = false;
				break;
			};
		};
	};

	return retval;
};

bool Sbecore::Xmlio::compareIntvec(
			const vector<int>& a
			, const vector<int>& b
		) {
	bool retval = false;

	if (a.size() == b.size()) {
		retval = true;

		for (unsigned int i = 0; i < a.size(); i++) {
			if (a[i] != b[i]) {
				retval = false;
				break;
			};
		};
	};

	return retval;
};

bool Sbecore::Xmlio::compareUintvec(
			const vector<uint>& a
			, const vector<uint>& b
		) {
	bool retval = false;

	if (a.size() == b.size()) {
		retval = true;

		for (unsigned int i = 0; i < a.size(); i++) {
			if (a[i] != b[i]) {
				retval = false;
				break;
			};
		};
	};

	return retval;
};

float Sbecore::Xmlio::compareFloatvec(
			const vector<float>& a
			, const vector<float>& b
		) {
	float retval = 1.0e9;
	float delta;

	if (a.size() == b.size()) {
		retval = 0.0;

		for (unsigned int i = 0; i < a.size(); i++) {
			delta = compareFloat(a[i], b[i]);
			if (delta > retval) retval = delta;
		};
	};

	return retval;
};

float Sbecore::Xmlio::compareFloatmat(
			const Floatmat& a
			, const Floatmat& b
		) {
	if ( (a.M == b.M) && (a.N == b.N) ) return(compareFloatvec(a.vec, b.vec));
	else return 1.0e9;
};

double Sbecore::Xmlio::compareDoublevec(
			const vector<double>& a
			, const vector<double>& b
		) {
	double retval = 1.0e9;
	double delta;

	if (a.size() == b.size()) {
		retval = 0.0;

		for (unsigned int i = 0; i < a.size(); i++) {
			delta = compareDouble(a[i], b[i]);
			if (delta > retval) retval = delta;
		};
	};

	return retval;
};

double Sbecore::Xmlio::compareDoublemat(
			const Doublemat& a
			, const Doublemat& b
		) {
	if ( (a.M == b.M) && (a.N == b.N) ) return(compareDoublevec(a.vec, b.vec));
	else return 1.0e9;
};

bool Sbecore::Xmlio::compareStringvec(
			const vector<string>& a
			, const vector<string>& b
		) {
	bool retval = false;

	if (a.size() == b.size()) {
		retval = true;

		for (unsigned int i = 0; i < a.size(); i++) {
			if (a[i] != b[i]) {
				retval = false;
				break;
			};
		};
	};

	return retval;
};

bool Sbecore::Xmlio::find(
			const set<uint>& mask
			, const uint item
		) {
	return(mask.find(item) != mask.end());
};

void Sbecore::Xmlio::insert(
			set<uint>& items
			, const uint item
		) {
	items.insert(item);
};

void Sbecore::Xmlio::insert(
			set<uint>& items
			, const set<uint>& _items
		) {
	for (auto it = _items.begin(); it != _items.end(); it++) insert(items, *it);
};

void Sbecore::Xmlio::push_back(
			vector<uint>& ics
			, const uint ix
		) {
	ics.push_back(ix);
};

void Sbecore::Xmlio::push_back(
			vector<uint>& ics
			, const vector<uint>& _ics
		) {
	for (auto it = _ics.begin(); it != _ics.end(); it++) push_back(ics, *it);
};
