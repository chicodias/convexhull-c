/*
 * Triang.c (em O(n*logn))
 */
#include "fmalloc.h"
#include "cg.h"
#include "dupla.h"
#include "erro.h"
#include "fgrafico.h"
#include "fconio.h"
#include "mindist.h"
#include "triang.h"
#include "sort.h"
#include "vetor.h"

/* variaveis de controle */
extern int animacao, pare, delay_time, writemode;

/* poligonos globais */
_poly poligono;
_poly poligono2;

/* retorna a ponta de baixo da aresta */
int baixo (int aresta)
{
  return ((poligono.p[aresta%poligono.np][Y] < poligono.p[aresta-1][Y]) ?
			  aresta%poligono.np : aresta-1);
}

/* retorna a ponta de cima da aresta */
int cima (int aresta)
{
  return ((poligono.p[aresta%poligono.np][Y] > poligono.p[aresta-1][Y]) ?
			  aresta%poligono.np : aresta-1);
}

/* retorna se p3 esta a esquerda do vetor p1,p2 */
int esquerda (int p1, int p2, int p3)
{
  return (left (pol (p1), pol (p2), pol (p3)));
}

/* Funcao de alocacao dinamica de uma celula para a arvore 23 */
_tree23 pega_cel_tree23 (void)
{
  _tree23 t;
  if ((t = (_tree23) malloc (sizeof (struct _cel_tree23))) == NULL)
    runerror (FALTA_MEMORIA);

  /* devolve a celula com alguns valores iniciais */
  t->esq = t->dir = t->meio = t->pai = NULL;
  t->einfo = t->dinfo = VAZIO;
  return (t);
}

/* Funcao que inicia uma arvore 23 com uma folha */
_tree23 inicio_tree23 (void)
{
  return (pega_cel_tree23 ());
}

/* Funcao que busca por uma aresta nos nos internos da arvore t */
_tree23 acha_aresta23 (int aresta, _tree23 t)
{
  if (folha (t))  /* se folha, nao achou a aresta */
    return (NULL);

  if (t->einfo == aresta || t->dinfo == aresta)  /* se achou */
    return (t);

  /* muitos casos especiais. Preciso de um livro para justifica-los ! */
  if (!right (pol(baixo(t->einfo)), pol(cima(t->einfo)),
	      pol(aresta%poligono.np)) &&
      !right (pol(baixo(t->einfo)), pol(cima(t->einfo)), pol(aresta-1)))
  {
    return (acha_aresta23 (aresta, t->esq));
  }
  if (!right (pol(baixo(t->einfo)), pol(cima(t->einfo)),
	      pol(aresta%poligono.np)) ||
      !right (pol(baixo(t->einfo)), pol(cima(t->einfo)), pol(aresta-1)))
  {
    if (!left(pol(baixo(aresta)), pol(cima(aresta)), pol(baixo(t->einfo))) &&
	!left(pol(baixo(aresta)), pol(cima(aresta)), pol(cima(t->einfo))))
      return (acha_aresta23 (aresta, t->esq));
  }

  if (t->dir != NULL)
  {
    if (!right (pol(baixo(t->dinfo)), pol(cima(t->dinfo)),
		pol(aresta%poligono.np)) &&
	!right (pol(baixo(t->dinfo)), pol(cima(t->dinfo)), pol(aresta-1)))
    {
      return (acha_aresta23 (aresta, t->meio));
    }
    if (!right (pol(baixo(t->dinfo)), pol(cima(t->dinfo)),
		pol(aresta%poligono.np)) ||
	!right (pol(baixo(t->dinfo)), pol(cima(t->dinfo)), pol(aresta-1)))
    {
      if (!left(pol(baixo(aresta)), pol(cima(aresta)), pol(baixo(t->dinfo)))&&
	  !left(pol(baixo(aresta)), pol(cima(aresta)), pol(cima(t->dinfo))))
	return (acha_aresta23 (aresta, t->meio));
    }
    return (acha_aresta23 (aresta, t->dir));
  }
  else
    return (acha_aresta23 (aresta, t->meio));
}

/* Funcao que retorna a qual folha (ou seja, qual regiao) pertence o
   vertice v na arvore t. */
_tree23 acha_vertice23 (int v, _tree23 t)
{
  int ante, prox;
  if (folha (t)) /* se folha, retornamos */
    return (t);

  /* por enquanto sao verificacoes normais */
  if (left (pol(baixo(t->einfo)), pol(cima(t->einfo)), pol(v)))
  {
    return (acha_vertice23 (v, t->esq));
  }
  if (t->dir != NULL)
    if (right (pol(baixo(t->dinfo)), pol(cima(t->dinfo)), pol(v)))
    {
      return (acha_vertice23 (v, t->dir));
    }

  /* agora vamos de casos especiais.
     Acho que aqui precisamos de dois livros :-) ! */
  ante = (t->einfo <= 1) ? poligono.np : t->einfo-1;
  prox = (t->einfo < poligono.np) ? t->einfo+1 : 1;
  if (v == baixo (t->einfo) && v == baixo (prox))
    if (esquerda (baixo (t->einfo), cima (t->einfo), cima (prox)))
      return (acha_vertice23 (v, t->esq));
  if (v == cima (t->einfo) && v == cima (prox))
    if (esquerda (baixo (t->einfo), cima (t->einfo), baixo (prox)))
      return (acha_vertice23 (v, t->esq));

  if (v == cima (ante) && v == cima (t->einfo))
    if (!esquerda (baixo (ante), cima (ante), baixo (t->einfo)))
      return (acha_vertice23 (v, t->esq));
  if (v == baixo (ante) && v == baixo (t->einfo))
    if (!esquerda (baixo (ante), cima (ante), cima (t->einfo)))
      return (acha_vertice23 (v, t->esq));


  if (t->dir != NULL)
  {
    ante = (t->dinfo <= 1) ? poligono.np : t->dinfo-1;
    prox = (t->dinfo < poligono.np) ? t->dinfo+1 : 1;

    if (v == baixo (t->dinfo) && v == baixo (prox))
      if (!esquerda (baixo (t->dinfo), cima (t->dinfo), cima (prox)))
	return (acha_vertice23 (v, t->dir));
    if (v == cima (t->dinfo) && v == cima (prox))
      if (!esquerda (baixo (t->dinfo), cima (t->dinfo), baixo (prox)))
	return (acha_vertice23 (v, t->dir));

    if (v == baixo (ante) && v == baixo (t->dinfo))
      if (esquerda (baixo (ante), cima (ante), cima (t->dinfo)))
	return (acha_vertice23 (v, t->dir));
    if (v == cima (ante) && v == cima (t->dinfo))
      if (esquerda (baixo (ante), cima (ante), baixo (t->dinfo)))
	return (acha_vertice23 (v, t->dir));
  }

  return (acha_vertice23 (v, t->meio));
}

/* Funcao que encontra qual o elemento (um vertice) da folha mais proxima
   `a esquerda da aresta na arvore t */
_tree23 vertice_esq (int aresta, _tree23 t)
{
  if (folha (t))
    return (t);

  /* vamos uma vez para esquerda */
  if (t->einfo == aresta)
    t = t->esq;
  else
    t = t->meio;

  /* depois sempre para direita */
  while (!folha (t))
    if (t->dir == NULL)
      t = t->meio;
    else
      t = t->dir;

  return (t);
}

/* Funcao que encontra qual o elemento (um vertice) da folha mais proxima
   `a direita da aresta na arvore t */
_tree23 vertice_dir (int aresta, _tree23 t)
{
  if (folha (t))
    return (t);

  /* vamos uma vez para direita */
  if (t->einfo == aresta)
    t = t->meio;
  else
    t = t->dir;

  /* depois sempre para esquerda */
  while (!folha (t))
    t = t->esq;

  return (t);
}

/* Funcao que encontra qual a folha mais proxima `a esquerda da folha dada
   na arvore t. Retorna NULL se nao existir */
_tree23 folha_esq (_tree23 folha)
{
  _tree23 u;
  u = folha->pai;
  if (folha == u->esq)
  {
    while (u != NULL && u->esq == folha)
    {
      folha = folha->pai;
      u = folha->pai;
    }
    if (u != NULL)
    {
      if (folha == u->meio)
	u = vertice_esq (u->einfo, u);
      else
	u = vertice_esq (u->dinfo, u);
    }
  }
  else if (folha == u->meio)
    u = vertice_esq (u->einfo, u);
  else
    u = vertice_esq (u->dinfo, u);
  return (u);
}

/* Funcao que encontra qual a folha mais proxima `a direita da folha dada
   na arvore t. Retorna NULL se nao existir */
_tree23 folha_dir (_tree23 folha)
{
  _tree23 u;
  u = folha->pai;
  if (folha == u->dir || (folha == u->meio && u->dir == NULL))
  {
    while (u != NULL &&
	   (u->dir == folha || (u->meio == folha && u->dir == NULL)))
    {
      folha = folha->pai;
      u = folha->pai;
    }
    if (u != NULL)
    {
      if (folha == u->meio)
	u = vertice_dir (u->dinfo, u);
      else
	u = vertice_dir (u->einfo, u);
    }
  }
  else if (folha == u->meio)
    u = vertice_dir (u->dinfo, u);
  else
    u = vertice_dir (u->einfo, u);
  return (u);
}


/* Funcao que substitue a aresta a1 pela a2 na arvore t, e devolve, caso
   haja necessidade de ser feita, alguma ligacao com os extremos destas
   arestas */
tlig subst (int a1, int a2, _tree23 t)
{
  tlig l;
  _tree23 u;
  t = acha_aresta23 (a1, t);

  l.i = l.f = VAZIO;

  if (t->einfo == a1)
    t->einfo = a2;
  else
    t->dinfo = a2;

  u = vertice_esq (a2, t);

  if (u->einfo != VAZIO) /* se entramos neste if, entao nao vamos entrar
			 no outro mais abaixo (**) */
  {
    if (u->dinfo == TEM_BICO) /* se existe algum ponto critico pendente */
    {
      l.i = baixo (a2); /* criamos a ligacao */
      l.f = u->einfo;
      if (animacao)
      {
	setwritemode (0); /* e desenhamos a diagonal */
	line (poligono.p[baixo (a2)][X], poligono.p[baixo (a2)][Y],
	      poligono.p[u->einfo][X], poligono.p[u->einfo][Y]);
	setwritemode (writemode);
      }
      u->dinfo = VAZIO;
    }

    u->einfo = baixo (a2);
  }

  u = vertice_dir (a2, t);

  if (u->einfo != VAZIO)   /* (**) */
  {
    if (u->dinfo == TEM_BICO) /* se existe algum ponto critico pendente */
    {
      l.i = baixo (a2); /* criamos e desenhamos a diagonal */
      l.f = u->einfo;
      if (animacao)
      {
	setwritemode (0);
	line (poligono.p[baixo (a2)][X], poligono.p[baixo (a2)][Y],
	      poligono.p[u->einfo][X], poligono.p[u->einfo][Y]);
	setwritemode (writemode);
      }
      u->dinfo = VAZIO;
    }

    u->einfo = baixo (a2);
  }
  return (l);
}

/* Funcao recursiva para retirar da arvore t, no no p, o elemento a.
   Retorna novo apontador para a arvore, caso seja alterado */
_tree23 retira_tree23a (int a, _tree23 p, _tree23 t)
{
  _tree23 q, w;

  if (p->dir != NULL) /* se podemos excluir sem problemas */
  {
    if (p->einfo == a)
    {
      free (p->meio);
      p->einfo = p->dinfo;
      p->meio  = p->dir;
      p->dinfo = VAZIO;
      p->dir = NULL;
    }
    else
    {
      p->dinfo = VAZIO;
      free (p->dir);
      p->dir = NULL;
    }
    return (t);
  }

  w = p->pai;
  if (w == NULL)  /* se eh raiz */
  {
    free (p->meio);
    w = p->esq;
    free (p);
    w->pai = NULL;
    return (w);  /* devolvemos a nova raiz */
  }
  if (w->meio == p)  /* achamos um irmao */
    q = w->esq;
  else
    q = w->meio;

  if (q->dir != NULL)  /* se irmao tem 2 elementos, basta redistribuir */
  {
    if (w->esq == p)
    {
      free (p->meio);

      p->einfo = w->einfo;
      p->meio  = q->esq;
      w->einfo = q->einfo;
      q->einfo = q->dinfo;
      q->esq   = q->meio;
      q->meio  = q->dir;
      q->dinfo = VAZIO;
      q->dir   = NULL;
      (p->meio)->pai = p;
    }
    else if (w->meio == p)
    {
      free (p->meio);

      p->einfo = w->einfo;
      p->meio  = p->esq;
      p->esq   = q->dir;
      w->einfo = q->dinfo;
      q->dinfo = VAZIO;
      q->dir   = NULL;
      (p->esq)->pai = p;
    }
    else
    {
      free (p->meio);

      p->einfo = w->dinfo;
      p->meio  = p->esq;
      p->esq   = q->dir;
      w->dinfo = q->dinfo;
      q->dinfo = VAZIO;
      q->dir   = NULL;
      (p->esq)->pai = p;
    }
    return (t);
  }

  /* Agora se irmao so tem um elemento, entao propagamos o problema */
  if (w->esq == p)
  {
    free (p->meio);

    p->einfo = w->einfo;
    p->meio  = q->esq;
    p->dinfo = q->einfo;
    p->dir   = q->meio;
    (p->meio)->pai = p;
    (p->dir)->pai  = p;
    return (retira_tree23a (w->einfo, w, t));

  }

  else if (w->meio == p)
  {
    free (p->meio);

    p->dinfo = w->einfo;
    p->einfo = q->einfo;
    p->dir   = p->esq;
    p->meio  = q->meio;
    p->esq   = q->esq;
    (p->meio)->pai = p;
    (p->esq)->pai  = p;

    q      = w->esq;
    w->esq = w->meio;
    w->meio= q;

    return (retira_tree23a (w->einfo, w, t));
  }

  else /* se pai tem 2 elementos, basta redistribuirmos */
  {
    free (p->meio);

    p->dinfo = w->dinfo;
    p->einfo = q->einfo;
    p->dir   = p->esq;
    p->meio  = q->meio;
    p->esq   = q->esq;
    (p->meio)->pai = p;
    (p->esq)->pai  = p;

    free (w->meio);
    w->meio = w->dir;
    w->dinfo = VAZIO;
    w->dir = NULL;
    return (t);

  }
}

/* Funcao que retira da arvore t o elemento a, devolvendo um novo
   apontador para a arvore */
_tree23 retira_tree23 (int a, _tree23 t)
{
  _tree23 u, p;
  if ((u = acha_aresta23 (a, t)) == NULL) /* se nao achamos a */
    return (t);
  if (!folha (u->esq)) /* se filhos do no de a nao sao externos */
  {
    p = vertice_esq (a, u);  /* encontramos um no que tem esta propriedade
				e trocamos com o no de a */
    p = p->pai;
    if (u->einfo == a)
      u->einfo = (p->dir != NULL) ? p->dinfo : p->einfo;
    else
      u->dinfo = (p->dir != NULL) ? p->dinfo : p->einfo;
    a = (p->dir == NULL) ? p->einfo : p->dinfo;
  }
  else
    p = u;

  return (retira_tree23a (a, p, t)); /* deixamos a recursao fazer o servico */
}

/* Funcao que insere o elemento a na arvore t, devolvendo um novo apontador
   para a arvore */
_tree23 coloca_tree23 (int a, _tree23 t)
{
  _tree23 n, q, p, u = acha_vertice23 (baixo (a), t);
  /* achamos p, onde iremos inserir */
  if ((p = u->pai) == NULL)  /* se p eh nulo, arvore nao tinha nos internos */
  {
    t->einfo = a;
    t->esq  = pega_cel_tree23 ();
    t->meio = pega_cel_tree23 ();
    (t->esq)->pai  = t;
    (t->meio)->pai = t;
  }
  else if (p->dir == NULL) /* se tem espaco no p, basta inserirmos */
  {
    if (u == p->esq)
    {
      p->dinfo = p->einfo;
      p->dir   = p->meio;
      p->meio  = p->esq;
      p->esq   = pega_cel_tree23 ();
      (p->esq)->pai = p;
      p->einfo = a;
    }
    else
    {
      p->dir   = pega_cel_tree23 ();
      (p->dir)->pai = p;
      p->dinfo = a;
    }
  }
  else  /* senao vamos quebrar p em dois (que serao p e q) e propagamos
	   o problema de insersao */
  {
    q = pega_cel_tree23 ();
    if (u == p->esq)
    {
      q->einfo = p->dinfo;
      q->esq   = p->meio;
      q->meio  = p->dir;
      p->dinfo = a;
      a        = p->einfo;
      p->einfo = p->dinfo;
      p->meio  = p->esq;
      p->esq   = pega_cel_tree23 ();
    }
    else if (u == p->meio)
    {
      q->einfo = p->dinfo;
      q->esq   = pega_cel_tree23 ();
      q->meio  = p->dir;
    }
    else
    {
      q->einfo = a;
      q->esq   = p->dir;
      q->meio  = pega_cel_tree23 ();
      a = p->dinfo;
    }

    /* guarantindo a estrutura */
    (p->esq)->pai  = p;
    (p->meio)->pai = p;
    (q->esq)->pai  = q;
    (q->meio)->pai = q;
    p->dinfo = VAZIO;
    p->dir   = NULL;


    n = q;
    u = p;
    p = u->pai;
    /* propagamos ate a raiz */
    while (p != NULL)
    {
      if (p->dir == NULL) /* opa, achamos um lugar para inserir */
      {
	if (u == p->esq)
	{
	  p->dinfo = p->einfo;
	  p->dir   = p->meio;
	  p->meio  = n;
	  (p->meio)->pai = p;
	  p->einfo = a;
	}
	else
	{
	  p->dir   = n;
	  (p->dir)->pai = p;
	  p->dinfo = a;
	}
	break; /* podemos parar */
      }
      else /* senao quebramos p novamente e continuamos propagando o
	      problema */
      {
	q = pega_cel_tree23 ();
	if (u == p->esq)
	{
	  q->einfo = p->dinfo;
	  q->esq   = p->meio;
	  q->meio  = p->dir;
	  p->dinfo = a;
	  a        = p->einfo;
	  p->einfo = p->dinfo;
	  p->meio  = n;
	}
	else if (u == p->meio)
	{
	  q->einfo = p->dinfo;
	  q->esq   = n;
	  q->meio  = p->dir;
	}
	else
	{
	  q->einfo = a;
	  q->esq   = p->dir;
	  q->meio  = n;
	  a = p->dinfo;
	}

	/* mantendo a estrutura correta */
	(p->esq)->pai  = p;
	(p->meio)->pai = p;
	(q->esq)->pai  = q;
	(q->meio)->pai = q;
	p->dinfo = VAZIO;
	p->dir = NULL;
	u = p;
	p = u->pai;
	n = q;
      }
    }
    if (p == NULL) /* se propagamos ate a raiz, entao a arvore
		      tem que aumentar ! */
    {
      n = pega_cel_tree23 ();
      n->einfo = a;
      n->esq   = t;
      n->meio  = q;
      t->pai   = n;
      q->pai   = n;
      return (n);   /* devolvemos a nova raiz */
    }

  }

  return (t); /* nao houve alteracao na raiz */
}

/* Funcao que triangulariza um poligono monotono em tempo linear.
   poly tem o poligono, pts tem uma lista duplamente ligada dos vertices
   do poligono que devem ser triangularizados, ini tem o vertice inicial */
double monotone (_poly poly, tdponto pts, int ini)
{
  int i, v, sort[MAX_PONTOS];
  double peso = 0.0;
  tdponto p;
  _dupla d = NULL;

  if (animacao)
    setwritemode (writemode);

  /* rearruma o poligono poly para que ele contenha somente os vertices que
     serao 'triangularizados' */
  poligono = poly;
  v = ini;
  for (i=0; i<poly.np; i++)
  {
    sort[i] = i;
    attrib (&(poly.p[i]), poligono.p[ini]);
    ini = pts[ini].prox;
    if (ini == v)
    {
      i++;
      break;
    }
  }
  poly.np = i;

  /* agora ja temos em poly e poligono os dados corretos */
  poligono = poly;

  /* cria uma nova lista ligada com estes novos vertices */
  p[0].prox = 1;
  p[0].ante = poly.np-1;
  p[poly.np-1].prox = 0;
  p[poly.np-1].ante = poly.np-2;
  for (i=1; i<poly.np-1; i++)
  {
    p[i].prox = i+1;
    p[i].ante = i-1;
  }

  if (animacao) /* se animacao, mostra o poligono em questao */
  {
    setcolor (CYAN);
    for (i=0; i<poly.np; i++)
      line (poly.p[i][X], poly.p[i][Y], poly.p[(i+1)%poly.np][X],
				      poly.p[(i+1)%poly.np][Y]);

    if (delay_time < 0) getch();
    else if (delay_time*7 > delay_time)
      delay (delay_time*7);
    else
      delay (delay_time);

    for (i=0; i<poly.np; i++)
      line (poly.p[i][X], poly.p[i][Y], poly.p[(i+1)%poly.np][X],
				      poly.p[(i+1)%poly.np][Y]);
    setcolor (WHITE);
  }

  if (poly.np < 4) /* se np < 4 nao ha o que fazer */
    return peso;

  /* ordenamos */
  triang_sort (sort, Y, poly.np);

  /* e agora vem o algoritmo */

  /* comecamos com os dois com menor Y */
  d = coloca_dupla (sort[0], d);
  d = coloca_dupla (sort[1], d);
  v = 2;

  /* enquanto nao olharmos todos */
  while (v < poly.np-1 && !pare)
  {
    if (p[sort[v]].prox == d->i) /* se esta na mesma parede (esq) da lista */
    {
      /* se esta com visao de outro ponto da lista */
      if (right (poly.p[sort[v]], poly.p[(d->prox)->i], poly.p[d->i]))
      {
	/* tracamos a diagonal e retiramos um ponto da lista */
	if (animacao)
	  line (poly.p[sort[v]][X], poly.p[sort[v]][Y],
		poly.p[(d->prox)->i][X], poly.p[(d->prox)->i][Y]);
        peso += qdist (poly.p[sort[v]], poly.p[(d->prox)->i]);
	p[p[d->i].ante].prox = p[d->i].prox;
	p[p[d->i].prox].ante = p[d->i].ante;
	d = retira_dupla (d, d);
	if (tam1_dupla (d)) /* lista tem que ter pelo menos dois elementos */
	{
	  d = coloca_dupla (sort[v], d);
	  v++;
	}
      }
      else /* senao colocamos na lista */
      {
	d = coloca_dupla (sort[v], d);
	v++;
      }
    }
    else if (p[sort[v]].ante == d->i) /* ou na mesma parede (dir) da lista */
    {
      /* se esta com visao de outro ponto da lista */
      if (right (poly.p[(d->prox)->i], poly.p[sort[v]], poly.p[d->i]))
      {
	/* tracamos a diagonal e retiramos um ponto da lista */
	if (animacao)
	  line (poly.p[sort[v]][X], poly.p[sort[v]][Y],
		poly.p[(d->prox)->i][X], poly.p[(d->prox)->i][Y]);
        peso += qdist (poly.p[sort[v]], poly.p[(d->prox)->i]);
	p[p[d->i].ante].prox = p[d->i].prox;
	p[p[d->i].prox].ante = p[d->i].ante;
	d = retira_dupla (d, d);
	if (tam1_dupla (d)) /* lista tem que ter pelo menos dois elementos */
	{
	  d = coloca_dupla (sort[v], d);
	  v++;
	}
      }
      else /* senao colocamos na lista */
      {
	d = coloca_dupla (sort[v], d);
	v++;
      }
    }
    else /* senao eh porque esta em parede oposta,
	    entao podemos tracar diagonais */
    {
      if (animacao)
	line (poly.p[sort[v]][X], poly.p[sort[v]][Y],
	      poly.p[((d->ante)->ante)->i][X],
	      poly.p[((d->ante)->ante)->i][Y]);
      peso += qdist (poly.p[sort[v]], poly.p[((d->ante)->ante)->i]);
      p[p[(d->ante)->i].ante].prox = p[(d->ante)->i].prox;
      p[p[(d->ante)->i].prox].ante = p[(d->ante)->i].ante;
      d = retira_dupla (d->ante, d);
      if (tam1_dupla (d)) /* lista tem que ter pelo menos dois elementos */
      {
	d = coloca_dupla (sort[v], d);
	v++;
      }
    }
  }

  /* agora so falta ligar o ultimo ponto com o que restou na lista */
  while ((d = retira_dupla (d, d)) != NULL) {
    if (!pare && animacao)
    {
      i = d->i;
      line (poly.p[sort[v]][X], poly.p[sort[v]][Y],
	    poly.p[i][X], poly.p[i][Y]);
    }
    peso += qdist (poly.p[sort[v]], poly.p[i]);
  }
  if (!pare && animacao)
    line (poly.p[sort[v]][X], poly.p[sort[v]][Y],
	  poly.p[i][X], poly.p[i][Y]);
  peso -= qdist (poly.p[sort[v]], poly.p[i]);

  if (animacao)
    setwritemode (0);
  setcolor (WHITE);
  return peso;
}

/* Funcao que verifica se um poligono esta no sentido horario */
int is_sentido_horario (_poly poly)
{
  int i, min = 0;
  setwritemode (1);
  for (i=1; i<poly.np; i++)
    if (poly.p[i][Y] < poly.p[min][Y])
      min = i;
  i = right (poly.p[min-1], poly.p[min], poly.p[min+1]);
  setwritemode (0);
  return (i);
}

/* Funcao que triangulariza um poligono poly em ordem de n*logn */
double triang (_poly poly)
{
  int nlig, /* numero de diagonais tracadas pela primeira parte do algoritmo*/
      v,
      sort[MAX_PONTOS]; /* vetor para ordenarmos (ordenamos os indices) */
  int i, ante, prox, st;
  _point ponto;
  tlig ligacao[MAX_PONTOS]; /* guarda as ligacoes temporariamente */
  ts s, aux, atual;  /* arvore de busca binaria */
  tdponto pts;
  _tree23 t, u, tree = inicio_tree23 (); /* arvore 2-3 */
  double peso = 0;

  if (is_sentido_horario (poly)) /* se sentido horario, entao invertemos */
  {
    for (i=0; i<((int)poly.np/2); i++)
    {
      attrib (&ponto, poly.p[i]);
      attrib (&poly.p[i], poly.p[poly.np-i-1]);
      attrib (&poly.p[poly.np-i-1], ponto);
    }
  }

  poligono = poly;

  if (animacao)
    setwritemode (writemode);
  setcolor (WHITE);

  for (i=0; i<poly.np; i++)
    sort[i] = i;

  /* ordenamos para a varredura */
  triang_sort (sort, Y, poly.np);

  /* nao conseguimos com dois pontos com mesma ordenada (Y) */
  for (v=0; v<poligono.np-1; v++)
    if (poligono.p[sort[v]][Y] == poligono.p[sort[v+1]][Y])
      return -1;

  v = 0;
  if (animacao)
  {
    setcolor (LIGHTBLUE);
    line (0, poly.p[sort[v]][Y], getmaxx()-1, poly.p[sort[v]][Y]);
    delay (200);
  }

  nlig = 0;
  /* enquanto nao olharmos todos os pontos (vertices) */
  while (v < poly.np && !pare)
  {
    ante = (sort[v] == 0) ? poly.np-1 : sort[v]-1;
    prox = (sort[v]+1)%poly.np;
    setcolor (WHITE);

    /* a varredura esta na posicao de v
       existem 3 casos possiveis:
	 caso 1: as duas arestas em questao (sao as que estao ligadas em v)
		 estao para tras da varredura, entao retiramos as duas
		 da estrutura
	 case 2: uma aresta esta para tras e outra para frente, entao
		 substituimos uma pela outra
	 case 3: as duas arestas estao para frente, entao inserimos as duas
		 na estrutura

	 * em todos os casos temos que resolver os problemas dos pontos
	   criticos.

	 Olhando para estes casos vemos o problema de existirem dois pontos
	 com mesma ordenada (Y). Isso causaria uma indecisao ao algoritmo. */

    if (poly.p[ante][Y] < poly.p[sort[v]][Y])
    {
      if (poly.p[prox][Y] < poly.p[sort[v]][Y]) /* caso 1 */
      {
	t = acha_vertice23 (sort[v], tree);

	/* resolvendo os vertices criticos, caso existam */
	st = t->einfo;
	if (st == VAZIO)
	{
	  u = folha_esq (t);
	  if (u->dinfo == TEM_BICO)
	  {
	    ligacao[nlig].i   = sort[v];
	    ligacao[nlig++].f = u->einfo;
	    if (animacao)
	    {
	      setwritemode (0);
	      line (poligono.p[sort[v]][X], poligono.p[sort[v]][Y],
		    poligono.p[u->einfo][X], poligono.p[u->einfo][Y]);
	      setwritemode (writemode);
	    }
	    t->dinfo = VAZIO;
	  }
	  t = folha_dir (t);
	}
	if (t->dinfo == TEM_BICO)
	{
	  ligacao[nlig].i   = sort[v];
	  ligacao[nlig++].f = t->einfo;
	  if (animacao)
	  {
	    setwritemode (0);
	    line (poligono.p[sort[v]][X], poligono.p[sort[v]][Y],
		  poligono.p[t->einfo][X], poligono.p[t->einfo][Y]);
	    setwritemode (writemode);
	  }
	  t->dinfo = VAZIO;
	}

	/* retiramos */
	tree = retira_tree23 (ante+1, tree);
	tree = retira_tree23 (sort[v]+1, tree);

	/* e rearruamos */
	t = acha_vertice23 (cima (ante+1), tree);
	if (st == VAZIO)
	{
	  t->einfo = cima (ante+1);
	  t->dinfo = TEM_BICO;
	}
	else
	  t->dinfo = t->einfo = VAZIO;
      }
      else /* caso 2 */
      {
	if (delay_time <0) getch();
	else delay (delay_time);
	ligacao[nlig] = subst (ante+1, sort[v]+1, tree);
	if (ligacao[nlig].i != VAZIO) nlig++;
      }
    }
    else
    {
      if (poly.p[prox][Y] < poly.p[sort[v]][Y]) /* caso 2 */
      {
	if (delay_time <0) getch();
	else delay (delay_time);
	ligacao[nlig] = subst (sort[v]+1, ante+1, tree);
	if (ligacao[nlig].i != VAZIO) nlig++;
      }
      else /* caso 3 */
      {
	t = acha_vertice23 (sort[v], tree);

	/* arrumamos pontos criticos */
	if (t->dinfo == TEM_BICO)
	{
	  ligacao[nlig].i   = sort[v];
	  ligacao[nlig++].f = t->einfo;
	  if (animacao)
	  {
	    setwritemode (0);
	    line (poligono.p[sort[v]][X], poligono.p[sort[v]][Y],
		  poligono.p[t->einfo][X], poligono.p[t->einfo][Y]);
	    setwritemode (writemode);
	  }
	  t->dinfo = VAZIO;
	}
	st = t->einfo;

	/* colocamos as arestas, na ordem correta */
	if (esquerda (baixo (ante+1), cima (ante+1), cima (sort[v]+1)))
	{
	  tree = coloca_tree23 (sort[v]+1, tree);
	  tree = coloca_tree23 (ante+1, tree);
	}
	else
	{
	  tree = coloca_tree23 (ante+1, tree);
	  tree = coloca_tree23 (sort[v]+1, tree);
	}

	/* rearrumamos a estrutura */
	t = acha_vertice23 (baixo (ante+1), tree);
	t->einfo = (st == VAZIO) ? baixo (ante+1) : VAZIO;
	if ((u = folha_esq (t)) != NULL)
	  u->einfo = (st == VAZIO) ? VAZIO : baixo (ante+1);
	if ((u = folha_dir (t)) != NULL)
	  u->einfo = (st == VAZIO) ? VAZIO : baixo (ante+1);

	if (st != VAZIO)  /* se bico interno, vamos ligar */
	{
	  ligacao[nlig].i   = st;
	  ligacao[nlig++].f = baixo (ante+1);
	  if (animacao)
	  {
	    setwritemode (0);
	    line (poligono.p[st][X], poligono.p[st][Y],
		  poligono.p[baixo(ante+1)][X], poligono.p[baixo(ante+1)][Y]);
	    setwritemode (writemode);
	  }
	}

      }
    }

    setwritemode (1);
    /* se animacao, entao atualizamos a linha de varredura */
    if (animacao)
    {
      setcolor (LIGHTBLUE);
      line (0, poly.p[sort[v]][Y], getmaxx()-1, poly.p[sort[v]][Y]);
    }
    v++;
    if (animacao && v < poly.np)
      line (0, poly.p[sort[v]][Y], getmaxx()-1, poly.p[sort[v]][Y]);

    setwritemode (writemode);
  }

  if (!folha (tree)) /* arvore tem que acabar vazia */
    runerror (INDEFINIDO);
  free (tree);

  if (animacao)
  {
    delay (500);
    setwritemode (0);
  }
  setcolor (WHITE);

  /* cria pts com todos os pontos para ser usado em 'percorre' */
  pts[0].ante = poligono.np-1;
  pts[0].prox = 1;
  pts[poligono.np-1].prox = 0;
  pts[poligono.np-1].ante = poligono.np-2;
  for (v=1; v<poligono.np-1; v++)
  {
    pts[v].prox = v+1;
    pts[v].ante = v-1;
  }

  /* colocamos cada ligacao numa ABB de maneira inteligente para podermos
     depois dividir o poligono em poligonos monotonos usando estas diagonais
     em tempo medio nlig*log(nlig). Este passo poderia ser feito em tempo
     O(nlig*log(nlig)) no pior caso, mas como o numero de ligacoes vai ser
     muito menor que o numero de vertices, isso nao eh um problema. E se o
     numero de ligacoes fosse grande (muito dificil!) entao teriamos que
     triangularizar pouca coisa, pois ja teriamos muitas diagonais. */
  s = NULL;
  for (v=0; v<nlig; v++)
  {
    peso += qdist (poligono.p[ligacao[v].i], poligono.p[ligacao[v].f]);
    /* criamos aux */
    if ((aux = (ts) malloc (sizeof (struct _s))) == NULL)
      runerror (FALTA_MEMORIA);
    if (ligacao[v].i > ligacao[v].f)
    {
      aux->i = ligacao[v].f;
      aux->f = ligacao[v].i;
    }
    else
    {
      aux->i = ligacao[v].i;
      aux->f = ligacao[v].f;
    }
    aux->dir = aux->esq = NULL;

    /* e agora colocamos aux na arvore, no lugar correto */
    if (s == NULL)
      s = aux;

    atual = s;
    /* enquanto nao acharmos, fazemos a busca */
    while (atual != aux)
    {
      if (aux->i == atual->i && aux->f == atual->f)
      {
	free (aux);
	break;
      }
      if (aux->i >= atual->i && aux->i <= atual->f &&
	  aux->f >= atual->i && aux->f <= atual->f)
      {
	if (atual->dir == NULL) /* se achou o lugar */
	  atual->dir = aux;
	atual = atual->dir;
      }
      else
      {
	if (atual->esq == NULL) /* se achou o lugar */
	  atual->esq = aux;
	atual = atual->esq;
      }
    }
  }

  /* guardamos poly em poligono2 global (necessario para 'percorre') */
  poligono2 = poly;

  /* e agora percorremos a ABB, triangularizando os poligonos monotonos
     representados pelas "folhas" */
  peso += percorre (s, pts, 0);
  if (s != NULL)
    free (s);

  return peso;
}

/* Funcao que percorre uma arvore binaria de busca s */
double percorre (ts s, tdponto pts, int i)
{
  int a, b;
  double peso = 0;
  if (s == NULL) /* se folha */
    peso += monotone (poligono2, pts, i);
  else
  {
    /* rearranja lista para o lado esquerdo */
    a = pts[s->f].ante;
    b = pts[s->i].prox;
    pts[s->f].ante = s->i;
    pts[s->i].prox = s->f;
    peso += percorre (s->esq, pts, s->i);

    /* volta lista */
    pts[s->f].ante = a;
    pts[s->i].prox = b;
    if (s->esq != NULL)
      free (s->esq);

    /* rearranja lista para o lado direita */
    a = pts[s->f].prox;
    b = pts[s->i].ante;
    pts[s->f].prox = s->i;
    pts[s->i].ante = s->f;
    peso += percorre (s->dir, pts, s->i);

    /* volta lista */
    pts[s->f].prox = a;
    pts[s->i].ante = b;
    if (s->dir != NULL)
      free (s->dir);
  }
  return peso;
}
/* Fim de TRIANG.C */
