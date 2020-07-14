#include "udf.h"
#include "stdio.h"


real kount;  /* define global variable kount */

DEFINE_RW_FILE(writer,fp)
{
	/* print to file using either float or long float (double) format, depending on data type */
	#if !RP_NODE
		if (sizeof(kount) == 4)
		{
			fprintf(fp, "(kount = %f)\n", kount);
			Message("kount = %f, type = float. \n",kount); 
		}
		else
		{
			fprintf(fp, "(kount = %lf)\n", kount);
			Message("kount = %lf, type = double. \n",kount);		
		}
	#endif
}   

DEFINE_RW_FILE(reader,fp)
{
	/* print to file using either float or long float (double) format, depending on data type */
	#if !RP_NODE  
		if (sizeof(kount) == 4)
		{
			fscanf(fp, "(kount = %f)\n", &kount);
			Message("kount = %f, type = float. \n",kount);
		}
		else
		{
			fscanf(fp, "(kount = %lf)\n", &kount);
			Message("kount = %lf, type = double. \n",kount);		
		}
	#endif
}

DEFINE_ON_DEMAND(initialize)
{
	#if !RP_NODE 
		kount = 0.;
		Message("kount = %f \n",kount); /* display value using float format */
	#endif
}

DEFINE_ON_DEMAND(print_kount)
{
	#if !RP_NODE 
		Message("kount = %f \n",kount); /* display value using float format */
	#endif

}

DEFINE_ON_DEMAND(increment)
{
	#if !RP_NODE 
		kount = kount + 1.;
		Message("kount = %f \n",kount); /* display value using float format */
	#endif
}
