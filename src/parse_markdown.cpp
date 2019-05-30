/*
 * parse_markdown.cpp
 *
 *  Created on: 30 oct. 2014
 *      Author: cameron
 */

#include <fstream>
#include <iostream>
#include <string>
#include <regex>

#include "parse_markdown.h"
#include "ofLog.h"
#include "ofUtils.h"

using namespace std;

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

int str_replace( string &s, string &search, string &replace , smatch & match) {
    
    int rmIter = 0;
    
    
    for( size_t pos = 0; ; pos += replace.length() )
    {
        pos = s.find( search, pos );
        if( pos == string::npos )
            break;
        
        rmIter += (search.length() - replace.size());
        s.erase( pos, search.length() );
        s.insert( pos, replace );
    }
    return rmIter;
}

bool search_headers_style(string& stringToReturn, smatch& match);

void verify_bold() {
    
}

ofxPrecisionTextStructure parsetext(string & source, bool asHtml) {
    ofxPrecisionTextStructure output;
    output.inSize = source.size();
    output.outSize = 0;
    output.removed = 0;
    std::istringstream f(source);
    string line;
    string outText;
    bool hadNl = ( source.back() == '\n' );
    while (getline(f, line)) {
        line += '\n';
        parseline(line, output, false);
    }
    
    if (!hadNl) {
        output.text = output.text.substr( 0 , output.text.size() - 1);
        output.outSize -= 1;
    }
    
    if ((output.text.size() != output.inSize - output.removed) || (output.text.size() != output.outSize)) {
        ofLogError("[ofxPrecisionText]") << "Mismatch in parsed markdown length";
    }
    return output;
}

void parseline(string& text, ofxPrecisionTextStructure & output, bool asHtml) {
    
    
    int rmv = 0;
    string stringToReturn = text;
    
    string toReplace;
    
    string search;
    
    smatch match;
    
    if (stringToReturn.empty() && current_list) {
        current_list = false;
        stringToReturn += "</ul>";
    }
    
    if ((stringToReturn.empty() == false) && !(regex_search(stringToReturn, match, bold_regex) || regex_search(stringToReturn, match, italic_regex)
                                               || regex_search(stringToReturn, match, url_regex) || search_headers_style(stringToReturn, match)
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
    
    while (regex_search(stringToReturn, match, bold_regex) || regex_search(stringToReturn, match, italic_regex)
           || regex_search(stringToReturn, match, url_regex) || search_headers_style(stringToReturn, match)
           || regex_search(stringToReturn, match, img_regex)
           || regex_search(stringToReturn, match, list_regex)) {
        if (regex_search(stringToReturn, match, bold_regex)) {
            if (current_list) {
                current_list = false;
                if (asHtml) stringToReturn += "</ul>";
            }
            if (need_paragraph) {
                if (asHtml) toReplace += "<p>";
                need_paragraph = false;
                current_paragraph = true;
            }
            if (asHtml) toReplace += "<b>";
            toReplace += match[1];
            if (asHtml) toReplace += "</b>";
            search += "**";
            search += match[1];
            search += "**";
            rmv += str_replace(stringToReturn, search, toReplace, match);
            int a = match.position() + output.outSize;
            int b = match.position() + toReplace.size() + output.outSize;
            if (a != b) {
                output.bold.push_back(a);
                output.bold.push_back(b);
            }
        }
        toReplace = "";
        search = "";
        if (regex_search(stringToReturn, match, italic_regex)) {
            if (current_list) {
                current_list = false;
                if (asHtml) toReplace += "</ul>";
            }
            if (need_paragraph) {
                if (asHtml) toReplace += "<p>";
                need_paragraph = false;
                current_paragraph = true;
            }
            if (asHtml) toReplace += "<i>";
            toReplace += match[1];
            if (asHtml) toReplace += "</i>";
            search += "*";
            search += match[1];
            search += "*";
            rmv += str_replace(stringToReturn, search, toReplace, match);
            int a = match.position() + output.outSize;
            int b = match.position() + toReplace.size() + output.outSize;
            if (a != b) {
                output.italic.push_back(a);
                output.italic.push_back(b);
            }
        }
        toReplace = "";
        search = "";
        if (regex_search(stringToReturn, match, list_regex)) {
            if (!current_list) {
                current_list = true;
                if (asHtml) toReplace += "<ul>";
            }
            if (asHtml) toReplace += "<li>";
            toReplace += match[1];
            if (asHtml) toReplace += "</li>";
            search += "*  ";
            search += match[1];
            rmv += str_replace(stringToReturn, search, toReplace, match);
        }
        toReplace = "";
        search = "";
        if (regex_search(stringToReturn, match, img_regex)) {
            if (current_list) {
                current_list = false;
                stringToReturn += "</ul>";
            }
            toReplace +="<img src=\"";
            toReplace += match[2];
            toReplace += "\"";
            toReplace += " alt=\"";
            toReplace += match[1];
            toReplace += "\">";
            search += "![";
            search += match[1];
            search += "]";
            search += "(";
            search += match[2];
            search += ")";
            rmv += str_replace(stringToReturn, search, toReplace, match);
        }
        toReplace = "";
        search = "";
        if (regex_search(stringToReturn, match, url_regex)) {
            if (current_list) {
                current_list = false;
                stringToReturn += "</ul>";
            }
            if (need_paragraph) {
                if (asHtml) toReplace += "<p>";
                need_paragraph = false;
                current_paragraph = true;
            }
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
            rmv += str_replace(stringToReturn, search, toReplace, match);
            ofxPrecisionTextHyperlink link;
            link.start = match.position() + output.outSize;
            link.end = match.position() + toReplace.size() + output.outSize;
            link.url = url;
            output.links.push_back(link);
        }
        toReplace = "";
        search = "";
        if (search_headers_style(stringToReturn, match)) {
            if (current_list) {
                current_list = false;
                stringToReturn += "</ul>";
            }
            if (regex_search(stringToReturn, match, h3_regex)) {
                if (asHtml) toReplace += "<h3>";
                toReplace += match[1];
                if (asHtml) toReplace += "</h3>";
                search += "### ";
                search += match[1];
                rmv += str_replace(stringToReturn, search, toReplace, match);
                output.h3.push_back(match.position() + output.outSize);
                output.h3.push_back(match.position() + toReplace.size() + output.outSize);
            }
            if (regex_search(stringToReturn, match, h2_regex)) {
                if (asHtml) toReplace += "<h2>";
                toReplace += match[1];
                if (asHtml) toReplace += "</h2>";
                search += "## ";
                search += match[1];
                rmv += str_replace(stringToReturn, search, toReplace, match);
                output.h2.push_back(match.position() + output.outSize);
                output.h2.push_back(match.position() + toReplace.size() + output.outSize);
            }
            if (regex_search(stringToReturn, match, h1_regex)) {
                if (asHtml) toReplace += "<h1>";
                toReplace += match[1];
                if (asHtml) toReplace += "</h1>";
                search += "# ";
                search += match[1];
                rmv += str_replace(stringToReturn, search, toReplace, match);
                output.h1.push_back(match.position() + output.outSize);
                output.h1.push_back(match.position() + toReplace.size() + output.outSize);
            }
        }
    }
    
    
    output.outSize += text.size();
    output.outSize -= rmv;
    output.removed += rmv;
    output.text += stringToReturn;
    
    return output;
    
}

bool search_headers_style(string& stringToReturn, smatch& match) {
    
    if (regex_search(stringToReturn, match, h1_regex) || regex_search(stringToReturn, match, h2_regex)
        || regex_search(stringToReturn, match, h3_regex))
        return true;
    return false;
    
}

