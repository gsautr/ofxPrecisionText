/*
 * parse_markdown.h
 *
 *  Created on: 30 oct. 2014
 *      Author: cameron
 */

#ifndef SRC_PARSE_MARKDOWN_H_
#define SRC_PARSE_MARKDOWN_H_

#include <string>
#include <regex>

#include "ofxPrecisionText.h"

using namespace std;


ofxPrecisionTextStructure parsetext(string & text, bool asHtml);

void parseline(string& text, ofxPrecisionTextStructure & output, bool asHtml);

int str_replace( string &s, string &search, string &replace , smatch & match);

bool search_headers_style(string& stringToReturn, smatch& match);

#endif /* SRC_PARSE_MARKDOWN_H_ */
