#include <stdio.h>
#include <stdlib.h>

typedef struct 	
{
	const char *sym;
	const double mass;
	int min,	
		max,	
		guess;	
} 
Element;

Element el[]=
	{{ "H",   1.0078250321, 0, 200, 0},
	{ "C",  12.000000000,  0, 100, 0},
	{ "N",  14.0030740052, 0, 6, 0},	
	{ "O",  15.9949146221, 0, 20, 0},
	{ "P",  30.97376151,   0, 4, 0},
	{ "S",  31.97207069,   0, 4, 0},
	{"Na", 22.989770, 0, 0, 0},
	{"K", 38.963708, 0, 0,0}		
};

int     nr_el,fnd,ntest;	
double  calc_mass(void);
long    do_calculations(double mass, double tolerance);


int main (void)
{
	char *data;
	double mz,tol;	
	int i;
	long charge,minh,maxh,minc,maxc,minn,maxn,mino,maxo,mins,maxs,minp,maxp,minna,maxna,mink,maxk;

	double electron=0.0005485799094;
	nr_el = sizeof(el)/sizeof(el[1]);	
	printf("%s%c%c\n","Content-Type:text/html;charset=iso-8859-1",13,10);
	data = getenv("QUERY_STRING");
	if(data == NULL)
		printf("<P>Error! Error in passing data from form to script.");
	else if(sscanf(data,"mass=%lf&tol=%lf",&mz,&tol)!=2)
		printf("<P>Error! Invalid data. Data must be numeric.");
	else
	{

// http://127.0.0.1/~bpb/cgi-bin/BruteForceFormulae.cgi?
// mass=506.995745&tol=0.01&charge=0
// &minh=0&maxh=1
// &minc=0&maxc=2
// &minn=0&maxn=3
// &mino=0&maxo=4
// &minp=0&maxp=5
// &mins=0&maxs=6
// &minna=0&maxna=7
// &mink=0&maxk=8

		if (sscanf(data,"mass=%lf&tol=%lf&charge=%ld&minh=%ld&maxh=%ld&minc=%ld&maxc=%ld&minn=%ld&maxn=%ld&mino=%ld&maxo=%ld&minp=%ld&maxp=%ld&mins=%ld&maxs=%ld&minna=%ld&maxna=%ld&mink=%ld&maxk=%ld",&mz,&tol,&charge,&minh,&maxh,&minc,&maxc,&minn,&maxn,&mino,&maxo,&minp,&maxp,&mins,&maxs,&minna,&maxna,&mink,&maxk) == 19)
		{
			el[0].min=minh;
			el[0].max=maxh;

			el[1].min=minc;
			el[1].max=maxc;

			el[2].min=minn;
			el[2].max=maxn;

			el[3].min=mino;
			el[3].max=maxo;

			el[4].min=minp;
			el[4].max=maxp;

			el[5].min=mins;
			el[5].max=maxs;

			el[6].min=minna;
			el[6].max=maxna;

			el[7].min=mink;
			el[7].max=maxk;

	

		// might as well make bounds on mass too

			for (i=0;i<nr_el;i++)
			{
				if (el[i].min<0 || el[i].max>400)
				{
					printf("please supply realistic constraints");
					exit(1);
				}
			}

			if (charge!=0 && charge!=1 && charge!=2 && charge!=3 && charge!=-1 && charge!=-2 && charge!=-3)
			{
				printf("please supply reaslistic constraints");
				exit(1);
			}

			mz=mz+charge*electron;
		// tol=5;
			tol=tol*mz/1e6;
			ntest=0;
			fnd=0;

			if (mz>2000 || tol>1)
			{
				printf("please supply reaslistic MASS and TOLERANCE constraints");
				exit(1);
			}
			printf("Target Mass = %lf<br>Uncertainty = %lf<br>",mz-charge*electron,tol);
			printf("<br>Elemental Constraints<br>");
			for (i=0;i<nr_el;i++)
				printf("For %s, the Minima is %d and the max is %d<br>",el[i].sym,el[i].min,el[i].max);
			printf("///<br>");
			do_calculations(mz, tol);       
		}
		printf("\n///<br>%d were found from %d tests",fnd,ntest);
	}
	return 0;
}

double calc_mass(void)
{
	int i;
	double sum = 0.0;
	for (i=0; i < nr_el; i++)
		sum += el[i].mass * el[i].guess;
	return (sum);
}

long do_calculations (double mz, double tol)
{

	double mass;			/* calc'd mass */
	double limit_lo, limit_hi;	/* mass limits */
	long i;			

	limit_lo = mz - tol;
	limit_hi = mz + tol;
	for (el[7].guess=el[7].min;el[7].guess<=el[7].max;el[7].guess++) 
	{ 
		for (el[6].guess=el[6].min;el[6].guess<=el[6].max;el[6].guess++) 
		{
			for (el[5].guess=el[5].min;el[5].guess<=el[5].max;el[5].guess++) 
			{ 
				for (el[4].guess=el[4].min;el[4].guess<=el[4].max;el[4].guess++) 
				{
					for (el[3].guess=el[3].min;el[3].guess<=el[3].max;el[3].guess++) 
					{ 
						for (el[2].guess=el[2].min;el[2].guess<=el[2].max;el[2].guess++) 
						{
							for (el[1].guess=el[1].min;el[1].guess<=el[1].max;el[1].guess++) 
							{ 
								for (el[0].guess=el[0].min;el[0].guess<=el[0].max;el[0].guess++) 
								{ 
									ntest=ntest+1;
									if (ntest>1e8)
									{
										printf("Limit Reached.  I will not continue your job.");
										exit(1);
									}
									mass = calc_mass();
									if ((mass >= limit_lo) && (mass <= limit_hi)) 
									{												
										fnd=fnd+1;
										printf("%5.4f,",mass);
										for (i = 0; i < nr_el; i++)			
											if (el[i].guess > 0)			
											printf("%s%d", el[i].sym, el[i].guess);	
										// printf("<br>");	
										printf(";<br>");
									}
									if (mass>limit_hi) break; //Hydrogen
								} 
								if (mass>limit_hi && el[0].guess==el[0].min) break;
							} 
					// if (mass>limit_hi) break;
							if (mass>limit_hi && el[1].guess==el[1].min) break;
						} 
				// if (mass>limit_hi) break;
						if (mass>limit_hi && el[2].guess==el[2].min) break;
					} 
			// if (mass>limit_hi) break;
					if (mass>limit_hi && el[3].guess==el[3].min) break;
				}
		// if (mass>limit_hi) break;
				if (mass>limit_hi && el[4].guess==el[4].min) break;
			}
		// if (mass>limit_hi) break;
			if (mass>limit_hi && el[5].guess==el[5].min) break;
		}
	// if (mass>limit_hi) break;

		if (mass>limit_hi && el[6].guess==el[6].min) break;
	}
	return (0);

}



