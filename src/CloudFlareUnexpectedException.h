#ifndef CLOUDFLAREUNEXPECTEDEXCEPTION_H
#define CLOUDFLAREUNEXPECTEDEXCEPTION_H

#include <exception>

class CloudFlareUnexpectedException : public std::exception
{
    public:
        virtual const char* what() const throw(){
            return "Cloudflare parse error";
        }
};

#endif // CLOUDFLAREUNEXPECTEDEXCEPTION_H
