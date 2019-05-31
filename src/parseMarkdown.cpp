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

#include <fstream>
#include <iostream>
#include <string>
#include <regex>

#include "parseMarkdown.h"
#include "ofLog.h"
#include "ofUtils.h"

using namespace std;
/**
 * Bool
 */
bool need_paragraph = true;
bool current_paragraph = false;
bool current_list = false;

/**
 * Regex
 */
//*some tex*
regex italic_regex("[<>a-zA-Z0-9 ]*\\*([a-zA-Z0-9 ]*)\\*[<>a-zA-Z0-9 ]*");
//**some text**
regex bold_regex("\\*\\*(.*)\\*\\*");
//*   Some text
regex list_regex("\\*\t(.*)");
//[some text](URL)
regex url_regex("[^!]*\\[(.*)\\]\\((.*)\\)");
//![some text](URL)
regex img_regex("!\\[(.*)\\]\\((.*)\\)");

/*
 * H1, H2 and H3 style headers regexs
 */
//# some text
regex h1_regex("# (.*)");
//## some text
regex h2_regex("## (.*)");
//### some text
regex h3_regex("### (.*)");

extern bool need_paragraph;
extern bool current_paragraph;
extern bool current_list;


bool sortInts(int a, int b) {
    return a < b;
}

vector<ofxPrecisionTextRegex> parseMatches(string subject, string reg ){
    
    vector<ofxPrecisionTextRegex> results;
    try {
        regex re(reg);
        sregex_iterator next(subject.begin(), subject.end(), re);
        sregex_iterator end;
        while (next != end) {
            smatch match = *next;
            ofxPrecisionTextRegex m;
            m.start = next->position();
            m.size = std::distance(next, end);
            m.match = next->str();
            //            ofLog() << m.match;
            results.push_back(m);
            next++;
        }
    } catch (std::regex_error& e) {
        // Syntax error in the regular expression
        ofLogError("Error in regular expression");
    }
    return results;
}

vector<int> parseIndexes(string & text, string regex) {
    
    vector<int> indexes;
    int iter = 0;
    for (auto & reg : parseMatches(text, regex)) {
        int start = reg.start;
        indexes.push_back(start);
    }
    
    return indexes;
}


vector<string> parseWithIndexes(int fromChar, string text, vector<int> indices) {
    indices.push_back(fromChar);
    indices.push_back(fromChar + text.size());
    ofSort(indices, sortInts);
    vector<string> output;
    string t = text;
    for (int i = 0; i < indices.size() - 1; i++ ) {
        int iA = indices[i] - fromChar;
        int iB = indices[i+1] - fromChar - iA;
        if (iA < text.size()) {
            string sub = t.substr(iA, iB);
            output.push_back( sub );
        }
    }
    return output;
    
}


void generate(string &s, string &regex) {
    
    std::regex r(regex);
    for(std::sregex_iterator i = std::sregex_iterator(s.begin(), s.end(), r);
        i != std::sregex_iterator();
        ++i ) {
        std::smatch m = *i;
        std::cout << m.str() << " at position " << m.position() << " " << m.str().length() << '\n';
        for (int ii = 0; ii < m.size(); ii++) {
        }
        
    }
}

int parseRegex( string &s, string &search, string &replace , smatch & match, vector<ofxPrecisionTextRegex> & list,  int type) {
    
    int removedCounter = 0;
    
    int origSize = search.length();
    
    
    for( size_t pos = 0; ; pos += replace.length() )
    {
        pos = s.find( search, pos );
        if( pos == string::npos )
            break;
        
        ofxPrecisionTextRegex r;
        r.toBeReplaced = search;
        r.originalStart = pos;
        r.start = pos;
        r.size = replace.length();
        r.end = pos + replace.length();
        r.match = match[2];
        r.type = type;
        list.push_back(r);
//
        int off = origSize - replace.size();
        
        for (auto & rr : list) {
            if (r.start < rr.start) {
                rr.start -= off;
            }
            if (r.end < rr.end) {
                rr.end -= off;
            }
        }
        
        s.erase( pos, origSize );
        
        removedCounter += (origSize - replace.size());
        s.insert( pos, replace );

    }
    return removedCounter;
}


ofxPrecisionStructure parseMarkdown(string & source, bool asHtml) {
    
    
    vector<ofxPrecisionTextRegex> list;
    
    ofxPrecisionStructure output;
    output.inSize = source.size();
    output.outSize = 0;
    output.removed = 0;


    /*-- Loop over lines --*/

    std::istringstream f(source);
    string line;
    string outText;
    bool hadNl = ( source.back() == '\n' ); // did it have a newline?

    int outSize = 0;
    while (getline(f, line)) {
        line += '\n';
        int outNew = parseLine(line, output, false, list);
        
        /*-- Parse REGEX candidates to indexes --*/
        
        for (auto & r : list) {
            if (r.type == PRECISION_BOLD) {
                output.bold.push_back(r.start + outSize);
                output.bold.push_back(r.end + outSize);
            }
            if (r.type == PRECISION_LINK) {
                
                ofxPrecisionTextHyperlink link;
                link.start = r.start + outSize;
                link.end = r.end + outSize;
                link.url = r.match;
                output.links.push_back(link);
                
            }
            
            if (r.type == PRECISION_ITALIC) {
                output.italic.push_back(r.start + outSize);
                output.italic.push_back(r.end + outSize);
            }
            if (r.type == PRECISION_H1) {
                output.h1.push_back(r.start + outSize);
                output.h1.push_back(r.end + outSize);
            }
            if (r.type == PRECISION_H2) {
                output.h2.push_back(r.start + outSize);
                output.h2.push_back(r.end + outSize);
            }
            if (r.type == PRECISION_H3) {
                output.h3.push_back(r.start + outSize);
                output.h3.push_back(r.end + outSize);
            }
        }
        
        outSize = outNew;
        list.clear();
    }
    
//    ofLog() << "Total" << output.bold.size();

    /*-- Preserve new lines --*/

    if (!hadNl) {
        output.text = output.text.substr( 0 , output.text.size() - 1);
        output.outSize -= 1;
    }
    
    
    /*-- Check parity of indexes and string lengths --*/
    
    if ((output.text.size() != output.inSize - output.removed) || (output.text.size() != output.outSize)) {
        ofLogError("[ofxPrecisionText]") << "Mismatch in parsed markdown length";
    }

    return output;
}


int parse(string & s, int type, vector<ofxPrecisionTextRegex> & list, bool findOnce) {
    
    int removedCounterForLine = 0;
    regex regex;
    smatch match;
    
    string toReplace = "";
    string search = "";
    
    bool isFoundOnce = false;
    bool asHtml = false;
    if (type == PRECISION_BOLD) regex = bold_regex;
    if (type == PRECISION_ITALIC) regex = italic_regex;
    if (type == PRECISION_LINK) regex = url_regex;
    if (type == PRECISION_H1) regex = h1_regex;
    if (type == PRECISION_H2) regex = h2_regex;
    if (type == PRECISION_H3) regex = h3_regex;
    
    while (regex_search(s, match, regex) && !isFoundOnce) {
        
        if (regex_search(s, match, regex)) {
            if (current_list) {
                current_list = false;
                if (asHtml) s += "</ul>";
            }
            if (need_paragraph) {
                if (asHtml) toReplace += "<p>";
                need_paragraph = false;
                current_paragraph = true;
            }
            if (type == PRECISION_BOLD) {
                
                if (asHtml) toReplace += "<b>";
                toReplace += match[1];
                if (asHtml) toReplace += "</b>";
                search += "**";
                search += match[1];
                search += "**";
            }
            
            if (type == PRECISION_LINK) {
                
                if (asHtml) toReplace +="<a href=\"";
                if (asHtml) toReplace += match[2];
                string url = match[2];
                if (asHtml) toReplace += "\">";
                toReplace += match[1];
                if (asHtml) toReplace += "</a>";
                search += "[";
                search += match[1];
                search += "]";
                search += "(";
                search += match[2];
                search += ")";
            }
            
            if (type == PRECISION_H3) {
                if (asHtml) toReplace += "<h3>";
                toReplace += match[1];
                if (asHtml) toReplace += "</h3>";
                search += "### ";
                search += match[1];
                isFoundOnce = true;
            }
            if (type == PRECISION_H2) {
                if (asHtml) toReplace += "<h2>";
                toReplace += match[1];
                if (asHtml) toReplace += "</h2>";
                search += "## ";
                search += match[1];
                isFoundOnce = true;
            }
            if (type == PRECISION_H1) {
                if (asHtml) toReplace += "<h1>";
                toReplace += match[1];
                if (asHtml) toReplace += "</h1>";
                search += "# ";
                search += match[1];
                isFoundOnce = true;
            }
            
            if (type == PRECISION_ITALIC) {
                if (asHtml) toReplace += "<i>";
                toReplace += match[1];
                if (asHtml) toReplace += "</i>";
                search += "*";
                search += match[1];
                search += "*";
            }
            
            removedCounterForLine += parseRegex(s, search, toReplace, match, list, type);
        }
        toReplace = "";
        search = "";
    }
    
    return removedCounterForLine;
}

int parseLine(string& text, ofxPrecisionStructure & output, bool asHtml, vector<ofxPrecisionTextRegex> & list) {
    
    
    int removedCounterForLine = 0;

    string stringToReturn = text;
    string toReplace;
    string search;
    smatch match;
    
    if (stringToReturn.empty() && current_list) {
        current_list = false;
        stringToReturn += "</ul>";
    }
    
    if ((stringToReturn.empty() == false) && !(regex_search(stringToReturn, match, bold_regex) || regex_search(stringToReturn, match, italic_regex)
                                               || regex_search(stringToReturn, match, url_regex) || searchHeadersStyle(stringToReturn, match)
                                               || regex_search(stringToReturn, match, img_regex)
                                               || regex_search(stringToReturn, match, list_regex))) {
        if (current_list) {
            current_list = false;
            if (asHtml) toReplace += "</ul>";
            toReplace += stringToReturn;
            stringToReturn = toReplace;
        }
        if (need_paragraph) {
            if (asHtml) toReplace += "<p>";
            toReplace += stringToReturn;
            stringToReturn = toReplace;
            need_paragraph = false;
            current_paragraph = true;
        }
    }
    
    if (stringToReturn.empty() && current_paragraph) {
        if (asHtml) stringToReturn += "</p>";
        need_paragraph = true;
        current_paragraph = false;
    }
    bool findOnce = true;
    
    removedCounterForLine += parse(stringToReturn, PRECISION_H3, list, findOnce);
    removedCounterForLine += parse(stringToReturn, PRECISION_H2, list, findOnce);
    removedCounterForLine += parse(stringToReturn, PRECISION_H1, list, findOnce);
    removedCounterForLine += parse(stringToReturn, PRECISION_BOLD, list);
    removedCounterForLine += parse(stringToReturn, PRECISION_LINK, list);
    removedCounterForLine += parse(stringToReturn, PRECISION_ITALIC, list);
    
    
//    while (regex_search(stringToReturn, match, bold_regex) || regex_search(stringToReturn, match, italic_regex)
//           || searchHeadersStyle(stringToReturn, match)
//           || regex_search(stringToReturn, match, img_regex)
//           || regex_search(stringToReturn, match, list_regex)) {
//
//
//        if (regex_search(stringToReturn, match, italic_regex)) {
//            if (current_list) {
//                current_list = false;
//                if (asHtml) toReplace += "</ul>";
//            }
//            if (need_paragraph) {
//                if (asHtml) toReplace += "<p>";
//                need_paragraph = false;
//                current_paragraph = true;
//            }
////            removedCounterForLine += parseRegex(stringToReturn, search, toReplace, match, list, PRECISION_ITALIC);
//        }
//        toReplace = "";
//        search = "";
//        if (regex_search(stringToReturn, match, list_regex)) {
//            if (!current_list) {
//                current_list = true;
//                if (asHtml) toReplace += "<ul>";
//            }
//            if (asHtml) toReplace += "<li>";
//            toReplace += match[1];
//            if (asHtml) toReplace += "</li>";
//            search += "*  ";
//            search += match[1];
//            removedCounterForLine += parseRegex(stringToReturn, search, toReplace, match, list);
//        }
//        toReplace = "";
//        search = "";
//        if (regex_search(stringToReturn, match, img_regex)) {
//            if (current_list) {
//                current_list = false;
//                stringToReturn += "</ul>";
//            }
//            toReplace +="<img src=\"";
//            toReplace += match[2];
//            toReplace += "\"";
//            toReplace += " alt=\"";
//            toReplace += match[1];
//            toReplace += "\">";
//            search += "![";
//            search += match[1];
//            search += "]";
//            search += "(";
//            search += match[2];
//            search += ")";
//            removedCounterForLine += parseRegex(stringToReturn, search, toReplace, match, list);
//        }
//        toReplace = "";
//        search = "";
//    }

    /*-- Funky stuff to check parity of indexes and string lentghs --*/
    
    
    output.outSize += text.size();
    output.outSize -= removedCounterForLine;
    output.removed += removedCounterForLine;
    output.text += stringToReturn;
    
    return output.outSize;
    
}

bool searchHeadersStyle(string& stringToReturn, smatch& match) {
    
    if (regex_search(stringToReturn, match, h1_regex) || regex_search(stringToReturn, match, h2_regex)
        || regex_search(stringToReturn, match, h3_regex))
        return true;
    return false;
    
}

