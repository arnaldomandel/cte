#include <unistd.h>
#include <err.h>
int erropt = 0;
#define IOPT(c,text,var) 	case c:\
	    if(!optarg) {\
		erropt = 1;\
		warn("Missing " text "\n");\
	    }\
	    else\
		var = atoi(optarg);\
	    break
#define FOPT(c,text,var) 	case c:\
	    if(!optarg) {\
		erropt = 1;\
		warn("Missing " text "\n");\
	    }\
	    else\
		var = atof(optarg);\
	    break
#define SOPT(c,text,var) 	case c:\
	    if(!optarg) {\
		erropt = 1;\
		warn("Missing " text "\n");\
	    }\
	    else\
		var = optarg;		\
	    break
//		var = strdup(optarg);	
//	    break
#define NOPT(c,var)	 	case c:\
	      var = 1;		\
	    break
