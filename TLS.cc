#include "TLS.h"

TLS::TLS(string key, string cert, string CA)
{
    SSL_load_error_strings();	
    OpenSSL_add_ssl_algorithms();
    this->key = key;
    this->cert = cert;
    this->CA = CA;
    method = TLSv1_2_method();
    ctx = SSL_CTX_new(method);
    if (!ctx)
    {
        ERR_print_errors_fp(stderr);
        exit(1);
    }
}
void TLS::Configure()
{
    //load certificate
    static unsigned char ssl_session_ctx_id = 1;
    if (SSL_CTX_use_certificate_file(ctx, cert.c_str(), SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stderr);
        exit(1);
    }
    //load key
    if (SSL_CTX_use_PrivateKey_file(ctx, key.c_str(), SSL_FILETYPE_PEM) <= 0)
    {
        ERR_print_errors_fp(stderr);
        exit(1);
    }
    //check private key and public key
    if (!SSL_CTX_check_private_key(ctx))
    {
        fprintf(stderr, "Private key does not match the certificate");
        exit(1);
    }
    //load CA
    if (!SSL_CTX_load_verify_locations(ctx, CA.c_str(), NULL))
    {
        ERR_print_errors_fp(stderr);
        exit(1);
    }
    //verify client
    SSL_CTX_set_session_id_context(ctx, &ssl_session_ctx_id, sizeof(ssl_session_ctx_id));
    SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, NULL);
    SSL_CTX_set_verify_depth(ctx, 1);
}
SSL_CTX* TLS::Get()
{
    return ctx;
}
