/*
 * ofxPrecisionText
 * Gilbert Sinnott
 * https://github.com/autr/ofxPrecisionText
 *
 * Adapted from:
 *      URL: https://github.com/goldsborough/markdownpp
 *      Date: 30 oct. 2014
 *      Author: cameron
 */

#define PRECISION_BOLD 1
#define PRECISION_ITALIC 2
#define PRECISION_H1 3
#define PRECISION_H2 4
#define PRECISION_H3 5
#define PRECISION_LINK 6

#ifndef SRC_PARSE_MARKDOWN_H_
#define SRC_PARSE_MARKDOWN_H_

#include <string>
#include <regex>

#include "ofxPrecisionText.h"

using namespace std;

struct ofxPrecisionTextRegexRules {
    int type;
    string regex;
};

ofxPrecisionStructure parseMarkdown(string & text, bool asHtml);

int parseLine(string& text, ofxPrecisionStructure & output, bool asHtml, vector<ofxPrecisionTextRegex> & list);

int parse(string & s, int type, vector<ofxPrecisionTextRegex> & list, bool findOnce = false);

int parseRegex( string &s, string &search, string &replace, smatch & match, vector<ofxPrecisionTextRegex> & list, int type = PRECISION_ITALIC);

bool searchHeadersStyle(string& stringToReturn, smatch& match);



vector<ofxPrecisionTextRegex>  parseMatches(string subject, string reg);

vector<int> parseIndexes(string & text, string regex);


vector<string> parseWithIndexes(int fromChar, string text, vector<int> indices);

#endif /* SRC_PARSE_MARKDOWN_H_ */
