/* shell.c  -  shell */

//#include <xinu.h>
#include <pipe.h>
#include <stdio.h>
#include <stdlib.h>
#include "shprototypes.h"

/************************************************************************/
/* Table of Xinu shell commands and the function associated with each	*/
/************************************************************************/
const	struct	cmdent	cmdtab[] = {
	{"argecho",	xsh_argecho},
	{"arp",		xsh_arp},
	{"cat",		xsh_cat},
	{"clear",	xsh_clear},
	{"date",	xsh_date},
	{"devdump",	xsh_devdump},
	{"echo",	xsh_echo},
	{"exit",	xsh_exit},
	{"help",	xsh_help},
	{"kill",	xsh_kill},
	{"ls",		xsh_ls},
	{"memdump",	xsh_memdump},
	{"memstat",	xsh_memstat},
	{"netinfo",	xsh_netinfo},
	{"ns",		xsh_ns},
	{"ping",	xsh_ping},
	{"ps",		xsh_ps},
	{"sleep",	xsh_sleep},
	{"udp",		xsh_udpdump},
	{"udpecho",	xsh_udpecho},
	{"udpeserver",	xsh_udpeserver},
	{"uptime",	xsh_uptime},
	{"?",		xsh_help}

};

uint32	ncmd = sizeof(cmdtab) / sizeof(struct cmdent);


/* initializing segtab */

struct seginfo segtab[NSEGS];



/************************************************************************/
/* shell  -  Provide an interactive user interface that executes	*/
/*	     commands.  Each command begins with a command name, has	*/
/*	     a set of optional arguments, has optional input or		*/
/*	     output redirection, and an optional specification for	*/
/*	     background execution (ampersand).  The syntax is:		*/
/*									*/
/*		   command_name [args*] [redirection] [&]		*/
/*									*/
/*	     Redirection is either or both of:				*/
/*									*/
/*				< input_file				*/
/*			or						*/
/*				> output_file				*/
/*									*/
/************************************************************************/

process	shell (
		did32	dev		/* ID of tty device from which	*/
	)				/*   to accept commands		*/
{
	char	buf[SHELL_BUFLEN];	/* Input line (large enough for	*/
					/*   one line from a tty device	*/
	int32	len;			/* Length of line read		*/
	char	tokbuf[SHELL_BUFLEN +	/* Buffer to hold a set of	*/
			SHELL_MAXTOK];  /* Contiguous null-terminated	*/
					/* Strings of tokens		*/
	int32	tlen;			/* Current length of all data	*/
					/*   in array tokbuf		*/
	int32	tok[SHELL_MAXTOK];	/* Index of each token in	*/
					/*   array tokbuf		*/
	int32	toktyp[SHELL_MAXTOK];	/* Type of each token in tokbuf	*/
	int32	ntok;			/* Number of tokens on line	*/
	pid32	child;			/* Process ID of spawned child	*/
	bool8	backgnd;		/* Run command in background?	*/
	char	*outname, *inname;	/* Pointers to strings for file	*/
					/*   names that follow > and <	*/
	did32	stdinput, stdoutput;	/* Descriptors for redirected	*/
					/*   input and output		*/
	int32	i;			/* Index into array of tokens	*/
	int32	j;			/* Index into array of commands	*/
	int32	msg;			/* Message from receive() for	*/
					/*   child termination		*/
	int32	tmparg;			/* Address of this var is used	*/
					/*   when first creating child	*/
					/*   process, but is replaced	*/
	char	*src, *cmp;		/* Pointers used during name	*/
					/*   comparison			*/
	bool8	diff;			/* Was difference found during	*/
					/*   comparison			*/
	char	*args[SHELL_MAXTOK];	/* Argument vector passed to	*/
					/*   builtin commands		*/

	

	/* Print shell banner and startup message */

	fprintf(dev, "\n\n%s%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
		SHELL_BAN0,SHELL_BAN1,SHELL_BAN2,SHELL_BAN3,SHELL_BAN4,
		SHELL_BAN5,SHELL_BAN6,SHELL_BAN7,SHELL_BAN8,SHELL_BAN9);

	fprintf(dev, "%s\n\n", SHELL_STRTMSG);

	/* Continually prompt the user, read input, and execute command	*/

	while (TRUE) {

		/* Display prompt */

		fprintf(dev, SHELL_PROMPT);

		/* Read a command */

		len = read(dev, buf, sizeof(buf));

		/* Exit gracefully on end-of-file */

		if (len == EOF) {
			break;
		}

		/* If line contains only NEWLINE, go to next line */

		if (len <= 1) {
			continue;
		}

		buf[len] = SH_NEWLINE;	/* terminate line */

		/* Parse input line and divide into tokens */

		ntok = lexan(buf, len, tokbuf, &tlen, tok, toktyp);


		/* Debugging lexan changes */

		//kprintf("start of debugging with ntok of %d and tokbuf is %s\n", ntok, tokbuf);
		

		
		int32 z = 0;
		/*
		for (z = 0; z < ntok; z++) {
			//kprintf("%d\n", toktyp[z]);
			kprintf("%s\n", tokbuf + tok[z]);
		

		}
		*/

		//kprintf("value of CONSOLE is %d\n", CONSOLE);
		
		/* additions for new algorithm */
		for (z = 0; z < NSEGS; z++) {
			segtab[z].inputd = dev;
			segtab[z].outputd = dev;
			
			//kprintf("inputd is %d and outputd is %d\n", segtab[z].inputd, segtab[z].outputd);
				
		}

		


		

		/* Handle parsing error */

		if (ntok == SYSERR) {
			fprintf(dev,"%s\n", SHELL_SYNERRMSG);
			continue;
		}

		/* If line is empty, go to next input line */

		if (ntok == 0) {
			fprintf(dev, "\n");
			continue;
		}


		/* If last token is '&', set background */

		if (toktyp[ntok-1] == SH_TOK_AMPER) {
			ntok-- ;
			tlen-= 2;
			backgnd = TRUE;
		} else {
			backgnd = FALSE;
		}






		
		/* start of handle pipes */
		int32 temp = 0;
		int32 npipes = 0;
		int32 pipeindex[NSEGS];
		int32 pipeincur = 0;
		int32 nsegs = 0;
		
		while (temp < ntok) {
			if (toktyp[temp] == SH_TOK_PIPE) {
				npipes++;
				pipeindex[pipeincur] = tok[temp];
				pipeincur++;
			}
			
			temp++;
		}
		

		//kprintf("number of pipes is %d\n", npipes);
		
		/* handles when there are more segments than NSEGS */
		nsegs = npipes + 1;

		if (nsegs > NSEGS) {
			kprintf("There are more segments than the number that can be handled\n");
			continue;
		}		


		/* handles pipe usage */

		
		int32 segin[nsegs];
		//int32 segind = 0;
		
		//int32 p = open(PIPE, "", 0);
		int32 pipeD[NSEGS];
		
		if (npipes != 0) {

			/* all segments before last */
			for (temp = 0; temp < nsegs - 1; temp++) {
				if (temp == 0) {
					segtab[temp].firstin = 0;
				} else {
					segtab[temp].firstin = pipeindex[temp - 1] + 2;
				}
				
				segtab[temp].lastin = pipeindex[temp];
				//segind++;
				segtab[temp].firstin = segtab[temp].firstin + 1 - (2 * temp);
				//segind++;
				//kprintf("first is %d and last is %d\n", segtab[temp].firstin, segtab[temp].lastin);
				
			}

			
			/* last segment */
			segtab[temp].firstin = pipeindex[temp - 1] + 3 - (2 * temp);
			segtab[temp].lastin = tlen - 1;

			//kprintf("first is %d and last is %d\n", segtab[temp].firstin, segtab[temp].lastin);

			
			/* open npipes pipes */
			struct pipecblk *piptr;
			for (temp = 0; temp < npipes; temp++) {
				pipeD[temp] = open(PIPE, "", 0);
				segtab[temp].outputd = pipeD[temp];
				segtab[temp + 1].inputd = pipeD[temp];
			}

						

			//int32 rand = 0;
			int32 segni = 0;
			char *blank;
			blank = " ";
			
			struct seginfo *segptr;

			/* original values */
			char otokbuf[SHELL_BUFLEN + SHELL_MAXTOK];
			int32 otlen;
			int32 otok[SHELL_MAXTOK];
			int32 otoktyp[SHELL_MAXTOK];
			int32 ontok;
			

			for (temp = 0; temp < nsegs; temp++) {
				
				char *toktemp;
				
				toktemp = &tokbuf[0] + tok[segni];
				

				segptr = &segtab[temp];
				
				segptr->segname = " ";
				
				while (toktemp[0] != '|' && segni < ntok) {
					
					strncat(segptr->segname, toktemp, sizeof(toktemp) + 1);
					strncat(segptr->segname, blank, 1);
					segni++;
					toktemp = &tokbuf[0] + tok[segni];
					
				}
				
				
				segni++;

				
				//kprintf("\nsegment is %s at temp %d\n\n", segtab[temp].segname + segtab[temp].firstin, temp);
	
				/* new ntok implementation */

				int32 size = segtab[temp].lastin - segtab[temp].firstin + 1;

				ontok = lexan(segtab[temp].segname + segtab[temp].firstin, size, otokbuf, &otlen, otok, otoktyp);


				/* Check for input/output redirection (default is none) */

				outname = inname = NULL;
				if ( (ontok >=3) && ( (otoktyp[ontok-2] == SH_TOK_LESS)
					   ||(otoktyp[ontok-2] == SH_TOK_GREATER))){
					if (otoktyp[ontok-1] != SH_TOK_OTHER) {
						fprintf(dev,"%s\n", SHELL_SYNERRMSG);
						continue;
					}
					if (otoktyp[ontok-2] == SH_TOK_LESS) {
						inname =  &otokbuf[otok[ontok-1]];
					} else {
						outname = &otokbuf[otok[ontok-1]];
					}
					ontok -= 2;
					otlen = otok[ontok];
				}


				if ( (ontok >=3) && ( (otoktyp[ontok-2] == SH_TOK_LESS)
				   ||(otoktyp[ontok-2] == SH_TOK_GREATER))){
					if (otoktyp[ontok-1] != SH_TOK_OTHER) {
						fprintf(dev,"%s\n", SHELL_SYNERRMSG);
						continue;
					}
					if (otoktyp[ontok-2] == SH_TOK_LESS) {
						if (inname != NULL) {
				    			fprintf(dev,"%s\n", SHELL_SYNERRMSG);
				    			continue;
						}
						inname = &otokbuf[otok[ontok-1]];
					} else {
						if (outname != NULL) {
				    			fprintf(dev,"%s\n", SHELL_SYNERRMSG);
				    			continue;
						}
						outname = &otokbuf[otok[ontok-1]];
					}
					ontok -= 2;
					otlen = otok[ontok];
				}

				/* Verify remaining tokens are type "other" */

				for (i=0; i<ontok; i++) {
					if (otoktyp[i] != SH_TOK_OTHER) {
						break;
					}
				}
				if ((ontok == 0) || (i < ontok)) {
					fprintf(dev, SHELL_SYNERRMSG);
					continue;
				}


				/* change to pipe device stuff */
				stdinput = stdoutput = dev;

				/* Lookup first token in the command table */
				
				for (j = 0; j < ncmd; j++) {
					src = cmdtab[j].cname;

					cmp = otokbuf;
					diff = FALSE;
					while (*src != NULLCH) {
						if (*cmp != *src) {
							diff = TRUE;
							break;
						}
						src++;
						cmp++;
					}
					if (diff || (*cmp != NULLCH)) {
						continue;
					} else {
						break;
					}
				}

				/* Handle command not found */

				if (j >= ncmd) {
					fprintf(dev, "command %s not found\n", otokbuf);
					continue;
				}

			

				/* Open files and redirect I/O if specified */

				if (inname != NULL) {
					stdinput = open(NAMESPACE,inname,"ro");
					if (stdinput == SYSERR) {
						fprintf(dev, SHELL_INERRMSG, inname);
						continue;
					}
				}
				if (outname != NULL) {
					stdoutput = open(NAMESPACE,outname,"w");
					if (stdoutput == SYSERR) {
						fprintf(dev, SHELL_OUTERRMSG, outname);
						continue;
					} else {
						control(stdoutput, F_CTL_TRUNC, 0, 0);
					}
				}

				/* Spawn child thread for non-built-in commands */

				child = create(cmdtab[j].cfunc,
					SHELL_CMDSTK, SHELL_CMDPRIO,
					cmdtab[j].cname, 2, ontok, &tmparg);

				/* If creation or argument copy fails, report error */

				if ((child == SYSERR) ||
				    (addargs(child, ontok, otok, otlen, otokbuf, &tmparg)
									== SYSERR) ) {
					fprintf(dev, SHELL_CREATMSG);
					continue;
				}

				/* Set stdinput and stdoutput in child to redirect I/O */

				//possible for pipes?

				proctab[child].prdesc[0] = stdinput;
				proctab[child].prdesc[1] = stdoutput;
				

				
				//proctab[child].prdesc[0] = segtab[handseg].inputd;
				//proctab[child].prdesc[1] = segtab[handseg].outputd;		



				msg = recvclr();
				resume(child);
				if (! backgnd) {
					msg = receive();
					while (msg != child) {
						msg = receive();
					}
				}
		
    			}



			

			

			//kprintf("one or more pipes \n");
			
				
			
			continue;

		}	


		/* use lexan for each segment and carry out the command */
		

		//int32 size;
		//size = segtab[handseg].lastin - segtab[handseg].firstin + 1;
				
		//kprintf("seginfo string is %s\n", segtab[handseg].segname + segtab[handseg].firstin);
		//ntok = lexan(segtab[handseg].segname + segtab[handseg].firstin, size, tokbuf, &tlen, tok, toktyp);
		
		
		//kprintf("new ntok is %d\n", ntok);
		/* Check for input/output redirection (default is none) */

		outname = inname = NULL;
		if ( (ntok >=3) && ( (toktyp[ntok-2] == SH_TOK_LESS)
				   ||(toktyp[ntok-2] == SH_TOK_GREATER))){
			if (toktyp[ntok-1] != SH_TOK_OTHER) {
				fprintf(dev,"%s\n", SHELL_SYNERRMSG);
				continue;
			}
			if (toktyp[ntok-2] == SH_TOK_LESS) {
				inname =  &tokbuf[tok[ntok-1]];
			} else {
				outname = &tokbuf[tok[ntok-1]];
			}
			ntok -= 2;
			tlen = tok[ntok];
		}


		if ( (ntok >=3) && ( (toktyp[ntok-2] == SH_TOK_LESS)
				   ||(toktyp[ntok-2] == SH_TOK_GREATER))){
			if (toktyp[ntok-1] != SH_TOK_OTHER) {
				fprintf(dev,"%s\n", SHELL_SYNERRMSG);
				continue;
			}
			if (toktyp[ntok-2] == SH_TOK_LESS) {
				if (inname != NULL) {
				    fprintf(dev,"%s\n", SHELL_SYNERRMSG);
				    continue;
				}
				inname = &tokbuf[tok[ntok-1]];
			} else {
				if (outname != NULL) {
				    fprintf(dev,"%s\n", SHELL_SYNERRMSG);
				    continue;
				}
				outname = &tokbuf[tok[ntok-1]];
			}
			ntok -= 2;
			tlen = tok[ntok];
		}

		/* Verify remaining tokens are type "other" */

		for (i=0; i<ntok; i++) {
			if (toktyp[i] != SH_TOK_OTHER) {
				break;
			}
		}
		if ((ntok == 0) || (i < ntok)) {
			fprintf(dev, SHELL_SYNERRMSG);
			continue;
		}

		stdinput = stdoutput = dev;

		/* Lookup first token in the command table */

		for (j = 0; j < ncmd; j++) {
			src = cmdtab[j].cname;
			
			/*
			kprintf("inside lookup tokens\n");
			kprintf("%s\n", src);
			*/

			cmp = tokbuf;
			diff = FALSE;
			while (*src != NULLCH) {
				if (*cmp != *src) {
					diff = TRUE;
					break;
				}
				src++;
				cmp++;
			}
			if (diff || (*cmp != NULLCH)) {
				continue;
			} else {
				break;
			}
		}

		/* Handle command not found */

		if (j >= ncmd) {
			fprintf(dev, "command %s not found\n", tokbuf);
			continue;
		}

		/* Handle built-in command */
		/*
		if (cmdtab[j].cbuiltin) { // No background or redirect. 
			if (inname != NULL || outname != NULL || backgnd){
				fprintf(dev, SHELL_BGERRMSG);
				continue;
			} else {
				// Set up arg vector for call 

				for (i=0; i<ntok; i++) {
					args[i] = &tokbuf[tok[i]];
				}

				// Call builtin shell function 

				if ((*cmdtab[j].cfunc)(ntok, args)
							== SHELL_EXIT) {
					break;
				}
			}
			continue;
		}
		*/		

		/* Open files and redirect I/O if specified */

		if (inname != NULL) {
			stdinput = open(NAMESPACE,inname,"ro");
			if (stdinput == SYSERR) {
				fprintf(dev, SHELL_INERRMSG, inname);
				continue;
			}
		}
		if (outname != NULL) {
			stdoutput = open(NAMESPACE,outname,"w");
			if (stdoutput == SYSERR) {
				fprintf(dev, SHELL_OUTERRMSG, outname);
				continue;
			} else {
				control(stdoutput, F_CTL_TRUNC, 0, 0);
			}
		}

		/* Spawn child thread for non-built-in commands */

		child = create(cmdtab[j].cfunc,
			SHELL_CMDSTK, SHELL_CMDPRIO,
			cmdtab[j].cname, 2, ntok, &tmparg);

		/* If creation or argument copy fails, report error */

		if ((child == SYSERR) ||
		    (addargs(child, ntok, tok, tlen, tokbuf, &tmparg)
							== SYSERR) ) {
			fprintf(dev, SHELL_CREATMSG);
			continue;
		}

		/* Set stdinput and stdoutput in child to redirect I/O */

		//possible for pipes?

		proctab[child].prdesc[0] = stdinput;
		proctab[child].prdesc[1] = stdoutput;
		

		
		//proctab[child].prdesc[0] = segtab[handseg].inputd;
		//proctab[child].prdesc[1] = segtab[handseg].outputd;		



		msg = recvclr();
		resume(child);
		if (! backgnd) {
			msg = receive();
			while (msg != child) {
				msg = receive();
			}
		}
		
    }

    /* Terminate the shell process by returning from the top level */

    fprintf(dev,SHELL_EXITMSG);
    return OK;
}
