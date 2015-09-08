/* 
 * File:   StringBlockGen.h
 * Author: phil
 *
 * Created on 8. September 2015, 15:33
 */

#ifndef STRINGBLOCKGEN_H
#define	STRINGBLOCKGEN_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class StringBlockGen {
public:
    StringBlockGen(const char* filename, std::string delim):delimiter(delim)
    {
        input.open(filename, std::ifstream::in);
    }
    
    std::vector<std::string> getNextBlock()
    {
        std::vector<std::string> block;
        std::string line;
        while( std::getline(input, line)  && line!=delimiter)
        {
            block.push_back(line);
        }
        return block;
    }
    
private:
    std::string delimiter;
    std::ifstream input;
};

#endif	/* STRINGBLOCKGEN_H */

