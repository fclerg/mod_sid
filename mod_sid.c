/* Include the required headers from httpd */
#include "httpd.h"
#include "http_core.h"
#include "http_protocol.h"
#include "http_request.h"

/* Define constant and prototypes of our functions in this module */
static void register_hooks(apr_pool_t *pool);
static int cookie_handler(request_rec *r);
const char *SIDS_FILE_PATH = “/var/log/sids.log";

/* Define our module as an entity and assign a function for registering hooks  */
module AP_MODULE_DECLARE_DATA   sid_module =
{
    STANDARD20_MODULE_STUFF,
    NULL,            // Per-directory configuration handler
    NULL,            // Merge handler for per-directory configurations
    NULL,            // Per-server configuration handler
    NULL,            // Merge handler for per-server configurations
    NULL,            // Any directives we may have for httpd
    register_hooks   // Our hook registering function
};


static int write_in_file(const char* string){
       FILE *fp;
       fp = fopen(SIDS_FILE_PATH, "a");

       // We associate an epoch time with the added SID
       fprintf(fp, "%s          %lu\n", string, (unsigned long)time(NULL));
       fclose(fp);
       return 0;
}

/* return the extracted SID from the given cookie */
static const char* extract_sid(const char *cookie, request_rec *r){
    char* regex  = "SID=([^;]*)";
    ap_regmatch_t regmatch[2];
    ap_regex_t* compiled_regex;
    compiled_regex = ap_pregcomp(r->pool, regex, AP_REG_EXTENDED );
    ap_regexec(compiled_regex, cookie, 2, regmatch, 0);
    char* sid;
    strncpy(sid, cookie + regmatch[1].rm_so, regmatch[1].rm_eo - regmatch[1].rm_so);
    return sid;
}


/* register_hooks: Adds a hook to the httpd process */
static void register_hooks(apr_pool_t *pool)
{
    /* Hook the request handler */
    ap_hook_handler(cookie_handler, NULL, NULL, APR_HOOK_LAST);
}


/* The handler function for the module.
 */
static int cookie_handler(request_rec *r)
{
    // 'ck-handler' is the name that will be asociated to the directive in https.conf
    if (!r->handler || strcmp(r->handler, "ck-handler")) return (DECLINED);

    /* Set the appropriate content type */
    ap_set_content_type(r, "text/html");

    /* Were we reached through a GET or a POST request ? */
    if ( ! (!strcmp(r->method, "POST") || !strcmp(r->method, "GET")) ) {
        ap_rputs("You did not use POST or GET!<br/>", r);
    }

    // Get the content of the cookie HTTP field of the request
    const char* cookie = apr_table_get(r->headers_in, "Cookie");

    // Extract the SID and stores in in the file
    const char * extracted_sid = extract_sid(cookie, r);
    write_in_file(extracted_sid);
    return OK;
}
