#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>

struct no {
	int chave;
	int bal;
	struct no *esq;
	struct no *dir;
};
typedef struct no No;

// ----- INSERIR -----

void rotacao_dir(No **pt, int * h) {

	No * ptu = (*pt)->esq;

	if (ptu->bal == -1) {
		
		(*pt)->esq = ptu->dir;
		ptu->dir = (*pt);
		(*pt) = ptu;
		(*pt)->bal = (*pt)->dir->bal = 0;

	} else {

		No * ptv = ptu->dir;
		ptu->dir = ptv->esq;
		(*pt)->esq = ptv->dir;
		ptv->esq = ptu;
		ptv->dir = (*pt);
		(*pt) = ptv;

		switch (ptv->bal) {
			case -1:
				ptu->bal = 0;
				(*pt)->dir->bal = 1;
				break;
			case 0:
				ptu->bal = 0;
				(*pt)->dir->bal = 0;
				break;
			case 1:
				ptu->bal = -1;
				(*pt)->dir->bal = 0;
				break;
		}

		(*pt)->bal = 0;

	}
	*h = 0;
}

void rotacao_esq(No **pt, int * h) {

	No * ptu = (*pt)->dir;

	if (ptu->bal == 1) {
		
		(*pt)->dir = ptu->esq;
		ptu->esq = (*pt);
		(*pt) = ptu;
		(*pt)->bal = (*pt)->esq->bal = 0;

	} else {

		No * ptv = ptu->esq;
		ptu->esq = ptv->dir;
		(*pt)->dir = ptv->esq;
		ptv->dir = ptu;
		ptv->esq = (*pt);
		(*pt) = ptv;
		
		switch (ptv->bal) {
			case 1:
				ptu->bal = 0;
				(*pt)->esq->bal = -1;
				break;
			case 0:
				ptu->bal = 0;
				(*pt)->esq->bal = 0;
				break;
			case -1:
				ptu->bal = 1;
				(*pt)->esq->bal = 0;
				break;
		}

		(*pt)->bal = 0;
	}
	*h = 0;
}

int inserir_avl(int x, No **pt, int * h) {
	int ins = 0;
	if ((*pt) == NULL) {

		No * novo = malloc(sizeof(No));
		novo->esq = novo->dir = NULL;
		novo->chave = x;
		novo->bal = 0;

		(*pt) = novo;
		*h = 1;

		return 1;

	} else {

		if (x == ((*pt)->chave)) {
			// printf("El. já existe");

			return 0;

		} else {

			if ( x < (*pt)->chave ) {
				ins = inserir_avl(x, &(*pt)->esq, h);

				if ((*h) == 1) {
					switch ((*pt)->bal) {
						case 1:
							(*pt)->bal = 0;
							*h = 0;
							break;
						case 0:
							(*pt)->bal = -1;
							*h = 1;
							break;
						case -1:
							rotacao_dir(&(*pt), h);
							break;
					}
				}

			} else {

				ins = inserir_avl(x, &(*pt)->dir, h);

				if ((*h) == 1) {
					switch ((*pt)->bal) {
						case -1:
							(*pt)->bal = 0;
							*h = 0;
							break;
						case 0:
							(*pt)->bal = 1;
							*h = 1;
							break;
						case 1:
							rotacao_esq(&(*pt), h);
							break;
					}
				}

			}

			return ins;
		}

	}
}

// ----- REMOVER -----

void rotacao_dir_rem(No **pt, int * h) {

	No * ptu = (*pt)->esq;

	if (ptu->bal <= 0 ) {
		(*pt)->esq = ptu->dir;
		ptu->dir = (*pt);
		(*pt) = ptu;

		if (ptu->bal == -1) {
			ptu->bal = (*pt)->dir->bal = 0;
			*h = 1;
		} else {
			ptu->bal = 1;
			(*pt)->dir->bal = -1;
			*h = 0;
		}

	} else {
		No * ptv = ptu->dir;
		ptu->dir = ptv->esq;
		(*pt)->esq = ptv->dir;
		ptv->esq = ptu;
		ptv->dir = (*pt);
		(*pt) = ptv;
		switch (ptv->bal) {
			case -1:
				ptu->bal = 0;
				(*pt)->dir->bal = 1;
				break;
			case 0:
				ptu->bal = (*pt)->dir->bal = 0;
				break;
			case 1:
				ptu->bal = -1;
				(*pt)->dir->bal = 0;
				break;
		}

		(*pt)->bal = 0;
		*h = 1;
	}

}

void rotacao_esq_rem(No **pt, int * h) {

	No * ptu = (*pt)->dir;

	if (ptu->bal >= 0 ) {
		(*pt)->dir = ptu->esq;
		ptu->esq = (*pt);
		(*pt) = ptu;

		if (ptu->bal == 1) {
			ptu->bal = (*pt)->esq->bal = 0;
			*h = 1;
		} else {
			ptu->bal = -1;
			(*pt)->esq->bal = 1;
			*h = 0;
		}

	} else {
		No * ptv = ptu->esq;
		ptu->esq = ptv->dir;
		(*pt)->dir = ptv->esq;
		ptv->dir = ptu;
		ptv->esq = (*pt);
		(*pt) = ptv;
		switch (ptv->bal) {
			case 1:
				ptu->bal = 0;
				(*pt)->esq->bal = -1;
				break;
			case 0:
				ptu->bal = (*pt)->esq->bal = 0;
				break;
			case -1:
				ptu->bal = +1;
				(*pt)->esq->bal = 0;
				break;
		}

		(*pt)->bal = 0;
		*h = 1;
	}

}

void balancear(No ** pt, char R, int * h) {
	if (*h == 1) {
		
		if (R == 'D') {

			switch ((*pt)->bal) {
				case 1: 
					(*pt)->bal = 0;
					break;
				case 0: 
					(*pt)->bal = -1;
					*h = 0;
					break;
				case -1: 
					rotacao_dir_rem(&(*pt), h);
					break;
			}

		} else {

			switch ((*pt)->bal) {
				case -1: 
					(*pt)->bal = 0;
					break;
				case 0: 
					(*pt)->bal = +1;
					*h = 0;
					break;
				case +1: 
					rotacao_esq_rem(&(*pt), h);
					break;
			}
		}

	}
}

void remover_avl(int x, No ** ptraiz, int * h) {

	No ** pt = ptraiz;
	
	if ((*pt) == NULL) {
		printf("Elemento não encontrado!");

	} else {
		
		if ( x < (*pt)->chave) {

			remover_avl(x, &(*pt)->esq, h);
			balancear(&(*pt), 'E', h);

		} else {
			
			if (x > (*pt)->chave) {
				remover_avl(x, &(*pt)->dir, h);
				balancear(&(*pt), 'D', h);

			} else {
				No * aux = (*pt);

				if ((*pt)->esq == NULL) {
					(*pt) = (*pt)->dir;
					*h = 1;
					free(aux);

				} else {

					if ((*pt)->dir == NULL) {
						(*pt) = (*pt)->esq;
						*h = 1;
						free(aux);

					} else {
						No * s = (*pt)->dir;
						
						if (s->esq == NULL) {
							(*pt)->dir = s->dir;
							s->esq = (*pt)->esq;
							s->bal = (*pt)->bal;
							(*pt) = s;
							*h = 1;
							free(aux);
						
						} else {
							No * PaiS = s;
							while (s->esq != NULL) {
								PaiS = s;
								s = s->esq;
							}

							int a = (*pt)->chave;
							(*pt)->chave = PaiS->esq->chave;
							PaiS->esq->chave = a;

							remover_avl(x, &(*pt)->dir, h);
						}
						balancear(&(*pt), 'D', h);
					}
				}
			}
		}
	}
}

// ----- CALCULAR ALTURA E VERIFICAR SE É AVL -----

int verifica_avl(No * pt, int * alt) {
	int alt_esq = 0;
	int alt_dir = 0;
	int verif_dir = 0;
	int verif_esq = 0;

	if (pt == NULL) {
		*alt = 0;
		return 1;
	}
	
	verif_esq = verifica_avl(pt->esq, &alt_esq);
	verif_dir = verifica_avl(pt->dir, &alt_dir);

	if (alt_dir > alt_esq) {
		*alt = alt_dir+1;
	} else {
		*alt = alt_esq+1;
	}

	if(abs(alt_dir - alt_esq) >= 2 || pt->bal != (alt_dir - alt_esq)) {
		return 0;
	}

	return verif_esq && verif_dir;

}

// ----- CONTAR NÓS -----

int contar_nos(No * pt) {
	int count = 1;
	int c1 = 0;
	int c2 = 0;

	if (pt == NULL) {
		return 0;
	}

	c1 = contar_nos(pt->esq);
	c2 = contar_nos(pt->dir);
	
	return count+c1+c2;
}

// ----- IMPRIMIR AVL -----

void pre_ordem(No * pt) {
	printf("%d-%d ", pt->chave, pt->bal);
	if	(pt->esq != NULL) 
		pre_ordem(pt->esq);
	if	(pt->dir != NULL) 
		pre_ordem(pt->dir);
}

// ----- TESTES -----

// função para desalocar toda a árvore

void desalocar(No ** pt) {

	if ((*pt) != NULL) {

		desalocar(&(*pt)->esq);
		desalocar(&(*pt)->dir);

		free((*pt));

	}

}

void teste_automatizado(int qtd_avl, int qtd_nos_ins, int qtd_nos_rem, int *qtd_ins_suc, int *qtd_rem_suc) {

	int i, j, chave, h, alt_avl, verif, c_indice, aux, ins;

	No * ptraiz = NULL;

	int * chaves = malloc(sizeof(int)*qtd_nos_ins);
	
	for (i = 1; i <= qtd_avl; i++){
		ins = 1;

		printf("\n-------RODADA %d-------\n", i);
		
		printf("\nInserções:");

		for(j = 0; j < qtd_nos_ins; j++ ) {
			h=-2;
			chave = rand() % 100001;

			ins = inserir_avl(chave, &ptraiz, &h);
			if(ins) {
				chaves[j] = chave;
			} else {
				j--;
			}
		}

		printf("\n\nNº de nós: %d", contar_nos(ptraiz));

		verif = verifica_avl(ptraiz, &alt_avl);

		printf("\nAltura: %d\nÉ AVL? %s", alt_avl, verif ? "sim" : "nao");

		if (contar_nos(ptraiz) == qtd_nos_ins && verif == 1){
			*qtd_ins_suc+=1;
		} else {
			break;
		}

		printf("\n\nRemoções:");

		for(j = 0; j < qtd_nos_rem; j++ ) {
			h=-2;
			c_indice = rand() % (qtd_nos_ins-j);

			remover_avl(chaves[c_indice], &ptraiz, &h);

			aux = chaves[c_indice];
			chaves[c_indice] = chaves[qtd_nos_ins-(j+1)];
			chaves[qtd_nos_ins-(j+1)] = aux;
		}

		printf("\n\nNº de nós: %d", contar_nos(ptraiz));

		verif = verifica_avl(ptraiz, &alt_avl);

		printf("\nAltura: %d\nContinua sendo AVL? %s", alt_avl, verif ? "sim" : "nao");

		if (contar_nos(ptraiz) == (qtd_nos_ins - qtd_nos_rem) && verif == 1) {
			*qtd_rem_suc+=1;
		} else {
			break;
		}

		printf("\n\nInserções: %s\nRemoções: %s\n", *qtd_ins_suc ? "sucesso":"falha", *qtd_rem_suc?"sucesso":"falha");

		desalocar(&ptraiz);

		ptraiz=NULL;
	}

}

int main(void) {
	setlocale(LC_ALL, "Portuguese");
	srand ( time(NULL) );
	
	int qtd_ins = 0, qtd_rem = 0;

	teste_automatizado(1000, 10000, 1000, &qtd_ins, &qtd_rem);

	printf("\n\nÁrvores com inserções bem sucedidas: %d\nÁrvores com remoções bem sucedidas: %d", qtd_ins, qtd_rem);

	return 0;
}

// Aluna: Natália Lídia Lima Coelho - 472327