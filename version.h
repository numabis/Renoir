#define APS_MAJORNUMBER          0
#define APS_MINORNUMBER          9
#define APS_BUILDNUMBER          2
#define APS_MODIFICATIONNUMBER   0

#define STR_HELPER(x) #x
#define STRH(x) STR_HELPER(x)

#define MY_PRODUCT_FILE_VERSION APS_MAJORNUMBER , APS_MINORNUMBER , APS_BUILDNUMBER , APS_MODIFICATIONNUMBER
#define MY_PRODUCT_FILE_VERSION2 STRH(APS_MAJORNUMBER) "." STRH(APS_MINORNUMBER) "." STRH(APS_BUILDNUMBER) "." STRH(APS_MODIFICATIONNUMBER)
#define MY_PRODUCT_VERSION STR(APS_MAJORNUMBER) "." STRH(APS_MINORNUMBER) STRH(APS_BUILDNUMBER) STRH(APS_MODIFICATIONNUMBER)
#define MY_FILE_VERSION		MY_PRODUCT_FILE_VERSION
#define MY_FILE_VERSION2	MY_PRODUCT_FILE_VERSION2
