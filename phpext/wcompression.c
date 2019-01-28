#define app_malloc emalloc
#define app_free efree
#define app_realloc erealloc

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_wcompression.h"

/* {{{ string wcompression_load( [ string $var ] )
 */
PHP_FUNCTION(wcompression_load)
{

    char *file_var;
    size_t file_var_len;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_STRING(file_var, file_var_len)
    ZEND_PARSE_PARAMETERS_END();


}
/* }}}*/

/* {{{ string wcompression_decode( [ string $var ] )
 */
PHP_FUNCTION(wcompression_decode)
{

    char *parameter_var;
    size_t parameter_var_len;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_STRING(parameter_var, parameter_var_len)
    ZEND_PARSE_PARAMETERS_END();


}
/* }}}*/

/* {{{ string wcompression_encode( [ string $var ] )
 */
PHP_FUNCTION(wcompression_encode)
{

    char *parameter_var;
    size_t parameter_var_len;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_STRING(parameter_var, parameter_var_len)
    ZEND_PARSE_PARAMETERS_END();


}
/* }}}*/


/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(wcompression)
{
#if defined(ZTS) && defined(COMPILE_DL_WCOMPRESSION)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	return SUCCESS;
}
/* }}} */



/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(wcompression)
{

    

	

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(wcompression)
{
	

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(wcompression)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "wcompression support", "enabled");
	php_info_print_table_end();
	
}
/* }}} */

/* {{{ arginfo
 */
ZEND_BEGIN_ARG_INFO(arginfo_wcompression_load, 0)
    ZEND_ARG_INFO(0, string_file)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_wcompression_decode, 0)
    ZEND_ARG_INFO(0, string_parameter)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_wcompression_encode, 0)
    ZEND_ARG_INFO(0, string_parameter)
ZEND_END_ARG_INFO()

/* }}} */

/* {{{ wcompression_functions[]
 */
const zend_function_entry wcompression_functions[] = {
    PHP_FE(wcompression_load, arginfo_wcompression_load)
    PHP_FE(wcompression_decode, arginfo_wcompression_decode)
    PHP_FE(wcompression_encode, arginfo_wcompression_encode)
	PHP_FE_END
};

/* }}} */

/* {{{ wcompression_module_entry
 */
zend_module_entry wcompression_module_entry = {
	STANDARD_MODULE_HEADER,
	"wcompression",						/* Extension name */
	wcompression_functions,				/* zend_function_entry */
	PHP_MINIT(wcompression),		/* PHP_MINIT - Module initialization */
	PHP_MSHUTDOWN(wcompression),	/* PHP_MSHUTDOWN - Module shutdown */
	PHP_RINIT(wcompression),				/* PHP_RINIT - Request initialization */
	NULL,								/* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(wcompression),				/* PHP_MINFO - Module info */
	PHP_WCOMPRESSION_VERSION,		/* Version */
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_WCOMPRESSION
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(wcompression)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */