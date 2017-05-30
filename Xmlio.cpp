/**
  * \file Xmlio.cpp
  * methods for XML tree input/output based on libxml2 (implementation)
  * \author Alexander Wirthmüller
  * \date created: 19 Feb 2007
  * \date modified: 6 Oct 2015
  */

#include "Xmlio.h"

/******************************************************************************
 namespace Xmlio
 ******************************************************************************/

bool Xmlio::has(
			set<uint>& items
			, const uint item
		) {
	return(items.find(item) != items.end());
};

bool Xmlio::hasAll(
			set<uint>& items
			, const set<uint>& _items
		) {
	for (set<uint>::iterator it=_items.begin();it!=_items.end();it++) if (!has(items, *it)) return false;
	return true;
};

bool Xmlio::hasAny(
			set<uint>& items
			, const set<uint>& _items
		) {
	for (set<uint>::iterator it=_items.begin();it!=_items.end();it++) if (has(items, *it)) return true;
	return false;
};

void Xmlio::add(
			set<uint>& items
			, const uint item
		) {
	items.insert(item);
};

string Xmlio::fromUTF8(
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

string Xmlio::toUTF8(
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

void Xmlio::fromBase64(
			char* inbuf
			, unsigned int inbuflen
			, char** outbuf
			, unsigned int& outbuflen
		) {
	char quad[4];
	char trip[3];

	unsigned int ix, cnt;

	*outbuf = NULL;
	outbuflen = 0;

	for (unsigned int i=inbuflen;i>0;i--) {
		if ((inbuf[i-1] == '\n') || (inbuf[i-1] == '\r') || (inbuf[i-1] == '\t') || (inbuf[i-1] == ' ')) {
		} else if (inbuf[i-1] == '=') outbuflen--;
		else break;
	};

	ix = 0;
	for (unsigned int i=0;i<inbuflen;i++) if ((inbuf[i] == '\n') || (inbuf[i] == '\r') || (inbuf[i] == '\t') || (inbuf[i] == ' ')) ix++;
	inbuflen -= ix;

	cnt = inbuflen/4;

	if ((inbuflen >= 4) && ((inbuflen%4) == 0)) {
		outbuflen += 3*cnt;
		*outbuf = new char[outbuflen];

		ix = 0;

		for (unsigned int i=0;i<cnt;i++) {
			while ((inbuf[ix] == '\n') || (inbuf[ix] == '\r') || (inbuf[ix] == '\t') || (inbuf[ix] == ' ')) ix++;
			quad[0] = inbuf[ix++];
			while ((inbuf[ix] == '\n') || (inbuf[ix] == '\r') || (inbuf[ix] == '\t') || (inbuf[ix] == ' ')) ix++;
			quad[1] = inbuf[ix++];
			while ((inbuf[ix] == '\n') || (inbuf[ix] == '\r') || (inbuf[ix] == '\t') || (inbuf[ix] == ' ')) ix++;
			quad[2] = inbuf[ix++];
			while ((inbuf[ix] == '\n') || (inbuf[ix] == '\r') || (inbuf[ix] == '\t') || (inbuf[ix] == ' ')) ix++;
			quad[3] = inbuf[ix++];

			for (unsigned int j=0;j<4;j++) {
				if ((quad[j] >= 'A') && (quad[j] <= 'Z')) quad[j] = quad[j] - 'A';
				else if ((quad[j] >= 'a') && (quad[j] <= 'z')) quad[j] = quad[j] - 'a' + 26;
				else if ((quad[j] >= '0') && (quad[j] <= '9')) quad[j] = quad[j] - '0' + 52;
				else if (quad[j] == '+') quad[j] = 62;
				else if (quad[j] == ' ') quad[j] = 62; // quick fix for HTTP encoding of space as plus
				else if (quad[j] == '/') quad[j] = 63;
				else if (quad[j] == '=') quad[j] = -128;
				else quad[j] = 0;
			};

			trip[0] = 4*quad[0] + (quad[1] & 0xf0)/16;
			trip[1] = 16*(quad[1] & 0x0f) + (quad[2] & 0xfc)/4;
			trip[2] = 64*(quad[2] & 0x03) + quad[3];

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

bool Xmlio::bigendian() {
	unsigned short int var = 255;
	char* buf = ((char*) &var);
	
	return(buf[0] == 0);
};

void Xmlio::invertBuffer(
			char* buf
			, const unsigned int len
			, const unsigned int varlen
		) {
	char c;

	for (unsigned int i=0;i<len;i++) {
		for (unsigned int j=0;j<varlen/2;j++) {
			c = buf[i*varlen+j];
			buf[i*varlen+j] = buf[i*varlen+(varlen-j-1)];
			buf[i*varlen+(varlen-j-1)] = c;
		};
	};
};

void Xmlio::parseFile(
			const string& fullpath
			, xmlDoc** doc
			, xmlXPathContext** docctx
		) {
	if (access(fullpath.c_str(), R_OK)) return;

	*doc = xmlParseFile(fullpath.c_str());
	*docctx = xmlXPathNewContext(*doc);
};

void Xmlio::parseBuffer(
			char* buf
			, int len
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

	//cout << string(buf, len) << endl;

	if (rmvns) {
		// - remove header and xmlns attributes in all nodes:
		// from '<?' to '?>\n' or '?>' (whichever comes first) ; HDRL to HDRR1/HDRR2
		// from '<' to '/>' or '>' (whichever comes first) ; TAGL to TAGR1/TAGR2
		// from ' xmlns="' to '"' ; XMLNSL to XMLNSR

		// tokenize
		ptr = 0;

		while (ptr < len) {
			for (unsigned int i=0;i<keys.size();i++) {
				match = false;

				if ((ptr+keys[i].length()) < len) {
					match = true;
					for (unsigned int j=0;j<keys[i].length();j++) {
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
		for (unsigned int i=0;i<ptrs.size();i++) {
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

		for (unsigned int i=0;i<ptrsSkipstart.size();i++) {
			ptrSkipstart = ptrsSkipstart[i];
			ptrSkipstop = ptrsSkipstop[i];

			shift += ptrSkipstop-ptrSkipstart;

			if ((i+1) < ptrsSkipstart.size()) memmove(&(buf[ptrSkipstop-shift]), &(buf[ptrSkipstop]), ptrsSkipstart[i+1]-ptrSkipstop);
			else memmove(&(buf[ptrSkipstop-shift]), &(buf[ptrSkipstop]), len-ptrSkipstop);
		};

		len = len-shift;
		//cout << "reduction in length: " << shift << endl;
	};

	*doc = xmlParseMemory(buf, len);
	*docctx = xmlXPathNewContext(*doc);
};

void Xmlio::closeParsed(
			xmlDoc* doc
			, xmlXPathContext* docctx
		) {
	xmlXPathFreeContext(docctx);
	xmlFreeDoc(doc);
};

void Xmlio::startwriteFile(
			const string& fullpath
			, xmlTextWriter** wr
		) {
	*wr = xmlNewTextWriterFilename(fullpath.c_str(), 0);

	xmlTextWriterStartDocument(*wr, NULL, "UTF-8", NULL);
};

void Xmlio::closewriteFile(
			xmlTextWriter* wr
		) {
	xmlTextWriterEndDocument(wr);
	xmlFreeTextWriter(wr);
};

void Xmlio::startwriteBuffer(
			xmlTextWriter** wr
			, xmlBuffer** buf
		) {
	*buf = xmlBufferCreate();
	(*buf)->alloc = XML_BUFFER_ALLOC_EXACT;

	*wr = xmlNewTextWriterMemory(*buf, 0);

	xmlTextWriterStartDocument(*wr, NULL, "UTF-8", NULL);
};

void Xmlio::closewriteBuffer(
			xmlTextWriter* wr
		) {
	xmlTextWriterEndDocument(wr);
	xmlFreeTextWriter(wr);
};

bool Xmlio::checkXPath(
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

bool Xmlio::checkAltXPaths(
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

bool Xmlio::checkUclcXPaths(
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

bool Xmlio::checkAttrUclcXpaths(
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

string Xmlio::extractRoot(
			xmlDoc* doc
		) {
	string retval;
	xmlNode* node = NULL;

	node = xmlDocGetRootElement(doc);
	if (node) retval = (const char*) node->name;

	return retval;
};

// NOTE: the following methods 'bool Xmlio::extractXxx()' should only be called in case xpath is proven to be valid

void Xmlio::extractBool(
			xmlXPathContext* docctx
			, const string& xpath
			, bool& val
		) {
	xmlXPathObject* obj;
	char* nodebuf = NULL;

	obj = xmlXPathEvalExpression(BAD_CAST xpath.c_str(), docctx);
	nodebuf = (char*) xmlNodeGetContent(obj->nodesetval->nodeTab[0]);

	if (nodebuf) {
		val = !(strcmp(nodebuf, "true"));
		delete[] nodebuf;

	} else val = false;;

	xmlXPathFreeObject(obj);
};

void Xmlio::extractTinyint(
			xmlXPathContext* docctx
			, const string& xpath
			, tinyint& val
		) {
	xmlXPathObject* obj;
	char* nodebuf = NULL;

	int _val = 0;

	obj = xmlXPathEvalExpression(BAD_CAST xpath.c_str(), docctx);
	nodebuf = (char*) xmlNodeGetContent(obj->nodesetval->nodeTab[0]);

	if (nodebuf) {
		_val = atoi(nodebuf);
		delete[] nodebuf;
	};

	if ((_val < -127) || (_val > 127)) val = 0; else val = _val;

	xmlXPathFreeObject(obj);
};

void Xmlio::extractUtinyint(
			xmlXPathContext* docctx
			, const string& xpath
			, utinyint& val
		) {
	xmlXPathObject* obj;
	char* nodebuf = NULL;

	int _val = 0;

	obj = xmlXPathEvalExpression(BAD_CAST xpath.c_str(), docctx);
	nodebuf = (char*) xmlNodeGetContent(obj->nodesetval->nodeTab[0]);

	if (nodebuf) {
		_val = atoi(nodebuf);
		delete[] nodebuf;
	};

	if ((_val < 0) || (_val > 255)) val = 0; else val = _val;

	xmlXPathFreeObject(obj);
};

void Xmlio::extractSmallint(
			xmlXPathContext* docctx
			, const string& xpath
			, smallint& val
		) {
	xmlXPathObject* obj;
	char* nodebuf = NULL;

	int _val = 0;

	obj = xmlXPathEvalExpression(BAD_CAST xpath.c_str(), docctx);
	nodebuf = (char*) xmlNodeGetContent(obj->nodesetval->nodeTab[0]);

	if (nodebuf) {
		_val = atoi(nodebuf);
		delete[] nodebuf;
	};

	if ((_val < -16383) || (_val > 16383)) val = 0; else val = _val;

	xmlXPathFreeObject(obj);
};

void Xmlio::extractUsmallint(
			xmlXPathContext* docctx
			, const string& xpath
			, usmallint& val
		) {
	xmlXPathObject* obj;
	char* nodebuf = NULL;

	int _val = 0;

	obj = xmlXPathEvalExpression(BAD_CAST xpath.c_str(), docctx);
	nodebuf = (char*) xmlNodeGetContent(obj->nodesetval->nodeTab[0]);

	if (nodebuf) {
		_val = atoi(nodebuf);
		delete[] nodebuf;
	};

	if ((_val < 0) || (_val > 32767)) val = 0; else val = _val;

	xmlXPathFreeObject(obj);
};

void Xmlio::extractInt(
			xmlXPathContext* docctx
			, const string& xpath
			, int& val
		) {
	xmlXPathObject* obj;
	char* nodebuf = NULL;

	obj = xmlXPathEvalExpression(BAD_CAST xpath.c_str(), docctx);
	nodebuf = (char*) xmlNodeGetContent(obj->nodesetval->nodeTab[0]);

	if (nodebuf) {
		val = atoi(nodebuf);
		delete[] nodebuf;

	} else val = 0;

	xmlXPathFreeObject(obj);
};

void Xmlio::extractUint(
			xmlXPathContext* docctx
			, const string& xpath
			, uint& val
		) {
	xmlXPathObject* obj;
	char* nodebuf = NULL;

	int _val = 0;

	obj = xmlXPathEvalExpression(BAD_CAST xpath.c_str(), docctx);
	nodebuf = (char*) xmlNodeGetContent(obj->nodesetval->nodeTab[0]);

	if (nodebuf) {
		_val = atoi(nodebuf);
		delete[] nodebuf;
	};

	if (_val < 0) val = 0; else val = _val;

	xmlXPathFreeObject(obj);
};

void Xmlio::extractBigint(
			xmlXPathContext* docctx
			, const string& xpath
			, bigint& val
		) {
	xmlXPathObject* obj;
	char* nodebuf = NULL;

	obj = xmlXPathEvalExpression(BAD_CAST xpath.c_str(), docctx);
	nodebuf = (char*) xmlNodeGetContent(obj->nodesetval->nodeTab[0]);

	if (nodebuf) {
		val = atoll(nodebuf);
		delete[] nodebuf;

	} else val = 0;

	xmlXPathFreeObject(obj);
};

void Xmlio::extractUbigint(
			xmlXPathContext* docctx
			, const string& xpath
			, ubigint& val
		) {
	xmlXPathObject* obj;
	char* nodebuf = NULL;

	long long int _val = 0;

	obj = xmlXPathEvalExpression(BAD_CAST xpath.c_str(), docctx);
	nodebuf = (char*) xmlNodeGetContent(obj->nodesetval->nodeTab[0]);

	if (nodebuf) {
		_val = atoll(nodebuf);
		delete[] nodebuf;
	};

	if (_val < 0) val = 0; else val = _val;

	xmlXPathFreeObject(obj);
};

void Xmlio::extractFloat(
			xmlXPathContext* docctx
			, const string& xpath
			, float& val
		) {
	xmlXPathObject* obj;
	char* nodebuf = NULL;

	obj = xmlXPathEvalExpression(BAD_CAST xpath.c_str(), docctx);
	nodebuf = (char*) xmlNodeGetContent(obj->nodesetval->nodeTab[0]);

	if (nodebuf) {
		val = (float) atof(nodebuf);
		delete[] nodebuf;

	} else val = 0.0;

	xmlXPathFreeObject(obj);
};

void Xmlio::extractDouble(
			xmlXPathContext* docctx
			, const string& xpath
			, double& val
		) {
	xmlXPathObject* obj;
	char* nodebuf = NULL;

	obj = xmlXPathEvalExpression(BAD_CAST xpath.c_str(), docctx);
	nodebuf = (char*) xmlNodeGetContent(obj->nodesetval->nodeTab[0]);

	if (nodebuf) {
		val = atof(nodebuf);
		delete[] nodebuf;

	} else val = 0.0;

	xmlXPathFreeObject(obj);
};

void Xmlio::extractString(
			xmlXPathContext* docctx
			, const string& xpath
			, string& val
		) {
	xmlXPathObject* obj;
	char* nodebuf = NULL;

	obj = xmlXPathEvalExpression(BAD_CAST xpath.c_str(), docctx);
	nodebuf = (char*) xmlNodeGetContent(obj->nodesetval->nodeTab[0]);

	if (nodebuf) {
		val.assign(nodebuf);
		delete[] nodebuf;

	} else val = "";

	xmlXPathFreeObject(obj);
};

void Xmlio::extractUtinyintvec(
			xmlXPathContext* docctx
			, const string& xpath
			, vector<utinyint>& vec
		) {
	xmlXPathObject* obj;
	char* nodebuf = NULL;

	char* buf = NULL;
	unsigned int len;

	vec.resize(0);

	obj = xmlXPathEvalExpression(BAD_CAST xpath.c_str(), docctx);
	nodebuf = (char*) xmlNodeGetContent(obj->nodesetval->nodeTab[0]);

	if (nodebuf) {
		fromBase64(nodebuf, strlen(nodebuf), &buf, len);

		if (buf) {
			vec.resize(len, 0);
			memcpy((void*) vec.data(), (void*) buf, len);

			delete[] buf;
		};

		delete[] nodebuf;
	};

	xmlXPathFreeObject(obj);
};

void Xmlio::extractUsmallintvec(
			xmlXPathContext* docctx
			, const string& xpath
			, vector<usmallint>& vec
		) {
	xmlXPathObject* obj;
	char* nodebuf = NULL;

	unsigned int varlen = sizeof(usmallint);

	char* buf = NULL;
	unsigned int len;

	vec.resize(0);

	obj = xmlXPathEvalExpression(BAD_CAST xpath.c_str(), docctx);
	nodebuf = (char*) xmlNodeGetContent(obj->nodesetval->nodeTab[0]);

	if (nodebuf) {
		fromBase64(nodebuf, strlen(nodebuf), &buf, len);

		if (buf) {
			if ((len%varlen) == 0) {
				if (bigendian()) invertBuffer(buf, len, varlen);

				vec.resize(len/varlen, 0);
				memcpy((void*) vec.data(), (void*) buf, len);
			};

			delete[] buf;
		};

		delete[] nodebuf;
	};

	xmlXPathFreeObject(obj);
};

void Xmlio::extractIntvec(
			xmlXPathContext* docctx
			, const string& xpath
			, vector<int>& vec
		) {
	xmlXPathObject* obj;
	char* nodebuf = NULL;

	unsigned int varlen = sizeof(int);

	char* buf = NULL;
	unsigned int len;

	vec.resize(0);

	obj = xmlXPathEvalExpression(BAD_CAST xpath.c_str(), docctx);
	nodebuf = (char*) xmlNodeGetContent(obj->nodesetval->nodeTab[0]);

	if (nodebuf) {
		fromBase64(nodebuf, strlen(nodebuf), &buf, len);

		if (buf) {
			if ((len%varlen) == 0) {
				if (bigendian()) invertBuffer(buf, len, varlen);

				vec.resize(len/varlen, 0);
				memcpy((void*) vec.data(), (void*) buf, len);
			};

			delete[] buf;
		};

		delete[] nodebuf;
	};

	xmlXPathFreeObject(obj);
};

void Xmlio::extractUintvec(
			xmlXPathContext* docctx
			, const string& xpath
			, vector<uint>& vec
		) {
	xmlXPathObject* obj;
	char* nodebuf = NULL;

	unsigned int varlen = sizeof(uint);

	char* buf = NULL;
	unsigned int len;

	vec.resize(0);

	obj = xmlXPathEvalExpression(BAD_CAST xpath.c_str(), docctx);
	nodebuf = (char*) xmlNodeGetContent(obj->nodesetval->nodeTab[0]);

	if (nodebuf) {
		fromBase64(nodebuf, strlen(nodebuf), &buf, len);

		if (buf) {
			if ((len%varlen) == 0) {
				if (bigendian()) invertBuffer(buf, len, varlen);

				vec.resize(len/varlen, 0);
				memcpy((void*) vec.data(), (void*) buf, len);
			};

			delete[] buf;
		};

		delete[] nodebuf;
	};

	xmlXPathFreeObject(obj);
};

void Xmlio::extractUbigintvec(
			xmlXPathContext* docctx
			, const string& xpath
			, vector<ubigint>& vec
		) {
	xmlXPathObject* obj;
	char* nodebuf = NULL;

	unsigned int varlen = sizeof(ubigint);

	char* buf = NULL;
	unsigned int len;

	vec.resize(0);

	obj = xmlXPathEvalExpression(BAD_CAST xpath.c_str(), docctx);
	nodebuf = (char*) xmlNodeGetContent(obj->nodesetval->nodeTab[0]);

	if (nodebuf) {
		fromBase64(nodebuf, strlen(nodebuf), &buf, len);

		if (buf) {
			if ((len%varlen) == 0) {
				if (bigendian()) invertBuffer(buf, len, varlen);

				vec.resize(len/varlen, 0);
				memcpy((void*) vec.data(), (void*) buf, len);
			};

			delete[] buf;
		};

		delete[] nodebuf;
	};

	xmlXPathFreeObject(obj);
};

void Xmlio::extractFloatvec(
			xmlXPathContext* docctx
			, const string& xpath
			, vector<float>& vec
		) {
	xmlXPathObject* obj;
	char* nodebuf = NULL;

	unsigned int varlen = sizeof(float);

	char* buf = NULL;
	unsigned int len;

	vec.resize(0);

	obj = xmlXPathEvalExpression(BAD_CAST xpath.c_str(), docctx);
	nodebuf = (char*) xmlNodeGetContent(obj->nodesetval->nodeTab[0]);

	if (nodebuf) {
		fromBase64(nodebuf, strlen(nodebuf), &buf, len);

		if (buf) {
			if ((len%varlen) == 0) {
				if (bigendian()) invertBuffer(buf, len, varlen);

				vec.resize(len/varlen, 0);
				memcpy((void*) vec.data(), (void*) buf, len);
			};

			delete[] buf;
		};

		delete[] nodebuf;
	};

	xmlXPathFreeObject(obj);
};

void Xmlio::extractFloatmat(
			xmlXPathContext* docctx
			, const string& xpath
			, vector<float>& mat
			, uint& M
			, uint& N
		) {
	extractFloatvecUclc(docctx, xpath, "Mat", "Mat", mat);
	extractUintUclc(docctx, xpath, "M", "M", M);
	extractUintUclc(docctx, xpath, "N", "N", N);

	if ((M*N) != mat.size()) {
		M = 0;
		N = 0;
		mat.resize(0);
	};
};

void Xmlio::extractDoublevec(
			xmlXPathContext* docctx
			, const string& xpath
			, vector<double>& vec
		) {
	xmlXPathObject* obj;
	char* nodebuf = NULL;

	unsigned int varlen = sizeof(double);

	char* buf = NULL;
	unsigned int len;

	vec.resize(0);

	obj = xmlXPathEvalExpression(BAD_CAST xpath.c_str(), docctx);
	nodebuf = (char*) xmlNodeGetContent(obj->nodesetval->nodeTab[0]);

	if (nodebuf) {
		fromBase64(nodebuf, strlen(nodebuf), &buf, len);

		if (buf) {
			if ((len%varlen) == 0) {
				if (bigendian()) invertBuffer(buf, len, varlen);

				vec.resize(len/varlen, 0);
				memcpy((void*) vec.data(), (void*) buf, len);
			};

			delete[] buf;
		};

		delete[] nodebuf;
	};

	xmlXPathFreeObject(obj);
};

void Xmlio::extractDoublemat(
			xmlXPathContext* docctx
			, const string& xpath
			, vector<double>& mat
			, uint& M
			, uint& N
		) {
	extractDoublevecUclc(docctx, xpath, "Mat", "Mat", mat);
	extractUintUclc(docctx, xpath, "M", "M", M);
	extractUintUclc(docctx, xpath, "N", "N", N);

	if ((M*N) != mat.size()) {
		M = 0;
		N = 0;
		mat.resize(0);
	};
};

void Xmlio::extractStringvec(
			xmlXPathContext* docctx
			, const string& xpath
			, vector<string>& vec
		) {
	xmlXPathObject* obj;
	char* nodebuf = NULL;

	string s;

	obj = xmlXPathEvalExpression(BAD_CAST xpath.c_str(), docctx);
	nodebuf = (char*) xmlNodeGetContent(obj->nodesetval->nodeTab[0]);

	if (nodebuf) {
		s.assign(nodebuf);
		delete[] nodebuf;
	};

	s = fromUTF8(s);
	StrMod::stringToVector(s, vec);

	xmlXPathFreeObject(obj);
};

bool Xmlio::extractBoolUclc(
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

bool Xmlio::extractTinyintUclc(
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

bool Xmlio::extractUtinyintUclc(
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

bool Xmlio::extractSmallintUclc(
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

bool Xmlio::extractUsmallintUclc(
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

bool Xmlio::extractIntUclc(
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

bool Xmlio::extractUintUclc(
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

bool Xmlio::extractBigintUclc(
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

bool Xmlio::extractUbigintUclc(
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

bool Xmlio::extractFloatUclc(
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

bool Xmlio::extractDoubleUclc(
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

bool Xmlio::extractStringUclc(
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

bool Xmlio::extractUtinyintvecUclc(
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

bool Xmlio::extractUsmallintvecUclc(
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

bool Xmlio::extractIntvecUclc(
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

bool Xmlio::extractUintvecUclc(
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

bool Xmlio::extractUbigintvecUclc(
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

bool Xmlio::extractFloatvecUclc(
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

bool Xmlio::extractFloatmatUclc(
			xmlXPathContext* docctx
			, const string& basexpath
			, const string& tag
			, const string& shorttag
			, vector<float>& mat
			, uint& M
			, uint& N
		) {
	string goodpath;

	if (checkAltXPaths(docctx, goodpath, basexpath, tag, shorttag, StrMod::uc(tag), StrMod::uc(shorttag), StrMod::lc(tag), StrMod::lc(shorttag))) {
		extractFloatmat(docctx, goodpath, mat, M, N);
		return true;
	};

	return false;
};

bool Xmlio::extractDoublevecUclc(
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

bool Xmlio::extractDoublematUclc(
			xmlXPathContext* docctx
			, const string& basexpath
			, const string& tag
			, const string& shorttag
			, vector<double>& mat
			, uint& M
			, uint& N
		) {
	string goodpath;

	if (checkAltXPaths(docctx, goodpath, basexpath, tag, shorttag, StrMod::uc(tag), StrMod::uc(shorttag), StrMod::lc(tag), StrMod::lc(shorttag))) {
		extractDoublemat(docctx, goodpath, mat, M, N);
		return true;
	};

	return false;
};

bool Xmlio::extractStringvecUclc(
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

bool Xmlio::extractBoolAttrUclc(
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

bool Xmlio::extractTinyintAttrUclc(
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

bool Xmlio::extractUtinyintAttrUclc(
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

bool Xmlio::extractSmallintAttrUclc(
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

bool Xmlio::extractUsmallintAttrUclc(
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

bool Xmlio::extractIntAttrUclc(
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

bool Xmlio::extractUintAttrUclc(
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

bool Xmlio::extractBigintAttrUclc(
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

bool Xmlio::extractUbigintAttrUclc(
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

bool Xmlio::extractFloatAttrUclc(
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

bool Xmlio::extractDoubleAttrUclc(
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

bool Xmlio::extractStringAttrUclc(
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

bool Xmlio::extractUtinyintvecAttrUclc(
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

bool Xmlio::extractUsmallintvecAttrUclc(
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

bool Xmlio::extractIntvecAttrUclc(
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

bool Xmlio::extractUintvecAttrUclc(
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

bool Xmlio::extractUbigintvecAttrUclc(
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

bool Xmlio::extractFloatvecAttrUclc(
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

bool Xmlio::extractFloatmatAttrUclc(
			xmlXPathContext* docctx
			, const string& basexpath
			, const string& tag
			, const string& shorttag
			, const string& attr
			, const string& attrval
			, vector<float>& mat
			, uint& M
			, uint& N
		) {
	string goodpath;

	if (checkAttrUclcXpaths(docctx, basexpath, goodpath, tag, shorttag, attr, attrval)) {
		extractFloatmat(docctx, goodpath, mat, M, N);
		return true;
	};

	return false;
};

bool Xmlio::extractDoublevecAttrUclc(
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

bool Xmlio::extractDoublematAttrUclc(
			xmlXPathContext* docctx
			, const string& basexpath
			, const string& tag
			, const string& shorttag
			, const string& attr
			, const string& attrval
			, vector<double>& mat
			, uint& M
			, uint& N
		) {
	string goodpath;

	if (checkAttrUclcXpaths(docctx, basexpath, goodpath, tag, shorttag, attr, attrval)) {
		extractDoublemat(docctx, goodpath, mat, M, N);
		return true;
	};

	return false;
};

bool Xmlio::extractStringvecAttrUclc(
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

void Xmlio::extractList(
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
	char* nodebuf = NULL;

	unsigned int ix;

	string s;

	ics.resize(0);
	shorttags.resize(0);

	s = basexpath + "/" + tag + "/@" + attr;
	obj = xmlXPathEvalExpression(BAD_CAST s.c_str(), docctx);

	if (obj) {
		if (obj->nodesetval) {
			// collect indices
			for (int i=0;i<obj->nodesetval->nodeNr;i++) {
				node = obj->nodesetval->nodeTab[i];

				if (node->type == XML_ATTRIBUTE_NODE) {
					nodebuf = (char*) xmlNodeGetContent(node);

					if (nodebuf) {
						ics.push_back(atol(nodebuf));
						shorttags.push_back(false);

						delete[] nodebuf;
					};
				};
			};
		};

		xmlXPathFreeObject(obj);
	};

	s = basexpath + "/" + shorttag + "/@" + attr;
	obj = xmlXPathEvalExpression(BAD_CAST s.c_str(), docctx);

	if (obj) {
		if (obj->nodesetval) {
			// collect nums
			for (int i=0;i<obj->nodesetval->nodeNr;i++) {
				node = obj->nodesetval->nodeTab[i];

				if (node->type == XML_ATTRIBUTE_NODE) {
					nodebuf = (char*) xmlNodeGetContent(node);

					if (nodebuf) {
						ics.push_back(atol(nodebuf));
						shorttags.push_back(true);

						delete[] nodebuf;
					};
				};
			};
		};

		xmlXPathFreeObject(obj);
	};

	// order by indices ascending
	for (unsigned int i=0;i<ics.size();i++) {
		for (unsigned int j=i+1;j<ics.size();j++) {
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

void Xmlio::writeBase64(
			xmlTextWriter* wr
			, const char* _buf
			, const unsigned int len
			, const unsigned int varlen
		) {
	if (bigendian()) {
		char* buf = new char[len*varlen];

		memcpy((void*) buf, (void*) _buf, len*varlen);
		invertBuffer(buf, len, varlen);
		xmlTextWriterWriteBase64(wr, buf, 0, len*varlen);

		delete[] buf;

	} else {
		xmlTextWriterWriteBase64(wr, (char*) _buf, 0, len*varlen);
	};
};

void Xmlio::writeBool(
			xmlTextWriter* wr
			, const string& tag
			, bool val
		) {
	xmlTextWriterWriteElement(wr, BAD_CAST tag.c_str(), BAD_CAST StrMod::boolToString(val).c_str());
};

void Xmlio::writeTinyint(
			xmlTextWriter* wr
			, const string& tag
			, tinyint val
		) {
	xmlTextWriterWriteElement(wr, BAD_CAST tag.c_str(), BAD_CAST to_string(val).c_str());
};

void Xmlio::writeUtinyint(
			xmlTextWriter* wr
			, const string& tag
			, utinyint val
		) {
	xmlTextWriterWriteElement(wr, BAD_CAST tag.c_str(), BAD_CAST to_string(val).c_str());
};

void Xmlio::writeSmallint(
			xmlTextWriter* wr
			, const string& tag
			, smallint val
		) {
	xmlTextWriterWriteElement(wr, BAD_CAST tag.c_str(), BAD_CAST to_string(val).c_str());
};

void Xmlio::writeUsmallint(
			xmlTextWriter* wr
			, const string& tag
			, usmallint val
		) {
	xmlTextWriterWriteElement(wr, BAD_CAST tag.c_str(), BAD_CAST to_string(val).c_str());
};

void Xmlio::writeInt(
			xmlTextWriter* wr
			, const string& tag
			, int val
		) {
	xmlTextWriterWriteElement(wr, BAD_CAST tag.c_str(), BAD_CAST to_string(val).c_str());
};

void Xmlio::writeUint(
			xmlTextWriter* wr
			, const string& tag
			, uint val
		) {
	xmlTextWriterWriteElement(wr, BAD_CAST tag.c_str(), BAD_CAST to_string(val).c_str());
};

void Xmlio::writeBigint(
			xmlTextWriter* wr
			, const string& tag
			, bigint val
		) {
	xmlTextWriterWriteElement(wr, BAD_CAST tag.c_str(), BAD_CAST to_string(val).c_str());
};

void Xmlio::writeUbigint(
			xmlTextWriter* wr
			, const string& tag
			, ubigint val
		) {
	xmlTextWriterWriteElement(wr, BAD_CAST tag.c_str(), BAD_CAST to_string(val).c_str());
};

void Xmlio::writeFloat(
			xmlTextWriter* wr
			, const string& tag
			, float val
		) {
	xmlTextWriterWriteElement(wr, BAD_CAST tag.c_str(), BAD_CAST to_string(val).c_str());
};

void Xmlio::writeDouble(
			xmlTextWriter* wr
			, const string& tag
			, double val
		) {
	xmlTextWriterWriteElement(wr, BAD_CAST tag.c_str(), BAD_CAST to_string(val).c_str());
};

void Xmlio::writeString(
			xmlTextWriter* wr
			, const string& tag
			, const string& val
		) {
	xmlTextWriterWriteElement(wr, BAD_CAST tag.c_str(), BAD_CAST toUTF8(val).c_str());
};

void Xmlio::writeUtinyintvec(
			xmlTextWriter* wr
			, const string& tag
			, const vector<utinyint>& vec
		) {
	xmlTextWriterStartElement(wr, BAD_CAST tag.c_str());
		writeBase64(wr, (const char*) vec.data(), vec.size(), 1);
	xmlTextWriterEndElement(wr);
};

void Xmlio::writeUsmallintvec(
			xmlTextWriter* wr
			, const string& tag
			, const vector<usmallint>& vec
		) {
	const unsigned int varlen = sizeof(usmallint); // should be 2

	xmlTextWriterStartElement(wr, BAD_CAST tag.c_str());
		writeBase64(wr, (const char*) vec.data(), vec.size(), varlen);
	xmlTextWriterEndElement(wr);
};

void Xmlio::writeIntvec(
			xmlTextWriter* wr
			, const string& tag
			, const vector<int>& vec
		) {
	const unsigned int varlen = sizeof(int); // should be 4

	xmlTextWriterStartElement(wr, BAD_CAST tag.c_str());
		writeBase64(wr, (const char*) vec.data(), vec.size(), varlen);
	xmlTextWriterEndElement(wr);
};

void Xmlio::writeUintvec(
			xmlTextWriter* wr
			, const string& tag
			, const vector<uint>& vec
		) {
	const unsigned int varlen = sizeof(uint); // should be 4

	xmlTextWriterStartElement(wr, BAD_CAST tag.c_str());
		writeBase64(wr, (const char*) vec.data(), vec.size(), varlen);
	xmlTextWriterEndElement(wr);
};

void Xmlio::writeUbigintvec(
			xmlTextWriter* wr
			, const string& tag
			, const vector<ubigint>& vec
		) {
	const unsigned int varlen = sizeof(ubigint); // should be 8

	xmlTextWriterStartElement(wr, BAD_CAST tag.c_str());
		writeBase64(wr, (const char*) vec.data(), vec.size(), varlen);
	xmlTextWriterEndElement(wr);
};

void Xmlio::writeFloatvec(
			xmlTextWriter* wr
			, const string& tag
			, const vector<float>& vec
		) {
	const unsigned int varlen = sizeof(float); // should be 4

	xmlTextWriterStartElement(wr, BAD_CAST tag.c_str());
		writeBase64(wr, (const char*) vec.data(), vec.size(), varlen);
	xmlTextWriterEndElement(wr);
};

void Xmlio::writeFloatmat(
			xmlTextWriter* wr
			, const string& tag
			, const vector<float>& mat
			, const uint& M
			, const uint& N
		) {
	/* example:
	<test>
		<M>3</M>
		<N>5</N>
		<mat>MTIzNA==</mat>
	</test> */

	const unsigned int varlen = sizeof(float); // should be 4

	xmlTextWriterStartElement(wr, BAD_CAST tag.c_str());
		xmlTextWriterWriteElement(wr, BAD_CAST "M", BAD_CAST to_string(M).c_str());
		xmlTextWriterWriteElement(wr, BAD_CAST "N", BAD_CAST to_string(N).c_str());

		xmlTextWriterStartElement(wr, BAD_CAST "mat");
			writeBase64(wr, (const char*) mat.data(), mat.size(), varlen);
		xmlTextWriterEndElement(wr);
	xmlTextWriterEndElement(wr);
};

void Xmlio::writeDoublevec(
			xmlTextWriter* wr
			, const string& tag
			, const vector<double>& vec
		) {
	const unsigned int varlen = sizeof(double); // should be 8

	xmlTextWriterStartElement(wr, BAD_CAST tag.c_str());
		writeBase64(wr, (const char*) vec.data(), vec.size(), varlen);
	xmlTextWriterEndElement(wr);
};

void Xmlio::writeDoublemat(
			xmlTextWriter* wr
			, const string& tag
			, const vector<double>& mat
			, const uint& M
			, const uint& N
		) {
	/* example:
	<test>
		<M>3</M>
		<N>5</N>
		<mat>MTIzNA==</mat>
	</test> */

	const unsigned int varlen = sizeof(double); // should be 8

	xmlTextWriterStartElement(wr, BAD_CAST tag.c_str());
		xmlTextWriterWriteElement(wr, BAD_CAST "M", BAD_CAST to_string(M).c_str());
		xmlTextWriterWriteElement(wr, BAD_CAST "N", BAD_CAST to_string(N).c_str());

		xmlTextWriterStartElement(wr, BAD_CAST "mat");
			writeBase64(wr, (const char*) mat.data(), mat.size(), varlen);
		xmlTextWriterEndElement(wr);
	xmlTextWriterEndElement(wr);
};

void Xmlio::writeStringvec(
			xmlTextWriter* wr
			, const string& tag
			, const vector<string>& vec
		) {
	string s;

	for (unsigned int i=0;i<vec.size();i++) {
		if (i != 0) s = s + ";";
		s = s + vec[i];
	};

	xmlTextWriterWriteElement(wr, BAD_CAST tag.c_str(), BAD_CAST toUTF8(s).c_str());
};

void Xmlio::writeBoolAttr(
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

void Xmlio::writeTinyintAttr(
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

void Xmlio::writeUtinyintAttr(
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

void Xmlio::writeSmallintAttr(
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

void Xmlio::writeUsmallintAttr(
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

void Xmlio::writeIntAttr(
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

void Xmlio::writeUintAttr(
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

void Xmlio::writeBigintAttr(
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

void Xmlio::writeUbigintAttr(
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

void Xmlio::writeFloatAttr(
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

void Xmlio::writeDoubleAttr(
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

void Xmlio::writeStringAttr(
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

void Xmlio::writeUtinyintvecAttr(
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

//	delete[] buf;
};

void Xmlio::writeUsmallintvecAttr(
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

void Xmlio::writeIntvecAttr(
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

void Xmlio::writeUintvecAttr(
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

//	delete[] buf;
};

void Xmlio::writeUbigintvecAttr(
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

//	delete[] buf;
};

void Xmlio::writeFloatvecAttr(
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

void Xmlio::writeFloatmatAttr(
			xmlTextWriter* wr
			, const string& tag
			, const string& attr
			, const string& attrval
			, const vector<float>& mat
			, const uint& M
			, const uint& N
		) {
	const unsigned int varlen = sizeof(float); // should be 4

	xmlTextWriterStartElement(wr, BAD_CAST tag.c_str());
		xmlTextWriterWriteAttribute(wr, BAD_CAST attr.c_str(), BAD_CAST attrval.c_str());

		xmlTextWriterWriteElement(wr, BAD_CAST "M", BAD_CAST to_string(M).c_str());
		xmlTextWriterWriteElement(wr, BAD_CAST "N", BAD_CAST to_string(N).c_str());

		xmlTextWriterStartElement(wr, BAD_CAST "mat");
			writeBase64(wr, (const char*) mat.data(), mat.size(), varlen);
		xmlTextWriterEndElement(wr);
	xmlTextWriterEndElement(wr);
};

void Xmlio::writeDoublevecAttr(
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

void Xmlio::writeDoublematAttr(
			xmlTextWriter* wr
			, const string& tag
			, const string& attr
			, const string& attrval
			, const vector<double>& mat
			, const uint& M
			, const uint& N
		) {
	const unsigned int varlen = sizeof(double); // should be 8

	xmlTextWriterStartElement(wr, BAD_CAST tag.c_str());
		xmlTextWriterWriteAttribute(wr, BAD_CAST attr.c_str(), BAD_CAST attrval.c_str());

		xmlTextWriterWriteElement(wr, BAD_CAST "M", BAD_CAST to_string(M).c_str());
		xmlTextWriterWriteElement(wr, BAD_CAST "N", BAD_CAST to_string(N).c_str());

		xmlTextWriterStartElement(wr, BAD_CAST "mat");
			writeBase64(wr, (const char*) mat.data(), mat.size(), varlen);
		xmlTextWriterEndElement(wr);
	xmlTextWriterEndElement(wr);
};

void Xmlio::writeStringvecAttr(
			xmlTextWriter* wr
			, const string& tag
			, const string& attr
			, const string& attrval
			, const vector<string>& vec
		) {
	string s;

	for (unsigned int i=0;i<vec.size();i++) {
		if (i != 0) s = s + ";";
		s = s + vec[i];
	};

	xmlTextWriterStartElement(wr, BAD_CAST tag.c_str());
		xmlTextWriterWriteAttribute(wr, BAD_CAST attr.c_str(), BAD_CAST attrval.c_str());
		xmlTextWriterWriteString(wr, BAD_CAST toUTF8(s).c_str());
	xmlTextWriterEndElement(wr);
};

float Xmlio::compareFloat(
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

double Xmlio::compareDouble(
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

bool Xmlio::compareUtinyintvec(
			const vector<utinyint>& a
			, const vector<utinyint>& b
		) {
	bool retval = false;

	if (a.size() == b.size()) {
		retval = true;

		for (unsigned int i=0;i<a.size();i++) {
			if (a[i] != b[i]) {
				retval = false;
				break;
			};
		};
	};

	return retval;
};

bool Xmlio::compareUsmallintvec(
			const vector<usmallint>& a
			, const vector<usmallint>& b
		) {
	bool retval = false;

	if (a.size() == b.size()) {
		retval = true;

		for (unsigned int i=0;i<a.size();i++) {
			if (a[i] != b[i]) {
				retval = false;
				break;
			};
		};
	};

	return retval;
};

bool Xmlio::compareIntvec(
			const vector<int>& a
			, const vector<int>& b
		) {
	bool retval = false;

	if (a.size() == b.size()) {
		retval = true;

		for (unsigned int i=0;i<a.size();i++) {
			if (a[i] != b[i]) {
				retval = false;
				break;
			};
		};
	};

	return retval;
};

bool Xmlio::compareUintvec(
			const vector<uint>& a
			, const vector<uint>& b
		) {
	bool retval = false;

	if (a.size() == b.size()) {
		retval = true;

		for (unsigned int i=0;i<a.size();i++) {
			if (a[i] != b[i]) {
				retval = false;
				break;
			};
		};
	};

	return retval;
};

float Xmlio::compareFloatvec(
			const vector<float>& a
			, const vector<float>& b
		) {
	float retval = 1.0e9;
	float delta;

	if (a.size() == b.size()) {
		retval = 0.0;

		for (unsigned int i=0;i<a.size();i++) {
			delta = compareFloat(a[i], b[i]);
			if (delta > retval) retval = delta;
		};
	};

	return retval;
};

float Xmlio::compareFloatmat(
			const vector<float>& a
			, const uint aM
			, const uint aN
			, const vector<float>& b
			, const uint bM
			, const uint bN
		) {
	if ( (aM == bM) && (aN == bN) )
		return(compareFloatvec(a, b));
	else
		return 1.0e9;
};

double Xmlio::compareDoublevec(
			const vector<double>& a
			, const vector<double>& b
		) {
	double retval = 1.0e9;
	double delta;

	if (a.size() == b.size()) {
		retval = 0.0;

		for (unsigned int i=0;i<a.size();i++) {
			delta = compareDouble(a[i], b[i]);
			if (delta > retval) retval = delta;
		};
	};

	return retval;
};

double Xmlio::compareDoublemat(
			const vector<double>& a
			, const uint aM
			, const uint aN
			, const vector<double>& b
			, const uint bM
			, const uint bN
		) {
	if ( (aM == bM) && (aN == bN) )
		return(compareDoublevec(a, b));
	else
		return 1.0e9;
};

bool Xmlio::compareStringvec(
			const vector<string>& a
			, const vector<string>& b
		) {
	bool retval = false;

	if (a.size() == b.size()) {
		retval = true;

		for (unsigned int i=0;i<a.size();i++) {
			if (a[i] != b[i]) {
				retval = false;
				break;
			};
		};
	};

	return retval;
};

bool Xmlio::find(
			const set<uint>& mask
			, const uint item
		) {
	return(mask.find(item) != mask.end());
};

void Xmlio::insert(
			set<uint>& items
			, const uint item
		) {
	items.insert(item);
};

void Xmlio::insert(
			set<uint>& items
			, const set<uint>& _items
		) {
	for (set<uint>::iterator it=_items.begin();it!=_items.end();it++) insert(items, *it);
};

/******************************************************************************
 class Xmlio::Block
 ******************************************************************************/

Xmlio::Block::Block() {
};

Xmlio::Block::~Block() {
};

bool Xmlio::Block::has(
			const uint item
		) {
	return(mask.find(item) != mask.end());
};

bool Xmlio::Block::hasAll(
			const set<uint>& items
		) {
	for (set<uint>::iterator it=items.begin();it!=items.end();it++) if (!has(*it)) return false;
	return true;
};

bool Xmlio::Block::hasAny(
			const set<uint>& items
		) {
	for (set<uint>::iterator it=items.begin();it!=items.end();it++) if (has(*it)) return true;
	return false;
};

void Xmlio::Block::add(
			const uint item
		) {
	mask.insert(item);
};

void Xmlio::Block::clear() {
	mask.clear();
};

/******************************************************************************
 class Xmlio::Feeditem
 ******************************************************************************/

Xmlio::Feeditem::Feeditem(
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

bool Xmlio::Feeditem::operator==(
			const Feeditem& comp
		) {
	return ( (Avail == comp.Avail) && (ix == comp.ix) && (ref == comp.ref) && (sref == comp.sref) && (Title1 == comp.Title1) && (Title2 == comp.Title2) && (Title3 == comp.Title3) );
};

bool Xmlio::Feeditem::operator!=(
			const Feeditem& comp
		) {
	return(!operator==(comp));
};

void Xmlio::Feeditem::cap(
			const bool tit1
			, const bool tit2
			, const bool tit3
		) {
	if (tit1) Title1 = StrMod::cap(Title1);
	if (tit2) Title2 = StrMod::cap(Title2);
	if (tit3) Title3 = StrMod::cap(Title3);
};

bool Xmlio::Feeditem::readXML(
			xmlXPathContext* docctx
			, string basexpath
			, bool addbasetag
		) {
	bool basefound;

	if (addbasetag)
		basefound = checkUclcXPaths(docctx, basexpath, basexpath, "Fi");
	else
		basefound = checkXPath(docctx, basexpath);

	if (basefound) {
		//extractBoolUclc(docctx, basexpath, "Avail", "Avail", Avail);
		//extractUintUclc(docctx, basexpath, "ix", "ix", ix);
		//extractUbigintUclc(docctx, basexpath, "ref", "ref", ref);
		extractStringUclc(docctx, basexpath, "sref", "sref", sref);
		extractStringUclc(docctx, basexpath, "Title1", "tit1", Title1);
		extractStringUclc(docctx, basexpath, "Title2", "tit2", Title2);
		extractStringUclc(docctx, basexpath, "Title3", "tit3", Title3);
	};

	return basefound;
};

void Xmlio::Feeditem::writeXML(
			xmlTextWriter* wr
			, unsigned int num
			, string difftag
		) {
	ostringstream str;

	if (difftag.length() == 0) difftag = "Fi";

	xmlTextWriterStartElement(wr, BAD_CAST difftag.c_str());

	str.str(""); str << num;
	xmlTextWriterWriteAttribute(wr, BAD_CAST "num", BAD_CAST str.str().c_str());

	//writeBool(wr, "Avail", Avail);
	//writeUint(wr, "ix", ix);
	//writeUbigint(wr, "ref", ref);
	writeString(wr, "sref", sref);
	writeString(wr, "tit1", Title1);
	writeString(wr, "tit2", Title2);
	writeString(wr, "tit3", Title3);

	xmlTextWriterEndElement(wr);
};

/******************************************************************************
 class Xmlio::Feed
 ******************************************************************************/

Xmlio::Feed::Feed(
			const string& tag
		) {
	this->tag = tag;
};

Xmlio::Feed::Feed(
			const Feed& src
		) {
	Feeditem* item;

	tag = src.tag;

	clear();
	for (unsigned int i=0;i<src.size();i++) {
		item = new Feeditem(*(src.nodes[i]));
		nodes.push_back(item);
	};
};

Xmlio::Feed::~Feed() {
	clear();
};

void Xmlio::Feed::clear() {
	for (unsigned int i=0;i<size();i++) delete nodes[i];
	nodes.resize(0);
};

unsigned int Xmlio::Feed::size() const {
	return(nodes.size());
};

void Xmlio::Feed::appendIxSrefTitles(
			const uint ix
			, const string& sref
			, const string& Title1
			, const string& Title2
			, const string& Title3
		) {
	Feeditem* item = new Feeditem(true, ix, 0, sref, Title1, Title2, Title3);

	nodes.push_back(item);
};

void Xmlio::Feed::appendIxRefSrefTitles(
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

void Xmlio::Feed::appendRefTitles(
			const ubigint ref
			, const string& Title1
			, const string& Title2
			, const string& Title3
		) {
	Feeditem* item = new Feeditem(true, 0, ref, "", Title1, Title2, Title3);

	nodes.push_back(item);
};

void Xmlio::Feed::appendRefSrefTitles(
			const ubigint ref
			, const string& sref
			, const string& Title1
			, const string& Title2
			, const string& Title3
		) {
	Feeditem* item = new Feeditem(true, 0, 0, sref, Title1, Title2, Title3);

	nodes.push_back(item);
};


void Xmlio::Feed::appendTitles(
			const string& Title1
			, const string& Title2
			, const string& Title3
		) {
	Feeditem* item = new Feeditem(true, 0, 0, "", Title1, Title2, Title3);

	nodes.push_back(item);
};

Xmlio::Feeditem* Xmlio::Feed::operator[](
			const uint ix
		) {
	Feeditem* retval = NULL;

	for (unsigned int i=0;i<size();i++) {
		if (nodes[i]->ix == ix) {
			retval = nodes[i];
			break;
		};
	};

	return retval;
};

Xmlio::Feeditem* Xmlio::Feed::getByNum(
			const uint num
		) {
	if ( (num <= size()) && (num > 0) ) {
		return(nodes[num-1]);
	} else {
		return NULL;
	};
};

uint Xmlio::Feed::getNumByIx(
			const uint ix
		) {
	uint retval = 0;

	for (unsigned int i=0;i<size();i++) {
		if (nodes[i]->ix == ix) {
			retval = i + 1;
			break;
		};
	};

	return retval;
};

uint Xmlio::Feed::getNumByRef(
			const ubigint ref
		) {
	uint retval = 0;

	for (unsigned int i=0;i<size();i++) {
		if (nodes[i]->ref == ref) {
			retval = i + 1;
			break;
		};
	};

	return retval;
};

uint Xmlio::Feed::getNumBySref(
			const string& sref
		) {
	uint retval = 0;

	for (unsigned int i=0;i<size();i++) {
		if (nodes[i]->sref == sref) {
			retval = i + 1;
			break;
		};
	};

	return retval;
};

uint Xmlio::Feed::getIxByNum(
			const uint num
		) {
	Feeditem* item = getByNum(num);
	if (item) return item->ix; else return 0;
};

ubigint Xmlio::Feed::getRefByNum(
			const uint num
		) {
	Feeditem* item = getByNum(num);
	if (item) return item->ref; else return 0;
};

string Xmlio::Feed::getSrefByNum(
			const uint num
		) {
	Feeditem* item = getByNum(num);
	if (item) return item->sref; else return "";
};

Xmlio::Feed& Xmlio::Feed::operator=(
			const Feed& src
		) {
	Feeditem* item;

	if (&src != this) {
		tag = src.tag;

		clear();
		for (unsigned int i=0;i<src.size();i++) {
			item = new Feeditem(*(src.nodes[i]));
			nodes.push_back(item);
		};
	};

	return(*this);
};

bool Xmlio::Feed::operator==(
			const Feed& comp
		) {
	bool retval;

	retval = (size() == comp.size());

	if (retval) {
		for (unsigned int i=0;i<size();i++) {
			retval = ( *(nodes[i]) == *(comp.nodes[i]) );

			if (!retval) break;
		};
	};

	return retval;
};

bool Xmlio::Feed::operator!=(
			const Feed& comp
		) {
	return(!operator==(comp));
};

void Xmlio::Feed::cap(
			const bool tit1
			, const bool tit2
			, const bool tit3
		) {
	for (unsigned int i=0;i<size();i++) nodes[i]->cap(tit1, tit2, tit3);
};

bool Xmlio::Feed::readXML(
			xmlXPathContext* docctx
			, string basexpath
			, bool addbasetag
		) {
	bool basefound;

	vector<unsigned int> ics;
	vector<bool> shorttags;

	Feeditem* item = NULL;

	string s;

	if (addbasetag)
		basefound = checkUclcXPaths(docctx, basexpath, basexpath, tag);
	else
		basefound = checkXPath(docctx, basexpath);

	extractList(docctx, basexpath, "Feeditem", "Fi", "num", ics, shorttags);

	clear();

	for (unsigned int i=0;i<ics.size();i++) {
		item = new Feeditem();

		if (shorttags[i]) s = basexpath + "/Fi[@num='" + to_string(ics[i]) + "']";
		else s = basexpath + "/Feeditem[@num='" + to_string(ics[i]) + "']";

		if (item->readXML(docctx, s)) nodes.push_back(item);
		else delete item;
	};

	return basefound;
};

void Xmlio::Feed::writeXML(
			xmlTextWriter* wr
			, string difftag
		) {
	// generate top XML tag
	if (difftag.length() == 0) difftag = tag;
	if (difftag.length() == 0) return;

	// XML output
	xmlTextWriterStartElement(wr, BAD_CAST difftag.c_str());
		for (unsigned int i=0;i<nodes.size();i++) if (nodes[i]->Avail) nodes[i]->writeXML(wr, i+1);
	xmlTextWriterEndElement(wr);
};
