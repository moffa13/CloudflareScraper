#ifndef CLOUDFLAREUNEXPECTEDEXCEPTION_H
#define CLOUDFLAREUNEXPECTEDEXCEPTION_H

#include <exception>
#include <stdexcept>

class CloudflareException : public std::exception
{
    public:
        explicit CloudflareException( const std::string& msg ) : _message(msg){}
        explicit CloudflareException( const char* msg ) : _message(msg){}

        virtual const char* what() const throw(){
            return _message.c_str();
        }
    private:
        std::string _message;
};

#endif // CLOUDFLAREUNEXPECTEDEXCEPTION_H
