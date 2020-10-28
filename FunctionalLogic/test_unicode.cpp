#include<iostream>

#include<regex>


bool test_unicode()
{
    std::locale old;
    std::locale::global(std::locale("en_US.UTF-8"));

    std::regex pattern("[[:alpha:]]+", std::regex_constants::extended);
    bool result = std::regex_match(std::string("abcdéfg"), pattern);

    std::locale::global(old);

    return result;
}



int main()
{
        if(test_unicode)
        std::cout<<"is unicode"<<std::endl;
        else
        {
            std::cout<<"not nuf-8"<<std::endl;
        }
        
}