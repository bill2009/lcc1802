/* copt version 1.00 (C) Copyright Christopher W. Fraser 1984 */

#include <ctype.h>
#include <stdio.h>
#define HSIZE 107
#define MAXLINE 100

int debug = 0;

struct lnode {
	char *l_text;
	struct lnode *l_prev, *l_next;
};

struct onode {
	struct lnode *o_old, *o_new;
	struct onode *o_next;
} *opts = 0;

/* connect - connect p1 to p2 */
connect(p1, p2) struct lnode *p1, *p2; {
	if (p1 == 0 || p2 == 0)
		error("connect: can't happen\n");
	p1->l_next = p2;
	p2->l_prev = p1;
}

/* error - report error and quit */
error(s) char *s; {
	fputs(s, stderr);
	exit(1);
}

/* getlst - link lines from fp in between p1 and p2 */
getlst(fp, quit, p1, p2) FILE *fp; char *quit; struct lnode *p1, *p2; {
	char *install(), lin[MAXLINE];

	connect(p1, p2);
	while (fgets(lin, MAXLINE, fp) != NULL && strcmp(lin, quit))
		insert(install(lin), p2);
}

/* init - read patterns file */
init(fp) FILE *fp; {
	struct lnode head, tail;
	struct onode *p, **next;

	next = &opts;
	while (*next)
		next = &((*next)->o_next);
	while (!feof(fp)) {
		p = (struct onode *) malloc((unsigned) sizeof(struct onode));

		getlst(fp, "=\n", &head, &tail);
		head.l_next->l_prev = 0;
		if (tail.l_prev)
			tail.l_prev->l_next = 0;
		p->o_old = tail.l_prev;

		getlst(fp, "\n", &head, &tail);
		tail.l_prev->l_next = 0;
		if (head.l_next)
			head.l_next->l_prev = 0;
		p->o_new = head.l_next;

		*next = p;
		next = &p->o_next;
	}
	*next = 0;
}

/* insert - insert a new node with text s before node p */
insert(s, p) char *s; struct lnode *p; {
	struct lnode *n;

	n = (struct lnode *) malloc(sizeof *n);
	n->l_text = s;
	connect(p->l_prev, n);
	connect(n, p);
}

/* install - install str in string table */
char *install(str) char *str; {
	register struct hnode *p;
	register char *p1, *p2, *s;
	register int i;
	static struct hnode {
		char *h_str;
		struct hnode *h_ptr;		  
	} *htab[HSIZE] = {0};
	
	s = str;
	for (i = 0; *s; i += *s++)
		;
	i %= HSIZE;

	for (p = htab[i]; p; p = p->h_ptr) 
		for (p1=str, p2=p->h_str; *p1++ == *p2++; )
			if (p1[-1] == '\0') 
				return (p->h_str);

	p = (struct hnode *) malloc(sizeof *p);
	p->h_str = (char *) malloc((s-str)+1);
	strcpy(p->h_str, str);
	p->h_ptr = htab[i];
	htab[i] = p;
	return (p->h_str);
}

/* main - peephole optimizer */
main(argc, argv) int argc; char **argv; {
	FILE *fp;
	int i;
	struct lnode head, *p, *opt(), tail;

	for (i = 1; i < argc; i++)
		if (strcmp(argv[i], "-D") == 0)
			debug = 1;
		else if ((fp=fopen(argv[i], "r")) == NULL) 
			error("copt: can't open patterns file\n");
		else
			init(fp);

	getlst(stdin, "", &head, &tail);
	head.l_text = tail.l_text = "";

	for (p = head.l_next; p != &tail; p = opt(p))
		;

	for (p = head.l_next; p != &tail; p = p->l_next)
		fputs(p->l_text, stdout);
}

/* match - match ins against pat and set vars */
int match(ins, pat, vars) char *ins, *pat, **vars; {
	char *p, lin[MAXLINE];

	while (*ins && *pat)
		if (pat[0] == '%' && isdigit(pat[1])) {
			for (p = lin; *ins && *ins != pat[2];)
				*p++ = *ins++;
			*p = 0;
			p = install(lin);
			if (vars[pat[1]-'0'] == 0)
				vars[pat[1]-'0'] = p;
			else if (vars[pat[1]-'0'] != p)
				return 0;
			pat += 2;
		}
		else if (*pat++ != *ins++)
			return 0;
	return *pat==*ins;
}

/* opt - replace instructions ending at r if possible */
struct lnode *opt(r) struct lnode *r; {
	char *vars[10];
	int i;
	struct lnode *c, *p, *rep();
	struct onode *o;

	for (o = opts; o; o = o->o_next) {
		c = r;
		p = o->o_old;
		for (i = 0; i < 10; i++)
			vars[i] = 0;
		while (p && c && match(c->l_text, p->l_text, vars)) {
			p = p->l_prev;
			c = c->l_prev;
		}
		if (p == 0)
			return rep(c, r->l_next, o->o_new, vars);
	}
	return r->l_next;
}

/* rep - substitute vars into new and replace lines between p1 and p2 */
struct lnode *rep(p1, p2, new, vars) struct lnode *p1, *p2, *new; char **vars; {
	char *args[10], *exec(), *subst(); 
	int i;
	struct bnode *b;
	struct lnode *p, *psav;

	for (p = p1->l_next; p != p2; p = psav) {
		psav = p->l_next;
		if (debug)
			fputs(p->l_text, stderr);
		free(p);
	}
	connect(p1, p2);
	if (debug)
		fputs("=\n", stderr);
	for (; new; new = new->l_next) {
		for (i = 0; i < 10; i++)
			args[i] = 0;
		insert(subst(new->l_text, vars), p2);
		if (debug)
			fputs(p2->l_prev->l_text, stderr);
	}
	if (debug)
		putc('\n', stderr);
	return p1->l_next;
}

/* subst - return result of substituting vars into pat */
char *subst(pat, vars) char *pat, **vars; {
	char *install(), lin[MAXLINE], *s;
	int i;

	i = 0;
	for (;;)
		if (pat[0] == '%' && isdigit(pat[1])) {
			for (s = vars[pat[1]-'0']; i < MAXLINE && (lin[i] = *s++); i++)
				;
			pat += 2;
		}
		else if (i >= MAXLINE)
			error("line too long\n");
		else if (!(lin[i++] = *pat++))
			return install(lin);
}