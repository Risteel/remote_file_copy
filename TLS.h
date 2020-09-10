#include <iostream>
#include <string>
#include <openssl/ssl.h>
#include <openssl/err.h>
using namespace std;

class TLS 
{
    public:
        TLS(){};
        TLS(string key, string cert, string CA);
        ~TLS() { SSL_CTX_free(ctx);}
        void Configure();
        SSL_CTX* Get();
    private:
        string key,cert,CA;
        const SSL_METHOD *method;
        SSL_CTX  *ctx;
};